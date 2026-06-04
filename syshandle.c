#ifndef UTILS_H   // "if UTILS_H is NOT defined yet..."
#define UTILS_H   // "...define it now, and include everything below"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#define MAX_IDS 100
#include "admin_menu.h"

//protect for skipping
void buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);//End of File
}

// Returns 1 if the string is all digits, 0 otherwise need for validate the contact number and IC number
int is_string_numeric(const char *str) {
    if (str == NULL || strlen(str) == 0) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}
 char usedIDs[MAX_IDS][10];  // store generated IDs
 int idCount = 0;

int isDuplicate(const char *id) {
    for (int i = 0; i < idCount; i++) {
        if (strcmp(id, usedIDs[i]) == 0)
        {
            return 1; // duplicate found
        }
    }
    return 0; // unique
}

// Function to generate a unique ID
void generateID(char *id, char prefix, int digits) {
    int randomNum;
    int max = 1;
    for (int i = 0; i < digits; i++) max *= 10; // e.g. digits=3 → max=1000

    do {
        randomNum = rand() % max;
        sprintf(id, "%c%0*d", prefix, digits, randomNum);
        //           
    } while (isDuplicate(id)); // one shared check

    strcpy(usedIDs[idCount], id);
    idCount++;
}
#endif
