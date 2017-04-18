//
//  TPTFilter.h
//

#ifndef TPTFilter_h
#define TPTFilter_h

enum {
    bq_type_lowpass = 0,
    bq_type_highpass,
    bq_type_bandpass,
    bq_type_notch,
    bq_type_peak,
    bq_type_lowshelf,
    bq_type_highshelf
};

class TPTFilter {
public:
    TPTFilter();
    TPTFilter(int type, double Fc, double Q, double sampleRate);
    ~TPTFilter();
    void setType(int type);
    void setQ(double Q);
    void setFc(double Fc);
    void setTPTFilter(int type, double Fc, double Q, double peakGain);
    double nextSample(double input);
    
protected:
    void calcTPTFilter(void);

    int type;
    double g, h;
    double Fc, Q, sampleRate;
    double z1, z2;
};

inline double TPTFilter::nextSample(double input) {
    
    const double yH = h * (input - (1.0 / Q + g) * z1 - z2);
    
    const double yB = g * yH + z1;
    z1 = g * yH + yB;
    
    const double yL = g * yB + z2;
    z2 = g * yB + yL;
    
    
    return yL;
}

#endif // TPTFilter_h
