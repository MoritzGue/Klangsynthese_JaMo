/***************************************************/
/*! \class OneZero
    \brief STK one-zero filter class.

    This class implements a one-zero digital filter.  A method is
    provided for setting the zero position along the real axis of the
    z-plane while maintaining a constant filter gain.

    by Perry R. Cook and Gary P. Scavone, 1995--2014.
*/
/***************************************************/

#include "OneZero.h"
#include <cmath>



OneZero :: OneZero( double theZero )
{

  this->setZero( theZero );
    z_1 = 0.0;
}

OneZero :: ~OneZero( void )
{
}

void OneZero :: setZero( double theZero )
{
  // Normalize coefficients for unity gain.
  if ( theZero > 0.0 )
    b_0 = 1.0 / ((double) 1.0 + theZero);
  else
    b_0 = 1.0 / ((double) 1.0 - theZero);

  b_1 = -theZero * b_0;
}

void OneZero :: setCoefficients(double b0, double b1)
{
  b_0 = b0;
  b_1 = b1;

}

