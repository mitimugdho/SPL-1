#include "ui.h"
#include "gregorian.h"
#include "bengali.h"
#include "convert.h"
#include "events.h"
#include "jdn.h"
#include <stdio.h>

#define BLUE  "\033[1;34m"
#define RESET "\033[0m"

void display_monthly_calendar(int month, int year) {
    printf("\n");
    printf("             %s %d  Dual Calendar View                   \n", 
           get_gregorian_month_name(month), year);
    int name_len = 0;
    const char* month_name = get_gregorian_month_name(month);
    while (month_name[name_len] != '\0') name_len++;
    
    int year_len = (year >= 1000) ? 4 : (year >= 100) ? 3 : (year >= 10) ? 2 : 1;
    int total_len = name_len + year_len + 1; 
    int spaces_needed = 13 - total_len;
    
    for (int i = 0; i < spaces_needed; i++) printf(" ");
    GregorianDate today = get_today_date();
    GregorianDate first_day;
    first_day.day = 1;
    first_day.month = month;
    first_day.year = year;
    
    int start_dow = get_day_of_week(first_day);
    int days_in_month = get_gregorian_days_in_month(month, year);
    printf("┌────────┬────────┬────────┬────────┬────────┬────────┬────────┐\n");
    printf(" │  Sun   │  Mon   │  Tue   │  Wed   │  Thu   │  Fri   │  Sat   │\n");
    printf(" ├────────┼────────┼────────┼────────┼────────┼────────┼────────┤\n");
    int day = 1;
    for (int week = 0; week < 6 && day <= days_in_month; week++) {
        printf(" │");
        for (int dow = 0; dow < 7; dow++) {
            if ((week == 0 && dow < start_dow) || day > days_in_month) {
                printf("        │");
            } else {
                GregorianDate current;
                current.day = day;
                current.month = month;
                current.year = year;
                
                BengaliDate b_date = gregorian_to_bengali(current);
                b_date.day= day;
                int is_today = (current.day == today.day && 
                               current.month == today.month && 
                               current.year == today.year);    
                if (is_today) {
                    printf(BLUE"  %2d    │"RESET, current.day );
                } else {
                    printf(" %2d     │", current.day);
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
    //printf("\nFormat: Gregorian/Bengali 
    //printf("Bengali months approximate: Check specific date for exact Bengali date\n");
}

void display_dual_date(GregorianDate g_date) {
    printf("\n╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    DUAL DATE DISPLAY                       ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");
    
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