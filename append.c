#include <stdio.h>
#include "hospital.h"

//utils

//

int appendApt(Appointment apts){
    FILE *file = fopen(APPOINTMENTFILE, "a");
    if(!file){
        printf("File doesn't exist");
        return 0;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%s\n",
        apts.patientID, apts.drID,
        apts.date, apts.time,
        apts.stat, apts.purpose);

    fclose(file);

    return 1;
}

int appendComplaint(Complaint c){
    FILE *file =  fopen(COMPLAINTFILE, "a");
    if(!file){
        printf("File doesn't exist");
        return 0;
    }

    fprintf(file, "%s,%s,%s,%s\n",
        c.patientID,c.details, c.date, c.time);

     
    fclose(file);
    return 1;
}
