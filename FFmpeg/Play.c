#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

void getPixelFormat(int format)
{
    if (format == AV_PIX_FMT_YUV420P)
    {
        printf("Pixel format: YUV420P\n");
    }
    else if (format == AV_PIX_FMT_YUYV422)
    {
        printf("Pixel format: YUYV422\n");
    }
    else if (format == AV_PIX_FMT_RGB24)
    {
        printf("Pixel format: RGB24\n");
    }
    else if (format == AV_PIX_FMT_BGR24)
    {
        printf("Pixel format: BGR24\n");
    }
    else if (format == AV_PIX_FMT_YUV422P)
    {
        printf("Pixel format: YUV422P\n");
    }
    else if (format == AV_PIX_FMT_YUV444P)
    {
        printf("Pixel format: YUV444P\n");
    }
    else if (format == AV_PIX_FMT_NV12)
    {
        printf("Pixel format: NV12\n");
    }
    else if (format == AV_PIX_FMT_NV21)
    {
        printf("Pixel format: NV21\n");
    }
    else
    {
        printf("Pixel format: Unknown (%d)\n", format);
    }
}
int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL\n");
        exit(1);
    }

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture;
    AVCodec *codec = NULL;
    AVCodecParameters *param = NULL;
    AVCodecContext *cdcContext = NULL;
    AVPacket *packet = NULL;
    AVFrame *frame = NULL;
    bool test = false;
    int videoStreamIndex = -1;
    int frameFinished = -1;

    if (argc < 2)
    {
        printf(" Running format : ./<executable> <video file name>\n");
        return 0;
    }

    AVFormatContext *fmtContext = avformat_alloc_context();
    avformat_open_input(&fmtContext, argv[1], NULL, NULL);

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
    frame = av_frame_alloc();

    window = SDL_CreateWindow("Playback", 0, 0,
                              cdcContext->width / 2,
                              cdcContext->height / 2,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, 0);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_IYUV,
                                SDL_TEXTUREACCESS_STREAMING,
                                cdcContext->width,
                                cdcContext->height);

    // number of frames to extract
    int runUntil = 100;
    bool done = false;

    // read the packets

        while (av_read_frame(fmtContext, packet) >= 0)
        {
            if (packet->stream_index == videoStreamIndex)
            {
                // send the encoded packet to codec
                avcodec_send_packet(cdcContext, packet);
                frameFinished = avcodec_receive_frame(cdcContext, frame);

                if (!frameFinished)
                {
                    runUntil--;
                    getPixelFormat(frame->format);

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

                    printf("Frame %d Done\n", runUntil);
                }

                if (runUntil == 0 && !test)
                {
                    break;
                }
            }
        }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}