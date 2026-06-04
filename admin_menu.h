#ifndef ADMIN_MENU
#define ADMIN_MENU

//Po Chang code
//menu
void admin_menu();

//admin_account_manage.c
void displayaccount(const char * doctorfile, const char * nursefile , const char* patientfile);
void namefixed(const char *input, char *output);
int isAlphaOnly(const char *str);
void searchaccount(const char *doctorFile, const char *nurseFile, const char *patientFile);
struct doctor
   {
        char dr_id[10];
        char dr_name[30];
        char dr_gender;
        char dr_contact[15];        // <-- string to store, cuz first number 0, int cannot store
        char dr_emergency[15]; 
        char dr_duty[30];
        char dr_specialism[30];
        char dr_employment[20];
        float dr_salary;
    };

void createDoctorRecord(struct doctor *f);
struct nurse
   {
        char nur_ID[10];
        char nur_name[30];
        char nur_gender;
        char nur_contact[15];  
        char nur_duty[30];
        char nur_position[30];
        char nur_department[30];
        char nur_employment[20];
        float nur_salary;
    };

void createNurseRecord(struct nurse *n);
struct patient
   {
        char Patient_id[11];
        char Patient_name[30];
        char Patient_ic[14];
        int Patient_age;
        char Patient_contact[15];  
        char Patient_email[30];
        char Patient_emergency[20];
    };

int isICunique(const char *ic);
void createPatientRecord(struct patient *p);
void modifyDoctor();
void modifyNurse();
void modifyPatient();
void modifyAccountMenu();
void account_management_sys();

//admin_bill
typedef struct{
    char bill_category[20];
    int bill_sub;
    float bill_sub_price; 
}BillItems;


struct bill{
    char bill_id[8];
    char patient_id[8];
    char admin_id[8];

    BillItems * items;
    int count;

    float bill_total;
    char bill_date[10];
    char payment_date[10];
    char payment_time[5];
    char payment_method[15];
};

void trim(char *str);
char *get_name(const char *filepath, const char *searchID);
void displayinvoice(const char *invoicefile);
void display_bill_sections(char catIDs[][20], int catQtys[], float catPrices[], 
                           int count, char *billID, char *patientID, 
                           char *adminID, char *billDate, 
                           char *billTotal, char *payMethod);
void searchBillByPatientIDAndDate(const char *filepath, const char *targetPID, const char *targetDate);
float getcategoryprice(const char *category);
float getmedprice(const char *medicine_id);
int idcheck(const char *filepath, const char *id, int columnIndex);
int validatePrefixedID(const char *id, char prefix);
void save_bill(struct bill *f);
float get_price(const char *filepath, const char *searchID);
void add_item(struct bill *b, const char *id, int qty, float unit_price);
void calculate_billing(struct bill *b);
void CreateBillRecord(struct bill *b);
void bill_management_sys();

//admin_complaint
void displaycomplain(const char * complainfile);
void searchcompalin(const char *complainFile);
void patient_complaint_sys();

//systemhandle
void buffer();
int is_string_numeric(const char *str);
int isDuplicate(const char *id);
void generateID(char *id, char prefix, int digits);


#endif