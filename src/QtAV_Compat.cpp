#include <QtAV/QtAV_Compat.h>
#include "prepost.h"
#include "libavutil/avstring.h"
#include "libavutil/common.h"

void ffmpeg_version_print()
{
    printf("FFMpeg/Libav version:\n"
           "libavcodec-%d.%d.%d\n"
           "libavformat-%d.%d.%d\n"
//	       "libavdevice-%d.%d.%d\n"
           "libavutil-%d.%d.%d\n"
           "libswscal-%d.%d.%d\n"
           , LIBAVCODEC_VERSION_MAJOR, LIBAVCODEC_VERSION_MINOR, LIBAVCODEC_VERSION_MICRO//,avcodec_version()
           , LIBAVFORMAT_VERSION_MAJOR, LIBAVFORMAT_VERSION_MINOR, LIBAVFORMAT_VERSION_MICRO //avformat_version()
//	       ,avdevice_version()
           , LIBAVUTIL_VERSION_MAJOR, LIBAVUTIL_VERSION_MINOR, LIBAVUTIL_VERSION_MICRO //avutil_version()
           , LIBSWSCALE_VERSION_MAJOR, LIBSWSCALE_VERSION_MINOR, LIBSWSCALE_VERSION_MICRO
           );
    fflush(0);
}

PRE_FUNC_ADD(ffmpeg_version_print);

#ifndef av_err2str

struct error_entry {
    int num;
    const char *tag;
    const char *str;
};

#if 0
#define ERROR_TAG(tag) AVERROR_##tag, #tag
static const struct error_entry error_entries[] = {
    { ERROR_TAG(BSF_NOT_FOUND),      "Bitstream filter not found"                     },
    { ERROR_TAG(BUG),                "Internal bug, should not have happened"         },
    { ERROR_TAG(BUG2),               "Internal bug, should not have happened"         },
    { ERROR_TAG(BUFFER_TOO_SMALL),   "Buffer too small"                               },
    { ERROR_TAG(DECODER_NOT_FOUND),  "Decoder not found"                              },
    { ERROR_TAG(DEMUXER_NOT_FOUND),  "Demuxer not found"                              },
    { ERROR_TAG(ENCODER_NOT_FOUND),  "Encoder not found"                              },
    { ERROR_TAG(EOF),                "End of file"                                    },
    { ERROR_TAG(EXIT),               "Immediate exit requested"                       },
    { ERROR_TAG(EXTERNAL),           "Generic error in an external library"           },
    { ERROR_TAG(FILTER_NOT_FOUND),   "Filter not found"                               },
    { ERROR_TAG(INVALIDDATA),        "Invalid data found when processing input"       },
    { ERROR_TAG(MUXER_NOT_FOUND),    "Muxer not found"                                },
    { ERROR_TAG(OPTION_NOT_FOUND),   "Option not found"                               },
    { ERROR_TAG(PATCHWELCOME),       "Not yet implemented in FFmpeg, patches welcome" },
    { ERROR_TAG(PROTOCOL_NOT_FOUND), "Protocol not found"                             },
    { ERROR_TAG(STREAM_NOT_FOUND),   "Stream not found"                               },
    { ERROR_TAG(UNKNOWN),            "Unknown error occurred"                         },
    { ERROR_TAG(EXPERIMENTAL),       "Experimental feature"                           },
};
#endif
int av_strerror(int errnum, char *errbuf, size_t errbuf_size)
{
    int ret = 0, i;
    const struct error_entry *entry = NULL;

    for (i = 0; i < FF_ARRAY_ELEMS(error_entries); i++) {
        if (errnum == error_entries[i].num) {
            entry = &error_entries[i];
            break;
        }
    }
    if (entry) {
        av_strlcpy(errbuf, entry->str, errbuf_size);
    } else {
#if HAVE_STRERROR_R
        ret = AVERROR(strerror_r(AVUNERROR(errnum), errbuf, errbuf_size));
#else
        ret = -1;
#endif
        if (ret < 0)
            snprintf(errbuf, errbuf_size, "Error number %d occurred", errnum);
    }

    return ret;
}

#endif //av_err2str

