// 1.staff management system
#include <stdio.h>
#include <ctype.h>  
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "admin_menu.h"

// search and view account management
//display all of  account 
void displayaccount(const char * doctorfile, const char * nursefile , const char* patientfile)
{//const is for lock the information, and not allow to replace it
    FILE *display;
    char line[256];

    printf("\n=====Doctor records=====\n");
    display = fopen(doctorfile, "r");
    if (display != NULL)
    {
        fgets(line, sizeof(line), display);
        while(fgets(line, sizeof(line), display)) // point to specific file
        {
            printf("%s", line);
            printf("\n");
        }
        fclose(display);
    }
    else
    {
        perror("Error opening the file.\n");// dislpay during it is unable to find the file
    }

    printf("\n=====Nurse records=====\n");
    display = fopen(nursefile, "r");
    if (display != NULL)
    {
        fgets(line, sizeof(line), display);
        while(fgets(line, sizeof(line), display)) // point to specific file
        {
            printf("%s", line);
            printf("\n");
        }
        fclose(display);
    }
    else
    {
        perror("Error opening the file!\n");// dislpay during it is unable to find the file
    }
     printf("\n=====Patient records=====\n");
    display = fopen(patientfile, "r");
    if (display != NULL)
    {
        fgets(line, sizeof(line), display);
        while(fgets(line, sizeof(line), display)) // point to specific file
        {
            printf("%s", line);
            printf("\n");
        }
        fclose(display);
    }
    else
    {
        perror("Error opening the file!\n");// dislpay during it is unable to find the file
    }
}

//fix user input to first word as upper
void namefixed(const char *input, char *output)
{
    int i = 0, j = 0;
    int newword = 1;

    while(input[i] != '\0')
    {
        if(input[i] == ' ')
        {
            output[j++] = ' ';
            newword = 1;
        } else if(newword)
        {
            output[j++] = toupper((unsigned char)input[i]);
            newword = 0;
        } else
        {
             output[j++] = tolower((unsigned char)input[i]);
        }
        i++;
    }
     output[j] = '\0';
}

// compare the string and check if it is only contain alphabetic character
int isAlphaOnly(const char *str) 
{
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]) && str[i] != ' ') { // allow spaces
            return 0;
        }
    }
    return 1;
}
//search by ID or name
void searchaccount(const char *doctorFile, const char *nurseFile, const char *patientFile) 
{
    int search;
    char line[256];
    char inform[50];

    do {
        printf("\n===== Account Search Menu =====\n");
        printf("1. Search by account name\n");
        printf("2. Search by account ID\n");
        printf("3. Back to account main menu\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &search) != 1) {
            printf("Invalid input. Please enter a number.\n");
            buffer();
            continue;
        }
        buffer(); // flush newline after valid int read

        if (search == 3) break;

        if (search < 1 || search > 2) {
            printf("Invalid option. Please choose 1, 2, or 3.\n");
            continue;
        }

        printf("Enter search by: ");
        fgets(inform, sizeof(inform), stdin);
        inform[strcspn(inform, "\n")] = '\0'; // strip newline

        if (search == 1 && !isAlphaOnly(inform)) {
            printf("Invalid input! Name must contain only letters.\n");
            continue;
        }

        // Normalize AFTER validation
        char informNorm[50];
        namefixed(inform, informNorm);

        int found = 0;
        const char *files[3] = {doctorFile, nurseFile, patientFile};

        for (int f = 0; f < 3; f++) {
            FILE *fp = fopen(files[f], "r");
            if (fp == NULL) {
                perror("Error opening file");
                continue;
            }

            fgets(line, sizeof(line), fp); // skip header

            while (fgets(line, sizeof(line), fp)) {
                char temp[256];
                strcpy(temp, line);

                char *ID   = strtok(temp, ",");
                char *name = strtok(NULL, ",");

                if (search == 1 && name) {
                    char nameNorm[256];
                    namefixed(name, nameNorm); // normalize CSV name too
                    if (strstr(nameNorm, informNorm) != NULL) {
                        printf("Match found: %s", line);
                        found = 1;
                    }
                } else if (search == 2 && ID) {
                    if (strcmp(ID, inform) == 0) {
                        printf("Match found: %s", line);
                        found = 1;
                    }
                }
            }
            fclose(fp);
        }

        if (!found)
            printf("No matching record found for '%s'!\n", inform);

    } while(search != 3);
}

// add staff management
//char usedIDs[MAX_IDS][100];  // store generated IDs
//int idCount = 0;
//add doctor
// struct doctor
//    {
//         char dr_id[10];
//         char dr_name[30];
//         char dr_gender;
//         char dr_contact[15];        // <-- string to store, cuz first number 0, int cannot store
//         char dr_emergency[15]; 
//         char dr_duty[30];
//         char dr_specialism[30];
//         char dr_employment[20];
//         float dr_salary;
    // };
// Function to check if ID already exists
/*int isDuplicate(const char *dr_id) {
    for (int i = 0; i < idCount; i++) {
        if (strcmp(dr_id, usedIDs[i]) == 0) {
            return 1; // duplicate found
        }
    }
    return 0; // unique
}*/

// Function to generate a unique DoctorID
/*void generateDoctorID(char *dr_id) {
    int randomNum;
    do {
        randomNum = rand() % 1000;  // random number 0–999
        sprintf(dr_id, "D%03d", randomNum);
    } while (isDuplicate(dr_id));  // keep trying until unique

    // Store the new ID
    strcpy(usedIDs[idCount], dr_id);
    idCount++;
}*/

// Create doctor record
void createDoctorRecord(struct doctor *f) {
    generateID(f->dr_id,'D', 3);
    printf("Assigned DoctorID: %s\n", f->dr_id);

    // Collect input
    buffer(); // clear newline
    printf("Enter Name: ");
    fgets(f->dr_name, sizeof(f->dr_name), stdin);
    f->dr_name[strcspn(f->dr_name, "\n")] = '\0';
    char fixed[50];
    namefixed(f->dr_name, fixed);
    strcpy(f->dr_name, fixed);

    printf("Enter doctor's gender (F/M): ");
    do {
        scanf(" %c", &f->dr_gender);   // space before %c skips whitespace/newline
        f->dr_gender = toupper( f->dr_gender);  // convert to uppercase

        if (f->dr_gender != 'F' && f->dr_gender != 'M') {
            printf("Please only enter 'F' or 'M'!\n");
            printf("Enter doctor's gender (F/M): ");
        } else {
            break; // valid input
        }
    } while (1);


   /* printf("Enter Contact Number: ");
    do {
        scanf(" %d", &f->dr_contact_num);

        int temp = f->dr_contact_num; // temperary memoried digits
        int digits = 0;
        while (temp != 0) 
        {
            temp /= 10; // chop off last digit
            digits++;   // count it
        }

        if (digits < 10 || digits > 11) 
        {
            printf("Invalid! Contact number must be 10 or 11 digits.\n");
            printf("Enter Contact Number: ");
        } else 
        {
            break; // valid
        }
    } while (1);*/

    printf("Enter Contact Number(10 - 11 digits only): ");
do {
    scanf("%14s", f->dr_contact);

    int digits = strlen(f->dr_contact);

    if (digits < 10 || digits > 11 || is_string_numeric(f->dr_contact) == 0) {
        printf("Invalid! Contact number must be 10 or 11 digits.\n");
        printf("Enter Contact Number(10 - 11 digits only): ");
    } else {
        break; // valid
    }
} while (1);

printf("Enter Emergency Contact(10 - 11 digits only): ");
do {
    scanf("%14s", f->dr_emergency);

    int digits = strlen(f->dr_emergency);

    if (digits < 10 || digits > 11 || is_string_numeric(f->dr_emergency) == 0) {
        printf("Invalid! Contact number must be 10 or 11 digits.\n");
        printf("Enter Emergency Contact(10 -11 digits only): ");
    } else {
        break; // valid
    }
} while (1);

    printf("Enter Duty Status (on/off): ");
    do {
        scanf("%29s", f->dr_duty);

        // Normalize: first letter uppercase, rest lowercase
        f->dr_duty[0] = toupper(f->dr_duty[0]);
        for (int i = 1; f->dr_duty[i] != '\0'; i++) {
            f->dr_duty[i] = tolower(f->dr_duty[i]);
        }

        // Check only "On" or "Off"
        if (strcmp(f->dr_duty, "On") != 0 &&
            strcmp(f->dr_duty, "Off") != 0) {
            printf("Please only enter 'on' or 'off'!\n");
            printf("Enter Duty Status (on/off): ");
        } else {
            break; // valid input
        }
    } while (1);
    
    do{
        int special;
        printf("1. ENT\n");
        printf("2. Emergency medical care\n");
        printf("3. Radiology\n");
        printf("4. General surgery\n");
        printf("5. Neurology\n");
        printf("Enter Specialism(1-5): ");
        scanf("%d", &special);
        
        if (special == 1)
        {
            strcpy(f->dr_specialism, "ENT"); //coping string and write in
            break;
        }
        else if (special == 2)
        {
            strcpy(f->dr_specialism, "Emergency medical care"); 
            break;
        }
        else if (special == 3)
        {
            strcpy(f->dr_specialism, "Radiology"); 
            break;
        }
         else if (special == 4)
        {
            strcpy(f->dr_specialism, "General surgery"); 
            break;
        }
         else if (special == 5)
        {
            strcpy(f->dr_specialism, "Neurology"); 
            break;
        }
        else
        {
             printf("Please choose the correct number(1-5)!\n");
             printf("Enter position(1-5): ");
        }
     }while (1);
    

    do{
        int status;
        printf("1. Permanent\n");
        printf("2. Contract-bond\n");
        printf("3. Intern\n");       
        printf("Enter Employment Status(1-3): ");

        scanf("%d", &status);
        if (status == 1)
        {
           strcpy(f->dr_employment, "Permanent");
           break;
        } 
        else if (status == 2)
        {
           strcpy(f->dr_employment, "Contract-bond");
           break;
        }
        else if (status == 3)
        {
           strcpy(f->dr_employment, "Intern");
           break;
        }
        else
        {
            printf("Please choose thwe correct number(1-3)!\n");
            printf("Enter Employment Status(1-3): ");
        }
      }while(1); 
     
    printf("Enter Salary: ");
    scanf("%f", &f->dr_salary);

    FILE *fptr = fopen("../file/staff/Dr.csv", "a"); //file position
    if (fptr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fptr, "%s,%s,%c,%s,%s,%s,%s,%s,%.2f\n",
        f->dr_id, f->dr_name,f->dr_gender, f->dr_contact, f->dr_emergency,
        f->dr_duty, f->dr_specialism, f->dr_employment, f->dr_salary);


    fclose(fptr);
    printf("Data written successfully! Assigned DoctorID: %s\n", f->dr_id);
}

//add nurse
// struct nurse
//    {
//         char nur_ID[10];
//         char nur_name[30];
//         char nur_gender;
//         char nur_contact[15];  
//         char nur_duty[30];
//         char nur_position[30];
//         char nur_department[30];
//         char nur_employment[20];
//         float nur_salary;
//     };

// Create nurse record
void createNurseRecord(struct nurse *n) 
{
    generateID(n->nur_ID,'N', 3);
    printf("Assigned NurseID: %s\n", n->nur_ID);

    // Collect input
    buffer(); // clear newline
    printf("Enter Name: ");
    fgets(n->nur_name, sizeof(n->nur_name), stdin);
    n->nur_name[strcspn(n->nur_name, "\n")] = '\0';
    char fixed[50];
    namefixed(n->nur_name, fixed);
    strcpy(n->nur_name, fixed);

    printf("Enter nurse's gender (F/M): ");
    do {
        scanf(" %c", &n->nur_gender);   // space before %c skips whitespace/newline
        n->nur_gender = toupper( n->nur_gender);  // convert to uppercase

        if (n->nur_gender != 'F' && n->nur_gender != 'M')
        {
            printf("Please only enter 'F' or 'M'!\n");
            printf("Enter nurse's gender (F/M): ");
        } 
        else
        {
            break; // valid input
        }
    } while (1);

    printf("Enter Contact Number(10 - 11 digits only): ");
do {
    scanf("%14s", n->nur_contact);

    int digits = strlen(n->nur_contact);

    if (digits < 10 || digits > 11 || is_string_numeric(n->nur_contact) == 0)
     {
        printf("Invalid! Contact number must be 10 or 11 digits.\n");
        printf("Enter Contact Number(10 - 11 digits only): ");
    } else {
        break; // valid
    }
} while (1);

    printf("Enter Duty Status (on/off): ");
    do {
        scanf("%29s", n->nur_duty);

        // Normalize: first letter uppercase, rest lowercase
        n->nur_duty[0] = toupper(n->nur_duty[0]);
        for (int i = 1; n->nur_duty[i] != '\0'; i++) {
            n->nur_duty[i] = tolower(n->nur_duty[i]);
        }

        // Check only "On" or "Off"
        if (strcmp(n->nur_duty, "On") != 0 &&
            strcmp(n->nur_duty, "Off") != 0) {
            printf("Please only enter 'on' or 'off'!\n");
            printf("Enter Duty Status (on/off): ");
        } else {
            break; // valid input
        }
    } while (1);

     do{
        int post;
        printf("1. Ward Nurse\n");
        printf("2. Outpatient Nurse\n");
        printf("3. Operating Room (OR) Nurse\n");
        printf("4. Critical Care (ICU) Nurse\n");
        printf("5. Specialized Positions\n");
        printf("Enter position(1-5): ");
        scanf("%d", &post);
        
        if (post == 1)
        {
            strcpy(n->nur_position, "Ward Nurse"); //coping string and write in
            break;
        }
        else if (post == 2)
        {
            strcpy(n->nur_position, "Outpatient Nurse"); 
            break;
        }
        else if (post == 3)
        {
            strcpy(n->nur_position, "Operating Room (OR) Nurse"); 
            break;
        }
         else if (post == 4)
        {
            strcpy(n->nur_position, "Critical Care (ICU) Nurse"); 
            break;
        }
         else if (post == 5)
        {
            strcpy(n->nur_position, "Specialized Positions"); 
            break;
        }
        else
        {
             printf("Please choose the correct number(1-5)!\n");
             printf("Enter position(1-5): ");
        }
     }while (1);
    
    do{
        int depart;
        printf("1. ENT\n");
        printf("2. Emergency medical care\n");
        printf("3. Radiology\n");
        printf("4. General surgery\n");
        printf("5. Neurology\n");
        printf("Enter department(1-5): ");
        scanf("%d", &depart);
        
        if (depart == 1)
        {
            strcpy(n->nur_department, "ENT"); 
            break;
        }
        else if (depart == 2)
        {
            strcpy(n->nur_department, "Emergency medical care"); 
            break;
        }
        else if (depart == 3)
        {
            strcpy(n->nur_department, "Radiology"); 
            break;
        }
         else if (depart == 4)
        {
            strcpy(n->nur_department, "General surgery"); 
            break;
        }
         else if (depart == 5)
        {
            strcpy(n->nur_department, "Neurology"); 
            break;
        }
        else
        {
             printf("Please choose the correct number(1-5)!\n");
             printf("Enter department(1-5): ");
        }
     }while (1);
    

    do{
        int status;
        printf("1. Permanent\n");
        printf("2. Contract-bond\n");
        printf("3. Intern\n");       
        printf("Enter Employment Status(1-3): ");

        scanf("%d", &status);
        if (status == 1)
        {
           strcpy(n->nur_employment, "Permanent");
           break;
        } 
        else if (status == 2)
        {
           strcpy(n->nur_employment, "Contract-bond");
           break;
        }
        else if (status == 3)
        {
           strcpy(n->nur_employment, "Intern");
           break;
        }
        else
        {
            printf("Please choose the correct number(1-3)!\n");
        }
      }while(1); 
     
    printf("Enter Salary: ");
    scanf("%f", &n->nur_salary);

    FILE *fptr = fopen("csv/Nurse.csv", "a"); //file position
    if (fptr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fptr, "%s,%s,%c,%s,%s,%s,%s,%s,%.2f\n",
        n->nur_ID, n->nur_name,n->nur_gender, n->nur_contact,n->nur_duty,
         n->nur_position,n->nur_department, n->nur_employment, n->nur_salary);


    fclose(fptr);
    printf("Data written successfully! Assigned NurseID: %s\n", n->nur_ID);
}

// struct patient
//    {
//         char Patient_id[11];
//         char Patient_name[30];
//         char Patient_ic[14];
//         int Patient_age;
//         char Patient_contact[15];  
//         char Patient_email[30];
//         char Patient_emergency[20];
//     };

int isICunique(const char *ic){
    FILE *patientfile = fopen("csv/Patient.csv", "r");
    if(patientfile == NULL)
{
    printf("Error opening patient file!\n");
    return 0;
}
    char line[512];
    while(fgets(line, sizeof(line), patientfile))
    {
        char temp[512];
        strcpy(temp, line); // copy line to temp for tokenization
        char *P_id = strtok(temp, ","); //patient ID
        char *p_name = strtok(NULL, ","); // patient name
        char *p_ic = strtok(NULL, ","); // patient IC
        if(p_ic &&strcmp(p_ic, ic) == 0)
    {
        fclose(patientfile);
        return 1; // IC already exists
    }

    }
    fclose(patientfile);
    return 0; // IC is unique
}

// Create patient record
void createPatientRecord(struct patient *p) 
{
    generateID(p->Patient_id,'P', 4);
    printf("Assigned PatientID: %s\n", p->Patient_id);

    // Collect input
   buffer(); // clear newline
    printf("Enter Name: ");
    fgets(p->Patient_name, sizeof(p->Patient_name), stdin);
    p->Patient_name[strcspn(p->Patient_name, "\n")] = '\0';
    char fixed[50];
    namefixed(p->Patient_name, fixed);
    strcpy(p->Patient_name, fixed);

    printf("Enterpatient IC number(12 digits only): ");
        do {
            scanf("%12s", p->Patient_ic);

            int digits = strlen(p->Patient_ic);

            if (digits < 12 || digits > 12 || is_string_numeric(p->Patient_ic) == 0 || isICunique(p->Patient_ic)) 
            {
                printf("Invalid! IC number must be 12 digits.\n");
                printf("Enter patient IC number(12 digits only): ");
            } else {
                break; // valid
            }
        } while (1);
    printf("Enter patient age: ");
    while(1){
    scanf("%d", &p->Patient_age);
    if (p->Patient_age < 0 || p->Patient_age > 120) 
    {
        printf("Invalid age! Please enter a valid age (0-120): ");
    } else {
        break;
    }
       }

    printf("Enter Contact Number(10-11 digits only): ");
do {
    scanf("%14s", p->Patient_contact);

    int digits = strlen(p->Patient_contact);

    if (digits < 10 || digits > 11 || is_string_numeric(p->Patient_contact) == 0) {
        printf("Invalid! Contact number must be 10 or 11 digits.\n");
        printf("Enter Contact Number(10-11 digits only): ");
    } else {
        break; // valid
    }
} while (1);

    printf("Enter patient email address: ");
    scanf("%s", p->Patient_email);

    printf("Enter patient's emergency contact number(10 - 11 digits): ");
    do {
        scanf("%14s", p->Patient_emergency);

        int digits = strlen(p->Patient_emergency);

        if (digits < 10 || digits > 11 || is_string_numeric(p->Patient_emergency) == 0) {
            printf("Invalid! Contact number must be 10 or 11 digits.\n");
            printf("Enter Contact Number(10-11 digits only): ");
        } else {
            break; // valid
        }
    } while (1);


    FILE *fptr = fopen("../file/Patient.csv", "a"); //file position
    if (fptr == NULL) 
    {
        printf("Error opening file!\n");
        return;
    }
   
    fprintf(fptr, "%s,%s,%s,%d,%s,%s,%s\n",
        p->Patient_id, p->Patient_name, p->Patient_ic, p->Patient_age, 
        p->Patient_contact, p->Patient_email, p->Patient_emergency);

    fclose(fptr);
    printf("Data written successfully! Assigned PatientID: %s\n", p->Patient_id);
}

void modifyDoctor() {
    char targetID[10];
    printf("Enter Doctor ID to modify: ");
    scanf("%s", targetID);
    buffer();
    
    remove("../file/staff/Dr_temp.csv");

    FILE *file = fopen("csv/Dr.csv", "r");
    FILE *temp = fopen("csv/Dr_temp.csv", "w");

    if (!file || !temp) {
        perror("Error opening file");
        return;
    }

    char line[2048];
    int found = 0;

    // Copy header
    fgets(line, sizeof(line), file);
    fprintf(temp, "%s", line);

    while (fgets(line, sizeof(line), file)) {
        char copy[2048];
        strcpy(copy, line);

        // Parse row
        char *drID               = strtok(copy, ",");
        char *drName             = strtok(NULL, ",");
        char *drGender           = strtok(NULL, ",");
        char *drContact          = strtok(NULL, ",");
        char *drEmergency        = strtok(NULL, ",");
        char *drDutyStatus       = strtok(NULL, ",");
        char *drSpecialism       = strtok(NULL, ",");
        char *drEmployment       = strtok(NULL, ",");
        char *drSalary           = strtok(NULL, ",\r\n");  //\r return line

        // Copy into struct
        struct doctor f;
        strcpy(f.dr_id,                drID               ? drID               : "");
        strcpy(f.dr_name,              drName             ? drName             : "");
        f.dr_gender =                  drGender           ? drGender[0]        : '?';
        strcpy(f.dr_contact,           drContact          ? drContact          : "");
        strcpy(f.dr_emergency,         drEmergency        ? drEmergency        : "");
        strcpy(f.dr_duty,              drDutyStatus       ? drDutyStatus       : "");
        strcpy(f.dr_specialism,        drSpecialism       ? drSpecialism       : "");
        strcpy(f.dr_employment,        drEmployment       ? drEmployment       : "");
        f.dr_salary =                  drSalary           ? atof(drSalary)     : 0.0;

        if (drID && strcmp(f.dr_id, targetID) == 0) {
            found = 1;
            int choice;
            char input[100];

            do {
                printf("\n==================== MODIFY DOCTOR ====================\n");
                printf("1. Name              [%s]\n",  f.dr_name);
                printf("2. Gender            [%c]\n",  f.dr_gender);
                printf("3. Contact           [%s]\n",  f.dr_contact);
                printf("4. Emergency Contact [%s]\n",  f.dr_emergency);
                printf("5. Duty Status       [%s]\n",  f.dr_duty);
                printf("6. Specialism        [%s]\n",  f.dr_specialism);
                printf("7. Employment Status [%s]\n",  f.dr_employment);
                printf("8. Salary            [%.2f]\n",f.dr_salary);
                printf("0. Save & Exit\n");
                printf("=======================================================\n");
                printf("Pick field to edit: ");
                scanf("%d", &choice);
                buffer();

                switch (choice) {
                    case 1:
                        printf("New Name: ");
                        fgets(f.dr_name, sizeof(f.dr_name), stdin);
                        f.dr_name[strcspn(f.dr_name, "\r\n")] = 0;
                        break;

                    case 2:
                        printf("New Gender (F/M): ");
                        do {
                            scanf(" %c", &f.dr_gender);
                            f.dr_gender = toupper(f.dr_gender);
                            if (f.dr_gender != 'F' && f.dr_gender != 'M') {
                                printf("Please only enter 'F' or 'M': ");
                            } else {
                                break;
                            }
                        } while (1);
                        buffer();
                        break;

                    case 3:
                        printf("New Contact (10-11 digits): ");
                        do {
                            scanf("%14s", input);
                            int len = strlen(input);
                            if ((len == 10 || len == 11) && is_string_numeric(input)) {
                                strcpy(f.dr_contact, input);
                                break;
                            } else {
                                printf("Invalid! Must be 10 or 11 digits: ");
                            }
                        } while (1);
                        buffer();
                        break;

                    case 4:
                        printf("New Emergency Contact (10-11 digits): ");
                        do {
                            scanf("%14s", input);
                            int len = strlen(input);
                            if ((len == 10 || len == 11) && is_string_numeric(input)) {
                                strcpy(f.dr_emergency, input);
                                break;
                            } else {
                                printf("Invalid! Must be 10 or 11 digits: ");
                            }
                        } while (1);
                        buffer();
                        break;

                    case 5:
                        printf("New Duty Status (on/off): ");
                        do {
                            scanf("%29s", input);
                            input[0] = toupper(input[0]);
                            for (int i = 1; input[i] != '\0'; i++)
                                input[i] = tolower(input[i]);
                            if (strcmp(input, "On") == 0 || strcmp(input, "Off") == 0) {
                                strcpy(f.dr_duty, input);
                                break;
                            } else {
                                printf("Please only enter 'on' or 'off': ");
                            }
                        } while (1);
                        buffer();
                        break;

                    case 6:
                        printf("1. ENT\n2. Emergency medical care\n");
                        printf("3. Radiology\n4. General surgery\n5. Neurology\n");
                        printf("New Specialism (1-5): ");
                        do {
                            int special;
                            scanf("%d", &special);
                            buffer();
                            if      (special == 1)
                             { strcpy(f.dr_specialism, "ENT");                    
                                break; }
                            else if (special == 2) 
                            { strcpy(f.dr_specialism, "Emergency medical care");
                                 break; }
                            else if (special == 3)
                             { strcpy(f.dr_specialism, "Radiology");
                                              break; }
                            else if (special == 4) 
                            { strcpy(f.dr_specialism, "General surgery");
                                        break; }
                            else if (special == 5)
                            { strcpy(f.dr_specialism, "Neurology"); 
                                            break; }
                            else printf("Invalid! Choose 1-5: ");
                        } while (1);
                        break;

                    case 7:
                        printf("1. Permanent\n2. Contract-bond\n3. Intern\n");
                        printf("New Employment Status (1-3): ");
                        do {
                            int status;
                            scanf("%d", &status);
                            buffer();
                            if      (status == 1)
                             { strcpy(f.dr_employment, "Permanent");    
                                 break; }
                            else if (status == 2) 
                            { strcpy(f.dr_employment, "Contract-bond");
                                 break; }
                            else if (status == 3) 
                            { strcpy(f.dr_employment, "Intern");    
                                    break; }
                            else printf("Invalid! Choose 1-3: ");
                        } while (1);
                        break;

                    case 8:
                        printf("New Salary: ");
                        scanf("%f", &f.dr_salary);
                        buffer();
                        break;

                    case 0:
                        printf("Saving changes...\n");
                        break;

                    default:
                        printf("Invalid choice. Try again.\n");
                }

            } while (choice != 0);

            // Write updated row — same format as createDoctorRecord
            fprintf(temp, "%s,%s,%c,%s,%s,%s,%s,%s,%.2f\n",
                    f.dr_id,
                    f.dr_name,
                    f.dr_gender,
                    f.dr_contact,
                    f.dr_emergency,
                    f.dr_duty,
                    f.dr_specialism,
                    f.dr_employment,
                    f.dr_salary);

        } else {
            fprintf(temp, "%s", line); // copy unchanged
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("csv/Dr.csv");
        rename("csv/Dr_temp.csv", "csv/Dr.csv");
        printf("Doctor %s updated successfully.\n", targetID);
    } else {
        remove(".csv/Dr_temp.csv");
        printf("Doctor ID %s not found.\n", targetID);
    }
}

void modifyNurse() {
    char targetID[10];
    printf("Enter Nurse ID to modify: ");
    scanf("%s", targetID);
    buffer();

    remove("../file/staff/Nurse_temp.csv");

    FILE *file = fopen("csv/Nurse.csv", "r");
    FILE *temp = fopen("csv/Nurse_temp.csv", "w");

    if (!file || !temp) {
        perror("Error opening file");
        return;
    }

    char line[2048];
    int found = 0;

    // Copy header
    fgets(line, sizeof(line), file);
    fprintf(temp, "%s", line);

    while (fgets(line, sizeof(line), file)) {
        char copy[2048];
        strcpy(copy, line);

        // Parse row 
        char *nurID               = strtok(copy, ",");
        char *nurName             = strtok(NULL, ",");
        char *nurGender           = strtok(NULL, ",");
        char *nurContact          = strtok(NULL, ",");
        char *nurDuty             = strtok(NULL, ",");
        char *nurPosition         = strtok(NULL, ",");
        char *nurDepartment       = strtok(NULL, ",");
        char *nurEmployment       = strtok(NULL, ",");
        char *nurSalary           = strtok(NULL, ",\r\n");

        // Copy into struct
        struct nurse n;
        strcpy(n.nur_ID,                nurID               ? nurID               : "");
        strcpy(n.nur_name,              nurName             ? nurName             : "");
        n.nur_gender =                  nurGender           ? nurGender[0]        : '?';
        strcpy(n.nur_contact,           nurContact          ? nurContact          : "");
        strcpy(n.nur_duty,              nurDuty             ? nurDuty             : "");
        strcpy(n.nur_position,          nurPosition         ? nurPosition         : "");
        strcpy(n.nur_department,        nurDepartment       ? nurDepartment       : "");
        strcpy(n.nur_employment,        nurEmployment       ? nurEmployment       : "");
        n.nur_salary =                  nurSalary           ? atof(nurSalary)     : 0.0;

        if (nurID && strcmp(n.nur_ID, targetID) == 0) {
            found = 1;
            int choice;
            char input[100];

            do {
                printf("\n==================== MODIFY NURSE ====================\n");
                printf("1. Name              [%s]\n",  n.nur_name);
                printf("2. Gender            [%c]\n",  n.nur_gender);
                printf("3. Contact           [%s]\n",  n.nur_contact);
                printf("4. Duty Status       [%s]\n",  n.nur_duty);
                printf("5. Position          [%s]\n",  n.nur_position);
                printf("6. Specialism        [%s]\n",  n.nur_department);
                printf("7. Employment Status [%s]\n",  n.nur_employment);
                printf("8. Salary            [%.2f]\n",n.nur_salary);
                printf("0. Save & Exit\n");
                printf("=======================================================\n");
                printf("Pick field to edit: ");
                scanf("%d", &choice);
                buffer();

                switch (choice) {
                    case 1:
                        printf("New Name: ");
                        fgets(n.nur_name, sizeof(n.nur_name), stdin);
                        n.nur_name[strcspn(n.nur_name, "\r\n")] = 0;
                        break;

                    case 2:
                        printf("New Gender (F/M): ");
                        do {
                            scanf(" %c", &n.nur_gender);
                            n.nur_gender = toupper(n.nur_gender);
                            if (n.nur_gender != 'F' && n.nur_gender != 'M') {
                                printf("Please only enter 'F' or 'M': ");
                            } else {
                                break;
                            }
                        } while (1);
                        buffer();
                        break;

                    case 3:
                        printf("New Contact (10-11 digits): ");
                        do {
                            scanf("%14s", input);
                            int len = strlen(input);
                            if ((len == 10 || len == 11) && is_string_numeric(input)) {
                                strcpy(n.nur_contact, input);
                                break;
                            } else {
                                printf("Invalid! Must be 10 or 11 digits: ");
                            }
                        } while (1);
                        buffer();
                        break;

                    case 4:
                        printf("New Duty Status (on/off): ");
                        do {
                            scanf("%29s", input);
                            input[0] = toupper(input[0]);
                            for (int i = 1; input[i] != '\0'; i++)
                                input[i] = tolower(input[i]);
                            if (strcmp(input, "On") == 0 || strcmp(input, "Off") == 0) {
                                strcpy( n.nur_duty, input);
                                break;
                            } else {
                                printf("Please only enter 'on' or 'off': ");
                            }
                        } while (1);
                        buffer();
                        break;

                    case 5:
                            printf("1. Ward Nurse\n2. Outpatient Nurse\n");
                            printf("3. Operating Room (OR) Nurse\n4. Critical Care (ICU) Nurse\n");
                            printf("5. Specialized Positions\nEnter position(1-5): ");
                            do{
                                int post;
                                scanf("%d", &post);
                                buffer();
                                if    (post == 1)
                                {strcpy(n.nur_position, "Ward Nurse");      
                                    break;}
                                else if (post == 2)
                                {strcpy(n.nur_position, "Outpatient Nurse"); 
                                break;}
                                 else if (post == 3)
                                {strcpy(n.nur_position, "Operating Room (OR) Nurse"); 
                                break;}
                                 else if (post == 4)
                                {strcpy(n.nur_position, "Critical Care (ICU) Nurse"); 
                                break;}
                                 else if (post == 5)
                                {strcpy(n.nur_position, "Specialized Positions"); 
                                break;}
                                else printf("Invalid! Choose 1-5: ");
                            }while(1); 
                            break; 
                            
                    case 6:
                        printf("1. ENT\n2. Emergency medical care\n");
                        printf("3. Radiology\n4. General surgery\n5. Neurology\n");
                        printf("New Specialism (1-5): ");
                        do {
                            int special;
                            scanf("%d", &special);
                            buffer();
                            if      (special == 1) 
                            { strcpy(n.nur_department, "ENT");                    
                                break; }
                            else if (special == 2) 
                            { strcpy(n.nur_department, "Emergency medical care"); 
                                break; }
                            else if (special == 3) 
                            { strcpy(n.nur_department, "Radiology");              
                                break; }
                            else if (special == 4) 
                            { strcpy(n.nur_department, "General surgery");        
                                break; }
                            else if (special == 5) 
                            { strcpy(n.nur_department, "Neurology");             
                                break; }
                            else printf("Invalid! Choose 1-5: ");
                        } while (1);
                        break;

                    case 7:
                        printf("1. Permanent\n2. Contract-bond\n3. Intern\n");
                        printf("New Employment Status (1-3): ");
                        do {
                            int status;
                            scanf("%d", &status);
                            buffer();
                            if      (status == 1) 
                            { strcpy(n.nur_employment, "Permanent");    
                                 break; }
                            else if (status == 2) 
                            { strcpy(n.nur_employment, "Contract-bond");
                                 break; }
                            else if (status == 3) 
                            { strcpy(n.nur_employment, "Intern");        
                                break; }
                            else printf("Invalid! Choose 1-3: ");
                        } while (1);
                        break;

                    case 8:
                        printf("New Salary: ");
                        scanf("%f", &n.nur_salary);
                        buffer();
                        break;

                    case 0:
                        printf("Saving changes...\n");
                        break;

                    default:
                        printf("Invalid choice. Try again.\n");
                }

            } while (choice != 0);

            // Write updated row — same format as createDoctorRecord
            fprintf(temp, "%s,%s,%c,%s,%s,%s,%s,%s,%.2f\n",
                   n.nur_ID,
                   n.nur_name,
                   n.nur_gender,
                   n.nur_contact,
                   n.nur_duty,
                   n.nur_position,
                   n.nur_department,
                   n.nur_employment,
                   n.nur_salary);

        } else {
            fprintf(temp, "%s", line); // copy unchanged
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("csv/Nurse.csv");
        rename("csv/Nurse_temp.csv", "csv/Nurse.csv");
        printf("Nurse %s updated successfully.\n", targetID);
    } else {
        remove("csv/Nurse_temp.csv");
        printf("/Nurse ID %s not found.\n", targetID);
    }
}

void modifyPatient() {
    char targetID[10];
    printf("Enter Patient ID to modify: ");
    scanf("%s", targetID);
    buffer();

    remove("csv/Patient_temp.csv");

    FILE *file = fopen("csv/Patient.csv", "r");
    FILE *temp = fopen("csv/Patient_temp.csv", "w");

    if (!file || !temp) {
        perror("Error opening file");
        return;
    }

    char line[2048];
    int found = 0;

    // Copy header
    fgets(line, sizeof(line), file);
    fprintf(temp, "%s", line);

    while (fgets(line, sizeof(line), file)) {
        char copy[2048];
        strcpy(copy, line);

        // Parse row
        char *patientID        = strtok(copy, ",");
        char *patientName      = strtok(NULL, ",");
        char *patientIC        = strtok(NULL, ",");
        char *patientAge       = strtok(NULL, ",");
        char *patientContact   = strtok(NULL, ",");
        char *patientEmail     = strtok(NULL, ",");
        char *patientEmergency = strtok(NULL, ",\r\n");

        // Copy into struct
        struct patient p;
        strcpy(p.Patient_id,         patientID        ? patientID        : "");
        strcpy(p.Patient_name,      patientName      ? patientName      : "");
        strcpy(p.Patient_ic,        patientIC        ? patientIC        : "");
        p.Patient_age =             patientAge       ? atoi(patientAge) : 0;
        strcpy(p.Patient_contact,   patientContact   ? patientContact   : "");
        strcpy(p.Patient_email,     patientEmail     ? patientEmail     : "");
        strcpy(p.Patient_emergency, patientEmergency ? patientEmergency : "");

        if (patientID && strcmp(p.Patient_id, targetID) == 0) {
            found = 1;
            int choice;
            char input[100];

            do {
                printf("\n==================== MODIFY PATIENT ====================\n");
                printf("1. Name          [%s]\n", p.Patient_name);
                printf("2. IC            [%s]\n", p.Patient_ic);
                printf("3. Age           [%d]\n", p.Patient_age);
                printf("4. Contact       [%s]\n", p.Patient_contact);
                printf("5. Email         [%s]\n", p.Patient_email);
                printf("6. Emergency     [%s]\n", p.Patient_emergency);
                printf("0. Save & Exit\n");
                printf("========================================================\n");
                printf("Pick field to edit: ");
                scanf("%d", &choice);
                buffer();

                switch (choice) {
                    case 1:
                        printf("New Name: ");
                        fgets(p.Patient_name, sizeof(p.Patient_name), stdin);
                        p.Patient_name[strcspn(p.Patient_name, "\r\n")] = 0;
                        break;

                    case 2:
                        printf("New IC: ");
                        scanf("%12s", input);
                        buffer();
                        if (strlen(input) == 12 &&
                            is_string_numeric(input) &&
                            !isICunique(input)) {
                            strcpy(p.Patient_ic, input);
                        } else {
                            printf("Invalid IC — not updated.\n");
                        }
                        break;

                    case 3:
                        printf("New Age: ");
                        scanf("%d", &p.Patient_age);
                        buffer();
                        if (p.Patient_age < 0 || p.Patient_age > 120) {
                            printf("Invalid age — not updated.\n");
                            p.Patient_age = atoi(patientAge); // restore old
                        }
                        break;

                    case 4:
                        printf("New Contact: ");
                        scanf("%14s", input);
                        buffer();
                        if ((strlen(input) == 10 || strlen(input) == 11) &&
                            is_string_numeric(input)) {
                            strcpy(p.Patient_contact, input);
                        } else {
                            printf("Invalid contact — not updated.\n");
                        }
                        break;

                    case 5:
                        printf("New Email: ");
                        scanf("%s", p.Patient_email);
                        buffer();
                        break;

                    case 6:
                        printf("New Emergency Contact: ");
                        scanf("%14s", input);
                        buffer();
                        if ((strlen(input) == 10 || strlen(input) == 11) &&
                            is_string_numeric(input)) {
                            strcpy(p.Patient_emergency, input);
                        } else {
                            printf("Invalid contact — not updated.\n");
                        }
                        break;

                    case 0:
                        printf("Saving changes...\n");
                        break;

                    default:
                        printf("Invalid choice. Try again.\n");
                }

            } while (choice != 0);

            // Write updated row — fixed capitalization to match struct
            fprintf(temp, "%s,%s,%s,%d,%s,%s,%s\n",
                    p.Patient_id,          
                    p.Patient_name,       
                    p.Patient_ic,         
                    p.Patient_age,       
                    p.Patient_contact,    
                    p.Patient_email,      
                    p.Patient_emergency); 

        } else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(file);
    fclose(temp);

    if (found) {
        remove("csv/Patient.csv");
        rename("csv/Patient_temp.csv", "csv/Patient.csv");
        printf("Patient %s updated successfully.\n", targetID);
    } else {
        remove("csv/Patient_temp.csv");
        printf("Patient ID %s not found.\n", targetID);
    }
}

void modifyAccountMenu() {
    int sub_choice;

    printf("1. Doctor\n2. Nurse\n3. Patient\n4. Back to account management main menu\n Which need to be modify: ");
    scanf("%d", &sub_choice);

    switch(sub_choice) {
        case 1: modifyDoctor();
        break;
        case 2: modifyNurse();
        break;
        case 3: modifyPatient();
        break;
        case 4:
        break;
        default: printf("Invalid option. please try again!\n");
    }
}

void account_management_sys()
{ 
  
    int choice, sub_choice, sub;
     do{
            printf("1. View/ Search account management\n");
            printf("2. Add new account\n");
            printf("3. Modify account\n");
            printf("4. Back to admin main menu\n");
            printf(" Enter the option: ");

            scanf("%d", &choice);

               if (choice == 1)
                {
                 do{
                    printf("1. View account information\n");
                    printf("2. Search account information\n");
                    printf("3. Back to account management menu\n");
                    printf("Enter the option: ");

                    scanf("%d", &sub_choice);

                   if(sub_choice == 1)
                   {
                   displayaccount("csv/Dr.csv", "csv/Nurse.csv", "csv/Patient.csv"); // path for file
                   }
                   else if(sub_choice == 2)
                   {
                    searchaccount("csv/Dr.csv", "csv/Nurse.csv", "csv/Patient.csv"); // path for file
                   }
                   else if(sub_choice == 3)
                   {
                    printf("Back to account management menu\n");
                   } 
                   else
                   {
                    printf("Invalid input. Plaese try again!\n");
                   }
                  }while(sub_choice != 3);

                }
                else if (choice == 2)
                { 
                    do{
                        printf("1. Add new doctor\n");
                        printf("2. Add new nurse\n");
                        printf("3. Add new patient\n");
                        printf("4. Back to staff management menu\n");
                        printf("Enter the option: ");

                        scanf("%d", &sub_choice);

                        if(sub_choice == 1)
                        {
                            struct doctor f;
                                // Seed random number generator
                                srand(time(NULL));
                                createDoctorRecord(&f);
                        }
                        else if(sub_choice == 2)
                        {
                             struct nurse n;
                            srand(time(NULL));
                            createNurseRecord(&n);
                        }
                        else if(sub_choice == 3)
                        {
                                struct patient p;
                                srand(time(NULL));
                                createPatientRecord(&p);
                        }
                        else if (sub_choice == 4)
                        {
                            printf("Back to management menu\n");
                        }
                        else
                        {
                            printf("Invalid input. Please try again!\n");
                        }
                    }while(sub_choice != 4);
                }
                else if(choice == 3)
                {
                   modifyAccountMenu();
                }
                 else if (choice == 4)
                {
                    printf("Back to admin main menu...\n");
                } else
                {
                    printf("Invalid option. please try again!\n");
                }

        } while (choice != 4);
}