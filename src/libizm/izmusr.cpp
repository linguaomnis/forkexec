#include <izm.hpp>

extern "C"
unsigned char izmusr_clean_db( IZMUSR_USERINFO_DB* _db )
{
    if( _db == NULL )
    {
        return( IZM_ERROR );
    }

    int i = 0;

    /* force set admin */
    strcpy( _db->user[0].username, IZMUSR_USERNAME_ADMIN );
    if( ! _db->user[0].used )
    {
        char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
        if( izmusr_get_passwordhash( IZMUSR_PASSWORD_INIT, passwordhash ) == NULL )
        {
            return( IZM_ERROR );
        }

        memset( &(_db->user[0]), 0x0, sizeof(_db->user[0]) );

        _db->user[0].userid = 0;
        strcpy( _db->user[0].username, IZMUSR_USERNAME_ADMIN );
        strcpy( _db->user[0].passwordhash, passwordhash );
        _db->user[0].used = 1;
    }

    /* cleanup */
    for( i = 1; i < IZMUSR_USER_MAXNUM; i++ )
    {
        if( ! _db->user[i].used )
        {
            memset( &(_db->user[i]), 0x0, sizeof(_db->user[i]) );            
            _db->user[i].userid = i;
        }
    }

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_load_db( IZMUSR_USERINFO_DB* _db, const char* _fpath )
{
    if( _db == NULL )
    {
        return( IZM_ERROR );
    }

    FILE* fp = NULL;
    int i = 0;

    if( ( fp = fopen( _fpath, "r" ) ) == NULL )
    {
        return( IZM_ERROR );
    }

    if( fread( _db, sizeof(IZMUSR_USERINFO_DB), 1, fp ) != 1 )
    {
        fclose( fp );
        return( IZM_ERROR );
    }

    fclose( fp );

    izmusr_clean_db( _db );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_save_db( const IZMUSR_USERINFO_DB* _db, const char* _fpath )
{
    if( _db == NULL )
    {
        return( IZM_ERROR );
    }

    FILE* fp = NULL;
    int i = 0;

    if( ( fp = fopen( _fpath, "w" ) ) == NULL )
    {
        return( IZM_ERROR );
    }

    const size_t size = fwrite( _db, sizeof(IZMUSR_USERINFO_DB), 1, fp );
    if( size != 1 )
    {
        fclose( fp );
        return( IZM_ERROR );
    }

    fclose( fp );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_userid( int _userid )
{
    if( _userid == IZMUSR_USERID_ADMIN )
        return( IZM_OK );

    if( IZMUSR_USERID_MIN <= _userid && _userid <= IZMUSR_USERID_MAX )
        return( IZM_OK );

    return( IZM_ERROR );
};

extern "C"
unsigned char izmusr_check_username( const char* _username )
{
    if( _username == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _username, IZMUSR_USERNAME_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _username ) < IZMUSR_USERNAME_MINLEN || IZMUSR_USERNAME_MAXLEN < strlen( _username ) )
    {
        return( IZM_ERROR );
    }

    int i = 0;

    for( i = 0; i < strlen( _username ); i++ )
    {
        const char c = _username[i];

        if( '0' <= c && c <= '9' )
            continue;

        if( 'A' <= c && c <= 'Z' )
            continue;

        if( 'a' <= c && c <= 'z' )
            continue;

        return( IZM_ERROR );
    }

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_password( const char* _password )
{
    if( _password == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _password, IZMUSR_PASSWORD_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _password ) < IZMUSR_PASSWORD_MINLEN || IZMUSR_PASSWORD_MAXLEN < strlen( _password ) )
        return( IZM_ERROR );

    int i = 0;

    for( i = 0; i < strlen( _password ); i++ )
    {
        const char c = _password[i];

        if( '0' <= c && c <= '9' )
            continue;

        if( 'A' <= c && c <= 'Z' )
            continue;

        if( 'a' <= c && c <= 'z' )
            continue;

        return( IZM_ERROR );
    }

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_passwordhash( const char* _passwordhash )
{
    if( _passwordhash == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _passwordhash, IZMMATH_MD5STR_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _passwordhash ) != IZMMATH_MD5STR_LEN )
        return( IZM_ERROR );

    int i = 0;

    for( i = 0; i < strlen( _passwordhash ); i++ )
    {
        const char c = _passwordhash[i];

        if( '0' <= c && c <= '9' )
            continue;

        if( 'a' <= c && c <= 'f' )
            continue;

        return( IZM_ERROR );
    }

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_dispname( const char* _dispname )
{
    if( _dispname == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _dispname, IZMUSR_DISPNAME_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _dispname ) < IZMUSR_DISPNAME_MINLEN || IZMUSR_DISPNAME_MAXLEN < strlen( _dispname ) )
        return( IZM_ERROR );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_location( const char* _location )
{
    if( _location == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _location, IZMUSR_LOCATION_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _location ) < IZMUSR_LOCATION_MINLEN || IZMUSR_LOCATION_MAXLEN < strlen( _location ) )
        return( IZM_ERROR );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_url( const char* _url )
{
    if( _url == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _url, IZMUSR_URL_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _url ) < IZMUSR_URL_MINLEN || IZMUSR_URL_MAXLEN < strlen( _url ) )
        return( IZM_ERROR );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_check_about( const char* _about )
{
    if( _about == NULL )
        return( IZM_ERROR );

    if( izmstr_check_nullterm( _about, IZMUSR_ABOUT_SIZE ) != IZM_OK )
        return( IZM_ERROR );

    if( strlen( _about ) < IZMUSR_ABOUT_MINLEN || IZMUSR_ABOUT_MAXLEN < strlen( _about ) )
        return( IZM_ERROR );

    return( IZM_OK );
};

extern "C"
IZMUSR_USERINFO* izmusr_find_user_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid )
{
    if( _db == NULL )
        return( NULL );

    if( izmusr_check_userid( _userid ) != IZM_OK )
        return( NULL );

    if( ! _db->user[_userid].used )
        return( NULL );

    return( &(_db->user[_userid]) );
};

extern "C"
const IZMUSR_USERINFO* izmusr_search_user_by_userid( const IZMUSR_USERINFO_DB* _db, const int _userid )
{
    if( _db == NULL )
        return( NULL );

    if( izmusr_check_userid( _userid ) != IZM_OK )
        return( NULL );

    if( ! _db->user[_userid].used )
        return( NULL );

    return( &(_db->user[_userid]) );
};

extern "C"
IZMUSR_USERINFO* izmusr_find_user_by_username( IZMUSR_USERINFO_DB* _db, const char* _username )
{
    if( _db == NULL )
        return( NULL );

    if( izmusr_check_username( _username ) != IZM_OK )
        return( NULL );

    int i = 0;
    for( i = 0; i < IZMUSR_USER_MAXNUM; i++ )
    {
        if( ! _db->user[i].used )
            continue;

        if( strcmp( _db->user[i].username, _username ) == 0 )
            return( &(_db->user[i]) ); 
    }

    return( NULL );
};

extern "C"
const IZMUSR_USERINFO* izmusr_search_user_by_username( const IZMUSR_USERINFO_DB* _db, const char* _username )
{
    if( _db == NULL )
        return( NULL );

    if( izmusr_check_username( _username ) != IZM_OK )
        return( NULL );

    int i = 0;
    for( i = 0; i < IZMUSR_USER_MAXNUM; i++ )
    {
        if( ! _db->user[i].used )
            continue;

        if( strcmp( _db->user[i].username, _username ) == 0 )
            return( &(_db->user[i]) ); 
    }

    return( NULL );
};

extern "C"
unsigned char izmusr_add_user( IZMUSR_USERINFO_DB* _db, const char* _username )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : add_user : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : add_user : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_find_user_by_username( _db, _username ) != NULL )
    {
        fprintf( stderr, "error : add_user : username already exists\n" );
        return( IZM_ERROR );
    }

    int i = 0;
    
    for( i = 0; i < IZMUSR_USER_MAXNUM; i++ )
    {
        if( ! _db->user[i].used )
        {
            char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
            if( izmusr_get_passwordhash( IZMUSR_PASSWORD_INIT, passwordhash ) == NULL )
            {
                return( IZM_ERROR );
            }

            strcpy( _db->user[i].username, _username );
            strcpy( _db->user[i].passwordhash, passwordhash );
            _db->user[i].used = 1;

            return( IZM_OK );
        }
    }

    fprintf( stderr, "error : add_user : no space\n" );
    return( IZM_ERROR );
};

extern "C"
unsigned char izmusr_delete_user_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( _userid == IZMUSR_USERID_ADMIN )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_userid : admin cannot be deleted\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : add_user : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->username, 0x0, sizeof(user->username) );
    memset( user->passwordhash, 0x0, sizeof(user->passwordhash) );
    user->used = 0;

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_delete_user_by_username( IZMUSR_USERINFO_DB* _db, const char* _username )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_username : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( strcmp( _username, IZMUSR_USERNAME_ADMIN ) == 0 )
    {
        fprintf( stderr, "error : izmusr_delete_user_by_username : admin cannot be deleted\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : add_user : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->username, 0x0, sizeof(user->username) );
    memset( user->passwordhash, 0x0, sizeof(user->passwordhash) );
    user->used = 0;

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_reset_password_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid )
{
    return(
            izmusr_change_password_by_userid(
                    _db, _userid, IZMUSR_PASSWORD_INIT
                )
        );
};

extern "C"
unsigned char izmusr_reset_password_by_username( IZMUSR_USERINFO_DB* _db, const char* _username )
{
    return(
            izmusr_change_password_by_username(
                    _db, _username, IZMUSR_PASSWORD_INIT
                )
        );
};

extern "C"
unsigned char izmusr_auth_by_userid_password( const IZMUSR_USERINFO_DB* _db, const int _userid, const char* _password )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_password : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_password : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_password( _password ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_password : _check_password failed : invalid password\n" );
        return( IZM_ERROR );
    }

    const IZMUSR_USERINFO* user = izmusr_search_user_by_userid( _db, _userid );

    if( user == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_password : user not found\n" );
        return( IZM_ERROR );
    }

    char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
    if( izmusr_get_passwordhash( _password, passwordhash ) == NULL )
    {
        return( IZM_ERROR );
    }

    return(
        strcmp( user->passwordhash, passwordhash ) == 0
        ?  IZM_OK : IZM_NG
    );
};

extern "C"
unsigned char izmusr_auth_by_username_password( const IZMUSR_USERINFO_DB* _db, const char* _username, const char* _password )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_password : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_password : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_password( _password ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_password : _check_password failed : invalid password\n" );
        return( IZM_ERROR );
    }

    const IZMUSR_USERINFO* user = izmusr_search_user_by_username( _db, _username );

    if( user == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_password : user not found\n" );
        return( IZM_ERROR );
    }

    char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
    if( izmusr_get_passwordhash( _password, passwordhash ) == NULL )
    {
        return( IZM_ERROR );
    }

    return(
        strcmp( user->passwordhash, passwordhash ) == 0
        ?  IZM_OK : IZM_NG
    );
};

extern "C"
unsigned char izmusr_auth_by_userid_passwordhash( const IZMUSR_USERINFO_DB* _db, const int _userid, const char* _passwordhash )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_passwordhash : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_passwordhash : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_passwordhash( _passwordhash ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_passwordhash : _check_passwordhash failed : invalid passwordhash\n" );
        return( IZM_ERROR );
    }

    const IZMUSR_USERINFO* user = izmusr_search_user_by_userid( _db, _userid );

    if( user == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_userid_passwordhash : user not found\n" );
        return( IZM_ERROR );
    }

    return(
        strcmp( user->passwordhash, _passwordhash ) == 0
        ?  IZM_OK : IZM_NG
    );
};

extern "C"
unsigned char izmusr_auth_by_username_passwordhash( const IZMUSR_USERINFO_DB* _db, const char* _username, const char* _passwordhash )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_passwordhash : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_passwordhash : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_passwordhash( _passwordhash ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_passwordhash : _check_passwordhash failed : invalid passwordhash\n" );
        return( IZM_ERROR );
    }

    const IZMUSR_USERINFO* user = izmusr_search_user_by_username( _db, _username );

    if( user == NULL )
    {
        fprintf( stderr, "error : izmusr_auth_by_username_passwordhash : user not found\n" );
        return( IZM_ERROR );
    }

    return(
        strcmp( user->passwordhash, _passwordhash ) == 0
        ?  IZM_OK : IZM_NG
    );
};

extern "C"
unsigned char izmusr_change_password_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _password_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_password_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_password_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_password( _password_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_password_by_userid : _check_password failed : invalid new password\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_password_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
    if( izmusr_get_passwordhash( _password_new, passwordhash ) == NULL )
    {
        return( IZM_ERROR );
    }

    memset( user->passwordhash, 0x0, sizeof(user->passwordhash) );

    strcpy( user->passwordhash, passwordhash );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_password_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _password_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_password_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_password_by_username : _check_userid failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_password( _password_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_password_by_username : _check_password failed : invalid new password\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_password_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
    if( izmusr_get_passwordhash( _password_new, passwordhash ) == NULL )
    {
        return( IZM_ERROR );
    }

    memset( user->passwordhash, 0x0, sizeof(user->passwordhash) );

    strcpy( user->passwordhash, passwordhash );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_username_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _username_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_username_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_username_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_username_by_userid : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_username_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->username, 0x0, sizeof(user->username) );

    strcpy( user->username, _username_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_username_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _username_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_username_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_username_by_username : _check_username failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_username( _username_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_username_by_username : _check_username failed : invalid new username\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_username_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->username, 0x0, sizeof(user->username) );

    strcpy( user->username, _username_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_dispname_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _dispname_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_dispname( _dispname_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_userid : _check_dispname failed : invalid username\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->dispname, 0x0, sizeof(user->dispname) );

    strcpy( user->dispname, _dispname_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_dispname_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _dispname_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_dispname( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_username : _check_dispname failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_dispname( _dispname_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_username : _check_dispname failed : invalid new dispname\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_dispname_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->dispname, 0x0, sizeof(user->dispname) );

    strcpy( user->dispname, _dispname_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_location_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _location_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_location_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_location_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_location( _location_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_location_by_userid : _check_location failed : invalid new location\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_location_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->location, 0x0, sizeof(user->location) );

    strcpy( user->location, _location_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_location_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _location_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_location_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_location( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_location_by_username : _check_location failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_location( _location_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_location_by_username : _check_location failed : invalid new location\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_location_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->location, 0x0, sizeof(user->location) );

    strcpy( user->location, _location_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_url_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _url_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_url_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_url_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_url( _url_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_url_by_userid : _check_url failed : invalid new url\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_url_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->url, 0x0, sizeof(user->url) );

    strcpy( user->url, _url_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_url_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _url_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_url_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_url( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_url_by_username : _check_url failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_url( _url_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_url_by_username : _check_url failed : invalid new url\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_url_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->url, 0x0, sizeof(user->url) );

    strcpy( user->url, _url_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_about_by_userid( IZMUSR_USERINFO_DB* _db, const int _userid, const char* _about_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_about_by_userid : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_userid( _userid ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_about_by_userid : _check_userid failed : invalid userid\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_about( _about_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_about_by_userid : _check_about failed : invalid new about\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_userid( _db, _userid ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_about_by_userid : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->about, 0x0, sizeof(user->about) );

    strcpy( user->about, _about_new );

    return( IZM_OK );
};

extern "C"
unsigned char izmusr_change_about_by_username( IZMUSR_USERINFO_DB* _db, const char* _username, const char* _about_new )
{
    if( _db == NULL )
    {
        fprintf( stderr, "error : izmusr_change_about_by_username : _db : NULL\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_about( _username ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_about_by_username : _check_about failed : invalid username\n" );
        return( IZM_ERROR );
    }

    if( izmusr_check_about( _about_new ) != IZM_OK )
    {
        fprintf( stderr, "error : izmusr_change_about_by_username : _check_about failed : invalid new about\n" );
        return( IZM_ERROR );
    }

    IZMUSR_USERINFO* user = NULL;

    if( ( user = izmusr_find_user_by_username( _db, _username ) ) == NULL )
    {
        fprintf( stderr, "error : izmusr_change_about_by_username : user not found\n" );
        return( IZM_ERROR );
    }

    memset( user->about, 0x0, sizeof(user->about) );

    strcpy( user->about, _about_new );

    return( IZM_OK );
};

extern "C"
const char* izmusr_get_passwordhash( const char* _password, char* _passwordhash )
{
    static char* passwordhash = NULL;

    passwordhash = izmmath_get_md5( _password, strlen(_password) );

    if( _passwordhash != NULL )
        strcpy( _passwordhash, passwordhash );

    return( passwordhash );
};

extern "C"
unsigned char izmusr_init_db( IZMUSR_USERINFO_DB* _db )
{
    if( _db == NULL )
        return( IZM_ERROR );

    memset( _db, 0x0, sizeof(IZMUSR_USERINFO_DB) );

    if( izmusr_clean_db( _db ) != IZM_OK )
        return( IZM_ERROR );

    char passwordhash[IZMUSR_PASSHASH_SIZE] = {};
    if( izmusr_get_passwordhash( IZMUSR_PASSWORD_INIT, passwordhash ) == NULL )
    {
        return( IZM_ERROR );
    }

    memset( &(_db->user[0]), 0x0, sizeof(_db->user[0]) );

    _db->user[0].userid = 0;
    strcpy( _db->user[0].username, IZMUSR_USERNAME_ADMIN );
    strcpy( _db->user[0].passwordhash, passwordhash );
    _db->user[0].used = 1;

    return( IZM_OK );
};

