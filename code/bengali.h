#ifndef BENGALI_H
#define BENGALI_H

extern const char* BENGALI_MONTHS[];
extern const int BENGALI_DAYS_IN_MONTH[];

typedef struct {
    int day;
    int month;  
    int year;
} BengaliDate;

void init_bengali_calendar();
int is_bengali_leap_year(int year);
int get_bengali_days_in_month(int month, int year);
int is_valid_bengali_date(BengaliDate date);
void print_bengali_date(BengaliDate date);
const char* get_bengali_month_name(int month);

#endif 