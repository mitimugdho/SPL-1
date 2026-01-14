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
#define STORAGE_ERROR_PERMISSION_DENIED 5

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
int storage_backup_events(const char* backup_filename);
int storage_restore_events(const char* backup_filename);
int storage_add_event(GregorianDate g_date, BengaliDate b_date, const char* description);
int storage_delete_event(int event_index);
int storage_update_event(int event_index, const char* new_description);
int storage_get_stats(StorageStats* stats);
int storage_clear_all_events();
int storage_export_to_text(const char* output_filename);
int storage_import_from_text(const char* input_filename);
int storage_file_exists(const char* filename);
long storage_get_file_size(const char* filename);
int storage_delete_file(const char* filename);
int storage_validate_event_data(GregorianDate g_date, BengaliDate b_date);
int storage_check_integrity();

#endif
