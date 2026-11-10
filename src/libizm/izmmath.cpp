#include <izm.hpp>

/* fmod は x が0を境に負と正で対称になっており、周期関数の基礎とするには不適。 */
/* x が負〜正で周期の連続性を保つ fmod2 を用意した。 */
extern "C"
double izmmath_fmod2( double x, double y )
{
    return( fmod( remainder( x + ( y / 2 ), y ) + ( y / 2 ), y ) );
};

