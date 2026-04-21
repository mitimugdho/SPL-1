#include <stdio.h>
#include "gregorian.h"
#include "bengali.h"
#include "convert.h"
#include "events.h"
#include "ui.h"
#include "jdn.h"
#include<string.h>

#define EVENTS_FILE "calendar_events.txt" //"calendar_events.dat"
#define CYAN    "\033[1;36m"
#define GREEN   "\033[0;32m"
#define RESET   "\033[0m"

void print_menu() {
    printf("                        "CYAN"MAIN MENU"RESET"                              \n");
    printf("  1. English to Bengali Date Conversion (E→B)\n");
    printf("  2. Bengali to English Date Conversion (B→E)\n");
    printf("  3. Add Event/Task\n");
    printf("  4. View Events for a Specific Date\n");
    printf("  5. View All Upcoming Events\n");
    printf("  6. View Today's Reminders\n");
    printf("  7. Display Monthly Calendar (Dual)\n");
    printf("  8. Mark Event as Done\n");      
    printf("  9. Reschedule Event\n");        
    printf("  10. Exit \n");
    printf("Enter your choice: ");
}

void handle_current_calender(){
    GregorianDate today=get_today_date();
    int month=today.month;
    int year=today.year;
    display_monthly_calendar(month, year);
}
void handle_gregorian_to_bengali() {
    GregorianDate g_date;
    printf("\n"GREEN"---English to Bengali Conversion---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &g_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Enter year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf("Invalid Gregorian date!\n");
        return;
    }
    
    BengaliDate b_date = gregorian_to_bengali(g_date);
    printf("              "GREEN"ENGLISH TO BENGALI CONVERSION"RESET"               \n");
    printf("English Date: ");
    print_gregorian_date(g_date);
    printf("\nBengali Date: ");
    print_bengali_date(b_date);
    printf("\nDay of Week:  %s\n", get_day_of_week_name(get_day_of_week(g_date)));
}
void handle_bengali_to_gregorian() {
    BengaliDate b_date;
    printf("\n"GREEN"--- Bengali to English Conversion ---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &b_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &b_date.month);
    printf("Enter Bengali year : ");
    scanf("%d", &b_date.year);
    
    if (!is_valid_bengali_date(b_date)) {
        printf("Invalid Bengali date!\n");
        return;
    }
    
    GregorianDate g_date = bengali_to_gregorian(b_date);
    printf("               "GREEN"BENGALI TO ENGLISH CONVERSION"RESET"                 \n");
    printf("Bengali Date: ");
    print_bengali_date(b_date);
    printf("\nEnglish Date: ");
    print_gregorian_date(g_date);
    printf("\nDay of Week:  %s\n", get_day_of_week_name(get_day_of_week(g_date)));
}
void handle_add_event() {
    GregorianDate g_date;
    char description[MAX_EVENT_TEXT];
    
    printf("\n"GREEN"--- Add Event/Reminder ---"RESET"\n");
    printf("Enter date for event:\n");
    printf("Day: ");
    scanf("%d", &g_date.day);
    printf("Month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf("Invalid date!\n");
        return;
    }
    
    BengaliDate b_date = gregorian_to_bengali(g_date);
    
    printf("Event description: ");
    getchar();
    fgets(description, MAX_EVENT_TEXT, stdin);
    description[strcspn(description, "\n")] = 0; 
    
    if (add_event(g_date, b_date, description)) {
        printf("\n Event added successfully!\n");
        printf("Date: ");
        print_gregorian_date(g_date);
        printf(" (");
        print_bengali_date(b_date);
        printf(")\n");
        
        save_events_to_file(EVENTS_FILE);
    }
}
void handle_view_events_by_date() {
    GregorianDate g_date;
    printf("\n"GREEN"--- View Events for Specific Date ---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &g_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Enter year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf("Invalid date!\n");
        return;
    }
    
    view_events_by_date(g_date);
}
void handle_mark_done() {
    if (event_count == 0) {
        printf("\nNo events to update.\n");
        return;
    }
    view_all_events();
    printf("\nEnter event number to mark as done: ");
    int num;
    scanf("%d", &num);
    mark_event_done(num - 1);
}
void handle_reschedule() {
    if (event_count == 0) {
        printf("\nNo events to reschedule.\n");
        return;
    }
    view_pending_events();
    printf("\nEnter event number to reschedule: ");
    int num;
    scanf("%d", &num);
    int real_index = -1;
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active && !events[i].is_done) {
            found++;
            if (found == num) {
                real_index = i;
                break;
            }
        }
    }
    if (real_index == -1) {
        printf("Invalid event number.\n");
        return;
    }
    GregorianDate new_date;
    printf("Enter new date:\n");
    printf("Day: ");
    scanf("%d", &new_date.day);
    printf("Month (1-12): ");
    scanf("%d", &new_date.month);
    printf("Year: ");
    scanf("%d", &new_date.year);

    if (!is_valid_gregorian_date(new_date)) {
        printf("Invalid date!\n");
        return;
    }
    reschedule_event(real_index, new_date);
}

void handle_monthly_calendar() {
    int month, year;
    printf("\n   Display Monthly Calendar\n");
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter year: ");
    scanf("%d", &year);
    
    if (month < 1 || month > 12) {
        printf("Invalid month!\n");
        return;
    }
    display_monthly_calendar(month,year);
}
int main() {
    int choice;
    init_events();  // initialize the array

    // Load previously saved events from file
    if (load_events_from_file(EVENTS_FILE)) {
        printf("Events loaded from file.\n");
    }
    handle_current_calender();
    while (1) {
        print_menu();
        scanf("%d", &choice);
        switch(choice){
            case 1: 
                handle_gregorian_to_bengali();
                break;
            case 2:
                handle_bengali_to_gregorian();
                break;
            case 3:
                handle_add_event();
                break;
            case 4:
                handle_view_events_by_date();
                break;
            case 5:
                view_all_events();
                break;
            case 6:
                view_today_events();
                break;
            case 7: 
                handle_monthly_calendar();
                break;
            case 8: handle_mark_done(); break;      
            case 9: handle_reschedule(); break;      
            case 10:
                save_events_to_file(EVENTS_FILE);
                printf("\n Events saved. Goodbye!\n\n");
                return 0;
            default:
               printf("Invalid choice! Please enter 1-10.\n");
        }
    }    
    return 0;
}