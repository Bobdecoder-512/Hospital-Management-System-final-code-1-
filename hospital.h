#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>


//Jovan code
#define MAX 1000
#define MAX_ID 10
#define MAX_NAME 100
#define MAX_SPEC 25
#define MAX_INPUT 151
#define MAX_CONTACT 13

#define DOCTORFILE "csv/Dr.csv"
#define NURSEFILE "csv/Nurse.csv"
#define PATIENTFILE "csv/Patient.csv"
#define APPOINTMENTFILE "csv/Appointment.csv"
#define SCHEDULEFILE "csv/Schedule.csv"
#define INVENTORYFILE "csv/Inventory.csv"
#define INVOICEFILE "csv/Bill_Payment.csv"
#define CATEGORYFILE "csv/category.csv"
#define COMPLAINTFILE "csv/Complaint.csv"


#define TOTAL_MEDS 10
#define TOTAL_CAT 7
#define LOAD 15

typedef struct{
    char id[MAX_ID];
    char name[MAX_NAME];
    char gender;
    char emergencyNum[MAX_CONTACT];
    char contact[MAX_CONTACT];
    char duty[4];
    char specialism[MAX_SPEC];
    char employment[MAX_SPEC];
    float salary;
}Doctor;

typedef struct 
{
    char id[MAX_ID];
    char name[MAX_NAME];
    int contact;
    char duty[4];
    char position[MAX_SPEC];
    char department[MAX_SPEC];
    char employment[MAX_SPEC];
    float salary;
} Nurse;

typedef struct 
{
    char id[MAX_ID];
    char name[MAX_NAME];
    char ic[15];
    int age;
    char contact[MAX_CONTACT];
    char email[MAX_NAME];
    char emergency[MAX_CONTACT];
} Patient;

typedef struct{
    char aptID[MAX_ID];
    char patientID[MAX_ID];
    char drID[MAX_ID];
    char date[11];
    char time[6];
    char stat[MAX_SPEC];
    char purpose[MAX_INPUT];
} Appointment;

typedef struct 
{
    char p_id[MAX_ID];
    char type[MAX_SPEC];
    char reason[MAX];
    char record[MAX_SPEC];
    char date[MAX_SPEC];
    char stay[MAX_SPEC]; //this is a number but i change string so that it can accept dash '-')
    char d_id[MAX_ID];
    char diagnosis[MAX];
} Status;

typedef struct 
{
    char drID[MAX_ID];
    char patientID[MAX_ID];
    char date[12];
    char time[7];
    char activity[100];
} Schedule;

typedef struct{
    char bill_category[20]; //id
    int bill_sub; //qty
    float bill_sub_price; //price (not unit price)
} BillItem;

typedef struct{
    char bill_id[8];
    char patient_id[8];
    char admin_id[8];

    BillItem items[20];
    int count;

    float bill_total;
    char bill_date[11];
    char payment_date[11];
    char payment_time[6];
    char payment_method[15];
} Bill;

typedef struct{
    char id[MAX_ID];
    char detail[MAX_SPEC];
    float price;
}Category;

typedef struct{
    char patientID[MAX_ID];
    char details[MAX_INPUT];
    char date[11];
    char time[6];
} Complaint;

typedef struct
{
    char id[MAX_ID];
    char name[MAX_SPEC];
    float price;
    char vendor[MAX_SPEC];
    int stock;
    char nurseID[MAX_ID];
}Inventory;


//utils
void getTime(char *date, char *strTime);


void readDoctor(Doctor* drs, int *count);
void readNurse(Nurse* nurses, int *count);
void readPatient(Patient* patients, int *count);
void readAppointment(Appointment* appointments, int* count);
void readComplaint(Complaint* complaints, int* count);
void readSchedule(Schedule* schedules, int* count);
void readInvoice(Bill *bills, int *count);
void readInventory(Inventory* inv, int *count);
void readCategory(Category* cats, int* count);

//Append
int appendApt(Appointment apts);
int appendComplaint(Complaint c);

//Write
int writeSchedule(Schedule schedules[], int count);
int writeAppointment(Appointment apts[], int count);
int writeInvoice(Bill bills[], int count);

//patient menu
void patient_menu();
//=================================================================================================================


//Verdy code
typedef struct
{
    char m_id[MAX_ID];
    char name[MAX];
    float price;
    char vendor[MAX];
    int stock;
    char n_id[MAX_ID];
} Medicine;

typedef struct
{
    char p_id[MAX_ID];
    char date[MAX_SPEC];
    char time[MAX_SPEC];
    int blood;
    float temp;
    char sym[MAX];
    char cm[MAX];
    char cond[MAX];
} Note;


//doctor functions
void doctor();
void schedule();
void diagnostic();
void add_sch();
void view_sch();
void search_sch();
void add_dia();
void view_dia();
void search_dia();

void nurse();
void inventory();
void notes();
void nurse_view_sch();
void add_inv();
void view_inv();
void search_inv();
void add_note();
void view_note();
void search_note();


//=================================================================================================================
//=================================================================================================================





//chuan feng code
typedef struct
{
    char admin_id[5];
    char admin_name[100];
    char admin_contact[20];
    char admin_duty[10];
    char admin_employment[20];
    char admin_salary[100];
}admin;

typedef struct
{
    char line[1000];
    char temporary_line[1000];
}fileline;

typedef struct
{
    char searchID[5];
    char searchname[100];

}search;

//login function
int verify_user_from_csv(const char *filename, const char *input_id, const char *input_pass, char *out_name);
int login_page();

//director's function
int check_isdigit(char *checkdigit);
int contactandIC_num_validation(int num_type, char *num, char *prompt, int min_num, int max_num);
int check_float(char *check_f);
int check_name(char *check_n);
void create_admin_account(admin *ptr_new_admin);
void select_staff_type();
void search_staff_account(int open_file_type);
int select_staff_employment();
void financial_sales_generator();

//director menu
void director();

//main menu
int main();


#endif