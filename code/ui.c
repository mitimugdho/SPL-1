#include <stdio.h>
#include "ui.h"
#include "bengali.h"

void print_month_grid(int month, int year) {
    printf("   Gregorian (Bengali) Calendar: %d/%d \n", month, year);
    printf(" Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
    
    // Logic to find which day of week the 1st falls on...
    
    for (int d = 1; d <= 30; d++) {
        Date g_date = {d, month, year};
        Date b_date = get_bengali_date(g_date);

        // Highlight "today" logic using ANSI colors
        // if (is_today(g_date)) printf("\033[1;31m"); 
        
        printf("%2d(%02d) ", d, b_date.day);
        
        // Reset color
        // printf("\033[0m");
    }
    printf("\n");
}