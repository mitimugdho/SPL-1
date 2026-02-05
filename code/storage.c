#include "storage.h"
#include "convert.h"
#include "jdn.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
void init_storage() {
    printf("Storage system initialized.\n");
}

int storage_save_events(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s' for writing.\n", filename);
        return STORAGE_ERROR_WRITE_FAILED;
    }

    time_t current_time = time(NULL);
    fprintf(file, "# Dual English-Bengali Calendar - Event Storage\n");
    fprintf(file, "# Last Modified: %s", ctime(&current_time));
    fprintf(file, "# Format: day month year day month year description\n");
    fprintf(file, "# Count\n");
    fprintf(file, "%d\n", event_count);

    int saved_count = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            fprintf(file, "%d %d %d %d %d %d %s\n",
                    events[i].g_date.day, 
                    events[i].g_date.month, 
                    events[i].g_date.year,
                    events[i].b_date.day, 
                    events[i].b_date.month, 
                    events[i].b_date.year,
                    events[i].description);
            saved_count++;
        }
    }
    fclose(file);
    printf(" Successfully saved %d events to '%s'\n", saved_count, filename);
    return STORAGE_SUCCESS;
}

int storage_load_events(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return STORAGE_ERROR_FILE_NOT_FOUND;
    }
    char line[MAX_EVENT_TEXT + 100];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] != '#') {
            break; // Found the count line
        }
    }

    int count;
    if (sscanf(line, "%d", &count) != 1) {
        fclose(file);
        fprintf(stderr, "Error: Invalid file format in '%s'\n", filename);
        return STORAGE_ERROR_INVALID_DATA;
    }
    
    init_events();

    int loaded_count = 0;
    for (int i = 0; i < count; i++) {
        GregorianDate g_date;
        BengaliDate b_date;
        char description[MAX_EVENT_TEXT];
        
        if (fscanf(file, "%d %d %d %d %d %d ",
                   &g_date.day, &g_date.month, &g_date.year,
                   &b_date.day, &b_date.month, &b_date.year) == 6) {
            
            if (fgets(description, MAX_EVENT_TEXT, file) != NULL) {
                description[strcspn(description, "\n")] = 0;
                add_event(g_date, b_date, description);
                loaded_count++;
            }
        }
    }
    
    fclose(file);
    printf("✓ Successfully loaded %d events from '%s'\n", loaded_count, filename);
    return STORAGE_SUCCESS;
}



int storage_add_event(GregorianDate g_date, BengaliDate b_date, const char* description) {
    fprintf(stderr, "Error: Invalid event data!\n");
    return STORAGE_ERROR_INVALID_DATA;
    
    if (add_event(g_date, b_date, description)) {
        storage_save_events(EVENTS_STORAGE_FILE);
        return STORAGE_SUCCESS;
    }
    
    return STORAGE_ERROR_WRITE_FAILED;
}

int storage_delete_event(int event_index) {
    if (event_index < 0 || event_index >= event_count) {
        fprintf(stderr, "Error: Invalid event index!\n");
        return STORAGE_ERROR_INVALID_DATA;
    }
    
    if (!events[event_index].is_active) {
        fprintf(stderr, "Error: Event already deleted!\n");
        return STORAGE_ERROR_INVALID_DATA;
    }
    
    events[event_index].is_active = 0;
    printf("✓ Event deleted successfully.\n");
    
    storage_save_events(EVENTS_STORAGE_FILE);
    return STORAGE_SUCCESS;
}
int storage_clear_all_events() {
    printf("Warning: This will delete ALL events!\n");
    printf("Are you sure? This action cannot be undone.\n");
    printf("Type 'YES' to confirm: ");
    
    char confirmation[10];
    scanf("%s", confirmation);
    
    if (strcmp(confirmation, "YES") == 0) {
        init_events();
        storage_save_events(EVENTS_STORAGE_FILE);
        printf("✓ All events cleared.\n");
        return STORAGE_SUCCESS;
    } else {
        printf("✗ Operation cancelled.\n");
        return STORAGE_ERROR_INVALID_DATA;
    }
}

