#ifndef _ACJ_h_
#define _ACJ_h_
#include "stdafx.h"
#include "Complex.h"
#include <assert.h>
#include <stdio.h>
#include "roundoff.h"

struct ACJ{
ACJ(const XComplex&ff,const XComplex&ff0= 0,
const XComplex&ff1= 0,const XComplex&ff2= 0,double err= 0)
:f(ff),f0(ff0),f1(ff1),f2(ff2),e(err),
size((1+2*EPS)*(absUB(ff0)+(absUB(ff1)+absUB(ff2))))
{}

XComplex f;
XComplex f0;
XComplex f1;
XComplex f2;
double e;

double size;
};


inline const ACJ operator-(const ACJ&x);
inline const ACJ operator+(const ACJ&x,const ACJ&y);
inline const ACJ operator-(const ACJ&x,const ACJ&y);
inline const ACJ operator+(const ACJ&x,double y);
inline const ACJ operator-(const ACJ&x,const ACJ&y);
inline const ACJ operator*(const ACJ&x,double y);
inline const ACJ operator/(const ACJ&x,double y);
inline const double absUB(const ACJ&x);
inline const double absLB(const ACJ&x);
inline const double size(const ACJ&x);
const ACJ operator*(const ACJ&x,const ACJ&y);
const ACJ operator/(const ACJ&x,const ACJ&y);
const ACJ operator/(double x,const ACJ&y);
const ACJ sqrt(const ACJ&x);
//#include "ACJ.inline"

inline const ACJ operator-(const ACJ&x)
{

	return ACJ(-x.f, -x.f0, -x.f1, -x.f2, x.e);


}
inline const ACJ operator+(const ACJ&x, const ACJ&y)
{
	AComplex r_f = x.f + y.f;
	AComplex r_f0 = x.f0 + y.f0;
	AComplex r_f1 = x.f1 + y.f1;
	AComplex r_f2 = x.f2 + y.f2;
	double r_error = (1 + 3 * EPS)*(
		(x.e + y.e)
		+ ((r_f.e + r_f0.e) + (r_f1.e + r_f2.e))
		);
	return ACJ(r_f.z, r_f0.z, r_f1.z, r_f2.z, r_error);


}
inline const ACJ operator-(const ACJ&x, const ACJ&y)
{

	AComplex r_f = x.f - y.f;
	AComplex r_f0 = x.f0 - y.f0;
	AComplex r_f1 = x.f1 - y.f1;
	AComplex r_f2 = x.f2 - y.f2;
	double r_error = (1 + 3 * EPS)*(
		(x.e + y.e)
		+ ((r_f.e + r_f0.e) + (r_f1.e + r_f2.e))
		);
	return ACJ(r_f.z, r_f0.z, r_f1.z, r_f2.z, r_error);


}
inline const ACJ operator+(const ACJ&x, double y)
{
	AComplex r_f = x.f + y;
	return ACJ(r_f.z, x.f0, x.f1, x.f2,
		(1 + EPS)*(x.e + r_f.e));


}
inline const ACJ operator-(const ACJ&x, double y)
{
	AComplex r_f = x.f - y;
	return ACJ(r_f.z, x.f0, x.f1, x.f2,
		(1 + EPS)*(x.e + r_f.e));


}
inline const ACJ operator*(const ACJ&x, double y)
{
	AComplex r_f = x.f*y;
	AComplex r_f0 = x.f0*y;
	AComplex r_f1 = x.f1*y;
	AComplex r_f2 = x.f2*y;
	double r_error = (1 + 3 * EPS)*(
		(x.e*fabs(y))
		+ ((r_f.e + r_f0.e) + (r_f1.e + r_f2.e))
		);
	return ACJ(r_f.z, r_f0.z, r_f1.z, r_f2.z, r_error);


}
inline const ACJ operator/(const ACJ&x, double y)
{
	AComplex r_f = x.f / y;
	AComplex r_f0 = x.f0 / y;
	AComplex r_f1 = x.f1 / y;
	AComplex r_f2 = x.f2 / y;
	double r_error = (1 + 3 * EPS)*(
		(x.e / fabs(y))
		+ ((r_f.e + r_f0.e) + (r_f1.e + r_f2.e))
		);
	return ACJ(r_f.z, r_f0.z, r_f1.z, r_f2.z, r_error);


}
inline const double absUB(const ACJ&x)
{
	return(1 + 2 * EPS)*(absUB(x.f) + (size(x) + x.e));


}
inline const double absLB(const ACJ&x)
{
	double v = (1 - EPS)*(absLB(x.f) - (1 + EPS)*(size(x) + x.e));
	return(v>0) ? v : 0;


}
inline const double size(const ACJ&x)
{
	return x.size;

}



#endif
