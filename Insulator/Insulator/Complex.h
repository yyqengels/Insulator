#ifndef _Complex_h_
#define _Complex_h_
#include <assert.h>
#include "roundoff.h"

struct XComplex{
double re;
double im;
XComplex(double r= 0,double i= 0):re(r),im(i){}
};

struct AComplex{
XComplex z;
double e;
AComplex(double r,double i,double err):z(r,i),e(err){}
};

inline const XComplex operator-(const XComplex&x);
inline const AComplex operator+(const AComplex&x,const AComplex&y);
inline const AComplex operator+(const XComplex&x,const XComplex&y);
inline const AComplex operator+(const XComplex&x,double y);
inline const AComplex operator-(const AComplex&x,const AComplex&y);
inline const AComplex operator-(const XComplex&x,const XComplex&y);
inline const AComplex operator-(const XComplex&x,double y);
inline const AComplex operator*(const XComplex&x,const XComplex&y);
inline const AComplex operator*(const XComplex&x,double y);
inline const AComplex operator/(const XComplex&x,double y);
inline const double absLB(const XComplex&x);
inline const double absUB(const XComplex&x);
AComplex operator/(const AComplex&x,const AComplex&y);
AComplex operator/(const XComplex&x,const XComplex&y);
AComplex operator/(double x,const XComplex&y);
AComplex sqrt(const XComplex&x);
//#include "Complex.inline"
inline const XComplex operator-(const XComplex&x)
{
	return XComplex(-x.re, -x.im);


}
inline const AComplex operator+(const XComplex&x, const XComplex&y)
{
	double re = x.re + y.re;
	double im = x.im + y.im;
	double e = HALFEPS*((1 + EPS)*(fabs(re) + fabs(im)));
	return AComplex(re, im, e);


}
inline const AComplex operator+(const XComplex&x, double y)
{
	double re = x.re + y;
	double e = HALFEPS*fabs(re);
	return AComplex(re, x.im, e);

}
inline const AComplex operator+(const AComplex&x, const AComplex&y)
{
	double re = x.z.re + y.z.re;
	double im = x.z.im + y.z.im;
	double e = (1 + 2 * EPS)*(HALFEPS*(fabs(re) + fabs(im)) + (x.e + y.e));
	return AComplex(re, im, e);


}
inline const AComplex operator-(const XComplex&x, const XComplex&y)
{
	double re = x.re - y.re;
	double im = x.im - y.im;
	double e = HALFEPS*((1 + EPS)*(fabs(re) + fabs(im)));
	return AComplex(re, im, e);


}
inline const AComplex operator-(const XComplex&x, double y)
{
	double re = x.re - y;
	double e = HALFEPS*fabs(re);
	return AComplex(re, x.im, e);


}
inline const AComplex operator-(const AComplex&x, const AComplex&y)
{
	double re = x.z.re - y.z.re;
	double im = x.z.im - y.z.im;
	double e = (1 + 2 * EPS)*(HALFEPS*(fabs(re) + fabs(im)) + (x.e + y.e));
	return AComplex(re, im, e);


}
inline const AComplex operator*(const XComplex&x, const XComplex&y)
{
	double re1 = x.re*y.re, re2 = x.im*y.im;
	double im1 = x.re*y.im, im2 = x.im*y.re;
	double e = EPS*((1 + 2 * EPS)*((fabs(re1) + fabs(re2)) + (fabs(im1) + fabs(im2))));
	return AComplex(re1 - re2, im1 + im2, e);


}
inline const AComplex operator*(const XComplex&x, double y)
{
	double re = x.re*y;
	double im = x.im*y;
	return AComplex(re, im, HALFEPS*((1 + EPS)*(fabs(re) + fabs(im))));


}
inline const AComplex operator/(const XComplex&x, double y)
{
	double re = x.re / y;
	double im = x.im / y;
	return AComplex(re, im, HALFEPS*((1 + EPS)*(fabs(re) + fabs(im))));


}
inline const double absLB(const XComplex&x)
{
	return(1 - 2 * EPS)*hypot(x.re, x.im);


}
inline const double absUB(const XComplex&x)
{
	return(1 + 2 * EPS)*hypot(x.re, x.im);

}



#endif
