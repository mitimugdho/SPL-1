#ifndef STORAGE_H
#define STORAGE_H

#include "gregorian.h"
#include "bengali.h"
#include "events.h"

#define EVENTS_STORAGE_FILE "calendar_events.txt"
#define BACKUP_STORAGE_FILE "calendar_events_backup.txt" 

#define STORAGE_SUCCESS 0
#define STORAGE_ERROR_FILE_NOT_FOUND 1
#define STORAGE_ERROR_WRITE_FAILED 2
#define STORAGE_ERROR_READ_FAILED 3
#define STORAGE_ERROR_INVALID_DATA 4

typedef struct {
    int total_events;
    int pending_events;
    int done_events;
    int overdue_events;
} StorageStats;

int  storage_save(void);
int  storage_load(void);

int storage_add_event(GregorianDate g_date, BengaliDate b_date, const char* description);
int  storage_mark_done(int display_index);
int  storage_reschedule(int display_index, GregorianDate new_date);
int  storage_delete_event(int display_index);
int  storage_clear_all(void);

StorageStats storage_get_stats(void);
void storage_print_stats(void);

#endif