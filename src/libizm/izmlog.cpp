#include <izm.hpp>

extern "C++" constexpr unsigned int LogfileSizeMaxDefault = 1000000;
extern "C++" constexpr unsigned int LogfileNumMaxDefault = 5;

extern "C++"
bool izm::printl(
    const char* fileName,
    const char* format,
    ...
)
{
    va_list args;
    va_start( args, format );

    bool ret = false;

    ret = izm::printl( std::string( fileName ), format, args );

    va_end( args );

    return( ret );
};

extern "C++"
bool izm::printl(
    const std::string& fileName,
    const char* format,
    ...
)
{
    bool ret = false;
    static char buf[ 1024 ];
    // memset( buf, 0x0, sizeof( buf ) );

    va_list args;
    va_start( args, format );

    vsnprintf( buf, sizeof( buf ), format, args );

    ret = izm::printl( fileName, std::string( buf ) );

    va_end( args );

    return( ret );
};

extern "C++"
bool izm::printl(
    const char* fileName,
    const std::string& logStr
)
{
    bool ret = false;

    ret = izm::printl( std::string( fileName ), logStr );

    return( ret );
};

extern "C++"
bool izm::printl(
    const std::string& fileName,
    const std::string& logStr
)
{
    bool ret = false;
    bool isEnabled = true;

    if( isEnabled )
    {

        unsigned int logfileSizeMax = LogfileSizeMaxDefault;
        unsigned int logfileNumMax = LogfileNumMaxDefault;
        std::istringstream logStrStream( logStr );

        // printf( "printl : logfileSizeMax = %d\n", logfileSizeMax );
        // printf( "printl : logfileNumMax = %d\n", logfileNumMax );

        // get time
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t nowLocal = std::chrono::system_clock::to_time_t(now);

        // rotate
        rotate( fileName, logfileSizeMax, logfileNumMax );

        // open file
        {
            std::ofstream ostrm( fileName, std::ios::out | std::ios::app );

            // printf( "printl : open file : %s\n", fileName.c_str() );

            for ( std::string logStrLine; std::getline( logStrStream, logStrLine ); ) {
                ostrm
                    << std::put_time( std::localtime( &nowLocal ), "%F %T" )
                    << " "
                    << logStrLine
                    << std::endl; 
            }

            // printf( "printl : close file : %s\n", fileName.c_str() );
        }

        ret = true;
    }
    else
    {
        ret = true;
    }

    return( ret );
};

extern "C++"
bool izm::rotate
(
    std::string fileName,
    unsigned int logfileSizeMax,
    unsigned int logfileNumMax
)
{
    bool ret = false;

    std::vector<FileSt> logFile;
    for( int i = 0; i < logfileNumMax; i++ )
    {
        logFile.push_back(
            FileSt { fileName + ( i == 0 ? std::string("") : std::string(".") + std::to_string(i) ), false }
        );
    }

    // get file stat
    for( auto& f : logFile )
    {
        if( stat( f.fileName.c_str(), &(f.fileStat) ) == 0 )
        {
            f.isExist = true;
        }
    }

    /*
    int i = 0;
    for( auto& f : logFile )
    {
        printf(
            "rotate : logFile(%d) : fileName = %s, isExist = %d, st_size = %d, st_mtime = %d\n",
            i, f.fileName.c_str(), f.isExist, f.fileStat.st_size, f.fileStat.st_mtime
        );
        i++;
    }
    */

    std::vector<FileSt>::iterator logFileItr = logFile.begin();
    std::vector<FileSt>::iterator logFileUnused = logFile.end();
    std::vector<FileSt>::iterator logFileOldest = logFile.end();

    if( ! logFileItr->isExist || logFileItr->fileStat.st_size < logfileSizeMax )
    {
        // do nothing
        // printf( "rotate : no rotate\n" );
    }
    else
    {
        logFileUnused = logFile.end();

        // find unused filename
        for( logFileItr++; logFileItr < logFile.end(); logFileItr++ )
        {
            if( ! logFileItr->isExist )
            {
                logFileUnused = logFileItr;
                break;
            }
        }

        if( logFileItr != logFile.end() )
        {
            // move logfile as unused filename
            rename( logFile.at(0).fileName.c_str(), logFileUnused->fileName.c_str() );

            // printf(
            //     "rotate : rotate : rename %s -> %s\n",
            //     logFile.at(0).fileName.c_str(), logFileUnused->fileName.c_str()
            // );
        }
        else
        {
            std::vector<FileSt>::iterator logFileOldest = logFile.begin();

            // delete oldest file
            for( logFileItr = logFile.begin(); logFileItr < logFile.end(); logFileItr++ )
            {
                if( logFileItr->fileStat.st_mtime < logFileOldest->fileStat.st_mtime )
                {
                    logFileOldest = logFileItr;
                }
            }

            unlink( logFileOldest->fileName.c_str() );
            rename( logFile.at(0).fileName.c_str(), logFileOldest->fileName.c_str() );

            // printf(
            //     "rotate : rotate : unlink %s, rename %s -> %s\n",
            //     logFileOldest->fileName.c_str(), logFile.at(0).fileName.c_str(), logFileOldest->fileName.c_str()
            // );
        }
    }

    ret = 0;

    return( ret );
}

