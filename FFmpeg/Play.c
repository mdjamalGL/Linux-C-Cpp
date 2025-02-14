
#define _XOPEN_SOURCE 600
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int demo(AVFormatContext *fmtContext , AVPacket *packet)
{
    printf("Read Frame Called\n");
    return av_read_frame(fmtContext, packet);
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL\n");
        exit(1);
    }

    //SDL Objects
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture;

    //FFmpeg Objects
    const AVCodec *codec = NULL;
    AVCodecParameters *param = NULL;
    AVCodecContext *cdcContext = NULL;
    AVPacket *packet = NULL;
    AVFrame *frame = NULL;
    AVRational time_base = {};
    int64_t lastPts = AV_NOPTS_VALUE;
    int64_t delay = 0;
    int64_t seekTimestamp;
    int seekTarget;


    //util objects
    int videoStreamIndex = -1;
    AVRational avgFrameRate = {};
    int frameFinished = -1;
    bool quit = false;
    bool playbackStop = false;

    if (argc < 2)
    {
        printf(" Running format : ./<executable> <video file name>\n");
        return 0;
    }

    AVFormatContext *fmtContext = avformat_alloc_context();
    if(fmtContext == NULL)
    {
        printf("Error : Format Context could not be allocated\n");
    }
    if(avformat_open_input(&fmtContext, argv[1], NULL, NULL) != 0)
    {
        printf("Error : could not open input\n");
    }

    avformat_find_stream_info(fmtContext, NULL);
    for (int i = 0; i < fmtContext->nb_streams; i++)
    {
        AVStream *stream = fmtContext->streams[i];
        AVCodecParameters *cParam = stream->codecpar;
        if (cParam->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            printf("Video Stream Info : \n");
            printf("  Type: Video\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
            printf("  Resolution: %dx%d\n", cParam->width, cParam->height);
            videoStreamIndex = i;
            avgFrameRate = stream->r_frame_rate;
            time_base = stream->time_base;

        }
    }

    param = fmtContext->streams[videoStreamIndex]->codecpar;
    codec = avcodec_find_decoder(param->codec_id); // store codec info

    // We create Codec context and fill it with the codec info for our case
    cdcContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(cdcContext, param);

    // open the codec
    avcodec_open2(cdcContext, codec, NULL);

    // To process the encoded packets and then extract frame from them
    packet = av_packet_alloc();
        if(packet == NULL)
        {
            printf("Error : Packet could not be allocated\n");
        }
    frame = av_frame_alloc();
        if(frame == NULL)
        {
            printf("Error : frame could not be allocated\n");
        }

    char windowName[50] = "Playback : ";
    strcat(windowName, argv[1]);

    window = SDL_CreateWindow(windowName, 0, 0,
                              cdcContext->width / 2,
                              cdcContext->height / 2,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);

    //SDL_PIXEL_FORMAT_IYUV is same as AV_PIX_FMT_YUV420P
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_IYUV,
                                SDL_TEXTUREACCESS_STREAMING,
                                cdcContext->width,
                                cdcContext->height);


    // read the packets
    while(playbackStop || demo(fmtContext, packet) == 0)
    {
        
        
        if(packet == NULL)
        {
            printf("Encoded Packet Read was unsuccessful\n");
        }
        SDL_Event event;
        while(SDL_PollEvent(&event) != 0)
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_KP_ENTER)
                {
                    //todo : stop and play
                    //somehow need to stop frame decoding in av_read_frame
                    playbackStop = playbackStop ? false : true;
                    if(playbackStop)
                    {
                        seekTarget = cdcContext->frame_number;
                        // seekTimestamp= av_rescale_q(cdcContext->frame_num, (AVRational){avgFrameRate.den, avgFrameRate.num}, time_base);
                        avformat_close_input(&fmtContext);
                        fmtContext = NULL;
                    }
                    else
                    {
                        printf("Playback Resumed : \n");
                        printf("Seeking to frame : %d\n", seekTarget);
                        fmtContext = avformat_alloc_context();
                        avformat_open_input(&fmtContext, argv[1], NULL, NULL);

                        printf("Seek to the timeStamp : %ld\n", seekTimestamp);
                        av_seek_frame(fmtContext, videoStreamIndex, seekTimestamp,NULL);
                        avcodec_flush_buffers(cdcContext);
                        lastPts = AV_NOPTS_VALUE;
                        continue;
                    }
                }
            }
        }
        if(playbackStop)
        {
            usleep((useconds_t)60000);
            continue;
        }
        if (packet->stream_index == videoStreamIndex)
        {
            // send the encoded packet to codec
            
            avcodec_send_packet(cdcContext, packet);
            frameFinished = avcodec_receive_frame(cdcContext, frame);
            
            if (!frameFinished)
            {           
                printf("playback %c : %d | %d\n",av_get_picture_type_char(frame->pict_type), playbackStop, cdcContext->frame_num);     
                if(frame->pts != AV_NOPTS_VALUE)
                {
                    if(lastPts != AV_NOPTS_VALUE)
                    {
                        delay = av_rescale_q(frame->pts - lastPts, time_base, AV_TIME_BASE_Q);
                        printf("frame : %d, Pts : %d, delay : %ld\n", cdcContext->frame_number, frame->pts, delay);
                        usleep(delay);
                    }
                    lastPts = frame->pts;
                }

                int ret = SDL_UpdateYUVTexture(texture, NULL, frame->data[0], frame->linesize[0],
                                                frame->data[1], frame->linesize[1],
                                                frame->data[2], frame->linesize[2]);
                if (ret == -1)
                {
                    printf("SDL Texture could not be loaded : %s\n", SDL_GetError());
                }

                SDL_RenderClear(renderer);
                SDL_RenderCopy(renderer, texture, NULL, NULL);
                SDL_RenderPresent(renderer);
            }
            seekTimestamp = frame->pts;
            av_frame_unref(frame);
        }
        av_packet_unref(packet);

        if(quit)
        {
            break;
        }
    }

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&cdcContext);
    avformat_close_input(&fmtContext);

    fmtContext = NULL;
    codec = NULL;
    param = NULL;
    cdcContext = NULL;
    packet = NULL;
    frame = NULL;

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    renderer = NULL;
    window = NULL;
    texture = NULL;

    
    SDL_Quit();
    return 0;
}