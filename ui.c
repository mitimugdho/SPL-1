#include "ui.h"
#include "gregorian.h"
#include "bengali.h"
#include "convert.h"
#include "events.h"
#include "jdn.h"
#include <stdio.h>

#define BLUE    "\033[1;34m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define RESET   "\033[0m"
#define YELLOW  "\033[1;33m"

void display_monthly_calendar(int month, int year) {
    GregorianDate today = get_today_date();
    GregorianDate first_day;
    first_day.day = 1;
    first_day.month = month;
    first_day.year = year;
    
    BengaliDate bengali_date = gregorian_to_bengali(first_day);
    printf("\n");
    printf("             "YELLOW"%s %d  %s-%s %d"RESET"             \n",
           get_gregorian_month_name(month), year,get_bengali_month_name(bengali_date.month),get_bengali_month_name(bengali_date.month+1),bengali_date.year);
    int start_dow = get_day_of_week(first_day);
    int days_in_month = get_gregorian_days_in_month(month, year);
    int day = 1;
    printf(" ┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐\n");
    printf(" │  Sun   │  Mon   │  Tue   │  Wed   │  Thu   │  Fri   │  Sat   │\n");
    printf(" │  %s   │  %s  │  %s   │  %s    │  %s │  %s    │  %s   │\n",
        get_bengali_day_short(0),get_bengali_day_short(1),get_bengali_day_short(2),get_bengali_day_short(3),get_bengali_day_short(4),get_bengali_day_short(5),get_bengali_day_short(6));
    printf(" ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤\n");

    for (int week = 0; week < 6 && day <= days_in_month; week++) {
        printf(" │");
        for (int dow = 0; dow < 7; dow++) {
            if ((week == 0 && dow < start_dow) || day > days_in_month) {
                printf("        │");
            } 
            else {
                GregorianDate current;
                current.day = day;
                current.month = month;
                current.year = year;
                
                BengaliDate b_date = gregorian_to_bengali(current);

                int is_today = (current.day == today.day && 
                               current.month == today.month && 
                               current.year == today.year);    
                if (is_today) {
                    printf(BLUE"  %2d/%s │"RESET, current.day, get_bengali_day_number(b_date.day)); // work here
                } else {
                    printf(" %2d/%s  │", current.day,get_bengali_day_number(b_date.day));
                }
                day++;
            }
        }
        printf("\n");
        if (day <= days_in_month) {
            printf(" ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤\n");
        }
    }
    
    printf(" └────────┴────────┴────────┴────────┴────────┴────────┴────────┘\n");
}

void display_dual_date(GregorianDate g_date) {
    printf("                    DUAL DATE DISPLAY                       \n");
    BengaliDate b_date = gregorian_to_bengali(g_date);
    
    printf("English (Gregorian): ");
    print_gregorian_date(g_date);
    printf(" (%s)\n", get_day_of_week_name(get_day_of_week(g_date)));
    
    printf("Bengali (Bangla):    ");
    print_bengali_date(b_date);
    printf("\n");
    
    long target_jdn = gregorian_to_jdn(g_date);
    int event_found = 0;
    
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn == target_jdn) {
                if (!event_found) {
                    printf("\nEvents on this date:\n");
                    event_found = 1;
                }
                printf("  • %s\n", events[i].description);
            }
        }
    }
    
    if (!event_found) {
        printf("\nNo events scheduled for this date.\n");
    }
    printf("\n");
}
