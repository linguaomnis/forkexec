#ifndef LIBIZM_HPP
#define LIBIZM_HPP

extern "C" {
#include <izm.h>
}

#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <array>
#include <vector>
#include <map>
#include <regex>

namespace izm {

    /*========================*/
    /* izmstr                 */
    /*========================*/
    
    enum class Result { OK, NG, ERROR, YES, NO };
    
    std::string decode_literal( std::string _str );
    std::string encode_literal( std::string _str, bool _force );
    std::string encode_literal( std::string _str );
    bool matchcharlist( char _c, std::vector<char> _charlist );
    std::vector<std::string>
        split( std::string _line, std::vector<char> _delimlist, std::vector<char> _endlist );
    std::vector<std::string>
        split( std::string _line, std::vector<char> _delimlist );
    std::vector<std::string>
        split( std::string _line );
    
    std::ostream& err( const std::string& _filepath, const unsigned int& _line_num, const std::string& _line_in );

    /*========================*/
    /* izmlog                 */
    /*========================*/

    typedef struct {
        std::string fileName;
        bool isExist;
        struct stat fileStat;
    } FileSt;

    bool printl(
        const char* fileName,
        const char* format,
        ...
    );

    bool printl(
        const std::string& fileName,
        const char* format,
        ...
    );

    bool printl(
        const char* fileName,
        const std::string& logStr
    );

    bool printl(
        const std::string& fileName,
        const std::string& logStr
    );

    bool rotate
    (
        std::string fileName,
        unsigned int logfileSizeMax,
        unsigned int logfileNumMax
    );

}

#endif
