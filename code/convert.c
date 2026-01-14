#include "convert.h"
#include "jdn.h"

BengaliDate gregorian_to_bengali(GregorianDate g_date) {
    BengaliDate b_date;

    b_date.day = 1; // uninitialized dekhay naile
    b_date.month = 1;
    b_date.year = 1;

    long jdn = gregorian_to_jdn(g_date);
    int g_year = g_date.year;
    int b_year;
    
    if (g_date.month < 4 || (g_date.month == 4 && g_date.day < 14)) {
        b_year = g_year - 594;
    } else {
        b_year = g_year - 593;
    }
    
    GregorianDate year_start;
    year_start.year = b_year + 593;
    year_start.month = 4;
    
    if (is_gregorian_leap_year(year_start.year)) {
        year_start.day = 15; // leap year e 15 april
    } else {
        year_start.day = 14;
    }
    
    long year_start_jdn = gregorian_to_jdn(year_start);

    if (jdn < year_start_jdn) {
        b_year--;
        year_start.year = b_year + 593;
        if (is_gregorian_leap_year(year_start.year)) {
            year_start.day = 15;
        } else {
            year_start.day = 14;
        }
        year_start_jdn = gregorian_to_jdn(year_start);
    }
    
    int day_of_year = (int)(jdn - year_start_jdn) + 1;
    int month = 1;
    int days_counted = 0;
    
    for (month = 1; month <= 12; month++) {
        int days_in_month = get_bengali_days_in_month(month, b_year);
        
        if (day_of_year <= days_counted + days_in_month) {
            b_date.day = day_of_year - days_counted;
            b_date.month = month;
            b_date.year = b_year;
            break;
        }
        
        days_counted += days_in_month;
    }
    
    return b_date;
}
GregorianDate bengali_to_gregorian(BengaliDate b_date) {
    int g_year = b_date.year + 593;
    GregorianDate year_start;
    year_start.year = g_year;
    year_start.month = 4;
    if (is_gregorian_leap_year(g_year)) {
        year_start.day = 15;
    } else {
        year_start.day = 14;
    }
    long year_start_jdn = gregorian_to_jdn(year_start);
    int days_to_add = 0;
    for (int m = 1; m < b_date.month; m++) {
        days_to_add += get_bengali_days_in_month(m, b_date.year);
    }
    days_to_add += b_date.day - 1;
    long target_jdn = year_start_jdn + days_to_add;
    return jdn_to_gregorian(target_jdn);
}
