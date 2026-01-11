#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    Date date;
    char title[50];
    char description[100];
} Event;


#endif