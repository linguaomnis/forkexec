#ifndef HRM_UTIL_H
#define HRM_UTIL_H

#include <stddef.h>

void nop( void );
double fmod2( double x, double y );
char* str_trim( char* _str );
unsigned int step_to_tick( unsigned int _step, unsigned int _sampling_frequency, unsigned int _bpm, unsigned int _spb, unsigned int _speed );
unsigned char stat16_stat8( short stat );
double tick_to_duratioon( unsigned int sampling_frequency, unsigned int tick );
unsigned int duration_to_tick( unsigned int sampling_frequency, double duration /* sec */ );

#define VECTOR_OK       (0)
#define VECTOR_NG       (1)
#define VECTOR_ERROR    (2)

typedef struct {
    size_t  length;
    size_t  alloclength;
    size_t  unitsize;
    void*   p;
} VECTOR;

void vector_init( VECTOR* _vp, size_t _unitsize );
void* vector_at( VECTOR* _vp, size_t _index );
unsigned char vector_insert( VECTOR* _vp, size_t _index, void* _data );
unsigned char vector_remove( VECTOR* _vp, size_t _index );

#endif
