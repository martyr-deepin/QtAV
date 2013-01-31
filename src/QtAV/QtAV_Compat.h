/******************************************************************************
	QtAV:  Media play library based on Qt and FFmpeg
	solve the version problem and diffirent api in FFmpeg and libav
    Copyright (C) 2012-2013 Wang Bin <wbsecg1@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef QTAV_COMPAT_H
#define QTAV_COMPAT_H

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

#ifndef AV_VERSION_INT
#define AV_VERSION_INT(a, b, c) (a<<16 | b<<8 | c)
#endif //AV_VERSION_INT


#if (LIBAVUTIL_VERSION_INT > AV_VERSION_INT(49, 15, 0))
#include <libavutil/error.h>
#include <libavutil/opt.h>
#endif
#ifdef __cplusplus
}
#endif //__cplusplus

#include "QtAV_Global.h"
/*!
 * Guide to uniform the api for different FFMpeg version(or other libraries)
 * We use the existing old api to simulater .
 * 1. The old version does not have this api: Just add it.
 * 2. The old version has similar api: Try using macro.
 * e.g. the old is bool my_play(char* data, size_t size)
 *      the new is bool my_play2(const ByteArray& data)
 * change:
 *    #define my_play2(data) my_play(data.data(), data.size());
 *
 * 3. The old version api is conflicted with the latest's. We can redefine the api
 * e.g. the old is bool my_play(char* data, size_t size)
 *      the new is bool my_play(const ByteArray& data)
 * change:
 *    typedef bool (*my_play_t)(const ByteArray&);
 *    static my_play_t my_play_ptr = my_play; //using the existing my_play(char*, size_t)
 *    #define my_play my_play_compat
 *    inline bool my_play_compat(const ByteArray& data)
 *    {
 *        return my_play_ptr(data.data(), data.size());
 *    }
 * 4. conflict macros
 * see av_err2str
 */

#if LIBAVCODEC_VERSION_INT <= AV_VERSION_INT(52, 20, 1)
//AVMediaType
#define AVMediaType             CodecType
#define AVMEDIA_TYPE_UNKNOWN    CODEC_TYPE_UNKNOWN
#define AVMEDIA_TYPE_VIDEO      CODEC_TYPE_VIDEO
#define AVMEDIA_TYPE_AUDIO      CODEC_TYPE_AUDIO
#define AVMEDIA_TYPE_DATA       CODEC_TYPE_DATA
#define AVMEDIA_TYPE_SUBTITLE   CODEC_TYPE_SUBTITLE
#define AVMEDIA_TYPE_ATTACHMENT CODEC_TYPE_ATTACHMENT
#define AVMEDIA_TYPE_NB         CODEC_TYPE_NB


#define avcodec_open2(_avctx, _codec, __options)  avcodec_open(_avctx, _codec)
#endif //LIBAVCODEC_VERSION_INT <= AV_VERSION_INT(52, 20, 1)
void ffmpeg_version_print();

//TODO: libav
//avutil: error.h
#if !defined(av_err2str) || (GCC_VERSION_AT_LEAST(4, 7, 2) && __cplusplus)
#ifdef av_err2str
#undef av_err2str
//#define av_make_error_string qtav_make_error_string
#else
/**
 * Put a description of the AVERROR code errnum in errbuf.
 * In case of failure the global variable errno is set to indicate the
 * error. Even in case of failure av_strerror() will print a generic
 * error message indicating the errnum provided to errbuf.
 *
 * @param errnum      error code to describe
 * @param errbuf      buffer to which description is written
 * @param errbuf_size the size in bytes of errbuf
 * @return 0 on success, a negative value if a description for errnum
 * cannot be found
 */
int av_strerror(int errnum, char *errbuf, size_t errbuf_size);

/**
 * Fill the provided buffer with a string containing an error string
 * corresponding to the AVERROR code errnum.
 *
 * @param errbuf         a buffer
 * @param errbuf_size    size in bytes of errbuf
 * @param errnum         error code to describe
 * @return the buffer in input, filled with the error description
 * @see av_strerror()
 */
static av_always_inline char *av_make_error_string(char *errbuf, size_t errbuf_size, int errnum)
{
	av_strerror(errnum, errbuf, errbuf_size);
	return errbuf;
}
#endif //av_err2str

#define AV_ERROR_MAX_STRING_SIZE 64
av_always_inline char* av_err2str(int errnum)
{
	static char str[AV_ERROR_MAX_STRING_SIZE];
	memset(str, 0, sizeof(str));
	return av_make_error_string(str, AV_ERROR_MAX_STRING_SIZE, errnum);
}

/**
 * Convenience macro, the return value should be used only directly in
 * function arguments but never stand-alone.
 */
//GCC: taking address of temporary array
/*
#define av_err2str(errnum) \
    av_make_error_string((char[AV_ERROR_MAX_STRING_SIZE]){0}, AV_ERROR_MAX_STRING_SIZE, errnum)
*/
#endif //!defined(av_err2str) || GCC_VERSION_AT_LEAST(4, 7, 2)

#if (LIBAVCODEC_VERSION_INT <= AV_VERSION_INT(52,23,0))
#define avcodec_decode_audio3(avctx, samples, frame_size_ptr, avpkt) \
    avcodec_decode_audio2(avctx, samples, frame_size_ptr, (*avpkt).data, (*avpkt).size);

#endif

#if (LIBAVCODEC_VERSION_INT <= AV_VERSION_INT(52,101,0))
#define av_dump_format(...) dump_format(__VA_ARGS__)
#endif

#endif
