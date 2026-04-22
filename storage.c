#include "storage.h"
#include "convert.h"
#include "jdn.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GREEN  "\033[0;32m"
#define RED    "\033[0;31m"
#define YELLOW "\033[1;33m"
#define RESET  "\033[0m"

static int copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "r");
    if (!in) return 0;
    FILE *out = fopen(dst, "w");
    if (!out) { 
        fclose(in);
        return 0;
    }
    char buf[512];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
        fwrite(buf, 1, n, out);
    fclose(in);
    fclose(out);
    return 1;
}
int storage_save(void) {
    copy_file(EVENTS_STORAGE_FILE, BACKUP_STORAGE_FILE);

    FILE *f = fopen(EVENTS_STORAGE_FILE, "w");
    if (!f) {
        printf(RED "Error: Cannot open '%s' for writing.\n" RESET, EVENTS_STORAGE_FILE);
        return STORAGE_ERROR_WRITE_FAILED;
    }
    time_t now = time(NULL);
    char *ts = ctime(&now);
    ts[strcspn(ts, "\n")] = '\0';
    fprintf(f, "# Dual Calendar - Event Storage\n");
    fprintf(f, "# Last saved: %s\n", ts);
    fprintf(f, "# Format: g_day g_month g_year  b_day b_month b_year  is_done description\n");
    fprintf(f, "%d\n", event_count);

    int saved = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            fprintf(f, "%d %d %d  %d %d %d  %d %s\n",
                    events[i].g_date.day,
                    events[i].g_date.month,
                    events[i].g_date.year,
                    events[i].b_date.day,
                    events[i].b_date.month,
                    events[i].b_date.year,
                    events[i].is_done,
                    events[i].description);
            saved++;
        }
    }
    fclose(f);
    printf(" Saved %d event(s)\n", saved);
    return STORAGE_SUCCESS;
}
int storage_load(void) {
    FILE *f = fopen(EVENTS_STORAGE_FILE, "r");
    if (!f) return STORAGE_ERROR_FILE_NOT_FOUND;

    char line[MAX_EVENT_TEXT + 64];

    while (fgets(line, sizeof(line), f)) {
        if (line[0] != '#') break;
    }

    int count = 0;
    if (sscanf(line, "%d", &count) != 1 || count < 0) {
        fclose(f);
        return STORAGE_ERROR_INVALID_DATA;
    }

    init_events();
    int loaded=0;

    for (int i = 0; i < count; i++) {
        GregorianDate g;
        BengaliDate   b;
        int is_done = 0;
        char rest[MAX_EVENT_TEXT];

        if (fscanf(f, "%d %d %d  %d %d %d  ",
                   &g.day, &g.month, &g.year,
                   &b.day, &b.month, &b.year) != 6) break;

        if (!fgets(rest, sizeof(rest), f)) break;
        rest[strcspn(rest, "\n")] = '\0';

        char desc[MAX_EVENT_TEXT];
        if (sscanf(rest, "%d %[^\n]", &is_done, desc) == 2) {
        } else {
            is_done = 0;
            strncpy(desc, rest, MAX_EVENT_TEXT - 1);
            desc[MAX_EVENT_TEXT - 1] = '\0';
        }
        add_event(g, b, desc);
        events[event_count - 1].is_done = is_done;
        loaded++;
    }
    printf(GREEN " Loaded %d event(s)\n" RESET,loaded);
    fclose(f);
    return STORAGE_SUCCESS;
}
int storage_add_event(GregorianDate g_date, BengaliDate b_date, const char *description) {
    if (!add_event(g_date, b_date, description))
        return STORAGE_ERROR_INVALID_DATA;
    return storage_save();
}
static int pending_display_to_real(int display_num) {
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active && !events[i].is_done) {
            found++;
            if (found == display_num) return i;
        }
    }
    return -1;
}
int storage_mark_done(int display_index) {
    int real = pending_display_to_real(display_index);
    if (real == -1) {
        printf(RED "Invalid event number.\n" RESET);
        return STORAGE_ERROR_INVALID_DATA;
    }
    mark_event_done(real);
    return storage_save();
}
int storage_reschedule(int display_index, GregorianDate new_date) {
    int real = pending_display_to_real(display_index);
    if (real == -2) {
        printf(RED "Invalid event number.\n" RESET);
        return STORAGE_ERROR_INVALID_DATA;
    }
    reschedule_event(real, new_date);  
    return storage_save();
}
int storage_delete_event(int display_index) {
    int found = 0, real = -1;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active) {
            found++;
            if (found == display_index) { 
                real = i; 
                break;
            }
        }
    }
    if (real == -1) {
        printf(RED "Invalid event number.\n" RESET);
        return STORAGE_ERROR_INVALID_DATA;
    }
    printf(YELLOW "Delete: \"%s\" ? (y/n): " RESET,
           events[real].description);
    char c;
    scanf(" %c", &c);
    if (c != 'y' && c != 'Y') {
        printf("Cancelled.\n");
        return STORAGE_SUCCESS;
    }
    events[real].is_active = 0;
    printf(GREEN " Event deleted.\n" RESET);
    return storage_save();
}
int storage_clear_all(void) {
    printf(RED "WARNING: This will delete ALL events and cannot be undone.\n" RESET);
    printf("Type YES to confirm: ");
    char buf[10];
    scanf("%9s", buf);
    if (strcmp(buf, "YES") != 0) {
        printf("Cancelled.\n");
        return STORAGE_SUCCESS;
    }
    init_events();
    printf(GREEN " All events cleared.\n" RESET);
    return storage_save();
}
StorageStats storage_get_stats(void) {
    StorageStats s = {0, 0, 0, 0};
    GregorianDate today = get_today_date();
    long today_jdn = gregorian_to_jdn(today);

    for (int i = 0; i < event_count; i++) {
        if (!events[i].is_active) continue;
        s.total_events++;
        if (events[i].is_done) {
            s.done_events++;
        } else {
            s.pending_events++;
            if (gregorian_to_jdn(events[i].g_date) < today_jdn)
                s.overdue_events++;
        }
    }
    return s;
}
void storage_print_stats(void) {
    StorageStats s = storage_get_stats();
    printf("  Events: %d total  |  %d pending  |  %d done",
            s.total_events, s.pending_events, s.done_events);
    if (s.overdue_events > 0)
        printf(RED "  |  %d OVERDUE" RESET, s.overdue_events);
    printf("\n");
}