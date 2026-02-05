#ifndef JDN_H
#define JDN_H
#include "gregorian.h"

long gregorian_to_jdn(GregorianDate date);
GregorianDate jdn_to_gregorian(long jdn);

#endif