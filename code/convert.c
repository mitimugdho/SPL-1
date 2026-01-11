#include "convert.h"
#include "jdn.h"

BengaliDate gregorian_to_bengali(GregorianDate g_date) {
    BengaliDate b_date;
    long jdn = gregorian_to_jdn(g_date);
    long days_since_epoch = jdn - BENGALI_EPOCH_JDN;
    
    int b_year = (int)(days_since_epoch / 365) + 1;
    
    BengaliDate temp;
    temp.day = 1;
    temp.month = 1;
    temp.year = b_year;
    
    GregorianDate year_start = bengali_to_gregorian(temp);
    long year_start_jdn = gregorian_to_jdn(year_start);
    
    while (year_start_jdn > jdn) {
        b_year--;
        temp.year = b_year;
        year_start = bengali_to_gregorian(temp);
        year_start_jdn = gregorian_to_jdn(year_start);
    }
    
    int day_of_year = jdn - year_start_jdn + 1;
    
    int month = 1;
    int days_counted = 0;
    
    while (month <= 12) {
        int days_in_month = get_bengali_days_in_month(month, b_year);
        if (days_counted + days_in_month >= day_of_year) {
            b_date.day = day_of_year - days_counted;
            b_date.month = month;
            b_date.year = b_year;
            break;
        }
        days_counted += days_in_month;
        month++;
    }
    
    return b_date;
}

GregorianDate bengali_to_gregorian(BengaliDate b_date) {
    int g_year = b_date.year + 593;
    
    GregorianDate year_start; //checking leap year dekhe boishakh kobe
    year_start.year = g_year;
    year_start.month = 4;
    
    if (is_gregorian_leap_year(g_year)) {
        year_start.day = 15;
    } else {
        year_start.day = 14;
    }
    
    long year_start_jdn = gregorian_to_jdn(year_start);
    
    int days_to_add = 0; // bengali year shuru theke days calculate
    for (int m = 1; m < b_date.month; m++) {
        days_to_add += get_bengali_days_in_month(m, b_date.year);
    }
    days_to_add += b_date.day - 1;
    
    long target_jdn = year_start_jdn + days_to_add;
    
    return jdn_to_gregorian(target_jdn);
}