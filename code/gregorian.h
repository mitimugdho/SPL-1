#ifndef GREGORIAN_H
#define GREGORIAN_H

typedef struct {
    int day;
    int month;
    int year;
} GregorianDate;

int is_gregorian_leap_year(int year);
int get_gregorian_days_in_month(int month, int year);
int is_valid_gregorian_date(GregorianDate date);
void print_gregorian_date(GregorianDate date);
const char* get_gregorian_month_name(int month);
const char* get_day_of_week_name(int dow);
int get_day_of_week(GregorianDate date);

#endif