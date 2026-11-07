#ifndef HRM_SEQ_H
#define HRM_SEQ_H

#include <tmrhrm_util.h>
#include <izm.h>

#define SONG_OK     (0)
#define SONG_NG     (1)
#define STR_MAX     (1024)

#define EVENT_TYPE_SONG_MIN     (1)
#define EVENT_TYPE_SONG_MAX     (127)
#define EVENT_TYPE_OSC_MIN      (128)
#define EVENT_TYPE_OSC_MAX      (255)

#define EVENT_DATA_MAXSIZE  (128 - sizeof(EVENT_HEAD))
typedef struct {
    unsigned int    tick;
    unsigned int    oscid;
    unsigned int    type;
    unsigned int    dummy0[1];
} EVENT_HEAD;

#define EVENT_TYPE_NULL             (0)
typedef struct {
    EVENT_HEAD      head;
    char            data[EVENT_DATA_MAXSIZE];
} EVENT;

#define EVENT_TYPE_SET_OSC          (EVENT_TYPE_SONG_MIN + 0)
typedef struct {
    EVENT_HEAD      head;
    unsigned char   oscid;
    unsigned char   type;
    unsigned char   dummy0[2]; 
    char            dummy1[EVENT_DATA_MAXSIZE - 4];
} EVENT_SET_OSC;

#define EVENT_TYPE_SET_FREQUENCY    (EVENT_TYPE_OSC_MIN + 0)
typedef struct {
    EVENT_HEAD      head;
    unsigned int    frequency;
    char            dummy0[EVENT_DATA_MAXSIZE - 4];
} EVENT_SET_FREQUENCY;

#define EVENT_TYPE_SET_VOLUME       (EVENT_TYPE_OSC_MIN + 1)
typedef struct {
    EVENT_HEAD      head;
    unsigned char   volume;
    char            dummy0[3];
    char            dummy1[EVENT_DATA_MAXSIZE - 4];
} EVENT_SET_VOLUME;

#define EVENT_TYPE_SET_ONOFF        (EVENT_TYPE_OSC_MIN + 2)
typedef struct {
    EVENT_HEAD      head;
    unsigned char   onoff;
    char            dummy0[3];
    char            dummy1[EVENT_DATA_MAXSIZE - 4];
} EVENT_SET_ONOFF;

#define EVENT_TYPE_SET_ADSR         (EVENT_TYPE_OSC_MIN + 3)
typedef struct {
    EVENT_HEAD      head;
    unsigned int    attack;
    unsigned int    decay;
    unsigned int    release;
    unsigned char   sustain;
    unsigned char   attack_set;
    unsigned char   decay_set;
    unsigned char   release_set;
    unsigned char   sustain_set;
    char            dummy0[3];
    char            dummy1[EVENT_DATA_MAXSIZE - 20];
} EVENT_SET_ADSR;

#define EVENT_TYPE_SET_AM           (EVENT_TYPE_OSC_MIN + 4)
typedef struct {
    EVENT_HEAD      head;
    unsigned char   am; /* 0..127 */
    char            dummy0[7];
    double          am_frequency; /* Hz */
    unsigned int    am_delay; /* msec */
    unsigned int    am_ascend; /* msec */
    unsigned char   am_set;
    unsigned char   am_frequency_set;
    unsigned char   am_delay_set;
    unsigned char   am_ascend_set;
    unsigned char   dummy1[4];
    char            dummy2[EVENT_DATA_MAXSIZE - 32];
} EVENT_SET_AM;

#define EVENT_TYPE_SET_FM           (EVENT_TYPE_OSC_MIN + 5)
typedef struct {
    EVENT_HEAD      head;
    unsigned int    fm;
    unsigned char   fm_set;
    char            dummy0[3];
    char            dummy1[EVENT_DATA_MAXSIZE - 8];
} EVENT_SET_FM;


unsigned int song_note_to_frequency( char* note );
unsigned int song_note_to_scale( char* note );
unsigned int song_scale_to_frequency( unsigned int scale );

typedef struct
{
    unsigned int sampling_frequency; /* [Hz] */
    unsigned int data_length;
    IZMVECTOR osclist;
    IZMVECTOR evnlist;
    IZMWAV16* wav;
} SONG;

unsigned char song_init( SONG* _p, const char* _fpath );
void song_event( SONG* _p, EVENT* _event );
unsigned char song_play( SONG* _p );
size_t song_export( SONG* _p, const char* _fpath );

#endif
