#include "journal.h"
#include "gregorian.h"
#include "events.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GREEN  "\033[0;32m"
#define YELLOW "\033[1;33m"
#define CYAN   "\033[1;36m"
#define RED    "\033[31m"
#define RESET  "\033[0m"

JournalEntry journal[MAX_JOURNAL_ENTRIES];
int          journal_count = 0;
int          calorie_goal  = DEFAULT_CALORIE_GOAL;

int journal_dates_equal(GregorianDate a, GregorianDate b) {
    return (a.day == b.day && a.month == b.month && a.year == b.year);
}
int journal_find_by_date(GregorianDate date) {
    for (int i = 0; i < journal_count; i++)
        if (journal[i].is_active && journal_dates_equal(journal[i].date, date))
            return i;
    return -1;
}
static int resolve_index(int display_index) {
    int found = 0;
    for (int i = 0; i < journal_count; i++)
        if (journal[i].is_active && ++found == display_index)
            return i;
    return -1;
}
static void print_entry(const JournalEntry *e, int num) {
    printf("\n%d. ", num);
    print_gregorian_date(e->date);

    int diff = e->calories - calorie_goal;
    const char *status, *color;
    if      (diff <= 0)   { status = "Within goal";   color = GREEN;  }
    else if (diff < 200)  { status = "Slightly over"; color = YELLOW; }
    else                  { status = "Over goal";      color = RED;    }

    printf("\n   " CYAN "Calories: " RESET "%d / %d kcal  %s[%s]" RESET "\n",
           e->calories, calorie_goal, color, status);
    if (e->note[0] != '\0')
        printf("   " CYAN "Note    : " RESET "%s\n", e->note);
}
void journal_init(void) {
    journal_count = 0;
    for (int i = 0; i < MAX_JOURNAL_ENTRIES; i++)
        journal[i].is_active = 0;
}

int journal_add(GregorianDate date, int calories, const char *note) {
    if (journal_count >= MAX_JOURNAL_ENTRIES) {
        printf(RED "Journal full!\n" RESET);
        return JOURNAL_ERROR_FULL;
    }
    if (journal_find_by_date(date) != -1) {
        printf(YELLOW "Entry already exists for this date. Use Edit to update.\n" RESET);
        return JOURNAL_ERROR_DUPLICATE;
    }
    if (calories < 0) calories = 0;

    journal[journal_count].date      = date;
    journal[journal_count].calories  = calories;
    journal[journal_count].is_active = 1;
    strncpy(journal[journal_count].note, note ? note : "", JOURNAL_NOTE_LEN - 1);
    journal[journal_count].note[JOURNAL_NOTE_LEN - 1] = '\0';
    journal_count++;

    int diff = calories - calorie_goal;
    if (diff <= 0)
        printf(GREEN "Entry added! You're within your %d kcal goal.\n" RESET, calorie_goal);
    else
        printf(YELLOW "Entry added! You're %d kcal over your goal today.\n" RESET, diff);
    return JOURNAL_SUCCESS;
}

int journal_edit(int display_index, int calories, const char *note) {
    int idx = resolve_index(display_index);
    if (idx == -1) { printf(RED "Invalid entry number.\n" RESET); return JOURNAL_ERROR_NOT_FOUND; }
    if (calories >= 0) journal[idx].calories = calories;
    if (note && note[0] != '\0') {
        strncpy(journal[idx].note, note, JOURNAL_NOTE_LEN - 1);
        journal[idx].note[JOURNAL_NOTE_LEN - 1] = '\0';
    }
    printf(GREEN "Entry updated.\n" RESET);
    return JOURNAL_SUCCESS;
}

int journal_delete(int display_index) {
    int idx = resolve_index(display_index);
    if (idx == -1) { printf(RED "Invalid entry number.\n" RESET); return JOURNAL_ERROR_NOT_FOUND; }
    journal[idx].is_active = 0;
    printf(GREEN "Entry deleted.\n" RESET);
    return JOURNAL_SUCCESS;
}

void journal_view_today(void) {
    GregorianDate today = get_today_date();
    printf("\n" GREEN "=== TODAY'S JOURNAL ===" RESET "\nDate: ");
    print_gregorian_date(today);
    printf("\n");
    int idx = journal_find_by_date(today);
    if (idx == -1)
        printf(YELLOW "No entry for today. Use 'Add Journal Entry'.\n" RESET);
    else
        print_entry(&journal[idx], 1);
    printf("\n");
}
void journal_view_all(void) {
    if (journal_count == 0) { printf(YELLOW "\nNo journal entries.\n" RESET); return; }
    printf("\n" GREEN "*** All Journal Entries ***" RESET "\n");
    int num = 0;
    for (int i = journal_count - 1; i >= 0; i--)
        if (journal[i].is_active) print_entry(&journal[i], ++num);
    if (num == 0) printf(YELLOW "No active entries.\n" RESET);
}
void journal_view_weekly_summary(void) {
    printf("\n" GREEN "*** Weekly Summary (Last 7 Days) ***" RESET "\n");
    time_t now = time(NULL);
    int total = 0, days = 0;
    for (int d = 0; d < 7; d++) {
        time_t t = now - (time_t)d * 86400;
        struct tm *tm_info = localtime(&t);
        GregorianDate day = { tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900 };
        int idx = journal_find_by_date(day);
        if (idx != -1) { total += journal[idx].calories; days++; }
    }
    if (days == 0) { printf(YELLOW "No entries in the last 7 days.\n" RESET); return; }
    int avg = total / days;
    printf("  Days logged  : %d / 7\n", days);
    printf("  Calorie goal : %d kcal/day\n", calorie_goal);
    printf("  Avg Calories : %d kcal/day  ", avg);
    if (avg <= calorie_goal)
        printf(GREEN "[On track! Avg is within goal]\n" RESET);
    else
        printf(RED "[%.0f%% over goal on average]\n" RESET,
               (double)(avg - calorie_goal) / calorie_goal * 100);
    printf("  Total        : %d kcal\n\n", total);
}

int journal_save(void) {
    FILE *fp = fopen(JOURNAL_FILE, "w");
    if (!fp) { printf(RED "Error: Could not write journal.\n" RESET); return JOURNAL_ERROR_WRITE_FAILED; }
    fprintf(fp, "GOAL %d\n", calorie_goal);
    for (int i = 0; i < journal_count; i++)
        if (journal[i].is_active)
            fprintf(fp, "%d %d %d %d \"%s\"\n",
                    journal[i].date.day, journal[i].date.month, journal[i].date.year,
                    journal[i].calories, journal[i].note);
    fclose(fp);
    return JOURNAL_SUCCESS;
}

int journal_load(void) {
    FILE *fp = fopen(JOURNAL_FILE, "r");
    if (!fp) return JOURNAL_ERROR_READ_FAILED;
    journal_init();
    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "GOAL", 4) == 0) { sscanf(line, "GOAL %d", &calorie_goal); continue; }
        if (journal_count >= MAX_JOURNAL_ENTRIES) break;
        int day, month, year, cal;
        char note[JOURNAL_NOTE_LEN] = {0};
        int fields = sscanf(line, "%d %d %d %d \"%149[^\"]\"", &day, &month, &year, &cal, note);
        if (fields < 4) continue;
        journal[journal_count].date.day   = day;
        journal[journal_count].date.month = month;
        journal[journal_count].date.year  = year;
        journal[journal_count].calories   = cal;
        journal[journal_count].is_active  = 1;
        strncpy(journal[journal_count].note, (fields == 5) ? note : "", JOURNAL_NOTE_LEN - 1);
        journal[journal_count].note[JOURNAL_NOTE_LEN - 1] = '\0';
        journal_count++;
    }
    fclose(fp);
    return JOURNAL_SUCCESS;
}

void journal_print_stats(void) {
    int active = 0;
    for (int i = 0; i < journal_count; i++) if (journal[i].is_active) active++;
    printf(CYAN "Journal:" RESET " %d entr%s loaded  |  Calorie goal: %d kcal/day\n",
           active, active == 1 ? "y" : "ies", calorie_goal);
}