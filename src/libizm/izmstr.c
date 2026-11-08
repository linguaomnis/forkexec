#include <izm.h>
#include <regex.h>

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

unsigned char izmstr_replace(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length,
    const const char* _replace,
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

#if 1 
    izmstr_shift(
            _strbuf,
            _strbufsize,
            _offset + _length,
            _strbufsize - (_offset + _length),
            _replacelength - _length,
            ' '
    );
#else
    char* shift_strbuf = _strbuf;
    const size_t shift_strbufsize = _strbufsize;
    const size_t shift_offset = _offset + _length;
    const size_t shift_length = _strbufsize - (_offset + _length);
    const int shift_shiftnum = _replacelength - _length;
    const char shift_fillchar = '\0';

    izmstr_shift(
            shift_strbuf,
            shift_strbufsize,
            shift_offset,
            shift_length,
            shift_shiftnum,
            shift_fillchar
    );
#endif

    if( 0 < _replacelength )
        memcpy( &(_strbuf[_offset]), _replace, _replacelength );

    return( IZM_OK );
};

unsigned char izmstr_remove(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const size_t _length
) {
    return( izmstr_replace( _strbuf, _strbufsize, _offset, _length, "", 0 ) );
};

unsigned char izmstr_remove_back(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _length
) {
    return( izmstr_replace( _strbuf, _strbufsize, strlen(_strbuf) - _length + 1, _length, "", 0 ) );
};

unsigned char izmstr_insert(
    char* _strbuf,
    const size_t _strbufsize,
    const size_t _offset,
    const const char* _insert,
    const size_t _insertsize
) {
    return( izmstr_replace( _strbuf, _strbufsize, _offset, 0, _insert, _insertsize ) );
};

unsigned char izmstr_insert_back(
    char* _strbuf,
    const size_t _strbufsize,
    const const char* _insert,
    const size_t _insertsize
) {
    return( izmstr_replace( _strbuf, _strbufsize, strlen(_strbuf), 0, _insert, _insertsize ) );
};

unsigned char izmstr_regexmatch(
    const char* _pattern,
    const char* _strbuf,
    IZMSTR_REGEXMATCH* _regexmatch
) {
    regex_t regex = {};
    IZMSTR_REGEXMATCH regexmatch = {};
    int cflag = REG_EXTENDED;

    if( _pattern == NULL || _strbuf == NULL )
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

    regexmatch.stat_exec = regexec( &regex, _strbuf, regex.re_nsub + 1, match, 0 );
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
                &(_strbuf[regexmatch.match[i].offset]),
                regexmatch.match[i].length
        );
    }

    if( _regexmatch != NULL )
        memcpy( _regexmatch, &regexmatch, sizeof(IZMSTR_REGEXMATCH) );

    return( IZM_OK );
};


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
        sprintf( replace_sub, "\\%d", i );

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
            sprintf( replace_sub, "\\%d", i );

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


#if 0
unsigned char izmstr_regexreplace_global(
    const char* _pattern,
    char* _strbuf,
    const size_t _strbufsize,
    const char* _replace
) {
    unsigned char ret = 0;

    while( 1 )
    {
        ret = izmstr_regexreplace(
                    _pattern,
                    _strbuf,
                    _strbufsize,
                    _replace
                );

        if( ret == IZM_OK )
        {
            continue;
        }
        else if( ret == IZM_NG /* no match */ )
        {
            return( IZM_OK );
        }
        else
        {
            return( IZM_ERROR );
        }
    }

    return( IZM_ERROR );
};
#endif










































