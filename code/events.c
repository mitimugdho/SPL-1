#include "events.h"
#include "convert.h"
#include "jdn.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

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
    
    event_count++;
    return 1;
}

void view_events_by_date(GregorianDate g_date) {
    int found = 0;
    long target_jdn = gregorian_to_jdn(g_date);
    
    printf("\n=== Events for ");
    print_gregorian_date(g_date);
    BengaliDate b_date = gregorian_to_bengali(g_date);
    printf(" (");
    print_bengali_date(b_date);
    printf(") ===\n");
    
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
    
    printf("\n=== All Upcoming Events ===\n");
    int found = 0;
    
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn >= today_jdn) {
                printf("\n%d. ", found + 1);
                print_gregorian_date(events[i].g_date);
                printf(" (");
                print_bengali_date(events[i].b_date);
                printf(")\n   %s\n", events[i].description);
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("No upcoming events.\n");
    }
}

void view_today_events() {
    GregorianDate today = get_today_date();
    long today_jdn = gregorian_to_jdn(today);
    
    printf("\n=== TODAY'S REMINDERS ===\n");
    printf("Date: ");
    print_gregorian_date(today);
    BengaliDate b_today = gregorian_to_bengali(today);
    printf(" (");
    print_bengali_date(b_today);
    printf(")\n");
    
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            long event_jdn = gregorian_to_jdn(events[i].g_date);
            if (event_jdn == today_jdn) {
                printf("\n>>> %s\n", events[i].description);
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("\nNo reminders for today.\n");
    }
    printf("========================\n");
}

int save_events_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Cannot open file for writing.\n");
        return 0;
    }
    
    fprintf(file, "%d\n", event_count);
    
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            fprintf(file, "%d %d %d %d %d %d %s\n",
                    events[i].g_date.day, events[i].g_date.month, events[i].g_date.year,
                    events[i].b_date.day, events[i].b_date.month, events[i].b_date.year,
                    events[i].description);
        }
    }
    
    fclose(file);
    return 1;
}

int load_events_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 0;  // File doesn't exist yet
    }
    
    int count;
    fscanf(file, "%d\n", &count);
    
    init_events();
    
    for (int i = 0; i < count; i++) {
        GregorianDate g_date;
        BengaliDate b_date;
        char description[MAX_EVENT_TEXT];
        
        if (fscanf(file, "%d %d %d %d %d %d ",
                   &g_date.day, &g_date.month, &g_date.year,
                   &b_date.day, &b_date.month, &b_date.year) == 6) {
            fgets(description, MAX_EVENT_TEXT, file); // Remove newline
            description[strcspn(description, "\n")] = 0;
            
            add_event(g_date, b_date, description);
        }
    }
    
    fclose(file);
    return 1;
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