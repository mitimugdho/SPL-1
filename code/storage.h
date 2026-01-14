#ifndef STORAGE_H
#define STORAGE_H

#include "gregorian.h"
#include "bengali.h"
#include "events.h"

#define EVENTS_STORAGE_FILE "calendar_events.dat"
#define CONFIG_STORAGE_FILE "calendar_configuration.dat"
#define BACKUP_STORAGE_FILE "calendar_backup.dat"

#define STORAGE_SUCCESS 0
#define STORAGE_ERROR_FILE_NOT_FOUND 1
#define STORAGE_ERROR_WRITE_FAILED 2
#define STORAGE_ERROR_READ_FAILED 3
#define STORAGE_ERROR_INVALID_DATA 4

typedef struct {
    int total_events;
    int active_events;
    int past_events;
    int future_events;
    long file_size;
} StorageStats;
void init_storage();
int storage_save_events(const char* filename);
int storage_load_events(const char* filename);
int storage_add_event(GregorianDate g_date, BengaliDate b_date, const char* description);
int storage_delete_event(int event_index);
int storage_clear_all_events();

#endif
