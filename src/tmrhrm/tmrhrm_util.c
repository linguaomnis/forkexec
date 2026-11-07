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

