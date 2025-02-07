#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <stdio.h>
#include <math.h>


void yuvToRgb(int *rgb, int y, int  u, int v)
{
    rgb[0] = y + 1.402 * (v - 128);
    rgb[1] = y - 0.344136 * (u - 128) - 0.714136 * (v - 128);
    rgb[2] = y + 1.772 * (u - 128);
}

int main(int argc, char *argv[])
{
    int videoStreamIndex = -1;
    AVCodec *codec = NULL;
    AVCodecParameters *param = NULL;
    AVCodecContext *cdcContext = NULL;
    AVPacket *packet = NULL;
    AVFrame *frame = NULL;

    if(argc < 2)
    {
        printf(" Running format : ./<executable> <video file name>\n");
        return 0;
    }
    
    AVFormatContext *fmtContext = avformat_alloc_context();
    avformat_open_input(&fmtContext, argv[1], NULL, NULL);

    avformat_find_stream_info(fmtContext, NULL);
    for(int i = 0; i < fmtContext->nb_streams; i++)
    {
        AVStream *stream = fmtContext->streams[i];
        AVCodecParameters *cParam = stream->codecpar;
        if (cParam->codec_type == AVMEDIA_TYPE_VIDEO) {
            printf("Video Stream Index : %d\n", i);
            printf("  Type: Video\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
            printf("  Resolution: %dx%d\n\n", cParam->width, cParam->height);
            videoStreamIndex = i;
        }
    }

    param = fmtContext->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(param->codec_id); //store codec info

    //We create Codec context and fill it with the codec info for our case
    cdcContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(cdcContext, param);

    //open the codec
    avcodec_open2(cdcContext, codec, NULL);

    //To process the encoded packets and then extract frame from them
    packet = av_packet_alloc();
    frame = av_frame_alloc();

    int runUntil = 50;

    //read the packets
    while(av_read_frame(fmtContext, packet) >= 0 && runUntil--)
    {
        if(packet->stream_index == videoStreamIndex)
        {
            //send the encoded packet to codec
            avcodec_send_packet(cdcContext, packet);
            avcodec_receive_frame(cdcContext, frame);
            char picture = av_get_picture_type_char(frame->pict_type);
            if(picture == 'I' || picture == 'B' || picture == 'P')
            {
                printf("Frame : %d | pts : %d | dts : %d\nwidth : %d | height : %d | line : %d\n\n", picture,
                cdcContext->frame_number,
                frame->pts,
                frame->pkt_dts,
                frame->width,
                frame->height,
                frame->linesize[0]
            );
            if (frame->format == AV_PIX_FMT_YUV420P) {
                printf("Pixel format: YUV420P\n");
            } else if (frame->format == AV_PIX_FMT_YUYV422) {
                printf("Pixel format: YUYV422\n");
            } else if (frame->format == AV_PIX_FMT_RGB24) {
                printf("Pixel format: RGB24\n");
            } else if (frame->format == AV_PIX_FMT_BGR24) {
                printf("Pixel format: BGR24\n");
            } else if (frame->format == AV_PIX_FMT_YUV422P) {
                printf("Pixel format: YUV422P\n");
            } else if (frame->format == AV_PIX_FMT_YUV444P) {
                printf("Pixel format: YUV444P\n");
            } else if (frame->format == AV_PIX_FMT_NV12) {
                printf("Pixel format: NV12\n");
            } else if (frame->format == AV_PIX_FMT_NV21) {
                printf("Pixel format: NV21\n");
            } else {
                printf("Pixel format: Unknown (%d)\n\n", frame->format);
            }
            
            if(frame->format == AV_PIX_FMT_YUV420P)
            printf("Y linesize : %d\nU linesize : %d\nV linesize : %d\n", frame->linesize[0], frame->linesize[1], frame->linesize[2]);

            FILE *fpt = NULL;
            fpt = fopen("img.ppm", "w");

            fprintf(fpt, "P5\n%d %d\n%d\n", frame->width, frame->height, 255);          
            
            //y plane
            for(int y = 0 ; y < frame->height; y++)
            {
                fwrite(frame->data[0] + y*frame->linesize[0], 1, frame->width, fpt);
            }
            printf("\nImage written to img.pgm : graymap format\n\n");
            
            // //u plane
            // for(int y = 0 ; y < frame->height/2; y++)
            // {
            //     fwrite(frame->data[1] + y*frame->linesize[1], 1, frame->width/2, fpt);
            // }

            // //v plane
            // for(int y = 0 ; y < frame->height/2; y++)
            // {
            //     fwrite(frame->data[2] + y*frame->linesize[2], 1, frame->width/2, fpt);
            // }

            fclose(fpt);
            break;
            }
        }
    }





    return 0;
}