#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "hospital.h"
extern char current_logged_in_id[20];

void add_sch()
{
    int day, month, year, hour, minute;
    Schedule s; 
    strcpy(s.drID, current_logged_in_id); //temporary for testing

    char treated_patient_id[5];
    char treated_patient[6];
    char need_of_patientid;

    printf("Is vacant or not?\nIf yes press '1', no press '2': ");
    scanf(" %c", &need_of_patientid);
    while( getchar() != '\n');

    int loop;
    do
    {
        switch (need_of_patientid)
        {
        case '1':
            strcpy(treated_patient, "N/A");
            break;

        case '2':
            do {
                printf("Enter treated patient ID: P");
                scanf(" %s", treated_patient_id);
                while(getchar() != '\n');
            } while (check_isdigit(treated_patient_id) == 0);
            sprintf(treated_patient, "P%04s", treated_patient_id);
            printf("Comfirmation: Treated Patient ID : %s\n", treated_patient);
            break;
        
        default:
            printf("ERROR: 'INVALID INPUT'");
            loop = 1;
            break;
    }
    } while (!isdigit(need_of_patientid) || loop == 1);

    //Input schedule date
    int valid_date = 1;
    do {
        
        printf("Enter date (DD/MM/YYYY): ");
        fgets(s.date, sizeof(s.date), stdin);
        s.date[strcspn(s.date, "\n")] = 0;
        
        //Date validation
        if (strlen(s.date) != 10 || s.date[2] != '/' || s.date[5] != '/') {
            valid_date = 0;
        }

        else if (sscanf(s.date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
            valid_date = 0;
        }

        else if (day <= 0 || day >= 32 || month <= 0 || month >= 13 || year <= 1000 || year >= 3000) {
            valid_date = 0;        
        }

        else
        {
            valid_date = 1;
        }
        

        if (valid_date == 0) {
            printf("Invalid date format!\n");
        }
    } while(valid_date == 0);
    
    //Input schedule time
    int valid_time = 1;
    do {
        printf("Enter time (00:00 - 23:59): ");
        fgets(s.time, sizeof(s.time), stdin);
        s.time[strcspn(s.time, "\n")] = 0;

        //Time validation
        if (strlen(s.time) != 5 || s.time[2] != ':') {
            valid_time = 0;
        }

        else if (sscanf(s.time, "%2d:%2d", &hour, &minute) != 2) {
            valid_time = 0;
        }

        else if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            valid_time = 0;        
        }

        else
        {
            valid_time = 1;
        }
        

        if (valid_time == 0) {
            printf("Invalid time format!\n");
        }
    } while( valid_time == 0);
    
    //Input activity
    if (need_of_patientid == '2')
    {
        printf("Enter activity: ");
        fgets(s.activity, sizeof(s.activity), stdin);
        s.activity[strcspn(s.activity, "\n")] = 0;
    }
    if(need_of_patientid == '1')
    {
        strcpy(s.activity, "VACANT");
    }
    

    // Append information
    FILE *sch = fopen("csv/Schedule.csv", "a");

    fprintf(sch, "%s,%s,%s,%s,%s\n", s.drID, treated_patient, s.date, s.time, s.activity);

    fclose(sch);    

    printf("\nSchedule successfully Added!\n");
}

void search_sch()
{
    Schedule s;
    FILE *sch;
    char line[1000];
    char searchDate[20];
    char doctorid[7] = ""; // Temporary for testing
    strcpy(doctorid, current_logged_in_id);
    char treated_patient[8];

    printf("Enter date to search (DD/MM/YYYY): ");
    fgets(searchDate, sizeof(searchDate), stdin);
    searchDate[strcspn(searchDate, "\n")] = 0;

    sch = fopen("csv/Schedule.csv", "r");

    if (sch == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    fgets(line, sizeof(line), sch);// skip header

    while (fgets(line, sizeof(line), sch)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(s.drID, strtok(temp, ","));
        strcpy(treated_patient, strtok(NULL, ","));
        strcpy(s.date, strtok(NULL, ","));
        strcpy(s.time, strtok(NULL, ","));
        strcpy(s.activity, strtok(NULL, ","));

        // Compare Dates and DoctorID
        if (strcmp(s.date, searchDate) == 0 && strcmp(s.drID, doctorid) == 0) {

            printf("%s - %s", s.time, s.activity);

            found = 1;
        }
    }

    if (!found) {
        printf("No records found or input is invalid!\n");
    }

    fclose(sch);
}

void view_sch()
{
    Schedule s;
    FILE *sch;
    char tempDate[20] = "0";
    char line[1000];
    char doctorid[7] = ""; // Temporary for testing
    strcpy(doctorid, current_logged_in_id);
    char treated_patient[8];
    int found;

    sch = fopen("csv/Schedule.csv", "r");

    if (sch == NULL) {
        printf("File could not be opened.\n");
    }

    fgets(line, sizeof(line), sch);// skip header

    while (fgets(line, sizeof(line), sch)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(s.drID, strtok(temp, ","));
        strcpy(treated_patient, strtok(NULL, ","));
        strcpy(s.date, strtok(NULL, ","));
        strcpy(s.time, strtok(NULL, ","));
        strcpy(s.activity, strtok(NULL, ","));

        // Compare DoctorID
        if (strcmp(s.drID, doctorid) == 0) {
            // Group activities on the same date
            if (strcmp(s.date, tempDate) == 1) {
                strcpy(tempDate, s.date);
                printf("\n%s:\n", s.date);
            }
            printf("%s - %s", s.time, s.activity);     

            found = 1;
        }
    }

    if (found != 1) {
        printf("No records found!\n");
    }

    fclose(sch);
}

void add_dia()
{
    int day, month, year, out = 0;
    Status ps; 
    strcpy(ps.d_id, current_logged_in_id); //temporary for testing

    //Input patient ID
    do {
        printf("Enter patient ID: ");
        fgets(ps.p_id, sizeof(ps.p_id), stdin);
        ps.p_id[strcspn(ps.p_id, "\n")] = 0;
        int valid = 0;

        //Check patient ID for validity
        Patient p;
        FILE *patient;
        char line[1000];
        char searchPatient[20];

        patient = fopen("csv/Patient.csv", "r");

        if (patient == NULL) {
            printf("File could not be opened.\n");
        }

        while (fgets(line, sizeof(line), patient)) {
            // Copy line
            char temp[1000];
            strcpy(temp, line);

            // Get first column
            strcpy(p.id, strtok(temp, ","));

            // Compare id
            if (strcmp(p.id, ps.p_id) == 0) {
                valid = 1;
            }
        }

        fclose(patient);

        if (valid == 1) {
            break;
        }
        else {
            printf("Patient ID not found or invalid input!\n");
            continue;
        }
    } while(1);

    //Input patient type
    do {
        char type_choice;
        printf("Enter patient type (1 for Inpatient, 2 for Outpatient): ");
        scanf(" %c", &type_choice);
        while (getchar() != '\n');

        switch(type_choice)
        {
            case '1':
                strcpy(ps.type, "Inpatient");
                break;
            case '2':
                strcpy(ps.type, "Outpatient");
                out = 1;
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        break;
    } while(1);

    //Input reason
    printf("Enter reason for admission: ");
    fgets(ps.reason, sizeof(ps.reason), stdin);
    ps.reason[strcspn(ps.reason, "\n")] = 0;

    //Input h. record
    do {
        if (out == 1) {
            strcpy(ps.record, "-");
            break;
        }

        char rec_choice;
        printf("Enter patient hospitalization record (1 for Admission, 2 for Discharge): ");
        scanf(" %c", &rec_choice);
        while (getchar() != '\n');

        switch(rec_choice)
        {
            case '1':
                strcpy(ps.record, "Admission");
                break;
            case '2':
                strcpy(ps.record, "Discharge");
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        break;
    } while(1);

    //Input h.date
    do {
        if (out == 1) {
            strcpy(ps.date, "-");
            break;
        }

        int valid = 1;
        
        printf("Enter hospitalization date (DD/MM/YYYY): ");
        fgets(ps.date, sizeof(ps.date), stdin);
        ps.date[strcspn(ps.date, "\n")] = 0;
        
        //Date validation
        if (strlen(ps.date) != 10 || ps.date[2] != '/' || ps.date[5] != '/') {
            valid = 0;
        }

        if (sscanf(ps.date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
            valid = 0;
        }

        if (day <= 0 || day >= 32 || month <= 0 || month >= 13 || year <= 1000 || year >= 3000) {
            valid = 0;        
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid date format!\n");
            continue;
        }
    } while(1);

    //Input length of stay
    do {
        if (out == 1) {
            strcpy(ps.stay, "-");

            break;
        }

        printf("Enter length of stay (in days): ");
        fgets(ps.stay, sizeof(ps.stay), stdin);
        ps.stay[strcspn(ps.stay, "\n")] = 0;
        int valid = 1;

        for(int i = 0; ps.stay[i] != '\0'; i++) {
            if(ps.stay[i] < '0' || ps.stay[i] > '9') {
                valid = 0;
            }
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid input!\n");
            continue;
        }
    } while(1);

    //Input diagnosis
    printf("Enter patient diagnosis: ");
    fgets(ps.diagnosis, sizeof(ps.diagnosis), stdin);
    ps.diagnosis[strcspn(ps.diagnosis, "\n")] = 0;

    // Append information
    FILE *dia = fopen("csv/Patient_status.csv", "a");

    fprintf(dia, "%s,%s,%s,%s,%s,%s,%s,%s\n", ps.p_id, ps.type, ps.reason, ps.record, ps.date, ps.stay, ps.d_id, ps.diagnosis);

    fclose(dia);    

    printf("\nPatient diagnosis successfully Added!\n");
}

void search_dia()
{
    Status ps;
    FILE *dia;
    char line[1000];
    char searchPatient[20];
    char doctorid[5] = ""; // Temporary for testing
    strcpy(doctorid, current_logged_in_id);

    printf("Enter a patient ID to search: ");
    fgets(searchPatient, sizeof(searchPatient), stdin);
    searchPatient[strcspn(searchPatient, "\n")] = 0;

    dia = fopen("csv/Patient_status.csv", "r");

    if (dia == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    while (fgets(line, sizeof(line), dia)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(ps.p_id, strtok(temp, ","));
        strcpy(ps.type, strtok(NULL, ","));
        strcpy(ps.reason, strtok(NULL, ","));
        strcpy(ps.record, strtok(NULL, ","));
        strcpy(ps.date, strtok(NULL, ","));
        strcpy(ps.stay, strtok(NULL, ","));
        strcpy(ps.d_id, strtok(NULL, ","));
        strcpy(ps.diagnosis, strtok(NULL, ","));

        // Compare PatientID and DoctorID
        if (strcmp(ps.p_id, searchPatient) == 0 && strcmp(ps.d_id, doctorid) == 0) {
            found = 1;

            if (strcmp(ps.type, "Inpatient") == 0) {
                printf("\nPatient Diagnosis [%s] - (Doctor: %s)", ps.p_id, ps.d_id);
                printf("\nType: %s", ps.type);
                printf("\nReason for admission: %s", ps.reason);
                printf("\nHospitalization record: %s", ps.record);
                printf("\nHospitalization date: %s", ps.date);
                printf("\nLength of stay: %s days", ps.stay);
                printf("\nDiagnosis: %s", ps.diagnosis);
            }
            else {
                printf("\nPatient Diagnosis [%s] - (Doctor: %s)", ps.p_id, ps.d_id);
                printf("\nType: %s", ps.type);
                printf("\nReason for admission: %s", ps.reason);
                printf("\nDiagnosis: %s", ps.diagnosis);
            }
        }
    }

    if (!found) {
        printf("No records found or input is invalid!\n");
    }

    fclose(dia);
}

void view_dia()
{
    Status ps;
    FILE *dia;
    char line[1000];
    char doctorid[5] = ""; // Temporary for testing
    strcpy(doctorid, current_logged_in_id);

    dia = fopen("csv/Patient_status.csv", "r");

    if (dia == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    printf("Showing patient information.. [Doctor: %s]\n", doctorid);

    while (fgets(line, sizeof(line), dia)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(ps.p_id, strtok(temp, ","));
        strcpy(ps.type, strtok(NULL, ","));
        strcpy(ps.reason, strtok(NULL, ","));
        strcpy(ps.record, strtok(NULL, ","));
        strcpy(ps.date, strtok(NULL, ","));
        strcpy(ps.stay, strtok(NULL, ","));
        strcpy(ps.d_id, strtok(NULL, ","));
        strcpy(ps.diagnosis, strtok(NULL, ","));

        // Compare DoctorID
        if (strcmp(ps.d_id, doctorid) == 0) {
            found = 1;

            if (strcmp(ps.type, "Inpatient") == 0) {
                printf("\nPatient Diagnosis [%s]", ps.p_id, ps.d_id);
                printf("\nType: %s", ps.type);
                printf("\nReason for admission: %s", ps.reason);
                printf("\nHospitalization record: %s", ps.record);
                printf("\nHospitalization date: %s", ps.date);
                printf("\nLength of stay: %s days", ps.stay);
                printf("\nDiagnosis: %s", ps.diagnosis);
            }
            else {
                printf("\nPatient Diagnosis [%s]", ps.p_id, ps.d_id);
                printf("\nType: %s", ps.type);
                printf("\nReason for admission: %s", ps.reason);
                printf("\nDiagnosis: %s", ps.diagnosis);
            }
        }
    }

    if (!found) {
        printf("No records found!\n");
    }

    fclose(dia);
}

// Collection of Menu
void schedule()
{
    char sch_choice;

    do {
        printf("\n======== Doctor Schedule Management ========\n");
        printf("1. Add Schedule\n");
        printf("2. View All Schedule\n");
        printf("3. Search Schedule\n");
        printf("4. Return to Menu\n");

        printf("\nEnter choice: ");
        scanf(" %c", &sch_choice);

        while(getchar() != '\n');

        switch(sch_choice)
        {
            case '1':
                printf("\n======== Add Schedule ========\n");
                add_sch();
                break;
            case '2':
                printf("\n======== View All Schedule ========\n");
                view_sch();
                break;
            case '3':
                printf("\n======== Search Schedule ========\n");
                search_sch();
                break;
            case '4':
                printf("\n======== Return to Menu ========\n");
                printf("Returning...\n\n");
                break;
            default:
                printf("\nInvalid choice! Please enter the correct input.\n");
                break;
        }
    }
    while(sch_choice != '4');
}

void diagnostic()
{
    char dia_choice;

    do {
        printf("\n======== Patient Diagnostic Management ========\n");
        printf("1. Provide Diagnostic Information\n");
        printf("2. View All Patient Reports\n");
        printf("3. Search Patient Report\n");
        printf("4. Return to Menu\n");

        printf("\nEnter choice: ");
        scanf(" %c", &dia_choice);

        while(getchar() != '\n');

        switch(dia_choice)
        {
            case '1':
                printf("\n======== Provide Diagnostic Information ========\n");
                add_dia();
                break;
            case '2':
                printf("\n======== View All Patient Reports ========\n");
                view_dia();
                break;
            case '3':
                printf("\n======== Search Patient Report ========\n");
                search_dia();
                break;
            case '4':
                printf("\n======== Return to Menu ========\n");
                printf("Returning...\n\n");
                break;
            default:
                printf("\nInvalid choice! Please enter the correct input.\n");
                break;
        }
    }
    while(dia_choice != '4');
}

void doctor() 
{
    char choice;

    do {
        printf("\n=========== Doctor Main Menu ===========\n");
        printf("1. Doctor Schedule Management\n");
        printf("2. Patient Diagnostic Management\n");
        printf("3. Log out\n");

        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        while(getchar() != '\n');
        
        switch(choice)
        {
            case '1':
                schedule();
                break;
            case '2':
                diagnostic();
                break;
            case '3':
                printf("\n======== Log out ========\n");
                printf("Logging out...\n");
                break;
            default:
                printf("\nInvalid choice! Please enter the correct input.\n");
                break;
        }
    }
    while(choice != '3');
    return;
}