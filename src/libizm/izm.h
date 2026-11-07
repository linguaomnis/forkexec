#ifndef LIBIZM_H
#define LIBIZM_H

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define IZM_OK       (0)
#define IZM_NG       (1)
#define IZM_ERROR    (2)

/*============================================================*/
/* IZMMATH                                                    */
/*============================================================*/
double izmmath_fmod2( double x, double y );

/*============================================================*/
/* IZMSTR                                                     */
/*============================================================*/
char* izmstr_trim( char* _str );

/*============================================================*/
/* IZMVECTOR                                                  */
/*============================================================*/
typedef struct {
    size_t  length;
    size_t  alloclength;
    size_t  unitsize;
    void*   p;
} IZMVECTOR;

void izmvector_init( IZMVECTOR* _vp, size_t _unitsize );
void* izmvector_at( IZMVECTOR* _vp, size_t _index );
unsigned char izmvector_insert( IZMVECTOR* _vp, size_t _index, void* _data );
unsigned char izmvector_remove( IZMVECTOR* _vp, size_t _index );

/*============================================================*/
/* IZMWAV                                                     */
/*============================================================*/

#define IZMWAV_FORMAT_ID_LINEARPCM         (1)
#define IZMWAV_FORMAT_CHANNELNUM_MONORAL   (1)
#define IZMWAV_FORMAT_CHANNELNUM_STEREO    (2)
#define IZMWAV_FORMAT_FREQUENCY_44100      (44100)

typedef struct _IZMWAV_HEAD {
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
} IZMWAV_HEAD;

typedef struct _IZMWAV8 {
    IZMWAV_HEAD     head;
    unsigned char*  data;
} IZMWAV8;

typedef struct _IZMWAV16 {
    IZMWAV_HEAD     head;
    short*          data;
} IZMWAV16;

extern void* izmwav_sethead(
    IZMWAV_HEAD* head,
    size_t data_length,
    unsigned int frequency,
    unsigned short bits
);

extern IZMWAV8* izmwav8_new(
    size_t  data_length,
    unsigned int frequency
);

extern IZMWAV16* izmwav16_new(
    size_t  data_length,
    unsigned int frequency
);

extern void izmwav8_del( IZMWAV8* wav );
extern void izmwav16_del( IZMWAV16* wav );
extern size_t izmwav8_write( IZMWAV8* wav, FILE* fp );
extern size_t izmwav16_write( IZMWAV16* wav, FILE* fp );
extern size_t izmwav8_fwrite( IZMWAV8* wav, const char* fpath, const char* mode );
extern size_t izmwav16_fwrite( IZMWAV16* wav, const char* fpath, const char* mode );

#endif
