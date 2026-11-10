#include <izm.hpp>

extern "C"
unsigned char izmvector_isinit( const IZMVECTOR* _vp )
{
    if( _vp == NULL )
        return( IZM_ERROR );

    if( _vp->unitsize == 0 )
        return( IZM_FALSE );

    if( _vp->p == NULL )
        return( IZM_FALSE );

    return( IZM_OK );
};

extern "C"
void izmvector_init( IZMVECTOR* _vp, const size_t _unitsize )
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

extern "C"
void izmvector_clear( IZMVECTOR* _vp )
{
    if( _vp == NULL )
    {
        return;
    }

    if( _vp->p != NULL
     && 0 < _vp->unitsize
     && 0 < _vp->alloclength
    ) {
        memset(
                _vp->p
                , 0x0
                , _vp->unitsize * _vp->alloclength
            );
    }

    _vp->length = 0;

    return;
};

extern "C"
void* izmvector_at( const IZMVECTOR* _vp, const size_t _index )
{
    if( _vp == NULL )
        return( NULL );

    if( _vp->unitsize == 0 )
        return( NULL );

    return(_vp->p + (_vp->unitsize * _index));
};

extern "C"
unsigned char izmvector_insert( IZMVECTOR* _vp, const size_t _index, const void* _data )
{
    if( _vp == NULL )
        return( IZM_NG );

    if( _vp->unitsize == 0 )
        return( IZM_NG );

    /* _vp->length == _index is OK (insert on back) */
    if( _index < 0 || _vp->length < _index )
        return( IZM_NG );

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
            return( IZM_NG );

        _vp->alloclength = alloclength_target; 
        _vp->p = p;
    }

    /* shift */
    if( 0 < _vp->length )
    {
        for( size_t i = _vp->length - 1; _index < i; i-- )
        {
            memcpy(
                izmvector_at( _vp, i ),
                izmvector_at( _vp, i - 1 ),
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

    return( IZM_OK );
};

extern "C"
unsigned char izmvector_remove( IZMVECTOR* _vp, const size_t _index )
{
    if( _vp == NULL )
        return( IZM_NG );

    if( _vp->unitsize == 0 )
        return( IZM_NG );

    if( _index < 0 || _vp->length <= _index )
        return( IZM_NG );

    /* shift */
    for( size_t i = _index; i < _vp->length - 1; i++ )
    {
        memcpy(
            izmvector_at( _vp, i ),
            izmvector_at( _vp, i + 1 ),
            _vp->unitsize
        );
    }

    _vp->length--;

    return( IZM_OK );
};

