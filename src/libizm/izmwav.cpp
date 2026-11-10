#include <izm.hpp>

extern "C"
void* izmwav_sethead(
    IZMWAV_HEAD* head,
    size_t data_length,
    unsigned int frequency,
    unsigned short bits
) {

    /* FORMAT CHUNK */
    memcpy( head->format_chunkid, "fmt ", sizeof(head->format_chunkid) );
    head->format_size = offsetof(IZMWAV_HEAD,data_chunkid) - offsetof(IZMWAV_HEAD,format_id);
    head->format_id = IZMWAV_FORMAT_ID_LINEARPCM;
    head->channelnum = IZMWAV_FORMAT_CHANNELNUM_MONORAL;
    head->frequency = frequency;
    head->bits = bits;
    head->blocksize = head->channelnum * (head->bits / 8);
    head->averagespeed = head->frequency * head->blocksize;

    /* DATA CHUNK */
    memcpy( head->data_chunkid, "data", sizeof(head->data_chunkid) );
    head->data_size = (bits / CHAR_BIT) * data_length;

    /* HEAD */
    memcpy( head->groupid, "RIFF", sizeof(head->groupid) );
    head->size = sizeof(IZMWAV_HEAD) - offsetof(IZMWAV_HEAD,rifftype) + head->data_size;
    memcpy( head->rifftype, "WAVE", sizeof(head->rifftype) );

    return( head );
};

extern "C"
IZMWAV8* izmwav8_new(
    size_t  data_length,
    unsigned int frequency
) {
    IZMWAV8* wav = NULL;

    if( ( wav = (IZMWAV8*)malloc( sizeof(IZMWAV8) ) ) == NULL )
    {
        fprintf( stderr, "error : wav8_new : memory allocation failed : wav\n" );
        return( NULL );
    }

    izmwav_sethead( &(wav->head), data_length, frequency, 8 );

    if( ( wav->data = (unsigned char*)malloc( wav->head.data_size ) ) == NULL )
    {
        fprintf( stderr, "error : wav8_new : memory allocation failed : data\n" );
        free( wav );
        return( NULL );
    }

    memset( wav->data, 0x0, wav->head.data_size );

    return( wav );
};

extern "C"
IZMWAV16* izmwav16_new(
    size_t  data_length,
    unsigned int frequency
) {
    IZMWAV16* wav = NULL;

    if( ( wav = (IZMWAV16*)malloc( sizeof(IZMWAV16) ) ) == NULL )
    {
        fprintf( stderr, "error : wav16_new : memory allocation failed : wav\n" );
        return( NULL );
    }

    izmwav_sethead( &(wav->head), data_length, frequency, 16 );

    if( ( wav->data = (short*)malloc( wav->head.data_size ) ) == NULL )
    {
        fprintf( stderr, "error : wav16_new : memory allocation failed : data\n" );
        free( wav );
        return( NULL );
    }

    memset( wav->data, 0x0, wav->head.data_size );

    return( wav );
};

extern "C"
void izmwav8_del( IZMWAV8* wav )
{
    if( wav == NULL ) { return; }
    if( wav->data != NULL ) { free( wav->data ); }
    free( wav );
    return;
};

extern "C"
void izmwav16_del( IZMWAV16* wav )
{
    if( wav == NULL ) { return; }
    if( wav->data != NULL ) { free( wav->data ); }
    free( wav );
    return;
};

extern "C"
size_t izmwav8_write( IZMWAV8* wav, FILE* fp )
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

extern "C"
size_t izmwav16_write( IZMWAV16* wav, FILE* fp )
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

extern "C"
size_t izmwav8_fwrite( IZMWAV8* wav, const char* fpath, const char* mode )
{
    FILE* fp = NULL;
    size_t size = 0;

    fp = fopen( fpath, mode );
    if( fp == NULL )
    {
        return( 1 );
    }

    size = izmwav8_write( wav, fp );

    fclose( fp );

    return( size );
};

extern "C"
size_t izmwav16_fwrite( IZMWAV16* wav, const char* fpath, const char* mode )
{
    FILE* fp = NULL;
    size_t size = 0;

    fp = fopen( fpath, mode );
    if( fp == NULL )
    {
        return( 1 );
    }

    size = izmwav16_write( wav, fp );

    fclose( fp );

    return( size );
};

