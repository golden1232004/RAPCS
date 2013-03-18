
#ifndef _COMPLEX_SIMPLE_H_
#define _COMPLEX_SIMPLE_H_

typedef struct complex_t {
    double re;
    double im;
} complex;


complex complex_from_polar(double r, double theta_radians);
double  complex_magnitude(complex c);
complex complex_add(complex left, complex right);
complex complex_sub(complex left, complex right);
complex complex_mult(complex left, complex right);


#endif /* #ifndef _COMPLEX_SIMPLE_H_ */