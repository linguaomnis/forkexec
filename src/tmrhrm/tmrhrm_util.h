#ifndef HRM_UTIL_H
#define HRM_UTIL_H

#include <stddef.h>
#include <izm.h>

void nop( void );
unsigned int step_to_tick( unsigned int _step, unsigned int _sampling_frequency, unsigned int _bpm, unsigned int _spb, unsigned int _speed );
unsigned char stat16_stat8( short stat );
double tick_to_duratioon( unsigned int sampling_frequency, unsigned int tick );
unsigned int duration_to_tick( unsigned int sampling_frequency, double duration /* sec */ );

#endif
