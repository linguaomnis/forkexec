#include <tmrhrm_wav.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

void* wav_sethead(
    WAV_HEAD* head,
    size_t data_length,
    unsigned int frequency,
    unsigned short bits
) {

    /* FORMAT CHUNK */
    memcpy( head->format_chunkid, "fmt ", sizeof(head->format_chunkid) );
    head->format_size = offsetof(WAV_HEAD,data_chunkid) - offsetof(WAV_HEAD,format_id);
    head->format_id = WAV_FORMAT_ID_LINEARPCM;
    head->channelnum = WAV_FORMAT_CHANNELNUM_MONORAL;
    head->frequency = frequency;
    head->bits = bits;
    head->blocksize = head->channelnum * (head->bits / 8);
    head->averagespeed = head->frequency * head->blocksize;

    /* DATA CHUNK */
    memcpy( head->data_chunkid, "data", sizeof(head->data_chunkid) );
    head->data_size = (bits / CHAR_BIT) * data_length;

    /* HEAD */
    memcpy( head->groupid, "RIFF", sizeof(head->groupid) );
    head->size = sizeof(WAV_HEAD) - offsetof(WAV_HEAD,rifftype) + head->data_size;
    memcpy( head->rifftype, "WAVE", sizeof(head->rifftype) );

    return( head );
};

WAV8* wav8_new(
    size_t  data_length,
    unsigned int frequency
) {
    WAV8* wav = NULL;

    if( ( wav = (WAV8*)malloc( sizeof(WAV8) ) ) == NULL )
    {
        fprintf( stderr, "error : wav8_new : memory allocation failed : wav\n" );
        return( NULL );
    }

    wav_sethead( &(wav->head), data_length, frequency, 8 );

    if( ( wav->data = (unsigned char*)malloc( wav->head.data_size ) ) == NULL )
    {
        fprintf( stderr, "error : wav8_new : memory allocation failed : data\n" );
        free( wav );
        return( NULL );
    }

    memset( wav->data, 0x0, wav->head.data_size );

    return( wav );
};

WAV16* wav16_new(
    size_t  data_length,
    unsigned int frequency
) {
    WAV16* wav = NULL;

    if( ( wav = (WAV16*)malloc( sizeof(WAV16) ) ) == NULL )
    {
        fprintf( stderr, "error : wav16_new : memory allocation failed : wav\n" );
        return( NULL );
    }

    wav_sethead( &(wav->head), data_length, frequency, 16 );

    if( ( wav->data = (short*)malloc( wav->head.data_size ) ) == NULL )
    {
        fprintf( stderr, "error : wav16_new : memory allocation failed : data\n" );
        free( wav );
        return( NULL );
    }

    memset( wav->data, 0x0, wav->head.data_size );

    return( wav );
};

void wav8_del( WAV8* wav )
{
    if( wav == NULL ) { return; }
    if( wav->data != NULL ) { free( wav->data ); }
    free( wav );
    return;
};

void wav16_del( WAV16* wav )
{
    if( wav == NULL ) { return; }
    if( wav->data != NULL ) { free( wav->data ); }
    free( wav );
    return;
};

size_t wav8_write( WAV8* wav, FILE* fp )
{
    size_t size = 0;

    if( fp == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : fp\n" );
        return( 0 );
    }
    if( wav == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : wav\n" );
        return( 0 );
    }
    if( wav->data == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : wav->data\n" );
        return( 0 );
    }

    size = 0;
    size += fwrite( &(wav->head), sizeof(wav->head), 1, fp );
    size += fwrite( wav->data, wav->head.data_size, 1, fp );

    return( size );
}

size_t wav16_write( WAV16* wav, FILE* fp )
{
    size_t size = 0;

    if( fp == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : fp\n" );
        return( 0 );
    }
    if( wav == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : wav\n" );
        return( 0 );
    }
    if( wav->data == NULL )
    {
        fprintf( stderr, "error : wav_write : NULL access : wav->data\n" );
        return( 0 );
    }

    size = 0;
    size += fwrite( &(wav->head), sizeof(wav->head), 1, fp );
    size += fwrite( wav->data, wav->head.data_size, 1, fp );

    return( size );
}

size_t wav8_fwrite( WAV8* wav, const char* fpath, const char* mode )
{
    FILE* fp = NULL;
    size_t size = 0;

    fp = fopen( fpath, mode );
    if( fp == NULL )
    {
        return( 1 );
    }

    size = wav8_write( wav, fp );

    fclose( fp );

    return( size );
};

size_t wav16_fwrite( WAV16* wav, const char* fpath, const char* mode )
{
    FILE* fp = NULL;
    size_t size = 0;

    fp = fopen( fpath, mode );
    if( fp == NULL )
    {
        return( 1 );
    }

    size = wav16_write( wav, fp );

    fclose( fp );

    return( size );
};




