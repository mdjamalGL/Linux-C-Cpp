#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <stdio.h>
#include <math.h>

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
            printf("  Time Base : %d/%d\n", stream->time_base.num, stream->time_base.den);
            printf("  R Frame Rate : %d/%d\n", stream->r_frame_rate.num, stream->r_frame_rate.den);
            printf("  Average frame Rate : %d/%d\n", stream->avg_frame_rate.num/stream->avg_frame_rate.den);
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
    AVRational fTB, pTB;
    int runUntil = 50;

    //read the packets
    while(av_read_frame(fmtContext, packet) >= 0 && runUntil--)
    {
        if(packet->stream_index == videoStreamIndex)
        {
            //send the encoded packet to codec
            avcodec_send_packet(cdcContext, packet);
            avcodec_receive_frame(cdcContext, frame);
            AVRational avgFrameRate = fmtContext->streams[videoStreamIndex]->avg_frame_rate;
            AVRational timebase = fmtContext->streams[videoStreamIndex]->time_base;
            char picture = av_get_picture_type_char(frame->pict_type);
            if(picture == 'I' || picture == 'B' || picture == 'P')
            {
                fTB = frame->time_base;
                pTB = packet->time_base;
                printf("Frame %c : %d Next PTS : %d ms | pts : %ld ms | dts : %d ms | seektimestamp : %d ms\n", picture,
                cdcContext->frame_num,
                av_rescale_q(1, (AVRational){avgFrameRate.den,avgFrameRate.num}, AV_TIME_BASE_Q),
                av_rescale_q(frame->pts , timebase, AV_TIME_BASE_Q),
                av_rescale_q(frame->pkt_dts, timebase, AV_TIME_BASE_Q),
                av_rescale_q(cdcContext->frame_number,(AVRational){avgFrameRate.den,avgFrameRate.num}, AV_TIME_BASE_Q)
                );
            }
        }
    }





    return 0;
}