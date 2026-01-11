#ifndef STORAGE_H
#define STORAGE_H

#include "utils.h"
#define STORAGE_FILE "events.txt"

int storage_add_event(Event event);
int storage_load_all(Event *buffer, int max_size);
int storage_get_by_date(Date date, Event *buffer, int max_size);
int storage_check_reminders(Date today); // event reminder checking everyday

#endif