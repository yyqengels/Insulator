#include "stdafx.h"
#include "SL2ACJ.h"
#include "ACJ.h"
#include "Complex.h"



const SL2ACJ operator*(const SL2ACJ&x,const SL2ACJ&y)
{
return SL2ACJ(
x.a*y.a+x.b*y.c,x.a*y.b+x.b*y.d,
x.c*y.a+x.d*y.c,x.c*y.b+x.d*y.d
);

}
const SL2ACJ inverse(const SL2ACJ&x)
{

return SL2ACJ(x.d,-x.b,-x.c,x.a);

}
const ACJ orthodist(const SL2ACJ&x)
{
ACJ t= x.a*x.d+x.b*x.c;
ACJ r= ACJ(sqrt(-((-t*t) + 1)));
ACJ r1= t+r;
if(r1.f.re*r1.f.re+r1.f.im*r1.f.im>=1)
return t+r;
else
return t-r;
}
const ACJ length(const SL2ACJ&x)
{
ACJ t= (x.a+x.d)*0.5;
ACJ r= ACJ(sqrt(-((-t*t)+1)));
ACJ r1= t+r;
if(r1.f.re*r1.f.re+r1.f.im*r1.f.im>=1)
return(t+r)*(t+r);
else
return(t-r)*(t-r);
}
const int notIdentity(const SL2ACJ&x)
{
return absLB(x.b)>0
||absLB(x.c)>0
||(absLB(-(-x.a+1))>0&&absLB(x.a+1)>0)
||(absLB(-(-x.d + 1))>0&&absLB(x.d+1)>0);

}
const int notFPower(const SL2ACJ&x)
{
return absLB(x.b)>0||absLB(x.c)>0;
}
const SL2ACJ shortGenerator(const ACJ&z)
{
ACJ sz= sqrt(z);
ACJ zero(0);
return SL2ACJ(sz,zero,zero,1/sz);
}
const SL2ACJ closeGenerator(const ACJ&x,const ACJ&z)
{
ACJ sx= sqrt(x),sz= sqrt(z);
ACJ sh= (sx-1/sx)*0.5;
ACJ ch= (sx+1/sx)*0.5;
return SL2ACJ(ch*sz,sh/sz,sh*sz,ch/sz);
}


/*:67*/
