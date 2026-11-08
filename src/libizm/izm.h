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
#define IZM_NONE     (3)
#define IZM_END      (4)

/*============================================================*/
/* IZMMATH                                                    */
/*============================================================*/
double izmmath_fmod2( double x, double y );

/*============================================================*/
/* IZMSTR                                                     */
/*============================================================*/
char* izmstr_trim( char* _str );

/* _strbuf のオフセット _offset から長さ _length の領域を _shiftnum だけずらす。 */
/* ずれた跡の領域には _fillchar を入れる。 */
/* はみ出す分は切り捨て。終端文字\0は考慮されない。 */
unsigned char izmstr_shift(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length,
    const int _shiftnum,
    const char _fillchar
);

/* _strbuf のオフセット _offset から長さ _length の領域を _replace で置き換える。 */
/* 置き換え後の文字列全体(終端文字\0含む)が _strbuf に入りきらない場合、 IZM_ERROR を返す。 */
/* IZM_ERROR を返す場合、 _strbuf は変化しない。 */
unsigned char izmstr_replace(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length,
    const const char* _replace,
    const size_t _replacelength
);

unsigned char izmstr_remove(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length
);

unsigned char izmstr_remove_back(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _length
);

unsigned char izmstr_insert(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const const char* _insert,
    const size_t _insertsize
);

unsigned char izmstr_insert_back(
    char* _strbuf,
    const size_t _strbufsize,
    const const char* _insert,
    const size_t _insertsize
);

/* 全体マッチ1個と()グループのマッチ数を足した数の上限 */
#define IZMSTR_REGEXMATCH_MAXMATCH          (16)
#define IZMSTR_REGEXMATCH_SUBMATCH_MAXSIZE  (PATH_MAX)

typedef struct _IZMSTR_REGEXMATCH_MATCH {
    size_t  offset;
    size_t  length;
    char    str[IZMSTR_REGEXMATCH_SUBMATCH_MAXSIZE];
} IZMSTR_REGEXMATCH_MATCH;

typedef struct _IZMSTR_REGEXMATCH {
    unsigned char           stat_comp; /* regcomp status */
    unsigned char           stat_exec; /* regexec status */
    IZMSTR_REGEXMATCH_MATCH match[IZMSTR_REGEXMATCH_MAXMATCH];
} IZMSTR_REGEXMATCH;

/* return : IZM_OK (match), IZM_END (no match), IZM_ERROR (error) */
/* ()グループのマッチ数が多すぎる場合、 stat_comp に REG_ESPACE が入る */
unsigned char izmstr_regexmatch(
    const char* _pattern,
    const char* _strbuf,
    IZMSTR_REGEXMATCH* _regexmatch
);

/* 最初にマッチした1箇所を文字列 _replace で置換する。 */
/* return : IZM_OK, IZM_ERROR (error) */
/* 置換した数が0回でも、エラーでなければ IZM_OK を返す。 */
/* _pattern の()グループに対する _replace のキャプチャ\\1,\\2,..が使用可能。 */
/* ただし全体マッチ1個とあわせて最大 IZMSTR_REGEXMATCH_MAXMATCH 個まで。 */
unsigned char izmstr_regexreplace(
    const char* _pattern,
    char* _strbuf,
    const size_t _strbufsize,
    const char* _replace
);

/* マッチした全箇所を文字列 _replace で置換する。 */
/* return : IZM_OK, IZM_ERROR (error) */
/* 置換した数が0回でも、エラーでなければ IZM_OK を返す。 */
/* _pattern の()グループに対する _replace のキャプチャ\\1,\\2,..が使用可能。 */
/* ただし全体マッチ1個とあわせて最大 IZMSTR_REGEXMATCH_MAXMATCH 個まで。 */
unsigned char izmstr_regexreplace_global(
    const char* _pattern,
    char* _strbuf,
    const size_t _strbufsize,
    const char* _replace
);

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
