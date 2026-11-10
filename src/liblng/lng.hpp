#ifndef LIBLNG_HPP
#define LIBLNG_HPP

//==========================================================================================
//
// liblng.hpp : header-only library for conlang, Indo-European languages oriented
//
//==========================================================================================

#include <izm.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <regex>
#include <fstream>

namespace lng {

    // declaration
    class Morph;

    template <size_t D1, size_t D2, size_t D3, size_t D4>
    class MorphTable;

    template <typename T, size_t D1>
    using Array1d = std::array<T, D1>;
    template <typename T, size_t D1, size_t D2>
    using Array2d = std::array<Array1d<T, D1>, D2>;
    template <typename T, size_t D1, size_t D2, size_t D3>
    using Array3d = std::array<Array2d<T, D1, D2>, D3>;
    template <typename T, size_t D1, size_t D2, size_t D3, size_t D4>
    using Array4d = std::array<Array3d<T, D1, D2, D3>, D4>;
    
    enum class MorphType {
            PREFIX,         // "-abc"
            POSTFIX,        // "abc-"
            REGEX,          // "/abc/ABC/"
            PLAIN,          // "abc"
            AUTO
    };
    
    class Morph : public std::string {
    private:
    
    public:
            MorphType type = MorphType::AUTO;
            std::regex_constants::match_flag_type regex_match_flag_type
                    = std::regex_constants::match_default;
    
            Morph() {
                    return;
            };
    
            Morph( std::string _str ) {
                    this->assign( _str );
                    return;
            };
    
            Morph( MorphType _type ) {
                    this->type = _type;
                    return;
            };
    
            Morph( std::string _str, MorphType _type ) {
                    this->assign( _str );
                    this->type = _type;
                    return;
            };
    
            Morph operator + ( const Morph &second ) {
                    std::string str1( *this );
                    std::string str2( second );
                    return( Morph( str1 + str2 ) );
            };
    
            Morph operator * ( const Morph &second ) {
                    std::string str1( *this );
                    std::string str2( second );
                    std::vector<std::string> _strlist = izm::split( str2, std::vector<char>{ '/' } );
    
                    MorphType _type = second.type;
    
                    if( _type == MorphType::AUTO ) {
                            if( str2.starts_with( "-" ) )
                            {
                                    _type = MorphType::POSTFIX; // -aaa
                            }
                            else if( str2.ends_with( "-" ) )
                            {
                                    _type = MorphType::PREFIX; // aaa-
                            }
                            else if( _strlist.size() == 2 )
                            {
                                    _type = MorphType::REGEX;
                            } 
                            else
                            {
                                    _type = MorphType::PLAIN;
                            } 
                    }
    
                    if( _type == MorphType::POSTFIX )
                    {
                            str2.erase( 0, 1 );
                            return( Morph( str1 + str2 ) );
                    }
                    else if( _type == MorphType::PREFIX )
                    {
                            str2.pop_back();
                            return( Morph( str2 + str1 ) );
                    }
                    else if( _type == MorphType::REGEX )
                    {
                            // REGEX
                            std::regex re( _strlist[0] );
                            std::string fmt( _strlist[1] );
                            return( Morph( std::regex_replace( str1, re, fmt, regex_match_flag_type ) ) );
                    } 
    
                    // PLAIN
                    return( second );
            };

            template <size_t D1, size_t D2, size_t D3, size_t D4>
            MorphTable<D1, D2, D3, D4> operator + ( MorphTable<D1, D2, D3, D4> &second )
            {
                    MorphTable<D1, D2, D3, D4> ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this) + second[_i4][_i3][_i2][_i1];
                    }}}}
                    return( ret );
            };
    
            template <size_t D1, size_t D2, size_t D3, size_t D4>
            MorphTable<D1, D2, D3, D4> operator * ( MorphTable<D1, D2, D3, D4> &second )
            {
                    MorphTable<D1, D2, D3, D4> ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this) * second[_i4][_i3][_i2][_i1];
                    }}}}
                    return( ret );
            };
    };


    template <size_t D1, size_t D2, size_t D3, size_t D4>
    class MorphTable : public Array4d<Morph, D1, D2, D3, D4> {
    private:
            bool init( std::vector<std::string> _list, std::string _filler ) {
                auto itr = _list.begin();
                for( int _i1 = 0; _i1 < D1; _i1++ ) {
                for( int _i2 = 0; _i2 < D2; _i2++ ) {
                for( int _i3 = 0; _i3 < D3; _i3++ ) {
                for( int _i4 = 0; _i4 < D4; _i4++ ) {
                    if( itr == _list.end() ) {
                        (*this)[_i4][_i3][_i2][_i1] = Morph( _filler );
                    } else {
                        (*this)[_i4][_i3][_i2][_i1] = Morph( *itr );
                        itr++;
                    }
                }}}}
                return( true );
            };
    
    public:
            MorphTable() { return; };

            MorphTable( std::vector<std::string> _list, std::string _filler ) {
                this->init( _list, _filler );
                return;
            };

            MorphTable( std::vector<std::string> _list ) {
                this->init( _list, "" );
                return;
            };

            MorphTable( std::initializer_list<const char*> _list, std::string _filler ) {
                std::vector<std::string> _list_vector;
                for( auto itr = _list.begin(); itr != _list.end(); itr++ ) {
                    _list_vector.push_back( *itr );
                }
                this->init( _list_vector, _filler );
                return;
            };

            MorphTable( std::initializer_list<const char*> _list ) {
                std::vector<std::string> _list_vector;
                for( auto itr = _list.begin(); itr != _list.end(); itr++ ) {
                    _list_vector.push_back( *itr );
                }
                this->init( _list_vector, "" );
                return;
            };

            Morph& at( size_t _i1 ) {
                    return( (*this)[0][0][0][_i1] );
            };
            Morph& at( size_t _i1, size_t _i2 ) {
                    return( (*this)[0][0][_i2][_i1] );
            };
            Morph& at( size_t _i1, size_t _i2, size_t _i3 ) {
                    return( (*this)[0][_i3][_i2][_i1] );
            };
            Morph& at( size_t _i1, size_t _i2, size_t _i3, size_t _i4 ) {
                    return( (*this)[_i4][_i3][_i2][_i1] );
            };
    
            MorphTable operator + ( Morph &second ) {
                    MorphTable ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this)[_i4][_i3][_i2][_i1] + second;
                    }}}}
                    return( ret );
            };
    
            MorphTable operator * ( Morph &second ) {
                    MorphTable ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this)[_i4][_i3][_i2][_i1] * second;
                    }}}}
                    return( ret );
            };

            MorphTable operator + ( MorphTable &second ) {
                    MorphTable ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this)[_i4][_i3][_i2][_i1] + second[_i4][_i3][_i2][_i1];
                    }}}}
                    return( ret );
            };
    
            MorphTable operator * ( MorphTable &second ) {
                    MorphTable ret;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                            ret[_i4][_i3][_i2][_i1] = (*this)[_i4][_i3][_i2][_i1] * second[_i4][_i3][_i2][_i1];
                    }}}}
                    return( ret );
            };

            void println( std::string delim_mid, std::string delim_head, std::string delim_tail ) {
                    std::cout << delim_head;
                    for( int _i1 = 0; _i1 < D1; _i1++ ) {
                    for( int _i2 = 0; _i2 < D2; _i2++ ) {
                    for( int _i3 = 0; _i3 < D3; _i3++ ) {
                    for( int _i4 = 0; _i4 < D4; _i4++ ) {
                        std::cout << ( (_i1 || _i2 || _i3 || _i4 ) ? delim_mid : "" );
                        std::cout << (*this)[_i4][_i3][_i2][_i1];
                    }}}}
                    std::cout << delim_tail << std::endl;
            };
            void println( std::string delim_mid, std::string delim_head ) {
                println( delim_mid, delim_head, "" );
            };
            void println( std::string delim_mid ) {
                println( delim_mid, "", "" );
            };
            void println() {
                println( ", ", "", "" );
            };
    };
};

#endif
