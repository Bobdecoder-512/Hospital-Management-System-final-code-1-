// 2. patient complaints system
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "admin_menu.h"


//view complains
void displaycomplain(const char * complainfile)
{//const is for lock the information, and not allow to replace it
    FILE *display;
    char line[256];

    printf("\n=====Patient complaqints list=====\n");
    display = fopen(complainfile, "r");
    if (display != NULL)
    {
        while(fgets(line, sizeof(line), display)) // point to specific file
        {
            printf("%s", line);
        }
        fclose(display);
    }
    else
    {
        perror("Error opening the file.\n");// dislpay during it is unable to find the file
    }
}

//search  omplaints
void searchcompalin(const char *complainFile) 
{
    char searchID[10]; 
    char line[256];

    printf("Enter patient ID to search (P****): ");
    scanf("%s", searchID);
    buffer(); // Clear input buffer

    for (int i = 0; searchID[i]; i++) {
        searchID[i] = toupper(searchID[i]);
    }

    FILE *fp = fopen(complainFile, "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    int found = 0;
    // Skip header row
    if (fgets(line, sizeof(line), fp))
    {
        printf("\nP_ID\tReason\t\tTime\tDate\n");
        printf("--------------------------------------------\n");
    }

    while (fgets(line, sizeof(line), fp)) 
    {
        char temp[256];
        strcpy(temp, line);

        // Tokenize based on your CSV structure
        char *P_id = strtok(temp, ",");
        char *reason = strtok(NULL, ",");
        char *time = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        // Compare string ID to string ID
        if(P_id != NULL && strcmp(P_id, searchID) == 0)
        {
            // Remove newline from the date string if present
            if (date != NULL) date[strcspn(date, "\n")] = 0;

            printf("%s\t%s\t%s\t%s\n", P_id, reason, time, date);
            found = 1;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No matching record found for %s\n", searchID);
    }
}

void patient_complaint_sys()
{
    int choice;
        do{
            printf("1. View patient complaints\n");
            printf("2. Search patient complaints\n");
            printf("3. Back to admin main menu\n");
            printf(" Enter the option: ");

            scanf("%d", &choice);
               
                    if (choice == 1)
                    {
                        displaycomplain("csv/Complaint.csv");
                    } 
                    else if (choice == 2)
                    {
                        searchcompalin("csv/Complaint.csv");
                    }
                    else if (choice == 3)
                    {
                        printf("Back to admin main menu\n");
                    } 
                    else
                    {
                        printf("Invalid option. please try again!\n");
                    }
                }while(choice != 3);
}