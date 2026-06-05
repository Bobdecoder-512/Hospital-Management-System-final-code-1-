#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <stdlib.h>
#include "hospital.h"

extern char current_logged_in_id[20];

#define DOCTOR_FILE 'doctor.csv'
#define TOTAL_SPECIALITY 20

//utils FUNCTION
void getTime(char *date, char *strTime){
    time_t now = time(NULL);

    struct tm *curTime = localtime(&now);
    //better not to use %x (date) and %X (time), since it follows local device format
    
    strftime(date, 11, "%d/%m/%Y", curTime);
    strftime(strTime, 6, "%H:%M", curTime); // H for 0-23, M for minutes..
}

int exitCode(char *exx){
    char x[25];
    strcpy(x, exx);
    strlwr(x);

    return strcmp(x, "exit") != 0;
}

int validateDate(const char *date) {
    int year, month, day;

    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3) {
        return 0;
    }

    if (month < 1 || month > 12) {
        return 0;
    }

    int daysInMonth[] = {
        31, 28, 31, 30,
        31, 30, 31, 31,
        30, 31, 30, 31
    };

    if ((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
        daysInMonth[1] = 29;
    }

    if (day < 1 || day > daysInMonth[month - 1]) {
        return 0;
    }

    return 1;
}

//Appointment Function
int id_inputMatcher(char* patientID, Appointment* appointments, int aptCount, int selectedApt){
    int dispNum = 1;
    for (int i = 0; i < aptCount; i++){
        if(strcmp(patientID, appointments[i].patientID) != 0){
            continue;
        }            
        if(strcmp(appointments[i].stat, "UP-COMING") != 0){ //Need to change the status later
            continue;
        }

        if(dispNum == selectedApt){
            return i;
        }
            
        dispNum++;        
    }

    return -1;
}

void getDrName(Doctor* drs, int drCount, char* drID, char* drName){
    strcpy(drName, " ");
    
    for(int i = 0; i < drCount; i++){
        if(strcmp(drs[i].id, drID) == 0){
            strcpy(drName, drs[i].name);
            return;
        }
    }
}

void headerApt(){
    printf("===============================================================================================================================================\n");
    printf("|%-3s|%-11s|%-6s|%-24s|%-12s|%-80s|\n", "No.","Date", "Time", "Dr. Name", "Status", "Purpose");
    printf("===============================================================================================================================================\n");
}

int dispUpComingApt(Appointment* appointments, int aptCount, char* patientID, Doctor* drs, int drCount, const char* label){ //const because the value is set from the begining(will be constant)
    int found = 0;
    int dispNum = 1;

    headerApt();    

    for(int i = 0; i < aptCount; i++){
        if(strcmp(patientID, appointments[i].patientID) != 0){
            continue;
        }

        if(strcmp(label, "View") != 0){
            if(strcmp(appointments[i].stat, "UP-COMING") != 0){
                continue;
            }
        }
        
        //logic for dates??

        char drName[MAX_NAME];
        getDrName(drs, drCount, appointments[i].drID, drName);

        printf("|%-3d|%-11s|%-6s|%-24s|%-12s|%-80s|\n", dispNum, appointments[i].date, appointments[i].time, drName, appointments[i].stat, appointments[i].purpose);

        dispNum++;
        found = 1;
    }
    if(!found){
        printf("No appointment record that can be %s\n", label);
    }

    printf("===============================================================================================================================================\n");
    return found;
}   

int handleDate(char* drID, Schedule* schedules, int scheCount, char shownDates[][11]){//passing 2D array
    int dateCount=0;

    printf("\nAvailable dates:\n");
    for (int i = 0; i < scheCount; i++){


        if(strcmp(drID, schedules[i].drID) != 0){
            continue;
        }

        if(strcmp(schedules[i].activity, "VACANT") != 0){
            continue;
        }

        int exists = 0; //reset

        for(int j= 0; j < dateCount; j++){
            if(strcmp(shownDates[j], schedules[i].date) == 0){
                exists = 1;
                break;
            }
        }
        
        if(!exists){
            strcpy(shownDates[dateCount], schedules[i].date);
            dateCount++;
        }
    }

    return dateCount;
}

int getDateInput(int dateCount, char date[][11]){
    int selectedDate;

    for(int i = 0; i < dateCount; i++){
        printf("%d. %s\n", i+1, date[i]);
    }

    while(1){
        printf("Select date: ");
        scanf("%d", &selectedDate);

        //get the index of the date
        if(selectedDate >= 1 && selectedDate <= dateCount){
            return selectedDate - 1;
        }

        printf("Invalid date selected...\n");
    }
}

int handleTime(Schedule* schedules,int scheCount, char* drID, char *date, char time[][6], int realIndex[]){
    int timeCount = 0;

    printf("\nTime slot selection (%s):\n", date);
    for(int i = 0; i < scheCount; i++){
        //filter the doctor
        if(strcmp(schedules[i].drID, drID) != 0){
            continue;
        }
        //filter the date
        if(strcmp(schedules[i].date, date) != 0){
            continue;
        }

        if(strcmp(schedules[i].activity, "VACANT") != 0){
            continue;
        }
        //get the time
        strcpy(time[timeCount], schedules[i].time);

        realIndex[timeCount] = i; 
        timeCount++;
        
    }
    return timeCount;
}

int getTimeInput(int timeCount, char time[][6]){
    int selectedTime;
    int timeIndex = -1;
    
    for(int i = 0; i < timeCount; i++){
        printf("%2d. %s\n", i+1, time[i]);
    }

    while(1){
        printf("Select time slot: ");
        scanf("%d", &selectedTime);

        for(int i = 0; i < timeCount; i++){
            if(selectedTime == i+1){
                return i;
            }
        }

        if(timeIndex == -1){
            printf("Invalid time slot...\n");
            continue;
        }
    }
}

int dateAndTime(char* drID, Schedule* schedules, int scheCount, char* aptDate, char* aptTime, int *index){
    while (1){
        char dates[20][11];
        int dateCount = handleDate(drID, schedules, scheCount, dates);
        if(dateCount == 0){
            printf("No schedule available...\n");
            return 0; //Fail
        }
        //Get the selected doctor schedule...
        int dateIndex = getDateInput(dateCount, dates);

        int realIndex[20];        
        char times[20][6];
        int timeCount = handleTime(schedules, scheCount, drID, dates[dateIndex], times, realIndex);

        if(timeCount == 0){
            printf("No time slot available...\n");
            continue; //go back select the date..
        }

        int timeIndex = getTimeInput(timeCount, times);

        strcpy(aptDate, dates[dateIndex]);
        strcpy(aptTime, times[timeIndex]);
        (*index) = realIndex[timeIndex];
        return 1;
    }
}

int bookApt(char* patientID){
    Doctor drs[MAX];
    Schedule schedules[MAX];
    Patient patients[MAX];
    int drCount, scheCount, patientCount;
    char specialist[TOTAL_SPECIALITY][24] = {""}; // Need to store the speciality to display and avoid redundancy.
    int speCount = 0;

    readDoctor(drs, &drCount);
    readSchedule(schedules, &scheCount);
    readPatient(patients, &patientCount);
    
    // Filtering the speciality (get all speciality without replicate)
    for(int i = 0; i < drCount; i++){
        int found = 0;
        for(int j = 0; j < speCount; j++){
            if(strcmp(drs[i].specialism, specialist[j])== 0){
                found = 1;
            }
        }

        if(!found){
            strcpy(specialist[speCount], drs[i].specialism);
            speCount++;
        }
    }

    printf("\nSpeciality:\n");
    for(int i = 0; i < speCount; i++){
        printf("%d. %s\n", i+1, specialist[i]);
    }

    char spec[25];
    int x;
    
    do{
        x = 0;

        printf("Enter preferred speciality: ");
        fgets(spec, sizeof(spec), stdin); 
        spec[strcspn(spec, "\n")] = '\0';

        for(int i = 0; i < speCount; i++){
            if(strcmp(spec, specialist[i]) == 0){ //return 0 difference
                x = 1;
                break;
            }
        }

        if(!exitCode(spec)){
            return 0;    
        }

        if(x == 0){
            printf("%s is not in the list. Please choose the speciality provided above ^^...\n", spec);

        }
        
    }while(x == 0);
    //Check on this one.. the number or tries for the user was based on the speciality Count!
    

    int dispNum = 1;

    printf("====================================================\n");
    printf("|%-3s|%-25s|%-20s|\n", "No.", "Doctor", "Speciality");
    printf("====================================================\n");
    for(int i = 0; i < drCount; i++){
        if (strcmp(drs[i].specialism, spec) == 0){
            printf("|%-3d|%-25s|%-20s|\n", dispNum, drs[i].name, drs[i].specialism);
            dispNum++;
        }
    }
    
    if(dispNum == 1){
        printf("No available Doctor...\n");
        return 0;
    }
    printf("====================================================\n");

    int selectedDr;
    int drIndex = -1;


    while(1){
        int dispNum = 1; //reset

        printf("Enter your choice (0 to exit): ");
        scanf("%d", &selectedDr);
        while (getchar() != '\n');
        
        if(selectedDr == 0){
            printf("Booking Canceled...\n");
            return 0;
        }

        for(int i = 0; i < drCount ; i++){
            if(strcmp(drs[i].specialism, spec) != 0){
                continue;
            }
            //use a new variable because i is not reliable
            if(dispNum == selectedDr){
                drIndex = i;
                break;
            }
            dispNum++;
        } 
        

        if(drIndex == -1){
            printf("Invalid choice..");
        } else{
            break;
        }
    }

    Doctor *drInfo = &drs[drIndex];

    
//Display doctor schedule
    
    char aptDate[11];
    char aptTime[6];
    int index;

    if(!dateAndTime(drInfo -> id, schedules, scheCount, aptDate, aptTime, &index)){// handle return 0
        printf("Booking cancelled..\nReturning...\nEnter to continue..\n");
        getchar();
        return 0;
    }

    getchar();
    char purpose[150];
    
    printf("Enter your purpose (N/A if do not want to enter the detail):\n");
    fgets(purpose, sizeof(purpose), stdin);
    purpose[strcspn(purpose, "\n")] = '\0'; //purpose[strlen(purpose)-1] = '\0'; 
    

    char confirm;
    printf("APPOINTMNET DETAILS:\n");
    printf("Patient: %s\n", patientID);
    printf("Doctor: %s\n", drInfo -> name);
    printf("Date: %s\n", aptDate);
    printf("Time: %s\n", aptTime);
    printf("Duration: 30 minutes\n");
    printf("Purpose: %s\n", purpose);

    while(1){
        printf("Confirm booking appointment? (y/n): ");
        scanf(" %c", &confirm);
        
        if(confirm == 'y'|| confirm == 'Y'){
            Appointment newApt;


            strcpy(newApt.patientID, patientID);
            strcpy(newApt.drID, drInfo->id);
            strcpy(newApt.date, aptDate);
            strcpy(newApt.time, aptTime);
            strcpy(newApt.purpose, purpose);
            strcpy(newApt.stat, "UP-COMING");

            strcpy(schedules[index].activity, "Consultation");

            if(!appendApt(newApt)){
                return 0;
            }

            if(!writeSchedule(schedules, scheCount)){
                return 0;
            }
            
            printf("Booking Successful. See you later ^^\n\n");
            return 1;
        } else {
            printf("Booking Cancelled...\nExiting....\n");
            return 0;
        }
    }
}

void searchByDate(Appointment appointments[], Doctor drs[], int aptCount, int drCount, char *patientID, char *searchDate){
    int found = 0;

    headerApt();

    for(int i = 0; i < aptCount; i++) {

        if(strcmp(appointments[i].patientID, patientID) != 0) {
            continue;
        }

        if(strcmp(appointments[i].date, searchDate) == 0) {
            char drName[MAX_NAME];
            getDrName(drs, drCount, appointments[i].drID, drName);

            printf("|%-3d|%-11s|%-6s|%-24s|%-12s|%-80s|\n", i+1 , appointments[i].date, appointments[i].time, drName, appointments[i].stat, appointments[i].purpose);

            found = 1;
        }
    }

    if(!found) {
        printf("No appointment found on %s\n", searchDate);
    }
    printf("===============================================================================================================================================\n");
}

void searchByStatus(Appointment appointments[], Doctor drs[], int aptCount, int drCount, char *patientID, char *status){
    int found = 0;

    headerApt();

    for(int i = 0; i < aptCount; i++) {

        if(strcmp(appointments[i].patientID, patientID) != 0) {
            continue;
        }

        if(strcmp(appointments[i].stat, status) == 0) {
            char drName[MAX_NAME];
            getDrName(drs, drCount, appointments[i].drID, drName);

            printf("|%-3d|%-11s|%-6s|%-24s|%-12s|%-80s|\n", i+1 , appointments[i].date, appointments[i].time, drName, appointments[i].stat, appointments[i].purpose);

            found = 1;
        }
        
    }

    if(!found) {
        printf("No %s appointment\n", status);
    }

     printf("===============================================================================================================================================\n");
}

int validateStatus(const char *status) {
    if (strcmp(status, "UP-COMING") == 0) {
        return 1;
    }

    if (strcmp(status, "COMPLETED") == 0) {
        return 1;
    }

    if (strcmp(status, "CANCELLED") == 0) {
        return 1;
    }

    return 0;
}

void viewSearchApt(char* patientID){
    Appointment appointments[MAX];
    Doctor drs[MAX];

    int aptCount, drCount;

    readAppointment(appointments, &aptCount);
    readDoctor(drs, &drCount);

    if(!dispUpComingApt(appointments, aptCount, patientID, drs, drCount, "View")){
        printf("There is no appointment record..\n");
        printf("========================================================================================================================================================\n");
        return;
    }

    int confirm;
    printf("Would you like to search for any appointment? (1 to continue) ");
    scanf("%d", &confirm);
    getchar();

    if(confirm == 1){
        do{
            int select;
            printf("===Select base on what?===\n");
            printf("1. Status\n2. Date\n3. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &select);
            getchar();

            switch(select){
                case 1:
                    char status[15];
                    
                    while (1) {
                        printf("Enter status (UP-COMING, CANCELLED, COMPLETED): ");
                        fgets(status, sizeof(status), stdin);

                        status[strcspn(status, "\n")] = '\0';
                        
                        strupr(status);

                        if (validateStatus(status)) {
                            searchByStatus(appointments, drs, aptCount, drCount, patientID, status);
                            break;
                        }

                        printf("Invalid status!\n");
                    }

                    
                    break;
                case 2:
                    char searchDate[11];

                    while(1){
                        printf("Enter date to search (DD/MM/YYYY): ");
                        fgets(searchDate, sizeof(searchDate), stdin);
                        searchDate[strcspn(searchDate, "\n")] = '\0';

                        if(validateDate(searchDate)) {
                            searchByDate(appointments, drs, aptCount,drCount, patientID, searchDate);
                            break;
                        }
                        else {
                            printf("Invalid date format.\n");
                        }                    
                    }
                    break;
                case 3:
                    printf("Exiting....");
                    return;
                default:
                    printf("Invalid Choice!");
                    break;
            }
        }while(confirm != 3);
    } else {
        return;
    }
}

int changeApt(char* patientID, Appointment* appointments, int aptCount, const char* label){
    int selectedApt;
    int aptIndex = -1;

    while(1){
        printf("Select an appointment for %s (0 to exit): ", label);
        scanf("%d", &selectedApt);

        if(selectedApt == 0){
            printf("%s appointment cancelled..\n", label);
            return -1;
        }
    
        aptIndex = id_inputMatcher(patientID, appointments, aptCount, selectedApt);
    
        if(aptIndex == -1){
            printf("Invalid input!\n");
        } else{
            return aptIndex; 
        }
    }
}

int doctorSchedule(Schedule schedules[], int count, char *date, char* time){
    for(int i = 0; i < count; i++){
        if(strcmp(schedules[i].date, date) != 0){
            continue;
        }

        if (strcmp(schedules[i].time, time) == 0){
            return i;
        }
    }
    return -1;
}

int rescheduleApt(char* patientID){
    Doctor drs[MAX];
    Schedule schedules[MAX];
    Appointment appointments[MAX];

    int drCount = 0, scheCount = 0, aptCount= 0;

    readSchedule(schedules, &scheCount);
    readDoctor(drs, &drCount);
    readAppointment(appointments, &aptCount);

    if(!dispUpComingApt(appointments, aptCount, patientID, drs, drCount, "Reschedule")){
        printf("No appointment record that can be rescheduled\n");
        printf("===============================================================================================================================================\n");
        return 0;
    }

    int aptIndex = changeApt(patientID, appointments, aptCount, "reschedule");

    if(aptIndex == -1){
        return 0;
    }

    char oldDate[11];
    char oldTime[6];
    strcpy(oldDate, appointments[aptIndex].date);
    strcpy(oldTime, appointments[aptIndex].time);

    char aptDate[11];
    char aptTime[6];
    int index;

    dateAndTime(appointments[aptIndex].drID, schedules, scheCount, aptDate, aptTime, &index);

    int oldScheIndex = doctorSchedule(schedules, scheCount, oldDate, oldTime); // old schedule
    int scheIndex = doctorSchedule(schedules, scheCount, aptDate, aptTime); //new schedule

    if(scheIndex == -1){
        printf("Error, Doctor schedule cannot be found!\nCannot make changes..\n");
        return 0;
    }

    Appointment *details = &appointments[aptIndex];

    char drName[MAX_NAME];
    getDrName(drs, drCount, details -> drID, drName);

    printf("RESCHEDULE DETAILS:\n");
    printf("Doctor: %s\n", drName);
    printf("Date: %s\n", details -> date);
    printf("Time: %s\n", details -> time);
    printf("Purpose: %s\n", details -> purpose);

    char confirm;

    while(1){
        printf("Confirm reschedule? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y'){
            strcpy(details->date, aptDate);
            strcpy(details->time, aptTime);
            
            strcpy(details->patientID, patientID);
            strcpy(schedules[scheIndex].activity, "Consultation");

            if(!writeAppointment(appointments, aptCount)){
                return 0;
            }

            if(!writeSchedule(schedules, scheCount)){
                return 0;
            }

            printf("Reschedule successful!");
            return 1;
        }else {
            printf("Reschedule cancelled...\nExiting.....\n");
            return 0;
        }
    }
}

int cancelApt(char* patientID){
    Doctor drs[MAX];
    Schedule schedules[MAX];
    Appointment appointments[MAX];
    int drCount, scheCount, aptCount;

    readSchedule(schedules, &scheCount);
    readDoctor(drs, &drCount);
    readAppointment(appointments, &aptCount);

    if(!dispUpComingApt(appointments, aptCount, patientID, drs, drCount, "cancel")){
        printf("No appointment record that can be rescheduled\n");
        printf("===============================================================================================================================================\n");
        printf("\nEnter to continue...");
        getchar(); //clean
        getchar(); //user input
        return 0;
    }

    int aptIndex = changeApt(patientID, appointments, aptCount, "cancellation");

    if(aptIndex == -1){
        return 0;
    }

    Appointment *details = &appointments[aptIndex];

    int oldScheIndex = doctorSchedule(schedules, scheCount, details->date, details->time); // old schedule


    char drName[MAX_NAME];
    getDrName(drs, drCount, details -> drID, drName);

    printf("CANCELLATION DETAILS:\n");
    printf("Doctor: %s\n", drName);
    printf("Date: %s\n", details -> date);
    printf("Time: %s\n", details -> time);
    printf("Purpose: %s\n", details -> purpose);

    char confirm;

    while(1){
        printf("Confirm cancellation?? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'Y' || confirm == 'y'){
            strcpy(details->stat, "CANCELLED");

            strcpy(schedules[oldScheIndex].activity, "VACANT");

            if(!writeAppointment(appointments, aptCount)){
                return 0;
            }

            if(!writeSchedule(schedules, scheCount)){
                return 0;
            }
            
            printf("Cancellation successful!");
            return 1;
        }else {
            printf("Appointment cancellation cancelled...\nExiting.....\n");
            return 0;
        }
    }
}
//End  Appointment Functionality

//======================================================================================================================================================================================================================================


//PAYMENT FUNCTION
void getMedicine(char* typeID, Inventory* invs, int count, char* type, float* price) {
    strcpy(type, " "); //Default
        
    for (int j = 0; j < count; j++) {
        if (strcmp(typeID, invs[j].id) == 0) { // find the exact one
        strcpy(type, invs[j].name);  // Overwrite default
        (*price) = invs[j].price;

        return;
        }
    }

}

int billMatch(Bill *bills, int billCount, char *patientID, int userChoice){
    int num = 1;
    for(int i = 0; i < billCount; i++){
        if (strcmp(patientID, bills[i].patient_id) == 1){
            continue;
        }

        if(strcmp(bills[i].payment_method, "Unpaid") == 1){
            continue;
        }

        if(num == userChoice){
            return i;
        }

        num++;
    }

    return -1;
}

void getCats(char* typeID, Category* cats, int count, char* type, float* price){
    strcpy(type, "");

    for (int j = 0; j < count; j++) {
        if (strcmp(typeID, cats[j].id) == 0) { // find the exact one
            strcpy(type, cats[j].detail);  // Overwrite default
            *price = cats[j].price;

            return;
        }
    }
}

int unpaidBill(char *patientID, Bill* bills, int count){

    printf("==============================OUTSTANDING BILL(S)=================================\n");
    printf("\n================================================================================\n");
    printf("|%-3s|%-11s|%-10s|%-12s|\n", "No.", "Date", "Total Item", "Total Amount");
    printf("================================================================================\n");

    int found = 0;

    for(int i = 0; i < count; i++){
        if(strcmp(patientID, bills[i].patient_id) != 0){
            continue;
        }
        
        //payMethod needed to avoid crash, if suddenly it compares with NULL (0) value... 
        if(strcmp(bills[i].payment_method, "Unpaid") == 0){
            printf("|%-2d|%-11s|%-10d|RM%10.2f|\n", i+1, bills[i].bill_date, count, bills[i].bill_total);
            found = 1;
        }
    }

    if(!found){
        printf("You don't have any outstanding invoice ^^..\n");
        printf("================================================================================\n");
        return 0;
    }

    printf("================================================================================\n");
    return 1;
}

void billDetails(char *patientName, char *billID, Bill* bill, int billCount, Category* cats, int catCount, Inventory* inventories, int inventCount){
    int found = 0;

    printf("------------------------------------------------------------------------\n");
    printf("|Bill ID: %-25s|Date Issued:  %10s           |\n", billID, bill->bill_date);
    printf("|Patient: %-25s|Payment Date: %10s           |\n", patientName, bill->payment_date);
    printf("------------------------------------------------------------------------\n");
    printf("|%-23s|%-10s|%-12s|%9s|%12s|\n", "Item Details", "Type", "Unit Price", "Qty", "Total Price");
    printf("========================================================================\n");

    for(int i= 0; i < bill->count; i++){
        if(bill->items[i].bill_category[0] == 'M')
        {
            char meds[MAX_SPEC];
            float price;
            getMedicine(bill->items[i].bill_category, inventories, inventCount, meds, &price);

            printf("|%-23s| Medicine |RM%10.2f|%9d|RM%10.2f|\n", meds, price, bill->items[i].bill_sub, bill->items[i].bill_sub_price);            
            printf("------------------------------------------------------------------------\n");
            found =1;
        }
        else if (bill->items[i].bill_category[0] == 'S')
        {
            char surgery[MAX_SPEC];
            float price;
            getCats(bill->items[i].bill_category, cats, catCount, surgery, &price);

            printf("|%-23s|%-10s|RM%10.2f|%9d|RM%10.2f|\n", surgery, "Surgery", price, bill->items[i].bill_sub, bill->items[i].bill_sub_price); 
            printf("------------------------------------------------------------------------\n");
            found =1;
        }
        else if(bill->items[i].bill_category[0] == 'R')
        {
            char room[MAX_SPEC];
            float price;
            getCats(bill->items[i].bill_category, cats, catCount, room, &price);

            printf("|%-23s|%-10s|RM%10.2f|%3d day(s)|RM%10.2f|\n", room, "Room", price, bill->items[i].bill_sub, bill->items[i].bill_sub_price); 
            printf("------------------------------------------------------------------------\n");
            found =1;
        }
        else{
            char consultation[MAX_SPEC];
            float price;
            getCats(bill->items[i].bill_category, cats, catCount, consultation, &price);

            printf("|%-23s|%-10s|RM%10.2f|%d time(s)|RM%10.2f|\n", consultation, "Consultation",price, bill->items[i].bill_sub, bill->items[i].bill_sub_price); 
            printf("------------------------------------------------------------------------\n");

            found =1;
        }
    }

    printf(" GRAND TOTAL: RM%-10.2f | STATUS: %s\n",
           bill->bill_total ? bill->bill_total : 0.00,
           bill->payment_method ? bill->payment_method : "Unpaid");
    printf("========================================================================\n");
    
    if(!found){
        printf("You don't have any payment history ^^..\n");
    }

    return;
}

void addPay(char* patientID){
    Bill bills[MAX];
    Category cats[LOAD];
    Inventory invent[LOAD];
    int billCount, catCount, inventCount;

    readInvoice(bills, &billCount);
    readCategory(cats, &catCount);
    readInventory(invent, &inventCount);  
    
    //Need to call the read function
    int dispNum = 1;
    int found = 0;

    //display unpaid bills
    if(!unpaidBill(patientID, bills, billCount)){
        printf("Enter to exit");
        getchar();
        return;
    }

    dispNum = 1; //reset

    while(1){
        int selectedBill;

        printf("Enter the bill number you want to pay (0 to cancel): ");
        scanf("%d", &selectedBill);
        
        if (selectedBill == 0) {
            printf("Payment cancelled.\n");
            return;
        }

        // Find selected bill
        int billIndex = billMatch(bills, billCount, patientID, selectedBill);
        if(billIndex == -1){
            printf("Invalid bill number\n");
            continue;
        }

        Bill *selectedBillPtr = &bills[billIndex];

        billDetails(patientID, selectedBillPtr->bill_id, &bills[billIndex], billCount, cats, catCount, invent, inventCount);

        char confirm;
        printf("\nConfirm payment? (y/n): ");
        scanf(" %c", &confirm);
        getchar();
        
        char date[11];
        char time[6];
        char payMeth[20];
        

        if (confirm == 'y' || confirm == 'Y') {
            do{
                printf("Payment Method (Card or Cash): ");
                fgets(payMeth, sizeof(payMeth),stdin);
                payMeth[strcspn(payMeth, "\n")] = '\0';

                strupr(payMeth);

                if(strcmp(payMeth, "CASH") != 0 || strcmp(payMeth, "CASH") != 0){
                    printf("Invalid payment method, Please try again!\n");
                }
            } while(strcmp(payMeth, "CASH") != 0 || strcmp(payMeth, "CASH") != 0);
            
            getTime(date, time);
            strcpy(selectedBillPtr->payment_date, date);
            strcpy(selectedBillPtr->payment_time, time);
            strcpy(selectedBillPtr->payment_method, payMeth);
            if (writeInvoice(bills, billCount)) {
                printf("Payment successful! Invoice %s marked as PAID.\n", 
                    selectedBillPtr->bill_id);
                return;
            } else {
                printf("Payment failed! Error updating records.\n");
                return;
            }
        } else {
            printf("Payment cancelled.\n");
            return;
        }

    }

}

void printBillRow(Bill* bill, int rowNum) {
    char payDate[12];
    char payMethod[16];
 
    if (strcmp(bill->payment_method, "Unpaid") == 0) {
        strcpy(payDate,   "N/A");
        strcpy(payMethod, "N/A");
    } else {
        strcpy(payDate,   bill->payment_date);
        strcpy(payMethod, bill->payment_method);
    }
 
    printf("|%-3d|%-11s|%-10d|RM %-10.2f|%-11s|%-14s|\n",
           rowNum, bill->bill_date, bill->count, bill->bill_total,
           payDate, payMethod);
}

void billHeader(){
    printf("\n=================================================================\n");
    printf("|%-3s|%-11s|%-10s|%-12s|%-11s|%-12s|\n",
            "No.", "Issued Date", "Total Item", "Total Amount", "Payment Date","Payment Method");
    printf("=================================================================\n");
}

void historyBills(Bill bills[], int billCount, char* patientID){
    int dispNum = 1;
    int found = 0;

    billHeader();

    for(int i = 0; i < billCount; i++)
    {
        if(strcmp(bills[i].patient_id, patientID)!= 0 ){
            continue;
        }

        printBillRow(&bills[i], dispNum);
        dispNum++;

    }
}


void amountFilter(Bill bills[], int billCount, char* patientID, Category* cats, int catCount, float fltrAmt){
    int dispNum = 1;
    int found = 0;

    billHeader();

    for(int i = 0; i < billCount; i++){
        if (strcmp(bills[i].patient_id, patientID) != 0) {
            continue;
        }

        if(fltrAmt < bills[i].bill_total){
            printBillRow(&bills[i], dispNum);
            found = 1;
        }
    }

    if(!found){
        printf("There is no record for that amount...\n");
    }
    
}

void statusFilter(Bill bills[], int billCount, char* patientID, Category cats[], int catCount, char *stat){
    int dispNum = 1;
    int found = 0;

    billHeader();

    for(int i = 0; i < billCount; i++){
        if (strcmp(bills[i].patient_id, patientID) != 0) {
            continue;
        }

        if(strcmp(stat, "Unpaid") == 0){
            printBillRow(&bills[i], dispNum);
            found = 1;            
        }

        if(strcmp(stat, "Unpaid") != 0){
            printBillRow(&bills[i], dispNum);
            found = 1;
        }
    }

    if(!found){
        printf("There is no record for %s record...\n", stat);
    }
}


void filterPayment(Bill bills[], int billCount, char* patientID, Category cats[], int catCount){
    int uc;

    do{
        printf("===FILTER OPTIONS===\n");
        printf("1. Amount\n2. Status\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &uc);
        getchar();

        switch(uc){
            case 1:
                int fltramt;

                printf("\nEnter amount: ");
                scanf("%d", &fltramt);

                amountFilter(bills, billCount, patientID, cats, catCount, fltramt);

                int confirm;

                printf("Do you wanna continue(1 for yes, 0 for no)? ");
                scanf("%d", &confirm);

                if(confirm == 1){
                    continue;
                }
                break;  
            case 2:
                char status[10];

                while(1){
                    printf("\nEnter status you want (0 for Unpaid, 1 for Paid): ");
                    fgets(status, sizeof(status), stdin);
                    status[strcspn(status, "\n")] = '\0';
                    
                    statusFilter(bills, billCount, patientID, cats, catCount, status);
                    break;
                }
                break;
            case 3:
                break;
            default:
                printf("Invalid Option!");

        }
    } while(uc != 3);
}

void viewSearchPay(char *patientID){
    Bill bills[MAX];
    Category cats[MAX];
    Inventory invs[MAX];
    int billCount, catCount, inventCount;

    readInvoice(bills, &billCount);
    readCategory(cats, &catCount);
    readInventory(invs, &inventCount);

    //Display all patient bills
    historyBills(bills, billCount, patientID);

    int uc;

    printf("\nWould you like to filter your payment history (1 to continue, 2 for exit)? "); //actually to exit, the program don't really care for the wrong number, cuz filtering option is way more important
    scanf("%d", &uc);

    if(uc == 1){
        filterPayment(bills, billCount, patientID, cats, catCount);
    } else {
        return;
    }
}

//COMPLAINT FUNCTION
void addCmplnt(char *patientID){
    Complaint complaint;
    char userComplaint[MAX_INPUT];
    char cDate[11];
    char cTime[6];

    do{
        printf("Enter any complaint that you want to issue (Max 150 characters):\n");
        fgets(userComplaint, sizeof(userComplaint), stdin);

        userComplaint[strcspn(userComplaint, "\n")] = 0; //removes extra \n

        if(strlen(userComplaint) == 0){
            printf("Complaint cannot be empty!");
        }
    } while(strlen(userComplaint) == 0);
    
    getTime(cDate, cTime);

    strcpy(complaint.patientID, patientID);
    strcpy(complaint.date, cDate);
    strcpy(complaint.time, cTime);
    strncpy(complaint.details, userComplaint, MAX_INPUT-1);
    complaint.details[MAX_INPUT - 1] = '\0';

    if(appendComplaint(complaint)){
        printf("Complaint added succesfully..\n");
        return;
    } else{
        printf("Complaint cannot be added!\n");
    }
}

void complaintDisp(Patient patient[], Complaint complaints, int patientCount){
    for(int j =0; j < patientCount; j++){
        if(strcmp(patient[j].id, complaints.patientID) != 0){
            continue;
        }
                    
        printf("%.4s*****@gmail.com\t\t(%s %s)\n%s\n",
            patient[j].email, complaints.date, complaints.time, complaints.details);
    }
}

void viewSearchCmplnt(char *patientID){
    Complaint complaints[MAX];
    Patient patient[MAX];
    int patientCount;
    int compCount;
    int uc;
    char keyWord[MAX_INPUT];

    readComplaint(complaints, &compCount);
    readPatient(patient, &patientCount);

    printf("1. View All Complaints\n2. Search Complaint\n3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &uc);
    getchar();

    switch(uc){
        case 1:
            for(int i = 0; i < compCount; i++){
                complaintDisp(patient, complaints[i], patientCount);
            }   
            break;
        case 2:
            
            printf("Enter keyword: "); 
            fgets(keyWord, sizeof(keyWord), stdin);
            keyWord[strcspn(keyWord, "\n")] = '\0';
            
            for(int i = 0; i < compCount; i++){
                if(strstr(complaints[i].details, keyWord) != NULL){
                    complaintDisp(patient, complaints[i], patientCount);
                }
            }
            
        break;
    }
}
// End Complaint FUntion

//Other menu..
void apt(char* patientID){
    int uc;
    do{
        printf("====APPOINTMENT MENU====\n");
        printf("1. Book an Appointment\n");
        printf("2. Appointment History\n");
        printf("3. Reschedule Appointment\n");
        printf("4. Cancel Appointment\n");
        printf("5. Exit\n");
    
        printf("Enter your choice: ");
        scanf("%d", &uc);
        getchar();
    
        switch (uc){
        case 1:
            if(!bookApt(patientID)){
                printf("Booking failed, please try again later..\n");
                getchar();
            }
            break;
        case 2:
            viewSearchApt(patientID);
            break;
        case 3:
            if(!rescheduleApt(patientID)){
                printf("Booking failed, please try again later..\n");
                getchar();
            }
            break;
        case 4:
            if(!cancelApt(patientID)){
                printf("Booking failed, please try again later..\n");
                getchar();
            }
            break;
        case 5:
            break;
        default:
            printf("Invalid choice! Please try again :).\n");
        }
    } while(uc != 5);

    printf("Exiting Appointment Menu...\n");
  
}

void payment(char *patientID){
    int uc;

    do{
        printf("\n====PAYMENT MENU=====\n");
        printf("1. Make a payment\n2. View and Search Invoice Payment\n3. Exit\n");
        
        printf("Enter your choice: ");
        scanf("%d", &uc);
        getchar();

        switch(uc){
            case 1:
                addPay(patientID);
                break;
            case 2:
                viewSearchPay(patientID);
                break;
            default:
                printf("Invalid input!\n");
        } 
    }while (uc != 3);
    
    printf("Exiting Payment Menu...\n");
}

void complaininggg(char *patientID){
    int uc;

    do{
        printf("\n====COMPLAINT MENU=====\n");
        printf("1. Make a Complaint\n2. View and Search Complaint\n3. Exit\n");
        
        printf("Enter your choice: ");
        scanf("%d", &uc);
        getchar();

        switch(uc){
            case 1:
                addCmplnt(patientID);
                break;
            case 2:
                viewSearchCmplnt(patientID);
                break;
            default:
               printf("Invalid input!\n");

        } 
    }while (uc != 3);

    printf("Exiting Complaint Menu...\n");
}


//MAIN FUNCTION
void patient_menu() { //change it to void patient later
    char patientID[MAX_ID] = "";
    strcpy(patientID, current_logged_in_id);
    
     // get patientID 

    int choice;

    printf("\n=====================================\n");
    printf("         PATIENT MENU SYSTEM         \n");
    printf("=====================================\n");
    

    do {
        printf("==========PATIENT MAIN MENU==========\n");
        printf("1. Appointment Scheduling\n");
        printf("2. Payment\n");
        printf("3. Make a Complaint\n");
        printf("4. Exit\n");
        
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
        case 1:
            apt(patientID);
            break;
        case 2:
            payment(patientID);
            break;
        case 3:
            complaininggg(patientID);
            break;
        default:
            printf("Invalid choice!\n");
        }
        
    } while(choice != 4);

    printf("Thank You For Visiting Our Site ^^!\n");

    return;
}