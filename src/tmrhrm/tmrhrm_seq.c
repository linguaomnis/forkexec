#include <tmrhrm_osc.h>
#include <tmrhrm_seq.h>
#include <tmrhrm_wav.h>
#include <tmrhrm_util.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

/* note     scale   frequency */
/* "0A"      0        27.5    */
/* "1A"     12        55.0    */
/* "2A"     24       110.0    */
/* "3A"     36       220.0    */
/* "4A"     48       440.0    */
/* "5A"     60       880.0    */
/* "6A"     72      1760.0    */
/* "7A"     84      3520.0    */
unsigned int song_note_to_frequency( char* note )
{
    return(
        song_scale_to_frequency(
            song_note_to_scale( note )
        )
    );
};

unsigned int song_note_to_scale( char* note )
{
    unsigned int scale = 0;


    if( note[0] == '0' )
        scale = 12 * 0;
    else if( note[0] == '1' )
        scale = 12 * 1;
    else if( note[0] == '2' )
        scale = 12 * 2;
    else if( note[0] == '3' )
        scale = 12 * 3;
    else if( note[0] == '4' )
        scale = 12 * 4;
    else if( note[0] == '5' )
        scale = 12 * 5;
    else if( note[0] == '6' )
        scale = 12 * 6;
    else if( note[0] == '7' )
        scale = 12 * 7;
    else if( note[0] == '8' )
        scale = 12 * 8;
    else if( note[0] == '9' )
        scale = 12 * 9;
    else
        scale = 0;

    if( note[1] == 'A' )
        scale += 0;
    else if( note[1] == 'b' )
        scale += 1;
    else if( note[1] == 'B' )
        scale += 2;
    else if( note[1] == 'C' )
        scale += 3 - 12;
    else if( note[1] == 'd' )
        scale += 4 - 12;
    else if( note[1] == 'D' )
        scale += 5 - 12;
    else if( note[1] == 'e' )
        scale += 6 - 12;
    else if( note[1] == 'E' )
        scale += 7 - 12;
    else if( note[1] == 'F' )
        scale += 8 - 12;
    else if( note[1] == 'g' )
        scale += 9 - 12;
    else if( note[1] == 'G' )
        scale += 10 - 12;
    else if( note[1] == 'a' )
        scale += 11 - 12;
    else
        scale += 0;

    return( scale ); 
};

unsigned int song_scale_to_frequency( unsigned int scale )
{
    return(
            scale == 0 ?
                0 :
                27.5 * pow( 2.0, ( 0.0 + scale / 12.0 ) )
        );
};

unsigned char song_init( SONG* _p, const char* _fpath )
{
    FILE* fp = NULL;
    char line[STR_MAX] = {};
    size_t linenum = 0;
    char token[8][STR_MAX] = {};
    _p->sampling_frequency = 44100;
    unsigned int bpm = 120;
    unsigned int spb = 16;
    unsigned int speed = 1;
    unsigned int bpm_prev = 120;
    unsigned int spb_prev = 16;
    unsigned int speed_prev = 1;
    _p->data_length = 0;
    unsigned int step = 0;
    unsigned int tick = 0;

    unsigned int step_prevtempochaned = 0;
    unsigned int tick_prevtempochaned = 0;

    vector_init( &(_p->osclist), sizeof(OSC) );
    vector_init( &(_p->evnlist), sizeof(EVENT) );
    if( _p->wav != NULL )
    {
        free( _p->wav );
    }

    if( ( fp = fopen( _fpath, "r" ) ) == NULL )
    {
        return( SONG_NG );
    }

    while( fgets( line, sizeof(line), fp ) )
    {
        linenum++;
        str_trim( line );

        memset( token, 0x0, sizeof(token) );
        int tokennum = sscanf( line, "%s %s %s %s %s %s %s %s\n"
                        , token[0]
                        , token[1]
                        , token[2]
                        , token[3]
                        , token[4]
                        , token[5]
                        , token[6]
                        , token[7]
                    );

        if( tokennum <= 0 || tokennum == EOF )
        {
            continue;
        }

        // fprintf( stdout, "DEBUG : linenum[%d] tokennum[%d] : ", linenum, tokennum );
        // for( size_t i = 0; i < 8; i++ )
        // {
        //     fprintf( stdout, "[%s]", token[i] == NULL ? "(null)" : token[i] );
        // }
        // fprintf( stdout, "\n" );

        if( strcmp( token[0], "_SAMPLING_FREQUENCY" ) == 0 )
        {
            if( tokennum != 2 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            _p->sampling_frequency = atoi( token[1] );

            // fprintf( stdout, "DEBUG : sampling_frequency[%d]\n", _p->sampling_frequency );
        }
        else if( strcmp( token[0], "_BPM" ) == 0 )
        {
            if( tokennum != 2 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            bpm_prev = bpm;
            bpm = atoi( token[1] );

            // fprintf( stdout, "DEBUG : bpm[%d]\n", bpm );
        }
        else if( strcmp( token[0], "_SPB" ) == 0 )
        {
            if( tokennum != 2 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            spb_prev = spb;
            spb = atoi( token[1] );

            // fprintf( stdout, "DEBUG : spb[%d]\n", spb );
        }
        else if( strcmp( token[0], "_SPEED" ) == 0 )
        {
            if( tokennum != 2 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            speed_prev = speed;
            speed = atoi( token[1] );

            // fprintf( stdout, "DEBUG : speed[%d]\n", _p->speed );
        }
        else if( strcmp( token[0], "OSC" ) == 0 )
        {
            if( tokennum != 3 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            unsigned char oscid = atoi( token[1] );

            unsigned char type = OSC_TYPE_NULL;
            if( strcmp( token[2], "NULL" ) == 0 ){
                type = OSC_TYPE_NULL;
            }else if( strcmp( token[2], "NOISE" ) == 0 ){
                type = OSC_TYPE_NOISE;
            }else if( strcmp( token[2], "SIN" ) == 0 ){
                type = OSC_TYPE_SIN;
            }else if( strcmp( token[2], "TRI" ) == 0 ){
                type = OSC_TYPE_TRI;
            }else if( strcmp( token[2], "SQUARE" ) == 0 ){
                type = OSC_TYPE_SQUARE;
            }else{
                fprintf( stderr, "error : %s:%d : unknown oscillator type : %s\n",
                            _fpath, linenum, token[1] );
                fclose( fp );
                return( SONG_NG );
            }

            EVENT_SET_OSC event = {
                { tick, 0, EVENT_TYPE_SET_OSC },
                oscid, type,
            };
            vector_insert( &_p->evnlist, _p->evnlist.length, &event );
            // fprintf( stdout, "osc : type[%d]\n", type );
        }
        else if( strcmp( token[0], "VOLUME" ) == 0 )
        {
            if( tokennum != 3 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            unsigned int oscid = atoi( token[1] );
            unsigned int volume = atoi( token[2] );

            EVENT_SET_VOLUME event = {
                { tick, oscid, EVENT_TYPE_SET_VOLUME },
                volume
            };
            vector_insert( &_p->evnlist, _p->evnlist.length, &event );
            // fprintf( stdout, "volume : oscid[%d] volume[%d]\n", oscid, volume );
        }
        else if( strcmp( token[0], "ADSR" ) == 0 )
        {
            if( tokennum != 6 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            unsigned int    oscid       = atoi( token[1] );

            unsigned int    attack      = 0;
            unsigned int    decay       = 0;
            unsigned char   sustain     = 0;
            unsigned int    release     = 0;

            unsigned char   attack_set  = 0;
            unsigned char   decay_set   = 0;
            unsigned char   sustain_set = 0;
            unsigned char   release_set = 0;

            if( strcmp( token[2], "-" ) != 0 )
            {
                attack = atoi( token[2] );
                attack_set = 1;
            }
            if( strcmp( token[3], "-" ) != 0 )
            {
                decay = atoi( token[3] );
                decay_set = 1;
            }
            if( strcmp( token[4], "-" ) != 0 )
            {
                sustain = atoi( token[4] );
                sustain_set = 1;
            }
            if( strcmp( token[5], "-" ) != 0 )
            {
                release = atoi( token[5] );
                release_set = 1;
            }

            EVENT_SET_ADSR event = {
                { tick, oscid, EVENT_TYPE_SET_ADSR },
                attack, decay, release, sustain,
                attack_set, decay_set, release_set, sustain_set,
            };
            vector_insert( &_p->evnlist, _p->evnlist.length, &event );
            // fprintf( stdout, "adsr : oscid[%d] attack[%d] decay[%d] sustain[%d] release[%d]\n", oscid, attack, decay, sustain, release );
        }
        else if( strcmp( token[0], "AM" ) == 0 )
        {
            if( tokennum != 6 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            unsigned int    oscid       = atoi( token[1] );

            unsigned char   am                  = 0; /* 0..255 */
            double          am_frequency        = 0; /* Hz */
            unsigned int    am_delay            = 0; /* msec */
            unsigned int    am_ascend           = 0; /* msec */

            unsigned char   am_set              = 0;
            unsigned char   am_frequency_set    = 0;
            unsigned char   am_delay_set        = 0;
            unsigned char   am_ascend_set       = 0;

            if( strcmp( token[2], "-" ) != 0 )
            {
                am = atoi( token[2] );
                am_set = 1;
            }
            if( strcmp( token[3], "-" ) != 0 )
            {
                am_frequency = atof( token[3] );
                am_frequency_set = 1;
            }
            if( strcmp( token[4], "-" ) != 0 )
            {
                am_delay = atoi( token[4] );
                am_delay_set = 1;
            }
            if( strcmp( token[5], "-" ) != 0 )
            {
                am_ascend = atoi( token[5] );
                am_ascend_set = 1;
            }

            EVENT_SET_AM event = {
                { tick, oscid, EVENT_TYPE_SET_AM },
                am,
                {},
                am_frequency,
                am_delay,
                am_ascend,
                am_set,
                am_frequency_set,
                am_delay_set,
                am_ascend_set,
            };
            vector_insert( &_p->evnlist, _p->evnlist.length, &event );
            // fprintf( stdout, "am : oscid[%u] am[%u] am_frequency[%f] am_delay[%u] am_ascend[%u]\n", oscid, am, am_frequency, am_delay, am_ascend );
        }
        else if( strcmp( token[0], "FM" ) == 0 )
        {
            if( tokennum != 3 )
            {
                fprintf( stderr, "error : %s:%d : invalid argnum\n",
                            _fpath, linenum );
                fclose( fp );
                return( SONG_NG );
            }

            unsigned int    oscid       = atoi( token[1] );

            unsigned int    fm                  = 0;
            unsigned char   fm_set              = 0;

            if( strcmp( token[2], "-" ) != 0 )
            {
                fm = atoi( token[2] );
                fm_set = 1;
            }

            EVENT_SET_FM event = {
                { tick, oscid, EVENT_TYPE_SET_FM },
                fm,
                fm_set
            };
            vector_insert( &_p->evnlist, _p->evnlist.length, &event );
            // fprintf( stdout, "fm : oscid[%u] fm[%u] fm_frequency[%f] fm_delay[%u] fm_ascend[%u]\n", oscid, fm, fm_frequency, fm_delay, fm_ascend );
        }
        else if( token[0][0] == '>' )
        {
            speed_prev = speed;
#if 0
            for( size_t i = 1; token[0][i] != '\0'; i++ )
            {
                if( token[0][i] != '>' )
                {
                    fprintf( stderr, "error : %s:%d : invalid speed indicator : %s\n",
                                _fpath, linenum, token[i] );
                    fclose( fp );
                    return( SONG_NG );
                }

                speed = speed * 2;
            }
#else
            if( strcmp( token[0], ">" ) == 0 )
                speed = 1;
            else if( strcmp( token[0], ">>" ) == 0 )
                speed = 2;
            else if( strcmp( token[0], ">>>" ) == 0 )
                speed = 4;
            else if( strcmp( token[0], ">>>>" ) == 0 )
                speed = 8;
            else
                speed = 1;
#endif

            for( size_t i = 1; i < tokennum; i++ )
            {
                unsigned int oscid = i - 1;

                if( token[i][0] == '-' && token[i][1] == '\0' )
                {
                    /* NOP */
                }
                else if( token[i][0] == '0' && token[i][1] == '\0' )
                {
                    /* note off */
                    const unsigned int onoff = OSC_ONOFF_OFF;

                    EVENT_SET_ONOFF event = {
                        { tick, oscid, EVENT_TYPE_SET_ONOFF },
                        onoff
                    };

                    vector_insert( &_p->evnlist, _p->evnlist.length, &event );
                    // fprintf( stdout, "onoff : step[%d] tick[%d] oscid[%d] onoff[%d]\n"
                    //                 , step 
                    //                 , tick
                    //                 , oscid
                    //                 , onoff 
                    //         );
                }
                else if( isdigit(token[i][0]) && isalpha(token[i][1]) && token[i][2] == '\0' )
                {
                    /* note on */
                    const unsigned int onoff = OSC_ONOFF_ON;
                    const unsigned int frequency = song_note_to_frequency( token[i] );

                    if( frequency == 0 )
                    {
                        fprintf( stderr, "error : %s:%d : invalid note : %s\n",
                                    _fpath, linenum, token[i] );
                        fclose( fp );
                        return( SONG_NG );
                    }

                    EVENT_SET_FREQUENCY event_frequency = {
                        { tick, oscid, EVENT_TYPE_SET_FREQUENCY },
                        frequency
                    };

                    vector_insert( &_p->evnlist, _p->evnlist.length, &event_frequency );

                    EVENT_SET_ONOFF event_onoff = {
                        { tick, oscid, EVENT_TYPE_SET_ONOFF },
                        onoff 
                    };

                    vector_insert( &_p->evnlist, _p->evnlist.length, &event_onoff );

                    // fprintf( stdout, "onoff : step[%d] tick[%d] oscid[%d] note[%s] frequency[%d] onoff[%d]\n"
                    //                 , step 
                    //                 , tick
                    //                 , oscid
                    //                 , token[i]
                    //                 , frequency
                    //                 , onoff 
                    //         );
                }
            }

            step = step + 1;
            tick = tick_prevtempochaned + step_to_tick(
                            step - step_prevtempochaned,
                            _p->sampling_frequency,
                            bpm,
                            spb,
                            speed
                        );
        }
        else
        {
            fprintf( stderr, "error : %s:%d : unknown control : %s\n",
                        _fpath, linenum, token[0] );
            fclose( fp );
            return( SONG_NG );
        }

        if( bpm_prev != bpm
         || spb_prev != spb
         || speed_prev != speed )
        {
            step_prevtempochaned = step;
            tick_prevtempochaned = tick;
        }
    }

    fclose( fp );

    _p->data_length = tick;

    fprintf( stdout, "data_length[%d]\n", _p->data_length );

    _p->wav = wav16_new( _p->data_length, _p->sampling_frequency );

    return( SONG_OK );
};

void song_event( SONG* _p, EVENT* _event )
{
    if( _event->head.type == EVENT_TYPE_SET_OSC )
    {
        EVENT_SET_OSC* event = (EVENT_SET_OSC*)_event;

        /* osclist に領域がない場合、増やす */
        for( size_t i = 0; event->oscid >= _p->osclist.length; i++ )
        {
            OSC osc_null;

            osc_init(
                    &osc_null,
                    _p->sampling_frequency,
                    OSC_TYPE_NULL,
                    _p->osclist.length
                );

            vector_insert( &_p->osclist, _p->osclist.length, &osc_null );
        }

        osc_init(
                vector_at( &(_p->osclist), event->oscid ),
                _p->sampling_frequency,
                event->type,
                event->oscid
            );

        fprintf( stdout, "song_event : type[%u](SET_OSC) oscid[%u] type[%u]\n"
                        , event->head.type, event->oscid, event->type
            );
    }

    return; 
};

unsigned char song_play( SONG* _p )
{
    unsigned int event_index = 0;

    if( _p->wav == NULL )
    {
        return( SONG_NG );   
    }

    for( unsigned int tick = 0; tick < _p->data_length; tick++ )
    {
        EVENT* event = (EVENT*)vector_at( &_p->evnlist, event_index );
        if( event->head.tick <= tick )
        {
            for( ; event_index < _p->evnlist.length; event_index++ )
            {
                event = (EVENT*)vector_at( &_p->evnlist, event_index );

                if( tick < event->head.tick )
                {
                    break;
                }

                if( EVENT_TYPE_SONG_MIN <= event->head.type
                 && event->head.type <= EVENT_TYPE_SONG_MAX )
                {
                    song_event( _p, event );
                }
                else if( EVENT_TYPE_OSC_MIN <= event->head.type
                 && event->head.type <= EVENT_TYPE_OSC_MAX )
                {
                    OSC* osc = (OSC*)vector_at( &_p->osclist, event->head.oscid );
                    osc_event( osc, event );
                }
                else
                {
                    fprintf( stderr, "error : invalid event type range : %u\n"
                            , event->head.type );
                    return( SONG_NG );
                }

                nop();
            }
        }

        _p->wav->data[tick] = 0;
        for( size_t i = 0; i < _p->osclist.length; i++ )
        {
            OSC* osc = (OSC*)vector_at( &_p->osclist, i );
            _p->wav->data[tick] += osc_stat( osc );
            osc_next( osc );
        }
    }
};

size_t song_export( SONG* _p, const char* _fpath )
{
    size_t size = 0;

    if( _p->wav == NULL )
    {
        return( 1 );
    }

    size = wav16_fwrite( _p->wav, _fpath, "w" );

    return( size );
};

