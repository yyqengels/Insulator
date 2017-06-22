
#ifndef _roundoff_h_
#define _roundoff_h_
#include <float.h>
#include <math.h>


#define EPS DBL_EPSILON
#define HALFEPS (EPS/2)


#ifdef __GNUG__
inline double fabs(double x){return x<0?-x:x;}
extern"C"{
#endif 
void initialize_roundoff();
int roundoff_ok();
#ifndef __sparc__
extern double infinity();
#endif 
#ifdef __GNUG__
}
#endif 

#endif

/*:58*/
