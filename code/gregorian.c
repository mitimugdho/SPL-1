#include "gregorian.h"
#include <stdio.h>

const char* GREGORIAN_MONTHS[] = {
    "",
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

const char* DAY_NAMES[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday",
    "Thursday", "Friday", "Saturday"
};

int is_gregorian_leap_year(int year) {
    if (year % 400 == 0)
        return 1;
    if (year % 100 == 0)
        return 0;
    if (year % 4 == 0)
        return 1;
    return 0;
}

int get_gregorian_days_in_month(int month, int year) {
    const int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12){
        return 0;
    }
    if (month == 2 && is_gregorian_leap_year(year)){
        return 29;
    }
    return days[month];
}

int is_valid_gregorian_date(GregorianDate date) {
    if (date.year < 1 || date.month < 1 || date.month > 12 || date.day < 1)
        return 0;
    
    int max_days = get_gregorian_days_in_month(date.month, date.year);
    return date.day <= max_days;
}

void print_gregorian_date(GregorianDate date) {
    printf("%d %s, %d", date.day, GREGORIAN_MONTHS[date.month], date.year);
}

const char* get_gregorian_month_name(int month) {
    if (month < 1 || month > 12)
        return "Invalid";
    return GREGORIAN_MONTHS[month];
}

const char* get_day_of_week_name(int dow) {
    if (dow < 0 || dow > 6)
        return "Invalid";
    return DAY_NAMES[dow];
}

int get_day_of_week(GregorianDate date) {
    int d = date.day;
    int m = date.month;
    int y = date.year;    
    if (m < 3) {
        m += 12;
        y -= 1;
    }
    int q = d;
    int k = y % 100;
    int j = y / 100;
    int h = (q + ((13 * (m + 1)) / 5) + k + (k / 4) + (j / 4) - (2 * j)) % 7;

    return (h + 6) % 7; // Convert korlam 0=Sunday
}