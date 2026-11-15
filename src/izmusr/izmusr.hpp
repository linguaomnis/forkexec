#ifndef IZMUSR_HPP
#define IZMUSR_HPP

#include <izm.hpp>

#define IZMUSR_OK       (0)
#define IZMUSR_NG       (1)
#define IZMUSR_ERROR    (2)
#define IZMUSR_DB_FNAME "izmusr.userinfo"

const char* _get_fpath_db( void )
{
    static int init = 0;
    static char fpath_db[PATH_MAX] = {};

    if( ! init )
    {
        const char* forkexec = getenv( "FORKEXEC" );

        sprintf( fpath_db, "%s/var/lib/%s", forkexec ? forkexec : "", IZMUSR_DB_FNAME );
        init = 1;
    }

    return( fpath_db );
};

#endif
