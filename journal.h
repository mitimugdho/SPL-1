#ifndef JOURNAL_H
#define JOURNAL_H

#include "gregorian.h"

#define JOURNAL_FILE "journal_entries.txt"
#define MAX_JOURNAL_ENTRIES 365
#define JOURNAL_NOTE_LEN 150
#define DEFAULT_CALORIE_GOAL 2000

#define JOURNAL_SUCCESS 0
#define JOURNAL_ERROR_FULL 1
#define JOURNAL_ERROR_WRITE_FAILED 2
#define JOURNAL_ERROR_READ_FAILED 3
#define JOURNAL_ERROR_NOT_FOUND 4
#define JOURNAL_ERROR_DUPLICATE 5

typedef struct {
    GregorianDate date;
    int  calories;
    char note[JOURNAL_NOTE_LEN];
    int  is_active;
} JournalEntry;

extern JournalEntry journal[MAX_JOURNAL_ENTRIES];
extern int journal_count;
extern int calorie_goal;

void journal_init(void);
int  journal_add(GregorianDate date, int calories, const char *note);
int  journal_edit(int display_index, int calories, const char *note);
int  journal_delete(int display_index);
void journal_view_today(void);
void journal_view_all(void);
void journal_view_weekly_summary(void);
int  journal_save(void);
int  journal_load(void);
//void journal_print_stats(void);
int  journal_find_by_date(GregorianDate date);
int  journal_dates_equal(GregorianDate a, GregorianDate b);

#endif