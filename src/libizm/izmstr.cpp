#include <izm.hpp>
#include <string>
#include <regex.h>

extern "C"
char* izmstr_trim( char* _str )
{
    for( size_t i = 0; _str[i] != '\0'; i++ )
    {
        if( _str[i] == '#' || _str[i] == '\n' )
        {
            _str[i] = '\0';
            break;
        }
    }

    return( _str );
};

extern "C"
unsigned char izmstr_shift(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length,
    const int _shiftnum,
    const char _fillchar
) {
    if( _strbuf == NULL )
        return( IZM_ERROR );

    if( 0 < _shiftnum )
    {
        /* 後にシフト */
        int from = 0;
        int to = 0;
        const int from_begin = _offset + _length - 1;
        const int from_end = _offset;
        char c = '\0';

        for( from = from_begin; from_end <= from; from-- )
        {
            to = from + _shiftnum; 

            if( from < 0 || _strbufsize <= from )
                continue;

            c = _strbuf[from];
            _strbuf[from] = _fillchar;

            if( to < 0 || _strbufsize <= to )
                continue;

            _strbuf[to] = c;
        }

        return( IZM_OK );
    }
    else if( _shiftnum < 0 )
    {
        /* 前にシフト */
        int from = 0;
        int to = 0;
        const int from_begin = _offset;
        const int from_end = _offset + _length - 1;
        char c = '\0';

        for( from = from_begin; from <= from_end; from++ )
        {
            to = from + _shiftnum; 

            if( from < 0 || _strbufsize <= from )
                continue;

            c = _strbuf[from];
            _strbuf[from] = _fillchar;

            if( to < 0 || _strbufsize <= to )
                continue;

            _strbuf[to] = c;
        }

        return( IZM_OK );
    }
    else
    {
        /* シフトなし */
    }

    return( IZM_OK );
};

extern "C"
unsigned char izmstr_replace(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length,
    const char* _replace,
    const size_t _replacelength
) {
    if( _strbuf == NULL || _replace == NULL )
        return( IZM_ERROR );

    if( strlen(_strbuf) < _offset + _length )
        return( IZM_ERROR );

    if( strlen(_strbuf) < _offset )
        return( IZM_ERROR );

    if( _length < _replacelength )
    {
        if( _strbufsize < (strlen(_strbuf) + 1) + _replacelength - _length )
            return( IZM_ERROR );
    }

    izmstr_shift(
            _strbuf,
            _strbufsize,
            _offset + _length,
            _strbufsize - (_offset + _length),
            _replacelength - _length,
            ' '
    );

    if( 0 < _replacelength )
        memcpy( &(_strbuf[_offset]), _replace, _replacelength );

    return( IZM_OK );
};

extern "C"
unsigned char izmstr_remove(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length
) {
    return( izmstr_replace( _strbuf, _strbufsize, _offset, _length, "", 0 ) );
};

extern "C"
unsigned char izmstr_remove_head(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _length
) {
    return( izmstr_replace( _strbuf, _strbufsize, 0, _length, "", 0 ) );
};

extern "C"
unsigned char izmstr_remove_back(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _length
) {
    return( izmstr_replace( _strbuf, _strbufsize, strlen(_strbuf) - _length + 1, _length, "", 0 ) );
};

extern "C"
unsigned char izmstr_insert(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const char* _insert,
    const size_t _insertlength
) {
    return( izmstr_replace( _strbuf, _strbufsize, _offset, 0, _insert, _insertlength ) );
};

extern "C"
unsigned char izmstr_insert_head(
    char* _strbuf,
    const size_t _strbufsize,
    const char* _insert,
    const size_t _insertlength
) {
    return( izmstr_replace( _strbuf, _strbufsize, 0, 0, _insert, _insertlength ) );
};

extern "C"
unsigned char izmstr_insert_back(
    char* _strbuf,
    const size_t _strbufsize,
    const char* _insert,
    const size_t _insertlength
) {
    return( izmstr_replace( _strbuf, _strbufsize, strlen(_strbuf), 0, _insert, _insertlength ) );
};

#if 0
extern "C"
unsigned char izmstr_regexmatch(
    const char* _pattern,
    const char* _str,
    IZMSTR_REGEXMATCH* _regexmatch
) {
    regex_t regex = {};
    IZMSTR_REGEXMATCH regexmatch = {};
    int cflag = REG_EXTENDED;

    if( _pattern == NULL || _str == NULL )
    {
        return( IZM_ERROR );
    }
    if( _regexmatch == NULL )
    {
        cflag |= REG_NOSUB;
    }

    regexmatch.stat_comp = regcomp( &regex, _pattern, cflag );

    if( regexmatch.stat_comp != 0 )
    {
        if( _regexmatch != NULL )
            memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );
        return( IZM_ERROR );
    }

    if( IZMSTR_REGEXMATCH_MAXMATCH < regex.re_nsub + 1 )
    {
        regexmatch.stat_comp = REG_ESPACE;
        if( _regexmatch != NULL )
            memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );
        return( IZM_ERROR );
    }

    regmatch_t match[IZMSTR_REGEXMATCH_MAXMATCH] = {};

    regexmatch.stat_exec = regexec( &regex, _str, regex.re_nsub + 1, match, 0 );
    regfree( &regex );

    if( regexmatch.stat_exec != 0 )
    {
        if( regexmatch.stat_exec != REG_NOMATCH )
        {
            if( _regexmatch != NULL )
                memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );

            return( IZM_ERROR );
        }

        if( _regexmatch != NULL )
            memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );
        return( IZM_END );
    }

    size_t i = 0;
    for( i = 0; i < regex.re_nsub + 1; i++ )
    {
        if( match[i].rm_so == -1 )
            continue;

        regexmatch.match[i].offset = match[i].rm_so;
        regexmatch.match[i].length = match[i].rm_eo - match[i].rm_so;

        if( sizeof(regexmatch.match[i].str) <= regexmatch.match[i].length )
        {

            if( _regexmatch != NULL )
                memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );

            return( IZM_ERROR );
        }

        memcpy(
                regexmatch.match[i].str,
                &(_str[regexmatch.match[i].offset]),
                regexmatch.match[i].length
        );
    }

    if( _regexmatch != NULL )
        memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );

    return( IZM_OK );
};


extern "C"
unsigned char izmstr_regexreplace(
    const char* _pattern,
    char* _strbuf,
    const size_t _strbufsize,
    const char* _replace
) {
    size_t i = 0;
    unsigned char ret = 0;
    IZMSTR_REGEXMATCH regexmatch = {};

    if( _pattern == NULL || _strbuf == NULL || _replace == NULL )
        return( IZM_ERROR );

    ret = izmstr_regexmatch( _pattern, _strbuf, &regexmatch );

    if( ret == IZM_ERROR )
    {
        return( IZM_ERROR );
    }
    else if( ret == IZM_END )
    {
        return( IZM_OK );
    }

    ret = izmstr_replace(
                _strbuf,
                _strbufsize,
                regexmatch.match[0].offset,
                regexmatch.match[0].length,
                _replace,
                strlen(_replace)
            );

    for( i = 1; i < IZMSTR_REGEXMATCH_MAXMATCH; i++ )
    {
        if( regexmatch.match[i].length == 0 )
            break;

        char replace_sub[PATH_MAX] = {};
        sprintf( replace_sub, "$%d", i );

        while( 1 )
        {
            const char* p = strstr( _strbuf, replace_sub );
            if( p == NULL )
                break;
             
            size_t offset = p - _strbuf;

            ret = izmstr_replace(
                        _strbuf,
                        _strbufsize,
                        offset,
                        strlen(replace_sub),
                        regexmatch.match[i].str,
                        strlen(regexmatch.match[i].str)
                    );
        }
    }
    
    return( IZM_OK );
};

extern "C"
unsigned char izmstr_regexreplace_global(
    const char* _pattern,
    char* _strbuf,
    const size_t _strbufsize,
    const char* _replace
) {
    size_t i = 0;
    unsigned char ret = 0;
    IZMSTR_REGEXMATCH regexmatch = {};

    if( _pattern == NULL || _strbuf == NULL || _replace == NULL )
        return( IZM_ERROR );

    size_t str_headpos = 0;
    size_t str_endpos = 0;
    while( 1 )
    {
        if( strlen(_strbuf) < str_headpos )
        {
            break;
        }

        ret = izmstr_regexmatch( _pattern, &(_strbuf[str_headpos]), &regexmatch );

        if( ret == IZM_ERROR )
        {
            return( IZM_ERROR );
        }
        else if( ret == IZM_END )
        {
            break;
        }

        ret = izmstr_replace(
                    &(_strbuf[str_headpos]),
                    _strbufsize - str_headpos,
                    regexmatch.match[0].offset,
                    regexmatch.match[0].length,
                    _replace,
                    strlen(_replace)
                );

        str_endpos = str_headpos + regexmatch.match[0].offset + strlen(_replace);

        for( i = 1; i < IZMSTR_REGEXMATCH_MAXMATCH; i++ )
        {
            if( regexmatch.match[i].length == 0 )
                break;

            char replace_sub[PATH_MAX] = {};
            sprintf( replace_sub, "$%d", i );

            while( 1 )
            {
                const char* p = strstr( &(_strbuf[str_headpos]), replace_sub );
                if( p == NULL )
                    break;
                 
                size_t offset = p - &(_strbuf[str_headpos]);

                ret = izmstr_replace(
                            &(_strbuf[str_headpos]),
                            _strbufsize - str_headpos,
                            offset,
                            strlen(replace_sub),
                            regexmatch.match[i].str,
                            strlen(regexmatch.match[i].str)
                        );

                str_endpos = str_endpos - strlen(replace_sub) + strlen(regexmatch.match[i].str);
            }
        }

        str_headpos = str_endpos;
    }
    
    return( IZM_OK );
};

extern "C"
char* izmstr_decode_literal(
    char* _strbuf,
    const size_t _strbufsize
) {
    if( _strbuf == NULL )
        return( NULL );

    /* 単独の '"' を消す */
    izmstr_regexreplace_global(
                "([^\\\\]|^)(\")",
                _strbuf,
                _strbufsize,
                "$1"
        );

    /* '\"' を '"' に変える */
    izmstr_regexreplace_global(
                "(\\\\)(\"|\\\\)",
                _strbuf,
                _strbufsize,
                "$2"
        );

    /* '\\' を '\' に変える */
    izmstr_regexreplace_global(
                "([\\\\])(\"|\\\\)",
                _strbuf,
                _strbufsize,
                "$2"
        );

    return( _strbuf );
};


/* _force : IZM_TRUE  : 空白の有無に関わらず両端に '"' をつける。 */
/*          IZM_FALSE : 空白がなければ両端に '"' をつけない。 */
extern "C"
char* izmstr_encode_literal(
    char* _strbuf,
    const size_t _strbufsize,
    const unsigned char _force
) {
    if( _strbuf == NULL )
        return( NULL );

    /* '\' を '\\' に変える */
    izmstr_regexreplace_global(
                "\\\\",
                _strbuf,
                _strbufsize,
                "\\\\"
        );

    /* '"' を '\"' に変える */
    izmstr_regexreplace_global(
                "\"",
                _strbuf,
                _strbufsize,
                "\\\""
        );

    /* _force フラグが立っている、またはそうでなくとも空白があれば、 */
    /* 両端に '"' をつける */
    if( _force == IZM_TRUE
     || izmstr_regexmatch( "[ \t]", _strbuf, NULL ) == IZM_OK )
    {
        const char* insert = "\"";
        izmstr_insert_head( _strbuf, _strbufsize, insert, strlen(insert) );
        izmstr_insert_back( _strbuf, _strbufsize, insert, strlen(insert) );
    }

    return( _strbuf );
};

extern "C"
unsigned char izmstr_split(
    const char* _str,
    IZMVECTOR* _strlist
) {
    if( _str == NULL || _strlist == NULL )
        return( IZM_ERROR );

    if( izmvector_isinit( _strlist ) != IZM_TRUE )
        return( IZM_ERROR );

    unsigned char ret = IZM_FALSE;
    IZMSTR_REGEXMATCH regexmatch = {};
    size_t str_headpos = 0;
    size_t count = 0;

    izmvector_clear( _strlist );

    while( 1 )
    {
        if( str_headpos < strlen(_str) )
            break;

        const char* search = &(_str[str_headpos]);

        ret = izmstr_regexmatch(
                        "\\S+",
                        search,
                        &regexmatch
                    );

        if( ret == IZM_ERROR )
        {
            return( IZM_ERROR );
        }
        else if( ret == IZM_END )
        {
            break;
        }

        const char* find = &(search[regexmatch.match[0].offset]); 
        const size_t find_length = regexmatch.match[0].length;

        if( _strlist->unitsize <= regexmatch.match[0].length )
        {
            izmvector_clear( _strlist );
            return( IZM_ERROR );
        }

        izmvector_insert(
                    _strlist,
                    count,
                    &(search[regexmatch.match[0].offset])
                );

        ((char*)izmvector_at( _strlist, count ))[regexmatch.match[0].length] = '\0'; 

        str_headpos += regexmatch.match[0].offset + regexmatch.match[0].length;
        count++;
    }

    return( IZM_OK );
};
#endif

extern "C++"
std::string izm::decode_literal( std::string _str )
{
    auto _str_tmp = _str;

    // 単独の " を消す
    while( true )
    {
        _str_tmp = _str;
        _str = std::regex_replace( _str_tmp, std::regex( "([^\\\\]|^)(\")" , std::regex::extended ), "$1" );
        if( _str_tmp == _str ) {
            break;
        }
    }

    // \", \\ を ", \ に変える
    _str = std::regex_replace( _str, std::regex( "(\\\\)(\"|\\\\)"  , std::regex::extended ), "$2" );
    _str = std::regex_replace( _str, std::regex( "([\\\\])(\"|\\\\)"  , std::regex::extended ), "$2" );
    return( _str );
};

// _force : true            : 空白の有無に関わらず両端に " をつける。
//          flase (default) : 空白がなければ両端に " をつけない。
extern "C++"
std::string izm::encode_literal( std::string _str, bool _force )
{
    // \ を \\ に変える
    _str = std::regex_replace( _str, std::regex( "\\\\", std::regex::extended ), "\\\\"   );

    // " を \" に変える
    _str = std::regex_replace( _str, std::regex( "([^\\\\])(\")" , std::regex::extended ), "$1\\$2"   );

    // _forceフラグが立っている、またはそうでなくとも空白があれば、
    // 両端に " をつける
    if( _force || std::regex_search( _str, std::regex( "[ \t]", std::regex::extended ) ) )
    {
        _str = "\"" + _str + "\"";
    }

    return( _str );
};
extern "C++"
std::string izm::encode_literal( std::string _str )
{
    return( izm::encode_literal( _str, false ) );
};

extern "C++"
bool izm::matchcharlist( char _c, std::vector<char> _charlist )
{
    for( auto itr = _charlist.begin(); itr != _charlist.end(); itr++ )
    {
        if( (*itr) == _c ) {
            return( true );
        }
    }
    return( false );
};

extern "C++"
std::vector<std::string>
izm::split( std::string _line, std::vector<char> _delimlist, std::vector<char> _endlist )
{
    std::vector<std::string> tokenlist;
    bool inLiteral = false;
    char prevChar = '\0';
    char thisChar = '\0';
    bool prevContextIsDelim = true;
    bool thisContextIsDelim = true;
    bool isContinue = true;
    bool escape = false;
    size_t pos = _line.length();

    size_t i = 0;
    while( isContinue )
    {
        prevChar = thisChar;
        thisChar = _line[i];

        if( escape ) {
            escape = false;
        } else {
            if( prevChar == '\\' && ( thisChar == '\\' || thisChar == '\"' ) ) {
                escape = true;
            }
        }
        
        prevContextIsDelim = thisContextIsDelim;
        if( (! escape) && thisChar == '"' )
        {
            inLiteral = (! inLiteral);
            thisContextIsDelim = false;
        }
        else if( matchcharlist( thisChar, _endlist ) || _line.length() <= i )
        {
            isContinue = false;
            thisContextIsDelim = true;
        }
        else
        {
            thisContextIsDelim = (! inLiteral)
                                 &&
                                 ( matchcharlist( thisChar, _delimlist ) )
                                 ;
        }

        if( prevContextIsDelim ) {
            if( thisContextIsDelim ) {
                // keep delim-context
            }else{
                // change delim_context -> nondelim-context
                pos = i;
            }
        } else {
            if( thisContextIsDelim ) {
                // change nondelim-context -> delim-context
                if( pos < i ) {
                    tokenlist.push_back( _line.substr( pos, (i - pos) ) );
                }
                pos = _line.length();
            }else{
                // keep nondelim-context
            }
        }

        i++;
    }

    return( tokenlist );
};

extern "C++"
std::vector<std::string>
izm::split( std::string _line, std::vector<char> _delimlist )
{
    return(
        split(
            _line,
            _delimlist,
            std::vector<char>{ '\0', '\n', '#' }
        )
    );
};

extern "C++"
std::vector<std::string>
izm::split( std::string _line )
{
    return(
        split(
            _line,
            std::vector<char>{ ' ', '\t' },
            std::vector<char>{ '\0', '\n', '#' }
        )
    );
};

extern "C++"
std::ostream& izm::err( const std::string& _filepath, const unsigned int& _line_num, const std::string& _line_in )
{
    return( std::cerr << "error : " << _filepath << "(" << _line_num << ")[" << _line_in << "] : " );
};

