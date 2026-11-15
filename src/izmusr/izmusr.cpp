#include <izmusr.hpp>
#include <izm.hpp>

static void _usage( void )
{
    fprintf( stdout, "\n" );
    fprintf( stdout, "    izmusr show-userinfo-userid-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-dispname-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-location-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-url-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-about-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-icon-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-by-userid <userid>\n" );
    fprintf( stdout, "    izmusr show-userinfo-by-username <username>\n" );
    fprintf( stdout, "    izmusr show-userinfo-all\n" );
    fprintf( stdout, "    izmusr auth-by-userid-password <userid> <password>\n" );
    fprintf( stdout, "    izmusr auth-by-username-password <username> <password>\n" );
    fprintf( stdout, "    izmusr auth-by-userid-passwordhash <userid> <passwordhash>\n" );
    fprintf( stdout, "    izmusr auth-by-username-passwordhash <username> <passwordhash>\n" );
    fprintf( stdout, "    izmusr change-password-by-userid <userid> <passowrd-new> <password>\n" );
    fprintf( stdout, "    izmusr change-password-by-username <username> <password-new> <password>\n" );
    fprintf( stdout, "    izmusr change-username-by-userid <userid> <username-new> <password>\n" );
    fprintf( stdout, "    izmusr change-username-by-username <username> <username-new> <password>\n" );
    fprintf( stdout, "    izmusr change-dispname-by-userid <userid> <dispname-new> <password>\n" );
    fprintf( stdout, "    izmusr change-dispname-by-username <username> <dispname-new> <password>\n" );
    fprintf( stdout, "    izmusr change-location-by-userid <userid> <location-new> <password>\n" );
    fprintf( stdout, "    izmusr change-location-by-username <username> <location-new> <password>\n" );
    fprintf( stdout, "    izmusr change-url-by-userid <userid> <url-new> <password>\n" );
    fprintf( stdout, "    izmusr change-url-by-username <username> <url-new> <password>\n" );
    fprintf( stdout, "    izmusr change-about-by-userid <userid> <about-new> <password>\n" );
    fprintf( stdout, "    izmusr change-about-by-username <username> <about-new> <password>\n" );
    fprintf( stdout, "    izmusr change-icon-by-userid <userid> <icon-new> <password>\n" );
    fprintf( stdout, "    izmusr change-icon-by-username <username> <icon-new> <password>\n" );
    fprintf( stdout, "    izmusr get-passwordhash <password>\n" );
    fprintf( stdout, "    \n" );
    fprintf( stdout, "    username, password allows length 1..32 and charset A..Z, a..z, 0..9\n" );
    fprintf( stdout, "\n" );
}

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
    else if( strcmp( subcommand, "show-userinfo-icon-by-username" ) == 0 )
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

        fprintf( stdout, "%s" , user->icon );

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
                "ABOUT[%s] ICON[%s]\n"
                , user->userid , user->username , "*"
                , user->dispname , user->location , user->url
                , user->about, user->icon
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
                "ABOUT[%s] ICON[%s]\n"
                , user->userid , user->username , "*"
                , user->dispname , user->location , user->url
                , user->about, user->icon
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
                    , "*"
                );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "auth-by-userid-password" ) == 0 )
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

        const char* password = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        unsigned char ret = izmusr_auth_by_userid_password( &db, userid, password );

        if( ret == IZM_OK )
        {
            fprintf( stdout, "OK\n" );
            return( IZMUSR_OK );
        }
        else if( ret == IZM_NG )
        {
            fprintf( stdout, "NG\n" );
            return( IZMUSR_NG );
        }

        fprintf( stderr, "error : auth failed\n" );
        return( IZMUSR_ERROR );
    }
    else if( strcmp( subcommand, "auth-by-username-password" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* password = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        unsigned char ret = izmusr_auth_by_username_password( &db, username, password );

        if( ret == IZM_OK )
        {
            fprintf( stdout, "OK\n" );
            return( IZMUSR_OK );
        }
        else if( ret == IZM_NG )
        {
            fprintf( stdout, "NG\n" );
            return( IZMUSR_NG );
        }

        fprintf( stderr, "error : auth failed\n" );
        return( IZMUSR_ERROR );
    }
    else if( strcmp( subcommand, "auth-by-userid-passwordhash" ) == 0 )
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

        const char* passwordhash = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        unsigned char ret = izmusr_auth_by_userid_passwordhash( &db, userid, passwordhash );

        if( ret == IZM_OK )
        {
            fprintf( stdout, "OK\n" );
            return( IZMUSR_OK );
        }
        else if( ret == IZM_NG )
        {
            fprintf( stdout, "NG\n" );
            return( IZMUSR_NG );
        }

        fprintf( stderr, "error : auth failed\n" );
        return( IZMUSR_ERROR );
    }
    else if( strcmp( subcommand, "auth-by-username-passwordhash" ) == 0 )
    {
        if( argc != 4 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* passwordhash = argv[3];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        unsigned char ret = izmusr_auth_by_username_passwordhash( &db, username, passwordhash );

        if( ret == IZM_OK )
        {
            fprintf( stdout, "OK\n" );
            return( IZMUSR_OK );
        }
        else if( ret == IZM_NG )
        {
            fprintf( stdout, "NG\n" );
            return( IZMUSR_NG );
        }

        fprintf( stderr, "error : auth failed\n" );
        return( IZMUSR_ERROR );
    }
    else if( strcmp( subcommand, "change-password-by-userid" ) == 0 )
    {
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* password_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* username_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* dispname_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* location_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* url_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
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
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* about_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
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
    else if( strcmp( subcommand, "change-icon-by-userid" ) == 0 )
    {
        if( argc != 5 )
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

        const char* icon_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_userid_password( &db, userid, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_icon_by_userid( &db, userid, icon_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change icon failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_save_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database save failed\n" );
            return( IZMUSR_ERROR );
        }

        return( IZMUSR_OK );
    }
    else if( strcmp( subcommand, "change-icon-by-username" ) == 0 )
    {
        if( argc != 5 )
        {
            _usage();
            return( IZMUSR_ERROR );
        }

        const char* username = argv[2];
        const char* icon_new = argv[3];
        const char* password = argv[4];

        IZMUSR_USERINFO_DB db = {};
        if( izmusr_load_db( &db, _get_fpath_db() ) != IZM_OK )
        {
            fprintf( stderr, "error : database load failed\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_auth_by_username_password( &db, username, password ) != IZM_OK )
        {
            fprintf( stderr, "error : permission denied\n" );
            return( IZMUSR_ERROR );
        }

        if( izmusr_change_icon_by_username( &db, username, icon_new ) != IZM_OK )
        {
            fprintf( stderr, "error : change icon failed\n" );
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

