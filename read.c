#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "hospital.h"

void readDoctor(Doctor* drs, int *count) {
    FILE *file = fopen(DOCTORFILE, "r");
    if (!file) {
        perror("Doctor file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];
    
    // Skip header
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    // LINE-BY-LINE (works with ANY CSV format!)
    while (fgets(line, sizeof(line), file)) {
        // Use sscanf for flexibility
        char id[MAX_ID];
        char name[MAX_NAME];
        char gender;
        char emergencyNum[MAX_CONTACT];
        char contact[MAX_CONTACT];
        char duty[4];
        char specialism[MAX_SPEC];
        char employment[MAX_SPEC];
        float salary;
        
        //parsing the csv file
        int fields = sscanf(line, "%9[^,], %100[^,], %c , %12[^,], %12[^,], %3[^,], %24[^,], %24[^,], %f%*c", 
         id, name, &gender, emergencyNum, contact, duty, specialism, employment, &salary); 

        if (fields != 9) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        // Copy The Value to the struct inside The "hospital.h"
        strncpy(drs[*count].id, id, MAX_ID-1);
        strncpy(drs[*count].name, name, MAX_NAME-1);
        drs[*count].gender = gender;
        strncpy(drs[*count].emergencyNum, emergencyNum, MAX_CONTACT-1);
        strncpy(drs[*count].contact, contact, MAX_CONTACT-1);
        strncpy(drs[*count].duty, duty, 3);
        strncpy(drs[*count].specialism, specialism, MAX_SPEC-1);
        strncpy(drs[*count].employment, employment, MAX_SPEC-1);
        drs[*count].salary = salary;

        // Null terminator for string
        drs[*count].id[MAX_ID-1] = drs[*count].name[MAX_NAME-1] = drs[*count].emergencyNum[MAX_CONTACT-1]= drs[*count].contact[MAX_CONTACT-1]= drs[*count].duty[3] = drs[*count].specialism[MAX_SPEC-1] = drs[*count].employment[MAX_SPEC-1] = '\0';


        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
}

void readNurse(Nurse* nurses, int *count){
    FILE *file = fopen(NURSEFILE, "r");

    if (!file) {
        printf("Nurse file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];
    
    // Skip header
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    // LINE-BY-LINE (works with ANY CSV format!)
    while (fgets(line, sizeof(line), file)) {
        // Use sscanf for flexibility
        char id[MAX_ID];
        char name[MAX_NAME];
        int contact;
        char duty[4];
        char position[MAX_SPEC];
        char dpmnt[MAX_SPEC];
        char employment[MAX_SPEC];
        float salary;
        
        //parsing the csv file
        int fields = sscanf(line, "%12[^,],%100[^,],%d,%3[^,],%25[^,],%25[^,],%25[^,],%f", 
        id, name, &contact, duty, position, dpmnt, employment, &salary); 

        if (fields != 8) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        // Copy The Value to the struct inside The "hospital.h"
        strncpy(nurses[*count].id, id, MAX_ID-1);
        strncpy(nurses[*count].name, name, MAX_NAME-1);
        nurses[*count].contact = contact;
        strncpy(nurses[*count].duty, duty, 3);
        strncpy(nurses[*count].position, position, MAX_SPEC-1);
        strncpy(nurses[*count].department, dpmnt, MAX_SPEC-1);
        strncpy(nurses[*count].employment, employment, MAX_SPEC-1);
        nurses[*count].salary = salary;

        // Null terminator for string
        nurses[*count].id[MAX_ID-1] = nurses[*count].name[MAX_NAME-1] = nurses[*count].duty[3] = nurses[*count].position[MAX_SPEC-1] =  nurses[*count].department[MAX_SPEC-1] = nurses[*count].employment[MAX_SPEC-1] = '\0';


        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
       
}

void readPatient(Patient* patients, int *count){
    FILE *file = fopen(PATIENTFILE, "r");

    if (!file) {
        printf("Patient file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char id[MAX_ID];
        char name[MAX_NAME];
        char ic[15];
        int age;
        char contact[MAX_CONTACT];
        char email[MAX_NAME];
        char emergency[MAX_CONTACT];
        //parsing the csv file
        int fields = sscanf(line, "%9[^,],%99[^,],%14[^,],%d,%12[^,],%100[^,],%12[^,]", 
        id, name, ic, &age, contact, email, emergency); 

        if (fields != 7) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        strncpy(patients[*count].id, id, MAX_ID-1);
        strncpy(patients[*count].name, name, MAX_NAME-1);
        strncpy(patients[*count].ic, ic, 14);
        patients[*count].age = age;
        strncpy(patients[*count].contact, contact, MAX_CONTACT-1);
        strncpy(patients[*count].email, email, MAX_NAME-1);
        strncpy(patients[*count].emergency, emergency, MAX_CONTACT-1);

        // Null terminator for string
        patients[*count].id[MAX_ID-1] = patients[*count].name[MAX_NAME-1] = patients[*count].email[MAX_NAME-1] = patients[*count].emergency[MAX_CONTACT-1] = patients[*count].contact[MAX_CONTACT-1] = patients[*count].ic[14] = '\0';


        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
       
}

void readAppointment(Appointment* appointments, int* count){
    FILE *file = fopen(APPOINTMENTFILE, "r");

    if (!file) {
        printf("Appointment file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char patientID[MAX_ID];
        char drID[MAX_ID];
        char date[11];
        char time[6];
        char stat[MAX_SPEC];
        char purpose[MAX_INPUT];
        //parsing the csv file
        int fields = sscanf(line, "%9[^,],%9[^,],%10[^,],%5[^,],%9[^,],%150[^,]", 
        patientID, drID, date, time, stat, purpose); 

        if (fields != 6) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        purpose[strcspn(purpose, "\n\r")]= '\0';

        strncpy(appointments[*count].patientID, patientID, MAX_ID-1);
        strncpy(appointments[*count].drID, drID, MAX_ID-1);
        strncpy(appointments[*count].date, date, 10);
        strncpy(appointments[*count].time, time, 5);
        strncpy(appointments[*count].stat, stat, MAX_SPEC-1);
        strncpy(appointments[*count].purpose, purpose, MAX_INPUT-1);

       appointments[*count].patientID[MAX_ID-1] = appointments[*count].drID[MAX_ID-1] = appointments[*count].date[10] = appointments[*count].time[5] = appointments[*count].stat[MAX_SPEC-1] =  appointments[*count].purpose[MAX_INPUT-1] = '\0';


        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
    
}

void readComplaint(Complaint* complaints, int* count){
    FILE *file = fopen(COMPLAINTFILE, "r");

    if (!file) {
        printf("Complaint file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char patientID[MAX_ID];
        char reason[MAX_INPUT];
        char date[11];
        char time[6];
        //parsing the csv file
        int fields = sscanf(line, "%9[^,],%150[^,],%10[^,],%5[^,]", 
        patientID, reason, date, time); 
        //later if the number of it being scanned changed font forget to change the fiels!!!!!
        if (fields != 4) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        time[strcspn(time, "\n\r")] = '\0';

        strncpy(complaints[*count].patientID, patientID, MAX_ID-1);
        strncpy(complaints[*count].date, date, 10);
        strncpy(complaints[*count].time, time, 5);
        strncpy(complaints[*count].details, reason, MAX_INPUT-1);

        complaints[*count].patientID[MAX_ID-1] = complaints[*count].details[MAX_INPUT-1] = complaints[*count].date[10] = complaints[*count].time[5] = '\0';


        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
    
}

void readSchedule(Schedule* schedules, int* count){
    FILE *file = fopen(SCHEDULEFILE, "r");

    if (!file) {
        perror("Schedule file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char patientID[MAX_ID];
        char drID[MAX_ID];
        char date[11];
        char time[6];
        char activity[100];
        //parsing the csv file
        int fields = sscanf(line, "%10[^,],%10[^,],%11[^,],%6[^,],%100[^,]", 
        drID, patientID, date, time, activity); 

        if (fields != 5) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        activity[strcspn(activity, "\n")] = '\0';

        strncpy(schedules[*count].patientID, patientID, MAX_ID-1);
        strncpy(schedules[*count].drID, drID, MAX_ID-1);
        strncpy(schedules[*count].date, date, 10);
        strncpy(schedules[*count].time, time, 5);
        strncpy(schedules[*count].activity, activity, 100);

        schedules[*count].patientID[MAX_ID-1] = schedules[*count].drID[MAX_ID-1] =schedules[*count].date[10] = schedules[*count].time[5]=schedules[*count].activity[100] = '\0';
        
        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
}

void readInvoice(Bill *bills, int *count){
    FILE *file = fopen(INVOICEFILE, "r");
    if (!file) {
        printf("Invoice file not found!\n");
        return;
    }

    *count = 0;
    char line[1024];

    fgets(line, sizeof(line), file); //skip header

    while(fgets(line, sizeof(line), file)){
        char dup[1024];
        strcpy(dup, line);

        char *b_ID = strtok(dup, ",");
        strcpy(bills[*count].bill_id, b_ID);

        char *p_ID = strtok(NULL, ",");
        strcpy(bills[*count].patient_id, p_ID);

        char *adminID = strtok(NULL, ",");
        strcpy(bills[*count].admin_id, adminID);

        char *itemCount = strtok(NULL, ",");
        int itemCounts = itemCount ? atoi(itemCount) : 0;
        bills[*count].count = itemCounts;

        for(int i = 0; i < itemCounts; i++){
            char *id = strtok(NULL, ",");
            strcpy(bills[*count].items[i].bill_category, id);

            char *quantity = strtok(NULL, ",");
            int qty = quantity ? atoi(quantity) : 0;
            bills[*count].items[i].bill_sub = qty;

            char *subprice = strtok(NULL, ",");
            float price = subprice ? atof(subprice) : 0.00;
            bills[*count].items[i].bill_sub_price = price;
        }

        char *billTotal = strtok(NULL, ",");
        float total = billTotal ? atof(billTotal) : 0.00;
        bills[*count].bill_total = total;

        char *billDate  = strtok(NULL, ",");
        strcpy(bills[*count].bill_date, billDate);

        char *payDate = strtok(NULL, ",");
        strcpy(bills[*count].payment_date, payDate);
        char *payTime = strtok(NULL, ",");
        strcpy(bills[*count].payment_time, payTime);

        char *payMethod = strtok(NULL, ",\r\n");
        strcpy(bills[*count].payment_method, payMethod);

        (*count)++;

        if(*count>= MAX){
            break;
        }
    }

    fclose(file);
}

void readInventory(Inventory* inv, int *count){

    FILE *file = fopen(INVENTORYFILE, "r");

    if (!file) {
        printf("Schedule file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char id[MAX_ID];
        char name[MAX_SPEC];
        float price;
        char vendor[MAX_SPEC];
        int stock;
        char nurseID[MAX_ID];
        //parsing the csv file
        int fields = sscanf(line, "%9[^,],%24[^,],%f,%24[^,],%d,%9[^,]", 
        id, name, &price, vendor, &stock, nurseID); 

        if (fields != 6) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        strncpy(inv[*count].id, id, MAX_ID-1);
        strncpy(inv[*count].name, name, MAX_SPEC-1);
        inv[*count].price = price;
        strncpy(inv[*count].vendor, vendor, MAX_SPEC-1);
        inv[*count].stock = stock;
        strncpy(inv[*count].nurseID, nurseID, MAX_ID-1);

        inv[*count].nurseID[MAX_ID-1] = inv[*count].vendor[MAX_SPEC-1] =inv[*count].name[MAX_SPEC-1] = inv[*count].id[MAX_ID-1] = '\0';

        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
}

void readCategory(Category* cats, int* count){
    FILE *file = fopen(CATEGORYFILE, "r");

    if (!file) {
        printf("Category file not found!\n");
        *count = 0;
        return;
    }

    *count = 0;
    char line[512];

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        char id[MAX_ID];
        char detail[MAX_SPEC];
        float price;
        //parsing the csv file
        int fields = sscanf(line, "%10[^,],%25[^,],%f", id, detail, &price);

        if (fields != 3) {
            printf("Parse error: %s\n", line);
            continue;
        }
        
        strncpy(cats[*count].id, id, MAX_ID-1);
        strncpy(cats[*count].detail, detail, MAX_SPEC-1);
        cats[*count].price = price;

       cats[*count].id[MAX_ID-1] = cats[*count].detail[MAX_SPEC-1] = '\0';

        (*count)++;
        
        if (*count >= MAX){
            break;
        } 
    }
    fclose(file);
}
