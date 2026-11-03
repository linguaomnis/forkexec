#ifndef HRM_OSC_H
#define HRM_OSC_H

#include <tmrhrm_seq.h>

#define OSC_TYPE_NULL   (0)
#define OSC_TYPE_NOISE  (1)
#define OSC_TYPE_SIN    (2)
#define OSC_TYPE_TRI    (3)
#define OSC_TYPE_SQUARE (4)

#define OSC_ONOFF_ON    (1)
#define OSC_ONOFF_OFF   (0)
#define OSC_ONOFF_NULL  (2)


typedef struct
{
    unsigned int sampling_frequency;
    unsigned int on_tick;
    unsigned int off_tick;
    unsigned int frequency;
    unsigned char oscid;
    unsigned char type;
    unsigned char volume; /* 0..255 */
    unsigned char onoff; 

    /* amplitude modulation */
    unsigned char am; /* 0..255 */
    double am_frequency; /* [Hz] */
    unsigned int am_delay; /* [msec] */
    unsigned int am_ascend; /* [msec] */

    /* frequency modulation */
    unsigned int fm;

    /* ADSR */
    unsigned int attack; /* [msec] */
    unsigned int decay; /* [msec] */
    unsigned char sustain; /* 0..255 */
    unsigned int release; /* [msec] */
    
    /* square */
    unsigned char square; /* 0..255 */

} OSC;



void osc_init( OSC* _p, unsigned int _sampling_frequency, unsigned char _type, unsigned char _oscid );
void osc_event( OSC* _p, EVENT* _event );
double osc_volume_rate( unsigned char _volume );
double osc_adsr_rate( unsigned int _sampling_frequency, unsigned char _onoff, unsigned int _on_tick, unsigned int _off_tick, unsigned int _attack, unsigned int _decay, unsigned char _sustain, unsigned int _release );
double osc_am_rate( unsigned int _sampling_frequency, unsigned char _onoff, unsigned int _on_tick, unsigned int _off_tick, unsigned char _am, double _am_frequency, unsigned int _am_delay, unsigned int _am_ascend );
void osc_next( OSC* _p );
short osc_stat( OSC* _p );



#endif
