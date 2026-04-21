#include "bengali.h"
#include <stdio.h>
const char* BENGALI_MONTHS[] = {
    "",
    "বৈশাখ", "জ্যৈষ্ঠ", "আষাঢ়", "শ্রাবণ", "ভাদ্র", "আশ্বিন",
    "কার্তিক", "অগ্রহায়ণ", "পৌষ", "মাঘ", "ফাল্গুন", "চৈত্র"
};
const char* BENGALI_DAY_NAMES_UNICODE[] = {
    "রবিবার",    
    "সোমবার",
    "মঙ্গলবার",
    "বুধবার",
    "বৃহস্পতিবার",
    "শুক্রবার",
    "শনিবার"
};
const char* BENGALI_DAY_SHORT[] = {
    "রবি", "সোম", "মঙ্গল", "বুধ", "বৃহস্পতি", "শুক্র", "শনি"
};
const char* BENGALI_DAY_NUMBER[] = {
    "০১",
    "০২",
    "০৩",
    "০৪",
    "০৫",
    "০৬",
    "০৭",
    "০৮",
    "০৯",
    "১০",
    "১১",
    "১২",
    "১৩","১৪","১৫","১৬","১৭","১৮","১৯","২০","২১","২২","২৩","২৪","২৫","২৬","২৭","২৮","২৯","৩০","৩১"
};
const int BENGALI_DAYS_IN_MONTH[] = {
    0, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 29, 30
};
void init_bengali_calendar() {    
}
int is_bengali_leap_year(int year) {
    int gregorian_year = year + 593;
    
    if (gregorian_year % 400 == 0)
        return 1;
    if (gregorian_year % 100 == 0)
        return 0;
    if (gregorian_year % 4 == 0)
        return 1;
    return 0;
}
int get_bengali_days_in_month(int month, int year) {
    if (month < 1 || month > 12){
        return 0;
    }
    if (month == 11 && is_bengali_leap_year(year)){
        return 30;
    }
    return BENGALI_DAYS_IN_MONTH[month];
}
int is_valid_bengali_date(BengaliDate date) {
    if (date.year < 1 || date.month < 1 || date.month > 12 || date.day < 1)
        return 0;
    
    int max_days = get_bengali_days_in_month(date.month, date.year);
    return date.day <= max_days;
}
void print_bengali_date(BengaliDate date) {
    printf("%s %s, %d ", get_bengali_day_number(date.day), BENGALI_MONTHS[date.month], date.year);
}
const char* get_bengali_month_name(int month) {
    if (month < 1)
        return "Invalid";
    else if(month >12){
        month=month%12;
    }
    return BENGALI_MONTHS[month];
}

const char* get_bengali_day_short(int dow) {
    if (dow < 0 || dow > 6)
        return "Invalid";
    return BENGALI_DAY_SHORT[dow];
}
const char* get_bengali_day_unicode(int dow) {
    if (dow < 0 || dow > 6)
        return "Invalid";
    return BENGALI_DAY_NAMES_UNICODE[dow];
}
const char* get_bengali_day_name(int dow) {
    if (dow < 0 || dow > 6)
        return "Invalid";
    return BENGALI_DAY_NAMES_UNICODE[dow];
}
const char* get_bengali_day_number(int bengali_day){
    if(bengali_day< 1 || bengali_day > 31){
        return "Invalid bengali date";
    }
    return BENGALI_DAY_NUMBER[bengali_day-1];
}