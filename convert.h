#ifndef CONVERT_H
#define CONVERT_H

#include "gregorian.h"
#include "bengali.h"

#define BENGALI_EPOCH_JDN 2214960  // April 14, 594 (1 Baishakh 1 Bongabdo)

BengaliDate gregorian_to_bengali(GregorianDate g_date);
GregorianDate bengali_to_gregorian(BengaliDate b_date);

#endif