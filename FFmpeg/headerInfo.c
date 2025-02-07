#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <stdio.h>

int main(int argc, char *argv[])
{

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
        printf("Stream #%d:\n", i);
        if (cParam->codec_type == AVMEDIA_TYPE_VIDEO) {
            printf("  Type: Video\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
            printf("  Resolution: %dx%d\n", cParam->width, cParam->height);
        } else if (cParam->codec_type == AVMEDIA_TYPE_AUDIO) {
            printf("  Type: Audio\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
            printf("  Sample Rate: %d Hz\n", cParam->sample_rate);
            printf("  Channels: %d\n", cParam->channels);
        } else if (cParam->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            printf("  Type: Subtitle\n");
            printf("  Codec: %s\n", avcodec_get_name(cParam->codec_id));
        } else {
            printf("  Type: Other\n");
        }
    }

    return 0;
}