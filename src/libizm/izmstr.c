#include <izm.h>

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

