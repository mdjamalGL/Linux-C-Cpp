#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/**
 * This code extracts the frames, and from frame its data in yub,
 * the yuv is then converted to RGB.
 * Since the conversion is single threaded, its slower even for a single
 * frame, and thus is not suitable for conversion during playback
 * os simple frame to picture extraction
 * 
 * Using libswscale, we can do this faster and efficiently
 */

void yuvToRgb(int *rgb, int y, int  u, int v)
{
    rgb[0] = y + 1.402 * (v - 128);
    rgb[1] = y - 0.344136 * (u - 128) - 0.714136 * (v - 128);
    rgb[2] = y + 1.772 * (u - 128);
}

void getPixelFormat(int format)
{
    if (format == AV_PIX_FMT_YUV420P) {
            printf("Pixel format: YUV420P\n");
        } else if (format == AV_PIX_FMT_YUYV422) {
            printf("Pixel format: YUYV422\n");
        } else if (format == AV_PIX_FMT_RGB24) {
            printf("Pixel format: RGB24\n");
        } else if (format == AV_PIX_FMT_BGR24) {
            printf("Pixel format: BGR24\n");
        } else if (format == AV_PIX_FMT_YUV422P) {
            printf("Pixel format: YUV422P\n");
        } else if (format == AV_PIX_FMT_YUV444P) {
            printf("Pixel format: YUV444P\n");
        } else if (format == AV_PIX_FMT_NV12) {
            printf("Pixel format: NV12\n");
        } else if (format == AV_PIX_FMT_NV21) {
            printf("Pixel format: NV21\n");
        } else {
            printf("Pixel format: Unknown (%d)\n", format);
        }
}

void writeImage(uint8_t **data, int *linesize,int width, int height, FILE *fpt)
{
    struct SwsContext *swCont = sws_getContext(
        width, height, AV_PIX_FMT_YUV420P,
        width, height, AV_PIX_FMT_RGB24,
        SWS_BILINEAR, NULL, NULL, NULL
    );

    int rgb_size = av_image_get_buffer_size(AV_PIX_FMT_RGB24, width, height, 1);
    uint8_t *rgb = (uint8_t *)malloc(rgb_size);
    AVFrame *rgb_frame = av_frame_alloc();

    av_image_fill_arrays(rgb_frame->data, rgb_frame->linesize, rgb, AV_PIX_FMT_RGB24, width, height, 1);

    sws_scale(swCont, data, linesize, 0, height, rgb_frame->data, rgb_frame->linesize);
    
    fprintf(fpt, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(rgb_frame->data[0], 1, rgb_size, fpt);
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
            printf("Video Stream Info : \n");
            printf("  Type: Video\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
            printf("  Resolution: %dx%d\n", cParam->width, cParam->height);
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

    //number of frames to extract
    int runUntil = 1;

    //read the packets
    while(av_read_frame(fmtContext, packet) >= 0)
    {
        if(packet->stream_index == videoStreamIndex)
        {
            //send the encoded packet to codec
            avcodec_send_packet(cdcContext, packet);
            avcodec_receive_frame(cdcContext, frame);
            char picture = av_get_picture_type_char(frame->pict_type);
            if(picture == 'I')
            {
                printf("Frame : %d | pts : %d | dts : %d\nwidth : %d | height : %d | line : %d\n", picture,
                cdcContext->frame_number,
                frame->pts,
                frame->pkt_dts,
                frame->width,
                frame->height,
                frame->linesize[0]);

                runUntil--;
                getPixelFormat(frame->format);

                printf("Data Info  : \n");
                printf("\t\tline0 : %d\n\t\tline1 : %d\n\t\tline2 : %d\n", frame->linesize[0], frame->linesize[1], frame->linesize[2]);

                FILE *fpt = NULL;
                char filename[10] = "img";
                char index[3]; sprintf(index, "%d", runUntil);
                char dot[] = ".ppm";
                strcat(filename, index);
                strcat(filename, dot);
                fpt = fopen(filename, "w");
                
                //write RGB data to image portable pixmap format
                writeImage(frame->data, frame->linesize, frame->width, frame->height, fpt);


                fclose(fpt);
                printf("Frame %d Done\n", runUntil);
            }
            
            if(runUntil == 0)
            {
                break;
            }
        }
    }
    return 0;
}