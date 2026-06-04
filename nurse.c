#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "hospital.h"
extern char current_logged_in_id[20];

void nurse_view_sch()
{
    Schedule s;
    FILE *sch;
    char searchDoctor[20];
    char line[1000];
    char treat_patient[7];

    printf("Enter doctor ID to view their schedule: ");
    fgets(searchDoctor, sizeof(searchDoctor), stdin);
    searchDoctor[strcspn(searchDoctor, "\n")] = 0;

    sch = fopen("csv/Schedule.csv", "r");

    if (sch == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    while (fgets(line, sizeof(line), sch)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(s.drID, strtok(temp, ","));
        strcpy(treat_patient, strtok(NULL, ","));
        strcpy(s.date, strtok(NULL, ","));
        strcpy(s.time, strtok(NULL, ","));
        strcpy(s.activity, strtok(NULL, ","));

        // Compare DoctorID
        if (strcmp(s.drID, searchDoctor) == 0) {
            if (found == 0){
                found = 1;
                printf("Here is the doctor schedule for %s\n", s.drID);
            }
            printf("%s %s - %s", s.date, s.time, s.activity);               
        }
    }

    if (!found) {
        printf("No records found or input is invalid!\n");
    }

    fclose(sch);
}

void add_inv()
{
    int num;
    Medicine m; 
    strcpy(m.n_id, current_logged_in_id); //temporary for testing

    //Medicine ID Generator
    srand(time(NULL));
    num = rand() % 1000; //Generates random number
    sprintf(m.m_id, "M%03d", num);
    printf("Adding entry for medicine %s\n", m.m_id);

    //Input name
    printf("Enter medicine name: ");
    fgets(m.name, sizeof(m.name), stdin);
    m.name[strcspn(m.name, "\n")] = 0;

    //Input price
    do {
        char input[100];
        float value;
        char extra;
        int valid = 1;
        
        printf("Enter medicine price: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        //Float validation
        if (sscanf(input, "%f%c", &value, &extra) == 1) {
            //Check for negative numbers
            if (atof(input) > 0) {
                m.price = atof(input);
            }
            else {
                valid = 0;
            }
        }
        else {
            valid = 0;
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid price!\n");
            continue;
        }
    } while(1);

    //Input vendor
    printf("Enter medicine vendor: ");
    fgets(m.vendor, sizeof(m.vendor), stdin);
    m.vendor[strcspn(m.vendor, "\n")] = 0;

    //Input stock
    do {
        char input[100];
        int value;
        char extra;
        int valid = 1;
        
        printf("Enter medicine stock: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        //Integer validation
        if (sscanf(input, "%d%c", &value, &extra) == 1) {
            //Check for negative numbers
            if (atoi(input) > 0) {
                m.stock = atoi(input);
            }
            else {
                valid = 0;
            }
        }
        else {
            valid = 0;
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid number!\n");
            continue;
        }
    } while(1);

    // Append information
    FILE *inv = fopen("csv/Inventory.csv", "a");

    fprintf(inv, "%s,%s,%.2f,%s,%d,%s\n", m.m_id, m.name, m.price, m.vendor, m.stock, m.n_id);

    fclose(inv);    

    printf("\nStock entry successfully added!\n");
}

void view_inv()
{
    Medicine m;
    FILE *inv;
    char line[1000];

    inv = fopen("csv/Inventory.csv", "r");

    if (inv == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    printf("| %-5s | %-30s | %-6s | %-15s | %-5s | %-8s |\n", "ID", "Medicine Name", "Price", "Vendor", "Stock", "Nurse ID");
    printf("|-------|--------------------------------|--------|-----------------|-------|----------|\n");

    // Skip header line
    fgets(line, sizeof(line), inv);

    while (fgets(line, sizeof(line), inv)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(m.m_id, strtok(temp, ","));
        strcpy(m.name, strtok(NULL, ","));
        m.price = atof(strtok(NULL, ","));
        strcpy(m.vendor, strtok(NULL, ","));
        m.stock = atoi(strtok(NULL, ","));
        strcpy(m.n_id, strtok(NULL, ","));
        m.n_id[strcspn(m.n_id, "\n")] = 0; //Remove newline

        // Prints everything
        printf("| %-5s | %-30s | %-6.2f | %-15s | %-5d | %-8s |\n", m.m_id, m.name, m.price, m.vendor, m.stock, m.n_id);

        found = 1;
    }

    if (!found) {
        printf("No records found!\n");
    }

    fclose(inv);
}

void search_inv()
{
    Medicine m;
    FILE *inv;
    char line[1000];
    char searchMed[20];

    printf("Enter a medicine ID to search: ");
    fgets(searchMed, sizeof(searchMed), stdin);
    searchMed[strcspn(searchMed, "\n")] = 0;

    inv = fopen("csv/Inventory.csv", "r");

    if (inv == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    while (fgets(line, sizeof(line), inv)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(m.m_id, strtok(temp, ","));
        strcpy(m.name, strtok(NULL, ","));
        m.price = atof(strtok(NULL, ","));
        strcpy(m.vendor, strtok(NULL, ","));
        m.stock = atoi(strtok(NULL, ","));
        strcpy(m.n_id, strtok(NULL, ","));

        m.n_id[strcspn(m.n_id, "\n")] = 0; //Remove newline

        // Compare MedicineID
        if (strcmp(m.m_id, searchMed) == 0) {
            found = 1;

            printf("\nMedicine Information [%s] - (Entry recorded by: %s)", m.m_id, m.n_id);
            printf("\nName: %s", m.name);
            printf("\nPrice: %.2f", m.price);
            printf("\nVendor: %s", m.vendor);
            printf("\nStock amount: %d\n", m.stock);
        }
    }

    if (!found) {
        printf("No records found or input is invalid!\n");
    }

    fclose(inv);
}

void add_note()
{
    int day, month, year, hour, minute;
    Note n;

    //Input patient ID
    do {
        printf("Enter patient ID: ");
        fgets(n.p_id, sizeof(n.p_id), stdin);
        n.p_id[strcspn(n.p_id, "\n")] = 0;
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
            if (strcmp(p.id, n.p_id) == 0) {
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

    //Input note date
    do {
        int valid = 1;
        
        printf("Enter note date (DD/MM/YYYY): ");
        fgets(n.date, sizeof(n.date), stdin);
        n.date[strcspn(n.date, "\n")] = 0;
        
        //Date validation
        if (strlen(n.date) != 10 || n.date[2] != '/' || n.date[5] != '/') {
            valid = 0;
        }

        if (sscanf(n.date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
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
    
    //Input note time
    do {
        int valid = 1;

        printf("Enter note time (00:00 - 23:59): ");
        fgets(n.time, sizeof(n.time), stdin);
        n.time[strcspn(n.time, "\n")] = 0;

        //Time validation
        if (strlen(n.time) != 5 || n.time[2] != ':') {
            valid = 0;
        }

        if (sscanf(n.time, "%2d:%2d", &hour, &minute) != 2) {
            valid = 0;
        }

        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            valid = 0;        
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid time format!\n");
            continue;
        }
    } while(1);

    //Input blood pressure
    do {
        char input[100];
        int value;
        char extra;
        int valid = 1;
        
        printf("Enter blood pressure (mm Hg): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        //Integer validation
        if (sscanf(input, "%d%c", &value, &extra) == 1) {
            //Check for negative numbers
            if (atoi(input) >= 0) {
                n.blood = atoi(input);
            }
            else {
                valid = 0;
            }
        }
        else {
            valid = 0;
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid number!\n");
            continue;
        }
    } while(1);

    //Input temperature
    do {
        char input[100];
        float value;
        char extra;
        int valid = 1;
        
        printf("Enter temperature (°C): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;
        
        //Float validation
        if (sscanf(input, "%f%c", &value, &extra) == 1) {
            n.temp = atof(input);
        }
        else {
            valid = 0;
        }

        if (valid == 1) {
            break;
        }
        else {
            printf("Invalid temperature!\n");
            continue;
        }
    } while(1);

    //Input symptoms
    printf("Enter symptoms: ");
    fgets(n.sym, sizeof(n.sym), stdin);
    n.sym[strcspn(n.sym, "\n")] = 0;

    //Input current medication
    printf("Enter current medication: ");
    fgets(n.cm, sizeof(n.cm), stdin);
    n.cm[strcspn(n.cm, "\n")] = 0;

    //Input patient condition
    do {
        char cond_choice;
        printf("Enter patient condition (1 = Stable, 2 = Critical, 3 = Improving): ");
        scanf(" %c", &cond_choice);
        while (getchar() != '\n');

        switch(cond_choice)
        {
            case '1':
                strcpy(n.cond, "Stable");
                break;
            case '2':
                strcpy(n.cond, "Critical");
                break;
            case '3':
                strcpy(n.cond, "Improving");
                break;
            default:
                printf("Invalid choice!\n");
                continue;
        }
        break;
    } while(1);

    // Append information
    FILE *note = fopen("csv/Note.csv", "a");

    fprintf(note, "%s,%s,%s,%d,%.1f,%s,%s,%s\n", n.p_id, n.date, n.time, n.blood, n.temp, n.sym, n.cm, n.cond);

    fclose(note);    

    printf("\nDaily note successfully added!\n");
}

void view_note()
{
    Note n;
    FILE *note;
    char line[1000];

    note = fopen("csv/Note.csv", "r");

    if (note == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    printf("| %-10s | %-12s | %-6s | %-22s | %-20s | %-15s | %-30s | %-10s |\n", "Patient ID", "Date", "Time", "Blood Pressure (mm Hg)", "Temperature (°C)", "Symptoms", "Current Medication", "Condition");
    printf("|------------|--------------|--------|------------------------|---------------------|-----------------|--------------------------------|------------|\n");

    // Skip header line
    fgets(line, sizeof(line), note);

    while (fgets(line, sizeof(line), note)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(n.p_id, strtok(temp, ","));
        strcpy(n.date, strtok(NULL, ","));
        strcpy(n.time, strtok(NULL, ","));
        n.blood = atoi(strtok(NULL, ","));
        n.temp = atof(strtok(NULL, ","));
        strcpy(n.sym, strtok(NULL, ","));
        strcpy(n.cm, strtok(NULL, ","));
        strcpy(n.cond, strtok(NULL, ","));
        
        n.cond[strcspn(n.cond, "\n")] = 0; //Remove newline

        // Prints everything
        printf("| %-10s | %-12s | %-6s | %-22d | %-19.1f | %-15s | %-30s | %-10s |\n", n.p_id, n.date, n.time, n.blood, n.temp, n.sym, n.cm, n.cond);

        found = 1;
    }

    if (!found) {
        printf("No records found!\n");
    }

    fclose(note);
}

void search_note()
{
    Note n;
    FILE *note;
    char line[1000];
    char searchPatient[20];
    char searchDate[20];

    printf("Enter the patient ID to search: ");
    fgets(searchPatient, sizeof(searchPatient), stdin);
    searchPatient[strcspn(searchPatient, "\n")] = 0;

    printf("Enter the date of the note to search: ");
    fgets(searchDate, sizeof(searchDate), stdin);
    searchDate[strcspn(searchDate, "\n")] = 0;

    note = fopen("csv/Note.csv", "r");

    if (note == NULL) {
        printf("File could not be opened.\n");
    }

    int found = 0;

    while (fgets(line, sizeof(line), note)) {
        // Copy line
        char temp[1000];
        strcpy(temp, line);

        // Split CSV columns
        strcpy(n.p_id, strtok(temp, ","));
        strcpy(n.date, strtok(NULL, ","));
        strcpy(n.time, strtok(NULL, ","));
        n.blood = atoi(strtok(NULL, ","));
        n.temp = atof(strtok(NULL, ","));
        strcpy(n.sym, strtok(NULL, ","));
        strcpy(n.cm, strtok(NULL, ","));
        strcpy(n.cond, strtok(NULL, ","));
        
        n.cond[strcspn(n.cond, "\n")] = 0; //Remove newline

        // Compare PatientID and Date
        if (strcmp(n.p_id, searchPatient) == 0 && strcmp(n.date, searchDate) == 0) {
            found = 1;

            printf("\nPatient Note [%s] - %s (%s)", n.p_id, n.date, n.time);
            printf("\nBlood Pressure (mm Hg): %d", n.blood);
            printf("\nTemperature (°C): %.1f", n.temp);
            printf("\nSymptoms: %s", n.sym);
            printf("\nCurrent Medications: %s", n.cm);
            printf("\nPatient Condition: %s\n", n.cond);
        }
    }

    if (!found) {
        printf("No records found or input is invalid!\n");
    }

    fclose(note);
}

// Collection of Menu
void inventory()
{
    char inv_choice;

    do {
        printf("\n======== Medical Inventory Management ========\n");
        printf("1. Add Stock Entries\n");
        printf("2. View All Inventory Stocks\n");
        printf("3. Search Inventory Stocks\n");
        printf("4. Return to Menu\n");

        printf("\nEnter choice: ");
        scanf(" %c", &inv_choice);

        while(getchar() != '\n');

        switch(inv_choice)
        {
            case '1':
                printf("\n======== Add Stock Entries ========\n");
                add_inv();
                break;
            case '2':
                printf("\n======== View All Inventory Stocks ========\n");
                view_inv();
                break;
            case '3':
                printf("\n======== Search Inventory Stocks ========\n");
                search_inv();
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
    while(inv_choice != '4');
}

void notes()
{
    char note_choice;

    do {
        printf("\n======== Daily Notes Management ========\n");
        printf("1. Record Daily Notes\n");
        printf("2. View All Notes\n");
        printf("3. Search Notes\n");
        printf("4. Return to Menu\n");

        printf("\nEnter choice: ");
        scanf(" %c", &note_choice);

        while(getchar() != '\n');

        switch(note_choice)
        {
            case '1':
                printf("\n======== Record Daily Notes ========\n");
                add_note();
                break;
            case '2':
                printf("\n======== View All Notes ========\n");
                view_note();
                break;
            case '3':
                printf("\n======== Search Notes ========\n");
                search_note();
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
    while(note_choice != '4');
}

void nurse() 
{
    char choice;

    do {
        printf("\n=========== Nurse Main Menu ===========\n");
        printf("1. Doctor Schedule Viewer\n");
        printf("2. Medical Inventory Management\n");
        printf("3. Daily Notes Management\n");
        printf("4. Log out\n");

        printf("\nEnter choice: ");
        scanf(" %c", &choice);

        while(getchar() != '\n');
        
        switch(choice)
        {
            case '1':
                printf("\n======== Doctor Schedule Viewer ========\n");
                nurse_view_sch();
                break;
            case '2':
                inventory();
                break;
            case '3':
                notes();
                break;
            case '4':
                printf("\n======== Log out ========\n");
                printf("Logging out...\n");
                break;
            default:
                printf("\nInvalid choice! Please enter the correct input.\n");
                break;
        }
    }
    while(choice != '4');
    return;
}