#include <stdio.h>
#include "gregorian.h"
#include "bengali.h"
#include "convert.h"
#include "events.h"
#include "storage.h"
#include "ui.h"
#include "jdn.h"
#include "journal.h"
#include<string.h>

#define EVENTS_FILE "calendar_events.txt" 
#define CYAN    "\033[1;36m"
#define GREEN   "\033[0;32m"
#define RED     "\033[31m"
#define RESET   "\033[0m"

void print_menu() {
    printf("                      "CYAN"MAIN MENU"RESET"                              \n");
    printf("  1. English to Bengali Date Conversion (E→B)\n");
    printf("  2. Bengali to English Date Conversion (B→E)\n");
    printf("  3. Add Event/Task\n");
    printf("  4. View Events for a Specific Date\n");
    printf("  5. View All Upcoming Events\n");
    printf("  6. Display Monthly Calendar (Dual)\n");
    printf("  7. Mark Event as Done\n");      
    printf("  8. View & Reschedule Pending Event\n");  
    printf("  9. Delete Event\n");  
    printf("  10. Journal Entry\n");    
    printf("  11. Exit \n");
    printf("Enter your choice: ");
}

void handle_current_calender(){
    GregorianDate today=get_today_date();
    int month=today.month;
    int year=today.year;
    display_monthly_calendar(month, year);
}
void handle_gregorian_to_bengali() {
    GregorianDate g_date;
    printf("\n"GREEN"---English to Bengali Conversion---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &g_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Enter year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf(RED"Invalid Gregorian date!\n"RESET);
        return;
    }
    printf("              "GREEN"ENGLISH TO BENGALI CONVERSION"RESET"               \n");
    display_dual_date(g_date);
}
void handle_bengali_to_gregorian() {
    BengaliDate b_date;
    printf("\n"GREEN"--- Bengali to English Conversion ---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &b_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &b_date.month);
    printf("Enter Bengali year : ");
    scanf("%d", &b_date.year);
    
    if (!is_valid_bengali_date(b_date)) {
        printf(RED"Invalid Bengali date!\n"RESET);
        return;
    }
    
    GregorianDate g_date = bengali_to_gregorian(b_date);
    printf("               "GREEN"BENGALI TO ENGLISH CONVERSION"RESET"                 \n");
    printf("Bengali Date: ");
    print_bengali_date(b_date);
    printf("\nEnglish Date: ");
    print_gregorian_date(g_date);
    printf("\nDay of Week:  %s\n", get_day_of_week_name(get_day_of_week(g_date)));
}
void handle_add_event() {
    GregorianDate g_date;
    char description[MAX_EVENT_TEXT];
    
    printf("\n"GREEN"--- Add Event/Task ---"RESET"\n");
    printf("Enter date:\n");
    printf("Day: ");
    scanf("%d", &g_date.day);
    printf("Month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf(RED"Invalid date!\n"RESET);
        return;
    }
    
    BengaliDate b_date = gregorian_to_bengali(g_date);
    
    printf("Event description: ");
    getchar();
    fgets(description, MAX_EVENT_TEXT, stdin);
    description[strcspn(description, "\n")] = 0; 
    if (storage_add_event(g_date, b_date, description) == STORAGE_SUCCESS) {
        printf("\n Event added:\n  ");
        print_gregorian_date(g_date);
        printf(" (");
        print_bengali_date(b_date);
        printf(")\n");
    }
}
void handle_view_events_by_date() {
    GregorianDate g_date;
    printf("\n"GREEN"--- View Events for Specific Date ---"RESET"\n");
    printf("Enter day: ");
    scanf("%d", &g_date.day);
    printf("Enter month (1-12): ");
    scanf("%d", &g_date.month);
    printf("Enter year: ");
    scanf("%d", &g_date.year);
    
    if (!is_valid_gregorian_date(g_date)) {
        printf(RED"Invalid date!\n"RESET);
        return;
    }
    
    view_events_by_date(g_date);
}
void handle_monthly_calendar() {
    int month, year;
    printf("\n   Display Monthly Calendar\n");
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    printf("Enter year: ");
    scanf("%d", &year);

    if(month >0 && month <13 && year > 0){
        display_monthly_calendar(month,year);
    }
    else{
        printf(RED"Invalid month or year!\n"RESET);
        clear_input_buffer();
        return;
    }
}
void handle_mark_done() {
    if (event_count == 0) {
        printf("\nNo events to update.\n");
        return;
    }
    view_pending_events();
    printf("\nEnter event number to mark as done: ");
    int num;
    if (scanf("%d", &num) != 1) {        
        printf(RED"Invalid input!\n"RESET);
        clear_input_buffer();
        return;
    }
    if (num > 0 && num <= event_count) {  
        storage_mark_done(num);
    } else {
        printf(RED"Invalid event number!\n"RESET);
    }
}
void handle_reschedule() {
    if (event_count == 0) {
        printf("\nNo events to reschedule.\n");
        return;
    }
    view_pending_events();
    printf("\nEvent number to reschedule(0 to cancel): ");
    int num;
    scanf("%d", &num);
    if(num == 0){
        printf("Rescheduling cancelled.\n");
        return;
    }
    int real_index = -1; 
    int found = 0;
    for (int i = 0; i < event_count; i++) {
        if (events[i].is_active && !events[i].is_done) {
            found++;
            if (found == num) {
                real_index = i;
                break;
            }
        }
    }
    if (real_index == -1) {
        printf(RED"Invalid event number.\n"RESET);
        return;
    }
    GregorianDate new_date;
    printf("Enter new date:\n");
    printf("Day: ");
    scanf("%d", &new_date.day);
    printf("Month (1-12): ");
    scanf("%d", &new_date.month);
    printf("Year: ");
    scanf("%d", &new_date.year);

    if (!is_valid_gregorian_date(new_date)) {
        printf(RED"Invalid date!\n"RESET);
        return;
    }
    storage_reschedule(num, new_date);
}
void handle_delete_event() {
    if (event_count == 0) { 
        printf("\nNo events to delete.\n");
         return;
        }
    view_all_events();
    printf("\nEnter event number to delete: ");
    int num; scanf("%d", &num);
    storage_delete_event(num);
}

void handle_add_journal_entry() {
    printf("\n" GREEN "--- Calories Consumed ---" RESET "\n");
    GregorianDate date;
    printf("Log for today? (1=Yes / 0=Enter date): ");
    int use_today; 
    scanf("%d", &use_today);
    if (use_today) {
        date = get_today_date();
    } 
    else {
        printf("Day: ");   scanf("%d", &date.day);
        printf("Month: "); scanf("%d", &date.month);
        printf("Year: ");  scanf("%d", &date.year);
        if (!is_valid_gregorian_date(date)) { 
            printf("Invalid date!\n"); 
            return;
        }
    }
    int calories;
    printf("Calories consumed today?(kcal/day): "); 
    if (scanf("%d", &calories) != 1) {
        printf(RED "Invalid input! Calories must be a number.\n" RESET);
        clear_input_buffer();
        return;
    }
    if (calories < 0) {
        printf(RED "Calories cannot be negative!\n" RESET);
        return;
    }
    char note[JOURNAL_NOTE_LEN];
    printf("Optional note (Enter to skip): ");
    getchar();
    fgets(note, JOURNAL_NOTE_LEN, stdin);
    note[strcspn(note, "\n")] = 0;
    journal_add(date, calories, note);
    journal_save();
}
void handle_edit_journal_entry() {
    if (journal_count == 0) { printf("\nNo journal entries to edit.\n"); return; }
    journal_view_all();
    printf("\nEnter entry number to edit: ");
    int num; scanf("%d", &num);
    int calories;
    printf("New calories (-1 to keep current): "); scanf("%d", &calories);
    char note[JOURNAL_NOTE_LEN];
    printf("New note (Enter to keep current): ");
    getchar();
    fgets(note, JOURNAL_NOTE_LEN, stdin);
    note[strcspn(note, "\n")] = 0;
    journal_edit(num, calories, note);
    journal_save();
}
void handle_delete_journal_entry() {
    if (journal_count == 0) { printf("\nNo journal entries to delete.\n"); return; }
    journal_view_all();
    printf("\nEnter entry number to delete: ");
    int num; scanf("%d", &num);
    journal_delete(num);
    journal_save();
}
void handle_set_calorie_goal() {
    printf("\n" GREEN "--- Set Calorie Goal ---" RESET "\n");
    printf("Current goal: %d kcal/day\n", calorie_goal);
    printf("New calorie goal: "); scanf("%d", &calorie_goal);
    if (calorie_goal <= 0) calorie_goal = DEFAULT_CALORIE_GOAL;
    printf(GREEN "Goal set to %d kcal/day.\n" RESET, calorie_goal);
    journal_save();
}
int main() {
    int choice;
    char q;
    init_events();  // initialize the array

    int loaded = storage_load();
    journal_init();
    journal_load();
    handle_current_calender();
    if (loaded == STORAGE_SUCCESS)
        storage_print_stats();
    else
        printf("No saved events found. Starting fresh.\n");
    view_today_events();
    while (1) {
        print_menu();
        scanf("%d", &choice);
        switch(choice){
            case 1: 
                handle_gregorian_to_bengali();
                break;
            case 2:
                handle_bengali_to_gregorian();
                break;
            case 3:
                handle_add_event();
                break;
            case 4:
                handle_view_events_by_date();
                break;
            case 5:
                view_all_events();
                break;
            case 6: 
                handle_monthly_calendar();
                break;
            case 7: handle_mark_done(); break;      
            case 8: handle_reschedule(); break;  
            case 9: 
                printf("1. Delete one or 2. All?\n");
                int p;
                scanf("%d",&p);
                if(p==1) handle_delete_event();    
                else if(p==2) storage_clear_all();
                else printf("Invalid input.");
                break;
            case 10:
                q = 'a';
                while(q!='h'){
                    printf("       "CYAN"JOUNAL MENU"RESET"      \n");
                    printf("a. Add journal entry\n");
                    printf("b. View Today's Journal\n");
                    printf("c. View All Journal Entries\n");
                    printf("d. Weekly Calorie Summary\n");
                    printf("e. Edit Journal Entry\n");
                    printf("f. Delete Journal Entry\n");
                    printf("g. Set Calorie Goal\n");
                    printf("h. Exit to main menu:\n");
                    printf("Enter journal choice:");
                    scanf(" %c",&q);
                    switch(q){
                        case 'a': handle_add_journal_entry();
                            break;
                        case 'b': journal_view_today();           
                            break;
                        case 'c': journal_view_all();             
                            break;
                        case 'd': journal_view_weekly_summary();  
                            break;
                        case 'e': handle_edit_journal_entry();    
                            break;
                        case 'f': handle_delete_journal_entry();  
                            break;
                        case 'g': handle_set_calorie_goal();      
                            break;
                        case 'h': printf("Returning to main menu...\n");
                            break;
                        default: printf(RED"Invalid choice! Please enter a-h.\n" RESET);
                    }
                }
                break;
            case 11:
                storage_save();
                journal_save();
                printf("\n Events and journal saved. Goodbye!\n\n");
                return 0;
            default:
               printf("Invalid choice! Please enter 1-10.\n");
        }
    }    
    return 0;
}