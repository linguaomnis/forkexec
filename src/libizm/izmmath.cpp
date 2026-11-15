#include <izm.hpp>

/* fmod は x が0を境に負と正で対称になっており、周期関数の基礎とするには不適。 */
/* x が負〜正で周期の連続性を保つ fmod2 を用意した。 */
extern "C"
double izmmath_fmod2( double x, double y )
{
    return( fmod( remainder( x + ( y / 2 ), y ) + ( y / 2 ), y ) );
};

extern "C"
char* izmmath_get_md5( const char* _data, size_t _size )
{
    MD5_CTX ctx = {};
    unsigned char md5[MD5_DIGEST_LENGTH] = {};
    static char md5str[IZMMATH_MD5STR_SIZE] = {};
    int i = 0;

    memset( md5str, 0x0, sizeof(md5str) );
    
    if( ! MD5_Init( &ctx ) )
    {
        return( NULL );
    }
    
    if( ! MD5_Update( &ctx, _data, _size ) )
    {
        return( NULL );
    }
    
    if( ! MD5_Final( md5, &ctx ) )
    {
        return( NULL );
    }

    for( i = 0; i < MD5_DIGEST_LENGTH; i++ )
    {
        sprintf( &(md5str[i*2]), "%02x", md5[i] );
    }
 
    return( md5str );
};
