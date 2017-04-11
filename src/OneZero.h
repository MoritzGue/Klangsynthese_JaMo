#ifndef STK_ONEZERO_H
#define STK_ONEZERO_H




/***************************************************/
/*! \class OneZero
  \brief STK one-zero filter class.

  This class implements a one-zero digital filter.  A method is
  provided for setting the zero position along the real axis of the
  z-plane while maintaining a constant filter gain.

  by Perry R. Cook and Gary P. Scavone, 1995--2014.
*/
/***************************************************/

class OneZero
{
 public:

  //! The default constructor creates a low-pass filter (zero at z = -1.0).
  OneZero( double theZero = -1.0 );

  //! Class destructor.
  ~OneZero();

  //! Set the b[0] coefficient value.
  void setB0( double b0 ) { b_0 = b0; };

  //! Set the b[1] coefficient value.
  void setB1( double b1 ) { b_1 = b1; };

  //! Set all filter coefficients.
  void setCoefficients( double b0, double b1);

  //! Set the zero position in the z-plane.
  /*!
    This method sets the zero position along the real-axis of the
    z-plane and normalizes the coefficients for a maximum gain of one.
    A positive zero value produces a high-pass filter, while a
    negative zero value produces a low-pass filter.  This method does
    not affect the filter \e gain value.
  */
  void setZero( double theZero );

  //! Return the last computed output value.
  //double lastOut( void ) const { return lastFrame_[0]; };

  //! Input one sample to the filter and return one output.
  float process(float input);

private:

    double b_0;
    double b_1;
    double z_1;

};

inline float OneZero :: process(float input)
{
    float y_n = b_1 * z_1 + b_0 * input;
    z_1 = input;
    return y_n;
}




#endif

