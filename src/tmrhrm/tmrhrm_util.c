#include <tmrhrm_util.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

void nop( void )
{
    return;
};

/* fmod は x が0を境に負と正で対称になっており、周期関数の基礎とするには不適。 */
/* x が負〜正で周期の連続性を保つ fmod2 を用意した。 */
double fmod2( double x, double y )
{
    return( fmod( remainder( x + ( y / 2 ), y ) + ( y / 2 ), y ) );
};

char* str_trim( char* _str )
{
    for( size_t i = 0; _str[i] != '\0'; i++ )
    {
        if( _str[i] == '#' || _str[i] == '\n' )
        {
            _str[i] = '\0';
            break;
        }
    }

    return( _str );
};

unsigned int step_to_tick(
    unsigned int _step,
    unsigned int _sampling_frequency,
    unsigned int _bpm,
    unsigned int _spb,
    unsigned int _speed
) {
    return( ( _sampling_frequency * _step * 60 ) / ( _bpm * _spb * _speed ) );
};

unsigned char stat16_stat8( short stat )
{
    return( ( stat * ( (0.0 + UCHAR_MAX)/(0.0 + USHRT_MAX) ) ) + ( 0.0 + UCHAR_MAX / 2.0 ) );
};

double tick_to_duratioon( unsigned int sampling_frequency, unsigned int tick )
{
    return( (0.0 + tick) / (0.0 + sampling_frequency) );
};

unsigned int duration_to_tick( unsigned int sampling_frequency, double duration /* sec */ )
{
    return( duration * sampling_frequency );
};

void vector_init( VECTOR* _vp, size_t _unitsize )
{
    if( _vp == NULL )
    {
        return;
    }

    if( _vp->p != NULL )
    {
        free( _vp->p );
        _vp->p = NULL;
        _vp->alloclength = 0;
    }

    _vp->length = 0;
    _vp->unitsize = _unitsize;

    return;
};

void* vector_at( VECTOR* _vp, size_t _index )
{
    if( _vp == NULL )
        return( NULL );

    if( _vp->unitsize == 0 )
        return( NULL );

    return(_vp->p + (_vp->unitsize * _index));
};

unsigned char vector_insert( VECTOR* _vp, size_t _index, void* _data )
{
    if( _vp == NULL )
        return( VECTOR_NG );

    if( _vp->unitsize == 0 )
        return( VECTOR_NG );

    /* _vp->length == _index is OK (insert on back) */
    if( _index < 0 || _vp->length < _index )
        return( VECTOR_NG );

    if( _vp->alloclength <= _vp->length /* It's full! */ )
    {
        size_t alloclength_target =
                    _vp->alloclength == 0 ?
                        1 : _vp->alloclength * 2;

        /* allocation failed (memory not changed) */
        void* p = _vp->p;
        if( p == NULL )
        {
            p = malloc( _vp->unitsize * alloclength_target );
        }
        else
        {
            p = realloc( p, _vp->unitsize * alloclength_target ); 
        }

        if( p == NULL )
            return( VECTOR_NG );

        _vp->alloclength = alloclength_target; 
        _vp->p = p;
    }

    /* shift */
    if( 0 < _vp->length )
    {
        for( size_t i = _vp->length - 1; _index < i; i-- )
        {
            memcpy(
                vector_at( _vp, i ),
                vector_at( _vp, i - 1 ),
                _vp->unitsize
            );
        }
    }

    memcpy(
        (_vp->p + (_vp->unitsize * _index)),
        _data,
        _vp->unitsize
    );

    _vp->length++;

    return( VECTOR_OK );
};

unsigned char vector_remove( VECTOR* _vp, size_t _index )
{
    if( _vp == NULL )
        return( VECTOR_NG );

    if( _vp->unitsize == 0 )
        return( VECTOR_NG );

    if( _index < 0 || _vp->length <= _index )
        return( VECTOR_NG );

    /* shift */
    for( size_t i = _index; i < _vp->length - 1; i++ )
    {
        memcpy(
            vector_at( _vp, i ),
            vector_at( _vp, i + 1 ),
            _vp->unitsize
        );
    }

    _vp->length--;

    return( VECTOR_OK );
};

