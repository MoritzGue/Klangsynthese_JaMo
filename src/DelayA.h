
#ifndef STK_DELAYA_H
#define STK_DELAYA_H

#include <array>
#include <vector>



/***************************************************/
/*! \class DelayA
    \brief STK allpass interpolating delay line class.

    This class implements a fractional-length digital delay-line using
    a first-order allpass filter.  If the delay and maximum length are
    not specified during instantiation, a fixed maximum length of 4095
    and a delay of 0.5 is set.

    An allpass filter has unity magnitude gain but variable phase
    delay properties, making it useful in achieving fractional delays
    without affecting a signal's frequency magnitude response.  In
    order to achieve a maximally flat phase delay response, the
    minimum delay possible in this implementation is limited to a
    value of 0.5.

    by Perry R. Cook and Gary P. Scavone, 1995--2014.
*/
/***************************************************/

class DelayA
{
public:

  //! Default constructor creates a delay-line with maximum length of 4095 samples and delay = 0.5.
  /*!
    An StkError will be thrown if the delay parameter is less than
    zero, the maximum delay parameter is less than one, or the delay
    parameter is greater than the maxDelay value.
   */  
  DelayA( float delay = 0.5, unsigned long maxDelay = 4095 );

  //! Class destructor.
  ~DelayA();

  //! Clears all internal states of the delay line.
  void clear( void );

  //! Get the maximum delay-line length.
  unsigned long getMaximumDelay( void ) { return inputs_.size() - 1; };
  
  //! Set the maximum delay-line length.
  /*!
    This method should generally only be used during initial setup
    of the delay line.  If it is used between calls to the tick()
    function, without a call to clear(), a signal discontinuity will
    likely occur.  If the current maximum length is greater than the
    new length, no memory allocation change is made.
  */
  void setMaximumDelay( unsigned long delay );

  //! Set the delay-line length
  /*!
    The valid range for \e delay is from 0.5 to the maximum delay-line length.
  */
  void setDelay( float delay );

  //! Return the current delay-line length.
  float getDelay( void ) const { return delay_; };

  //! Return the value at \e tapDelay samples from the delay-line input.
  /*!
    The tap point is determined modulo the delay-line length and is
    relative to the last input value (i.e., a tapDelay of zero returns
    the last input value).
  */
  float tapOut( unsigned long tapDelay );

  //! Set the \e value at \e tapDelay samples from the delay-line input.
  void tapIn( float value, unsigned long tapDelay );

  //! Return the last computed output value.
  float lastOut( void ) const { return lastFrame_[0]; };

  //! Return the value which will be output by the next call to tick().
  /*!
    This method is valid only for delay settings greater than zero!
   */
  float nextOut( void );

  //! Input one sample to the filter and return one output.
  float tick( float input );


protected:  

  unsigned long inPoint_;
  unsigned long outPoint_;
  float delay_;
  float alpha_;
  float coeff_;
  float apInput_;
    std::vector<float> lastFrame_;
    std::vector<float> inputs_;
    float gain_;
  float nextOutput_;
  bool doNextOut_;
};

inline float DelayA :: nextOut( void )
{
  if ( doNextOut_ ) {
    // Do allpass interpolation delay.
    nextOutput_ = -coeff_ * lastFrame_[0];
    nextOutput_ += apInput_ + ( coeff_ * inputs_[outPoint_] );
    doNextOut_ = false;
  }

  return nextOutput_;
}

inline float DelayA :: tick( float input )
{
  inputs_[inPoint_++] = input * gain_;

  // Increment input pointer modulo length.
  if ( inPoint_ == inputs_.size() )
    inPoint_ = 0;

  lastFrame_[0] = nextOut();
  doNextOut_ = true;

  // Save the allpass input and increment modulo length.
  apInput_ = inputs_[outPoint_++];
  if ( outPoint_ == inputs_.size() )
    outPoint_ = 0;

  return lastFrame_[0];
}

#endif
