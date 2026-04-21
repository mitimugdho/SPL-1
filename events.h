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
    int is_done;
} Event;

extern Event events[MAX_EVENTS];
extern int event_count;

void init_events(void);
int add_event(GregorianDate g_date, BengaliDate b_date, const char* description);
void view_events_by_date(GregorianDate g_date);
void view_all_events();
void view_pending_events();
void view_today_events();
void mark_event_done(int event_index);
void reschedule_event(int event_index, GregorianDate new_date);
GregorianDate get_today_date();

#endif