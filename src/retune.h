#ifndef RETUNE_H
#define RETUNE_H


class retune
{

 public:

  //! The default constructor creates an allpass filter.
  retune();

  //! Class destructor.
  ~retune();

  void setC(double f1);

  double process(double xn);
 
 private:
  double f1;
  double c;
  double xn_1;
  double yn_1;
  const double pi = 3.1415926535897;
};

inline double retune::process(double xn)
{
    //double xn = input;
    double yn = (c * xn) + xn_1 - (c * yn_1);
    xn_1 = xn;
    yn_1 = yn;
    return yn;
}
#endif
