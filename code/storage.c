#include <stdio.h>
#include "storage.h"

void save_event(Event e) {
    FILE *f = fopen("database.txt", "a");
    if (f == NULL) return; // Error handling
    
    // Serialize: 2025 10 15 Meeting Description...
    fprintf(f, "%d %d %d %s\n", e.date.year, e.date.month, e.date.day, e.description);
    fclose(f);
}