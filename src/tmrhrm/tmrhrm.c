#include <tmrhrm_osc.h>
#include <tmrhrm_seq.h>
#include <tmrhrm_util.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

void usage( void )
{
    printf( "tmrhrm <input-fpath> <output-fpath>\n" );
};

int main( const int argc, const char** argv )
{
    if( argc != 3 )
    {
        fprintf( stderr, "error : too much or less arguments\n" );
        usage();
        return( 1 );
    }

    const char* in_fpath = argv[1];
    const char* out_fpath = argv[2];

    SONG song = {};
    song_init( &song, in_fpath );
    song_play( &song );
    song_export( &song, out_fpath );

    return( 0 );
}

