#include <izm.hpp>
#include <iostream>
#include <string>

int main( const int argc, const char** argv )
{
    std::string usage(
        "$ izmPrintLog <filename> <logstr>\n"
    );

    std::string fileName;
    std::string logStr;

    switch( argc )
    {
    case 3:
        fileName = std::string( argv[1] );
        logStr = std::string( argv[2] );
        break;
    default:
        std::cerr << "error : invalid argument" << std::endl;
        std::cerr << usage;
        return( 1 );
    }

    izm::printl( fileName, logStr );
    return( 0 );
}
