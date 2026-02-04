#ifndef EVENTS_H
#define EVENTS_H

#include "gregorian.h"
#include "bengali.h"

#define MAX_EVENTS 100
#define MAX_EVENT_TEXT 200

typedef struct {
    GregorianDate g_date;
    BengaliDate b_date;
    char description[MAX_EVENT_TEXT];
    int is_active;
} Event;

extern Event events[MAX_EVENTS];
extern int event_count;

void init_events();
int add_event(GregorianDate g_date, BengaliDate b_date, const char* description);
void view_events_by_date(GregorianDate g_date);
void view_all_events();
void view_today_events();
int save_events_to_file(const char* filename);
int load_events_from_file(const char* filename);
GregorianDate get_today_date();

#endif