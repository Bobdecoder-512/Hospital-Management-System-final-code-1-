#include <stdio.h>
#include "hospital.h"

int writeSchedule(Schedule schedules[], int count) {
    FILE *file = fopen(SCHEDULEFILE, "w");
    if (!file) {
        printf("File doesn't exist\n");
        return 0;
    }
    
    // Write header
    fprintf(file, "schedule_id, dr_id, date, time, activity\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s\n",
                schedules[i].drID, schedules[i].patientID,
                schedules[i].date, schedules[i].time, schedules[i].activity);
    }
    
    fclose(file);
    
    return 1;
}

int writeAppointment(Appointment apts[], int count){
    FILE *file = fopen(APPOINTMENTFILE, "w");
    if (!file) {
        printf("File doesn't exist\n");
        return 0;
    }
    
    // Write header
    fprintf(file, "apt_id, patient_id, dr_id, date, time, stat, purpose\n");
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s\n",
            apts[i].patientID, apts[i].drID, 
            apts[i].date, apts[i].time, 
            apts[i].stat, apts[i].purpose);
    }
    
    fclose(file);

    return 1;
}

int writeInvoice(Bill bills[], int count){
        FILE *file = fopen(INVOICEFILE, "w");
    if (!file) {
        printf("File doesn't exist\n");
        return 0;
    }
    
    // Write header
    fprintf(file, "bill_id,patient_id,admin_id,item_count,bill_category,bill_sub,bill_sub_price,bill_total,bill_date,payment_date,payment_time,payment_method\n");

    for(int i = 0; i < count; i++){

        fprintf(file, "%s,%s,%s,%d", 
            bills[i].bill_id, bills[i].patient_id, 
            bills[i].admin_id, bills[i].count);

        for(int j = 0; j < bills[i].count; j++){

            fprintf(file, ",%s,%d,%.2f", 
                bills[i].items[j].bill_category, bills[i].items[j].bill_sub, bills[i].items[j].bill_sub_price);
        }

        fprintf(file, ",%.2f,%s,%s,%s,%s\n", 
            bills[i].bill_total, bills[i].bill_date,
            bills[i].payment_date, bills[i].payment_time, bills[i].payment_method);
    }

    fclose(file);

    return 1;
}