#ifndef HRM_WAV_H
#define HRM_WAV_H
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#define WAV_FORMAT_ID_LINEARPCM         (1)
#define WAV_FORMAT_CHANNELNUM_MONORAL   (1)
#define WAV_FORMAT_CHANNELNUM_STEREO    (2)
#define WAV_FORMAT_FREQUENCY_44100      (44100)

typedef struct _WAV_HEAD {
    /* HEAD */
    char            groupid[4];
    unsigned int    size;
    char            rifftype[4];

    /* FORMAT CHUNK */
    char            format_chunkid[4];
    unsigned int    format_size;
    unsigned short  format_id;          /* フォーマットID 1=LinearPCM */
    unsigned short  channelnum;         /* チャンネル数 1=mono 2=stereo */
    unsigned int    frequency;          /* サンプリング周波数 [Hz] */
    unsigned int    averagespeed;       /* 平均データ速度 [byte/s] */
    unsigned short  blocksize;          /* ブロックサイズ [byte/sample] */
    unsigned short  bits;               /* 量子化ビット数 [bit] */

    /* DATA CHUNK */
    char            data_chunkid[4];
    unsigned int    data_size;
} WAV_HEAD;

typedef struct _WAV8 {
    WAV_HEAD        head;
    unsigned char*  data;
} WAV8;

typedef struct _WAV16 {
    WAV_HEAD        head;
    short*          data;
} WAV16;

extern void* wav_sethead(
    WAV_HEAD* head,
    size_t data_length,
    unsigned int frequency,
    unsigned short bits
);

extern WAV8* wav8_new(
    size_t  data_length,
    unsigned int frequency
);

extern WAV16* wav16_new(
    size_t  data_length,
    unsigned int frequency
);

extern void wav8_del( WAV8* wav );
extern void wav16_del( WAV16* wav );
extern size_t wav8_write( WAV8* wav, FILE* fp );
extern size_t wav16_write( WAV16* wav, FILE* fp );
extern size_t wav8_fwrite( WAV8* wav, const char* fpath, const char* mode );
extern size_t wav16_fwrite( WAV16* wav, const char* fpath, const char* mode );

#endif
