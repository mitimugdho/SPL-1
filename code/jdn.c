#include "jdn.h"

long gregorian_to_jdn(int day, int month, int year) {
    
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    return day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
}

long gregorian_to_jdn(GregorianDate date) {
    int a = (14 - date.month) / 12;
    int y = date.year + 4800 - a;
    int m = date.month + 12 * a - 3;
    long jdn = date.day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
    return jdn;
}
GregorianDate jdn_to_gregorian(long jdn) {
    GregorianDate date;    
    long a = jdn + 32044;
    long b = (4 * a + 3) / 146097;
    long c = a - (146097 * b) / 4;
    long d = (4 * c + 3) / 1461;
    long e = c - (1461 * d) / 4;
    long m = (5 * e + 2) / 153;
    
    date.day = e - (153 * m + 2) / 5 + 1;
    date.month = m + 3 - 12 * (m / 10);
    date.year = 100 * b + d - 4800 + m / 10;
    
    return date;
}