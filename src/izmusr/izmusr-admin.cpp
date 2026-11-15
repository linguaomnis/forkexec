#include <izmusr.hpp>
#include <izm.hpp>
#include <fcntl.h>

static void _usage( void )
{
    fprintf( stdout, "\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-userid-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-dispname-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-location-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-url-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-about-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-by-userid <userid>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin show-userinfo-all\n" );
    fprintf( stdout, "    izmusr-admin add-user <username>\n" );
    fprintf( stdout, "    izmusr-admin delete-user-by-userid <userid>\n" );
    fprintf( stdout, "    izmusr-admin delete-user-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin reset-password-by-userid <userid>\n" );
    fprintf( stdout, "    izmusr-admin reset-password-by-username <username>\n" );
    fprintf( stdout, "    izmusr-admin change-username-by-userid <userid> <username-new>\n" );
    fprintf( stdout, "    izmusr-admin change-username-by-username <username> <username-new>\n" );
    fprintf( stdout, "    izmusr-admin change-password-by-userid <userid> <password-new>\n" );
    fprintf( stdout, "    izmusr-admin change-password-by-username <username> <password-new>\n" );
    fprintf( stdout, "    izmusr-admin change-location-by-userid <userid> <location-new>\n" );
    fprintf( stdout, "    izmusr-admin change-location-by-username <username> <location-new>\n" );
    fprintf( stdout, "    izmusr-admin change-url-by-userid <userid> <url-new>\n" );
    fprintf( stdout, "    izmusr-admin change-url-by-username <username> <url-new>\n" );
    fprintf( stdout, "    izmusr-admin change-about-by-userid <userid> <about-new>\n" );
    fprintf( stdout, "    izmusr-admin change-about-by-username <username> <about-new>\n" );
    fprintf( stdout, "    izmusr-admin get-passwordhash <password>\n" );
    fprintf( stdout, "    izmusr-admin init-db\n" );
    fprintf( stdout, "    \n" );
    fprintf( stdout, "    username, password allows length 1..32 and charset A..Z, a..z, 0..9\n" );
    fprintf( stdout, "\n" );
};

int main( const int argc, const char** argv )
{
    if( argc < 2 )
    {
        _usage();
        return( IZMUSR_ERROR );
    }

    const char* subcommand = argv[1];

    if( 0 )
    {
        // dummy
    }
    else if( strcmp( subcommand, "show-userinfo-userid-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%d" , user->userid );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-dispname-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%s" , user->dispname );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-location-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%s" , user->location );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-url-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%s" , user->url );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-about-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%s" , user->about );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-by-userid" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_userid( &db, userid );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout,
                "USERID[%d]\n" "USERNAME[%s]\n" "PASSWORDHASH[%s]\n"
                "DISPNAME[%s]\n" "LOCATION[%s]\n" "URL[%s]\n"
                "ABOUT[%s]\n"
                , user->userid , user->username , user->passwordhash
                , user->dispname , user->location , user->url
                , user->about
            );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        const IZMUSR_USERINFO* user = izmusr_search_user_by_username( &db, username );
        if( user == NULL )
        {
            fprintf( stderr, "error : user not found\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout,
                "USERID[%d]\n" "USERNAME[%s]\n" "PASSWORDHASH[%s]\n"
                "DISPNAME[%s]\n" "LOCATION[%s]\n" "URL[%s]\n"
                "ABOUT[%s]\n"
                , user->userid , user->username , user->passwordhash
                , user->dispname , user->location , user->url
                , user->about
            );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "show-userinfo-all" ) == 0 )
    {
        if( argc != 2 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        int i = 0;
        for( i = 0; i < IZMUSR_USER_MAXNUM; i++ )
        {
            if( ! db.user[i].used )
                continue;

            fprintf( stdout, "%4d %-32s %-32s\n"
                    , db.user[i].userid
                    , db.user[i].username
                    , db.user[i].passwordhash
                );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "add-user" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_add_user( &db, username ) != IZM_OK )
        {
            fprintf( stderr, "error : add user failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "delete-user-by-userid" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_delete_user_by_userid( &db, userid ) != IZM_OK )
        {
            fprintf( stderr, "error : delete user failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "delete-user-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_delete_user_by_username( &db, username ) != IZM_OK )
        {
            fprintf( stderr, "error : delete user failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "reset-password-by-userid" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_reset_password_by_userid( &db, userid ) != IZM_OK )
        {
            fprintf( stderr, "error : reset password failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "reset-password-by-username" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_reset_password_by_username( &db, username ) != IZM_OK )
        {
            fprintf( stderr, "error : reset password failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-password-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* password_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_password_by_userid( &db, userid, password_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change password failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-password-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* password_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_password_by_username( &db, username, password_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change password failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-username-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* username_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_username_by_userid( &db, userid, username_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change username failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-username-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* username_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_username_by_username( &db, username, username_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change username failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-dispname-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* dispname_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_dispname_by_userid( &db, userid, dispname_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change dispname failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-dispname-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* dispname_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_dispname_by_username( &db, username, dispname_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change dispname failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-location-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* location_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_location_by_userid( &db, userid, location_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change location failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-location-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* location_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_location_by_username( &db, username, location_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change location failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-url-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* url_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_url_by_userid( &db, userid, url_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change url failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-url-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* url_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_url_by_username( &db, username, url_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change url failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-about-by-userid" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        char* endptr = NULL;
        const int userid = strtol( argv[2], &endptr, 10 );
        if( argv[2][0] != '\0' && *endptr != '\0' )
        {
            fprintf( stderr, "error : invalid userid\n" );
            return( IZMUSR_ERROR );
        }

        const char* about_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_about_by_userid( &db, userid, about_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change about failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-about-by-username" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* about_new = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_about_by_username( &db, username, about_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change about failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "get-passwordhash" ) == 0 )
    {
        if( argc != 3 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* password = argv[2];

        char passwordhash[IZMUSR_PASSHASH_SIZE] = {};

        if( izmusr_get_passwordhash( password, passwordhash ) == NULL )
        {
            fprintf( stderr, "error : passwordhash get failed\n" );
            return( IZMUSR_ERROR );
        }

        fprintf( stdout, "%s", passwordhash );

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "init-db" ) == 0 )
    {
        if( argc != 2 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        IZMUSR_USERINFO_DB db = {};

        if( izmusr_init_db( &db ) != IZM_OK )
        {
            fprintf( stderr, "error : database init failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        chown( _get_fpath_db(), 0, 0 );

        chmod( _get_fpath_db(), S_IWUSR | S_IRUSR );

        return( IZMUSR_OK );
    }
    else
    {
        fprintf( stderr, "error : unknown subcommand : %s\n", subcommand );
        _usage();
        return( IZMUSR_ERROR );
    }

    fprintf( stderr, "error : unknown error\n" );
    _usage();
    return( IZMUSR_ERROR );
}

