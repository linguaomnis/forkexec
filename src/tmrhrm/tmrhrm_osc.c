#include <tmrhrm_osc.h>
#include <tmrhrm_util.h>
#include <izm.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

void osc_init(
    OSC* _p,
    unsigned int _sampling_frequency,
    unsigned char _type,
    unsigned char _oscid
) {
    _p->sampling_frequency = _sampling_frequency;
    _p->oscid = _oscid;
    _p->type = _type;
    _p->on_tick = 0;
    _p->off_tick = 0;
    _p->frequency = 440;
    _p->volume = 100;
    _p->onoff = OSC_ONOFF_NULL;

    /* amplitude modulation */
    _p->am = 0;
    _p->am_frequency = 1.0;
    _p->am_delay = 0;
    _p->am_ascend = 0;

    /* frequency modulation */
    _p->fm = 0;

    /* ADSR */
    _p->attack = 0;
    _p->decay = 0;
    _p->sustain = UCHAR_MAX; 
    _p->release = 0;

    /* square */
    _p->square = 127;

    if( _p->type == OSC_TYPE_NOISE )
    {
        srand( (unsigned)time(NULL)*54321 );
    }

    return;
};

void osc_event( OSC* _p, EVENT* _event )
{
    if( _event->head.type == EVENT_TYPE_SET_FREQUENCY )
    {
        EVENT_SET_FREQUENCY* event = (EVENT_SET_FREQUENCY*)_event;
        _p->frequency = event->frequency;
        fprintf( stdout, "osc_event : type[%u](SET_FREQUENCY) oscid[%u] frequency[%u]\n"
                        , event->head.type, _p->oscid, event->frequency
            );
    }
    else if( _event->head.type == EVENT_TYPE_SET_VOLUME )
    {
        EVENT_SET_VOLUME* event = (EVENT_SET_VOLUME*)_event;
        _p->volume = event->volume;
        fprintf( stdout, "osc_event : type[%u](SET_VOLUME) oscid[%u] volume[%u]\n"
                        , event->head.type, _p->oscid, event->volume
            );
    }
    else if( _event->head.type == EVENT_TYPE_SET_ONOFF )
    {
        EVENT_SET_ONOFF* event = (EVENT_SET_ONOFF*)_event;

        if( _p->onoff != event->onoff )
        {
            _p->onoff = event->onoff; 
        }

        if( _p->onoff == OSC_ONOFF_ON )
        {
            _p->off_tick = 0;
            _p->on_tick = 0;
        }
        else if( _p->onoff == OSC_ONOFF_OFF )
        {
            _p->off_tick = 0;
        }

        fprintf( stdout, "osc_event : type[%u](SET_ONOFF) oscid[%u]"
                        " onoff[%u] on_tick[%u] off_tick[%u]"
                        "\n"
                        , event->head.type, _p->oscid
                        , event->onoff, _p->on_tick, _p->off_tick
            );
    }
    else if( _event->head.type == EVENT_TYPE_SET_ADSR )
    {
        EVENT_SET_ADSR* event = (EVENT_SET_ADSR*)_event;
        if( event->attack_set )
            _p->attack = event->attack;
        if( event->decay_set )
            _p->decay = event->decay;
        if( event->sustain_set )
            _p->sustain = event->sustain;
        if( event->release_set )
            _p->release = event->release;

        fprintf( stdout, "osc_event : type[%u](SET_ADSR) oscid[%u]"
                        " attack[%u] attack_set[%u]"
                        " decay[%u] decay_set[%u]"
                        " sustain[%u] sustain_set[%u]"
                        " release[%u] release_set[%u]"
                        "\n"
                        , event->head.type, _p->oscid
                        , event->attack, event->attack_set
                        , event->decay, event->decay_set
                        , event->sustain, event->sustain_set
                        , event->release, event->release_set
            );
    }
    else if( _event->head.type == EVENT_TYPE_SET_AM )
    {
        EVENT_SET_AM* event = (EVENT_SET_AM*)_event;
        if( event->am_set )
            _p->am = event->am;
        if( event->am_frequency_set )
            _p->am_frequency = event->am_frequency;
        if( event->am_delay_set )
            _p->am_delay = event->am_delay;
        if( event->am_ascend_set )
            _p->am_ascend = event->am_ascend;

        fprintf( stdout, "osc_event : type[%u](SET_AM) oscid[%u]"
                        " am[%u] am_set[%u]"
                        " am_frequency[%f]"
                        " am_frequency_set[%u]"
                        " am_delay[%u] am_delay_set[%u]"
                        " am_ascend[%u] am_ascend_set[%u]"
                        "\n"
                        , event->head.type, _p->oscid
                        , event->am, event->am_set
                        , event->am_frequency, event->am_frequency_set
                        , event->am_delay, event->am_delay_set
                        , event->am_ascend, event->am_ascend_set
            );
    }
    else if( _event->head.type == EVENT_TYPE_SET_FM )
    {
        EVENT_SET_FM* event = (EVENT_SET_FM*)_event;
        if( event->fm_set )
            _p->fm = event->fm;

        fprintf( stdout, "osc_event : type[%u](SET_FM) oscid[%u]"
                        " fm[%u] fm_set[%u]"
                        "\n"
                        , event->head.type, _p->oscid
                        , event->fm, event->fm_set
            );
    }
    else
    {
        fprintf( stderr, "warning : unknown event : type[%u]\n", _event->head.type );
    }

    return; 
};

double osc_volume_rate( unsigned char _volume )
{
    return( ( 0.0 + _volume ) / UCHAR_MAX );
};

double osc_adsr_rate(
    const unsigned int _sampling_frequency,
    const unsigned char _onoff,
    const unsigned int _on_tick,
    const unsigned int _off_tick,
    const unsigned int _attack /* msec */,
    const unsigned int _decay /* msec */,
    const unsigned char _sustain /* 0..127 */,
    const unsigned int _release /* msec */
) {
    unsigned int attack_tick = duration_to_tick( _sampling_frequency, (0.0 + _attack) / 1000 );
    unsigned int decay_tick = duration_to_tick( _sampling_frequency, (0.0 + _decay) / 1000 );
    double sustain_rate = (0.0 + _sustain) / (0.0 + UCHAR_MAX);
    unsigned int release_tick = duration_to_tick( _sampling_frequency, (0.0 + _release) / 1000 );

    double rate = 0.0;
    unsigned char* phase = "-";

    if( _onoff == OSC_ONOFF_ON )
    {
        if( _on_tick < attack_tick )
        {
            rate =
                (0.0 + attack_tick) > 0 ?
                    (0.0 + _on_tick) / (0.0 + attack_tick) :
                    1.0
            ;
            phase = "A";
        }
        else if( _on_tick < attack_tick + decay_tick )
        {
            rate =
                decay_tick > 0 ?
                    1.0 - (_on_tick - attack_tick) * (1.0 - sustain_rate) / (0.0 + decay_tick) :
                    sustain_rate
            ;
            phase = "D";
        }
        else
        {
            rate = sustain_rate;
            phase = "S";
        }
    }
    else if( _onoff == OSC_ONOFF_OFF )
    {
        if( _off_tick < release_tick )
        {
            rate =
                release_tick > 0 ?
                    sustain_rate - (0.0 + _off_tick) * sustain_rate / (0.0 + release_tick) :
                    0.0
            ;
            phase = "R";
        }
        else
        {
            rate = 0.0;
            phase = "-";
        }
    }
    else
    {
        rate = 0.0;
        phase = "*";
    }

#if 0
    if( rate < 0.0 || 1.0 < rate )
        fprintf( stderr, "error : invalid rate : %f : _on_tick[%u] _off_tick[%u] _attack[%u] attack_tick[%u] _decay[%u] decay_tick[%u] _sustain[%u] sustain_rate[%f] _release[%u] release_tick[%u]\n"
                , rate, _on_tick, _off_tick
                , _attack, attack_tick
                , _decay, decay_tick
                , _sustain, sustain_rate
                , _release, release_tick
        );

    fprintf( stdout, "onoff[%u] phase[%s] rate[%f] _on_tick[%u] _off_tick[%u] _attack[%u] attack_tick[%u] _decay[%u] decay_tick[%u] _sustain[%u] sustain_rate[%f] _release[%u] release_tick[%u]\n"
            , _onoff, phase, rate, _on_tick, _off_tick
            , _attack, attack_tick
            , _decay, decay_tick
            , _sustain, sustain_rate
            , _release, release_tick
    );
#endif

    return( rate );
};

double osc_am_rate(
    const unsigned int _sampling_frequency,
    const unsigned char _onoff,
    const unsigned int _on_tick,
    const unsigned int _off_tick,
    const unsigned char _am /* 0..127 */,
    const double _am_frequency /* Hz */,
    const unsigned int _am_delay /* msec */,
    const unsigned int _am_ascend /* msec */
) {
    unsigned int am_delay_tick = duration_to_tick( _sampling_frequency, (0.0 + _am_delay) / 1000 );
    unsigned int am_ascend_tick = duration_to_tick( _sampling_frequency, (0.0 + _am_ascend) / 1000 );
    double am_rate = (0.0 + _am) / (0.0 + UCHAR_MAX);

    double rate = 1.0;
    unsigned char* phase = "-";

    if( _onoff == OSC_ONOFF_NULL )
    {
        rate = 0.0;
        phase = "*";
    }
    else
    {
        if( _on_tick < am_delay_tick )
        {
            rate = 1.0;
            phase = "D";
        }
        else if( _on_tick < am_delay_tick + am_ascend_tick )
        {
            rate = ( sin(
                    2.0 * M_PI
                    * _on_tick
                    * _am_frequency
                    / _sampling_frequency
                ) / 2.0 )
                * ((am_rate / (am_ascend_tick)) * (_on_tick - am_delay_tick))
                - ((am_rate / (am_ascend_tick * 2.0)) * (_on_tick - am_delay_tick)) + 1
            ;
            phase = "A";
        }
        else
        {
            rate = ( sin(
                    2.0 * M_PI
                    * _on_tick
                    * _am_frequency
                    / _sampling_frequency
                ) / 2.0 )
                * am_rate 
                + ( 1.0 - ( am_rate / 2.0 ) )
            ;
            phase = "-";
        }
    }

#if 0
    fprintf( stdout, "onoff[%u] phase[%s] rate[%f] _on_tick[%u] _off_tick[%u] _am[%u] am_rate[%f] _am_frequency[%f] _am_delay[%u] am_delay_tick[%u] _am_ascend[%u] am_ascend_tick[%u]\n"
            , _onoff, phase, rate, _on_tick, _off_tick
            , _am, am_rate
            , _am_frequency
            , _am_delay, am_delay_tick
            , _am_ascend, am_ascend_tick
    );
#endif

    return( rate );
};

void osc_next( OSC* _p )
{
    _p->on_tick++;
    _p->off_tick++;
    return;
};

double osc_tri( double x )
{
    double x2 = izmmath_fmod2( x, 2.0 * M_PI );

    if( x2 <= ( M_PI / 2.0 ) )
    {
        double x3 = x2;
        return( ( 2.0 / M_PI ) * x3 );
    }
    else if( x2 <= ( 3.0 * M_PI / 2.0 ) )
    {
        double x3 = x2 - M_PI;
        return( - ( 2.0 / M_PI ) * x3 );
    }
    else
    {
        double x3 = x2 - ( 2.0 * M_PI );
        return( ( 2.0 / M_PI ) * x3 );
    }

    return( 0 );
};

double osc_square( double x, unsigned char rate /* 0..255 */ )
{
    double x2 = izmmath_fmod2( x, 2.0 * M_PI );
    double rate2 = (double)rate / (double)UCHAR_MAX;

    if( x2 <= ( 2.0 * M_PI ) * rate2 )
    {
        return( 1.0 );
    }
    else
    {
        return( -1.0 );
    }

    return( 0 );
};

short osc_stat( OSC* _p )
{
    // fprintf( stdout, "DEBUG : release[%u]\n", _p->release );
    //
    short stat = 0;
    
    double rate =
                osc_volume_rate(
                        _p->volume
                    )
                * osc_adsr_rate(
                        _p->sampling_frequency,
                        _p->onoff,
                        _p->on_tick,
                        _p->off_tick,
                        _p->attack,
                        _p->decay,
                        _p->sustain,
                        _p->release
                    )
                * osc_am_rate(
                        _p->sampling_frequency,
                        _p->onoff,
                        _p->on_tick,
                        _p->off_tick,
                        _p->am,
                        _p->am_frequency,
                        _p->am_delay,
                        _p->am_ascend
                    )
                ;

    if( rate < 0.0 || 1.0 < rate )
    {
        fprintf( stderr, "error : invalid rate : %f\n", rate );
        abort();
    }

    double x =
            (
                ( 2.0 * M_PI * _p->on_tick ) * _p->frequency
                + sin( _p->on_tick ) * _p->fm
            )
            / _p->sampling_frequency;

    if( _p->type == OSC_TYPE_NULL )
    {
        return( 0 );
    }
    else if( _p->type == OSC_TYPE_SIN )
    {
        stat = sin( x ) * SHRT_MAX;
    }
    else if( _p->type == OSC_TYPE_TRI )
    {
        stat = osc_tri( x ) * SHRT_MAX;
    }
    else if( _p->type == OSC_TYPE_SQUARE )
    {
        stat = osc_square( x, _p->square ) * SHRT_MAX;
    }
    else if( _p->type == OSC_TYPE_NOISE )
    {
        stat = ( rand() % USHRT_MAX - SHRT_MAX );
    }

    return( stat * rate );
};



