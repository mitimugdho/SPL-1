#include "events.h"
#include "convert.h"
#include "jdn.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GREEN   "\033[0;32m"
#define RESET   "\033[0m"

Event events[MAX_EVENTS];
int event_count = 0;

void init_events() {
    event_count = 0;
    for (int i = 0; i < MAX_EVENTS; i++) {
        events[i].is_active = 0;
    }
}
int add_event(GregorianDate g_date, BengaliDate b_date, const char* description) {
    if (event_count >= MAX_EVENTS) {
        printf("Error: Maximum event limit reached!\n");
        return 0;
    }
    events[event_count].g_date = g_date;
    events[event_count].b_date = b_date;
    strncpy(events[event_count].description, description, MAX_EVENT_TEXT - 1);
    events[event_count].description[MAX_EVENT_TEXT - 1] = '\0';
    events[event_count].is_active = 1;
    events[event_count].is_done = 0;   

    event_count++;
    return 1;
}
void view_events_by_date(GregorianDate g_date) {
    int found = 0;
    long target_jdn = gregorian_to_jdn(g_date);
    
    printf("\n"GREEN"*** Events for"RESET" ");
    print_gregorian_date(g_date);
    BengaliDate b_date = gregorian_to_bengali(g_date);
    printf(" (");
    print_bengali_date(b_date);
    printf(") "GREEN"***"RESET"\n");
    
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn == target_jdn) {
                printf("\n%d. %s\n", found + 1, events[i].description);
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("No events found for this date.\n");
    }
}
void view_all_events() {
    if (event_count == 0) {
        printf("\nNo events stored.\n");
        return;
    }
    GregorianDate today = get_today_date();
    long today_jdn = gregorian_to_jdn(today);
    printf("\n"GREEN"*** All Upcoming Events ***"RESET"\n");
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn >= today_jdn) {
                printf("\n%d. ", found + 1);
                print_gregorian_date(events[i].g_date);
                printf(" (");
                print_bengali_date(events[i].b_date);
                char* string;
                if(events[i].is_done) {
                    string = "DONE";
                }
                else{
                    string = "PENDING";
                }
                printf(")\n   %s [%s]\n", events[i].description,
                       string);  
                found++;
            }
        }
    }
    if (found == 0) {
        printf("No upcoming events.\n");
    }
}
void view_pending_events() {
    if (event_count == 0) {
        printf("\nNo events stored.\n");
        return;
    }

    GregorianDate today = get_today_date();
    long today_jdn = gregorian_to_jdn(today);

    printf("\n"GREEN"*** Pending Events (Overdue + Upcoming) ***"RESET"\n");
    int found = 0;

    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active && !events[i].is_done) {
            printf("\n%d. ", found + 1);
            print_gregorian_date(events[i].g_date);
            printf(" (");
            print_bengali_date(events[i].b_date);
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            printf(")\n   %s [%s]\n", events[i].description,
                   event_jdn < today_jdn ? "OVERDUE" : "PENDING");
            found++;
        }
    }

    if (found == 0) {
        printf("No pending events.\n");
    }
}
void view_today_events() {
    GregorianDate today = get_today_date();
    long today_jdn = gregorian_to_jdn(today);
    
    printf("\n"GREEN"=== TODAY'S REMINDERS ==="RESET"\n");
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn == today_jdn) {
                printf("  • %s\n", events[i].description);
                found++;
            }
        }
    }
    if (found == 0) {
        printf("\nNo reminders for today.\n");
    }
}
void mark_event_done(int event_index) {
    if (event_index < 0 || event_index >= event_count ||
        !events[event_index].is_active) {
        printf("Error: Invalid event number.\n");
        return;
    }
    events[event_index].is_done = 1;
    printf("\nEvent marked as DONE: %s\n", events[event_index].description);
}
void reschedule_event(int event_index, GregorianDate new_date) {
    if (event_index < 0 || event_index >= event_count || !events[event_index].is_active) {
        printf("Error: Invalid event number.\n");
        return;
    }
    else if(event_index == -1){
        printf("Rescheduling cancelled.\n");
        return;
    }
    events[event_index].g_date = new_date;
    events[event_index].b_date = gregorian_to_bengali(new_date);
    events[event_index].is_done = 0;  // reseting to pending on reschedule
    printf("\nEvent rescheduled: %s\nNew date: ", events[event_index].description);
    print_gregorian_date(new_date);
    printf(" (");
    print_bengali_date(events[event_index].b_date);
    printf(")\n");   
}
GregorianDate get_today_date() {
    GregorianDate today;
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    
    today.day = tm_info->tm_mday;
    today.month = tm_info->tm_mon + 1;
    today.year = tm_info->tm_year + 1900;
    
    return today;
}