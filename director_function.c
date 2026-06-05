#include <stdio.h>
#include <stdlib.h> //for using srand() to seed random number generator
#include <time.h>
#include <string.h>
#include <ctype.h>
#include "hospital.h"
#include "admin_menu.h"
extern char current_logged_in_id[20];

int verify_user_from_csv(const char *filename, const char *input_id, const char *input_pass, char *out_name) {
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0; 
    }

    char line[1024];
    int success = 0;

    fgets(line, sizeof(line), file); // clear header
    while (fgets(line, sizeof(line), file)) {
        // clear '\n' and '\r'
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;

        // make a copy to prevent disrupt original file
        char temp_line[1024];
        strcpy(temp_line, line);

        // take ID
        char *csv_id = strtok(temp_line, ",");
        // take First Name
        char *csv_name = strtok(NULL, ",");
        char *csv_firstname = strtok(csv_name, " ");

        if (csv_id != NULL && csv_name != NULL) {
            // 💡 combien ID and First name become password
            char expected_password[100];
            sprintf(expected_password, "%s%s", csv_id, csv_name);

            // Compare user input (varification)
            if (strcmp(csv_id, input_id) == 0 && strcmp(expected_password, input_pass) == 0) {
                success = 1;
                if (out_name != NULL) {
                    strcpy(out_name, csv_name); //return name for print
                }
                break;
            }
        }
    }

    fclose(file);
    return success;
}



int login_page() {
    char input_id[50];
    char input_pass[100];
    char user_name[50] = "";
    int login_success = 0;
    int role = 0; // 1: Doctor, 2: Nurse, 3: Admin, 4: Patient

    printf("\n==================================\n");
    printf("    HOSPITAL SYSTEM LOGIN PAGE    \n");
    printf("==================================\n");

    printf("Enter User ID: ");
    fgets(input_id, sizeof(input_id), stdin);
    input_id[strcspn(input_id, "\n")] = 0; // clear '\n'
    input_id[strcspn(input_id, "\r")] = 0;

    printf("Enter Password: ");
    fgets(input_pass, sizeof(input_pass), stdin);
    input_pass[strcspn(input_pass, "\n")] = 0; // clear '\n'
    input_pass[strcspn(input_pass, "\r")] = 0;

    // check csv file
    // 1. check is doctor
    if (verify_user_from_csv("csv/Dr.csv", input_id, input_pass, user_name))
    {
        login_success = 1;
        role = 1;
    }
    // 2. check is nurse
    else if (verify_user_from_csv("csv/Nurse.csv", input_id, input_pass, user_name))
    {
        login_success = 1;
        role = 2;
    }
    // 3. check is admin
    else if (verify_user_from_csv("csv/Admin.csv", input_id, input_pass, user_name))
    {
        login_success = 1;
        role = 3;
    }
    // 4. check is patient
    else if (verify_user_from_csv("csv/Patient.csv", input_id, input_pass, user_name))
    {
        login_success = 1;
        role = 4;
    }
    // 5. check is director
    else if (verify_user_from_csv("csv/Director.csv", input_id, input_pass, user_name))
    {
        login_success = 1;
        role = 5;
    }

    // login process
    if (login_success == 1)
    {
        printf("\nLogin Successful! Welcome back, %s.\n", user_name);
        strcpy(current_logged_in_id, input_id);
        
        switch (role) {
            case 1:
                printf("Entering Doctor Menu...\n");
                return 1;
                break;
            case 2:
                printf("Entering Nurse Menu...\n");
                return 1;
                break;
            case 3:
                printf("Entering Admin Menu...\n");
                return 1;
                break;
            case 4:
                printf("Entering Patient Menu...\n");
                return 1;
                break;
            case 5:
                printf("Entering Director Menu...\n");
                return 1;
                break;
        }
    }
    else {
        printf("\n[ERROR] Invalid ID or Password! Please try again.\n");
    }
}



// check the string is digit or not (for contact number and IC) because 'isdigit' only can check for a character not string
int check_isdigit(char *checkdigit)
{
    for(int i = 0; i < strlen(checkdigit); i++)
    {
        if(!isdigit(checkdigit[i]) || isspace(checkdigit[i])) return 0; //not a valid input
    }
    return 1;
}



// function to check user input integer validation
int contactandIC_num_validation(int num_type, char *num, char *prompt, int min_num, int max_num)
{
    while (1)
    {
        printf("%s", prompt);
        fgets(num, 20, stdin);
        num[strcspn(num, "\n")] = 0;

        if (check_isdigit(num) != 1)// when user insert char or string not number
        {
            printf("ERROR: 'INVALID INPUT OR HAVE A SPACE BETWEEN NUMBERS' PLEASE ENTER AGAIN\n");
        }
        
        else if (strlen(num) < min_num || strlen(num) > max_num)
        {
            printf("ERROR: 'INVALID RANGE' PLEASAE ENTER 10 / 11 NUMBERS\n");
        }
        else
        {
            if(num_type == 1)
            {
                sprintf(num, "01%s", num);
                return 1; // return 1 means record successful
            }

            else if(num_type ==2)
            {
                // remember add one more for ic format with "-"
                return 1;// return 1 means record successful
            }

            else
            {
                printf("ERROR: 'NOT NUMBER TYPE' PLEASE CHECK YOUR FUNTION\n");
            }
        }
        
    }
    
}



//function for price/salary format validation (in string type)
int check_float(char *check_f)
{
    int length_of_float = strlen(check_f);

    if(strlen(check_f) == 3) return 0;

    for (int i = length_of_float -1 ; i > length_of_float -3; i--)
    {
        if (!isdigit(check_f[i]) ) return 0;
    }


    for (int i = length_of_float - 4 ; i >= 0; i--)
    {
        if (!isdigit(check_f[i])) return 0;
    }

    
    for (int i = length_of_float - 3 ; i >= length_of_float - 3; i--)
    {
        if (check_f[i] != '.' ) return 0;
    }

    return 1;
}



//function for name format validation
int check_name(char *check_n)
{
    if(strlen(check_n) == 0  || check_n[0] == ' ') return 0;

    for (int i = 0; i < strlen(check_n); i++)
    {
        if (isdigit(check_n[i]) || (!isalpha(check_n[i]) && !isblank(check_n[i]))) return 0;
    }
    return 1;
}



//function for generate new admin account
void create_admin_account(admin *ptr_new_admin)
{
    int admin_random_num;
    char adminduty;
    char adminemployment;

    srand(time(NULL)); //seed random number generator to get unique random number everytime the program start
    admin_random_num = rand() % 1000; //generate random number from 0 ~ 999
    sprintf(ptr_new_admin->admin_id, "A%03d", admin_random_num); //admin_id will become Axxx (xxx is random numbers)
    printf("New Admin ID is %s\n", ptr_new_admin->admin_id);

    //get new admin name
    do
    {
        printf("Admin Name: ");
        fgets(ptr_new_admin->admin_name, sizeof(ptr_new_admin->admin_name), stdin);//get admin name
        ptr_new_admin->admin_name[strcspn(ptr_new_admin->admin_name, "\n")] = 0; //remove new line "\n"

        if (!check_name(ptr_new_admin->admin_name))
        {
            printf("ERROR: 'INVALID INPUT' PLEASE ENTER AGAIN\n");//display error while invalid input
        }
        
    } while (!check_name(ptr_new_admin->admin_name));

    //get admin contact number
    contactandIC_num_validation(1, ptr_new_admin->admin_contact, "Contact number (without'-'): 01", 8, 9);

    //get admin duty status
    do
    {
        printf("Admin Duty status\n1. on\n2. off\n:");
        scanf(" %c", &adminduty);
        while( getchar() != '\n');

        if(!isdigit(adminduty))
        {
            printf("ERROR: 'INVALID INPUT OR HAVE A SPACE BETWEEN NUMBERS' PLEASE ENTER AGAIN\n");
        }
        else
        {
             switch (adminduty)
            {
            case '1':
                strcpy(ptr_new_admin->admin_duty, "on");
                break;

            case '2':
                strcpy(ptr_new_admin->admin_duty, "off");
                break;
            
            default:
                printf("ERROR: 'INVALID INPUT' PLEASE ENTER AGAIN\n");
                break;
            }
        }
    } while (adminduty > '2' || adminduty < '1');

    //get admin employment status
    do
    {
        printf("Admin Employment status\n1. permanent\n2. contract-bond\n3. intern\n:");
        scanf(" %c", &adminemployment);
        while( getchar() != '\n');

        if(!isdigit(adminemployment))
        {
            printf("ERROR: 'INVALID INPUT OR HAVE A SPACE BETWEEN NUMBERS' PLEASE ENTER AGAIN\n");
        }
        else
        {
            switch (adminemployment)
            {
            case '1':
                strcpy(ptr_new_admin->admin_employment, "Permanent");
                break;

            case '2':
                strcpy(ptr_new_admin->admin_employment, "Contract-bond");
                break;

            case '3':
                strcpy(ptr_new_admin->admin_employment, "Intern");
                break;
            
            default:
                printf("ERROR: 'INVALID INPUT' PLEASE ENTER AGAIN\n");
                break;
            }
        }
    } while (adminemployment > '3' || adminemployment < '1');

    //get admin salary
    int valid_float;
    do
    {
        printf("Admin Salary [example: RM1234.00]: RM");
        fgets(ptr_new_admin->admin_salary, sizeof(ptr_new_admin->admin_salary), stdin);
        ptr_new_admin->admin_salary[strcspn(ptr_new_admin->admin_salary, "\n")] = 0; //remove '\n'

        valid_float = check_float(ptr_new_admin->admin_salary);
        if (valid_float == 1)
        {
            printf("VALID INPUT\n");
        }
        else
        {
            printf("ERROR: 'INVALID INPUT' PLEASE ENTER AGAIN\n");
        }
        
    } while (valid_float == 0);

    //open Admin.csv and append value into file
    FILE *create_admin_file = fopen("csv/Admin.csv", "a");
    char admin_file_row[100];// maximum number of row is 100

    if(create_admin_file != NULL)
    {
        //append value
        fprintf(create_admin_file, "%s,%s,%s,%s,%s,%s\n", 
            ptr_new_admin->admin_id,ptr_new_admin->admin_name,ptr_new_admin->admin_contact,ptr_new_admin->admin_duty,ptr_new_admin->admin_employment,ptr_new_admin->admin_salary);
        printf("CREATE SUCCESSFUL ^_^\n");
    }

    else
    {
        printf("!!!CANNOT FIND THIS FILE!!!\n");
    }

    fclose(create_admin_file);

    return; //create successful
}



//function for select staff types 这里要把里面的function改成select_search_method
void select_staff_type()
{

    char staff_type;
    int select_staff_loop;
    do
    {
        printf("====WELCOME TO SEARCH STAFF ACCOUNT====\n"
                "Choose a Staff type:\n1. Admin\n2. Doctor\n3. Nurse\n4. Exit\nEnter your choice: ");
        scanf(" %c", &staff_type);
        while (getchar() != '\n');
        
        if (isdigit(staff_type))
        {
            switch (staff_type)
            {
            case '1':
                search_staff_account(1);
                select_staff_loop = 1;
                break;
            
            case '2':
                search_staff_account(2);
                select_staff_loop = 1;
                break;
            
            case '3':
                search_staff_account(3);
                select_staff_loop = 1;
                break;
                
            case '4':
                printf("Exiting......\n\n");
                select_staff_loop = 0;
                break;

            default:
                printf("Please enter a correct number ^_^\n\n");
                break;
            }
        }
        else
        {
            select_staff_loop = 1; // invalid input, loop program
        }
    } while (select_staff_loop == 1 || staff_type > '4' || staff_type < '1');

    return; //when 'while loop' break, back to director menu with value '1'
    
}



//function for search staff account
void search_staff_account(int open_file_type)
{
    fileline search_staff;
    switch (open_file_type)
    {
    case 1:
        
        char searchadminemployment[20];
        int search_admin;

        int adminemployment = select_staff_employment();

        if (adminemployment == 1)
        {
            strcpy(searchadminemployment, "Permanent");
        }
        else if (adminemployment == 2)
        {
            strcpy(searchadminemployment, "Contract-bond");
        }
        else
        {
            strcpy(searchadminemployment, "Intern");
        }

        FILE *search_admin_file = fopen("csv/Admin.csv", "r");

        if(!search_admin_file){printf("ERROR: 'FILE NOT FOUND\n");}

        printf("\n");
        printf("========ADMIN INFORMATION========\n"
                "%-6s | %-35s | %-13s | %-12s | %-18s | %-11s\n"
                "-------------------------------------------------------------------------------------------------------------------\n", 
                "ID", "Name", "Contact", "Duty Status", "Employment Status", "Salary (RM)");
        fgets(search_staff.line, sizeof(search_staff.line), search_admin_file);//read first line but skip it

        //start to read first line of data
        while (fgets(search_staff.line, sizeof(search_staff.line), search_admin_file))
        {
            //make a copy of data to read, so that when user seach it won't affect the original data
            strcpy(search_staff.temporary_line, search_staff.line);

            //split row of data into pieces by using strtok
            char *search_admin_id = strtok(search_staff.temporary_line, ",");
            char *search_admin_name = strtok(NULL, ",");
            char *search_admin_contact = strtok(NULL, ",");
            char *search_admin_duty = strtok(NULL, ",");
            char *search_admin_employment = strtok(NULL, ",");
            char *search_admin_salary = strtok(NULL, ",");

            //compare search admin id to find the exact id
            if(searchadminemployment != NULL && strcmp(search_admin_employment, searchadminemployment) == 0)
            {
                printf("%-6s | %-35s | %-13s | %-12s | %-18s | %s\n", search_admin_id, search_admin_name, search_admin_contact, search_admin_duty, search_admin_employment, search_admin_salary);
                search_admin = 1;
            }
        }
        printf("\n");
        if(search_admin != 1){printf("ERROR: 'ADMIN ACCOUNT NOT FOUND'\n\n");}

        fclose(search_admin_file);
        break;
        

    case 2:
        char searchdremployment[20];
        int search_dr;

        int doctoremployment = select_staff_employment();

        if (doctoremployment == 1)
        {
            strcpy(searchdremployment, "Permanent");
        }
        else if (doctoremployment == 2)
        {
            strcpy(searchdremployment, "Contract-bond");
        }
        else
        {
            strcpy(searchdremployment, "Intern");
        }        

        FILE *search_dr_file = fopen("csv/Dr.csv", "r");

        if(!search_dr_file){printf("ERROR: 'FILE NOT FOUND\n");}

        printf("\n");
        printf("========DOCTOR INFORMATION========\n"
                "%-6s | %-35s | %-7s | %-20s | %-13s | %-12s | %-13s | %-18s | %s\n"
                "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", 
                "ID", "Name", "Gender", "Emergency Contact", "Contact", "Duty Status", "Specialism", "Employment Status", "Salary (RM)");
        fgets(search_staff.line, sizeof(search_staff.line), search_dr_file);//read first line but skip it

        //start to read first line of data
        while (fgets(search_staff.line, sizeof(search_staff.line), search_dr_file))
        {
            //make a copy of data to read, so that when user seach it won't affect the original data
            strcpy(search_staff.temporary_line, search_staff.line);

            //split row of data into pieces by using strtok
            char *search_dr_id = strtok(search_staff.temporary_line, ",");
            char *search_dr_name = strtok(NULL, ",");
            char *search_dr_gender = strtok(NULL, ",");
            char *search_dr_emergency = strtok(NULL, ",");
            char *search_dr_contact = strtok(NULL, ",");
            char *search_dr_duty = strtok(NULL, ",");
            char *search_dr_specialism = strtok(NULL, ",");
            char *search_dr_employment = strtok(NULL, ",");
            char *search_dr_salary = strtok(NULL, ",");

            //compare search admin id to find the exact id
            if(searchdremployment != NULL && strcmp(search_dr_employment, searchdremployment) == 0)
            {
                printf("%-6s | %-35s | %-7s | %-20s | %-13s | %-12s | %-13s | %-18s | %s\n", 
                    search_dr_id, search_dr_name, search_dr_gender, search_dr_emergency, search_dr_contact, search_dr_duty, search_dr_specialism, search_dr_employment, search_dr_salary);
                search_dr = 1; // signal for account founded
            }
        }
        printf("\n");
        if(search_dr != 1){printf("ERROR: 'DOCTOR ACCOUNT NOT FOUND'\n\n");} //if account not found

        fclose(search_dr_file);
        break;

    case 3:// still fixing
        char searchnurseemployment[20];
        int search_nurse;

        int nurseemployment = select_staff_employment();

        if (nurseemployment == 1)
        {
            strcpy(searchnurseemployment, "Permanent");
        }
        else if (nurseemployment == 2)
        {
            strcpy(searchnurseemployment, "Contract-bond");
        }
        else
        {
            strcpy(searchnurseemployment, "Intern");
        }        

        FILE *search_nurse_file = fopen("csv/Nurse.csv", "r");

        if(!search_nurse_file){printf("ERROR: 'FILE NOT FOUND\n");}
        
        printf("\n");
        printf("========NURSE INFORMATION========\n"
                "%-6s | %-35s | %-7s | %-13s | %-12s | %-25s | %-13s | %-18s | %s\n"
                "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", 
                "ID", "Name", "Gender", "Contact", "Duty Status", "Position", "Department", "Employment Status", "Salary (RM)");
        fgets(search_staff.line, sizeof(search_staff.line), search_nurse_file);//read first line but skip it

        //start to read first line of data
        while (fgets(search_staff.line, sizeof(search_staff.line), search_nurse_file))
        {
            //make a copy of data to read, so that when user seach it won't affect the original data
            strcpy(search_staff.temporary_line, search_staff.line);

            //split row of data into pieces by using strtok
            char *search_nurse_id = strtok(search_staff.temporary_line, ",");
            char *search_nurse_name = strtok(NULL, ",");
            char *search_nurse_gender = strtok(NULL, ",");
            char *search_nurse_contact = strtok(NULL, ",");
            char *search_nurse_duty = strtok(NULL, ",");
            char *search_nurse_position = strtok(NULL, ",");
            char *search_nurse_department = strtok(NULL, ",");
            char *search_nurse_employment = strtok(NULL, ",");
            char *search_nurse_salary = strtok(NULL, ",");

            //compare search admin id to find the exact id
            if(searchnurseemployment != NULL && strcmp(search_nurse_employment, searchnurseemployment) == 0)
            {
                printf("%-6s | %-35s | %-7s | %-13s | %-12s | %-25s | %-13s | %-18s | %s\n", 
                        search_nurse_id, search_nurse_name, search_nurse_gender, search_nurse_contact, search_nurse_duty, search_nurse_position, search_nurse_department, search_nurse_employment, search_nurse_salary);
                search_nurse = 1; // signal for account founded
            }
        }
        printf("\n");
        if(search_nurse != 1){printf("ERROR: 'NURSE ACCOUNT NOT FOUND'\n\n");} //if account not found

        fclose(search_nurse_file);
        break;
    
    default:
        break;
    }

    return;
}



//function for select employment
int select_staff_employment()
{
    char staff_employment;
    int select_employment_loop;
    do
    {
        printf("====PLEASE SELECT AN EMPLOYMENT TYPE\n"
                "1. Permanent\n2. Contract-bond\n3. Intern\nEnter your choise: ");
        scanf(" %c", &staff_employment);
        while(getchar() != '\n');

        if (isdigit(staff_employment))
        {
            switch (staff_employment)
            {
            case '1':
                return 1;
                break;

            case '2':
                return 2;
                break;
                
            case '3':
                return 3;
                break;
            
            default:
                printf("Please enter a correct number ^_^\n\n");
                break;
            }
        }
        else
        {
            select_employment_loop = 1;
        }
        
    } while (select_employment_loop == 1 || staff_employment > '3' || staff_employment < '1');
    
}



//function for generate sales report
void financial_sales_generator()
{
    fileline unpaid_sales;
    fileline paid_sales;
    char search_report_year[5];
    char get_search_report_year[5];
    float total_unpaid_amount = 0;
    float total_paid_amount = 0;
    int generate_sales_report;
    int length_of_input;
    
    do
    {
        printf("Enter report year: ");
        fgets(get_search_report_year, sizeof(get_search_report_year), stdin);
        get_search_report_year[strcspn(get_search_report_year, "\n")] = 0;

        if (strlen(get_search_report_year) == 4)
        {
            length_of_input = 1;
        } else {
            length_of_input = 0;
        }

    } while (!check_isdigit(get_search_report_year) || length_of_input == 0);
    sprintf(search_report_year, "%s", get_search_report_year);
    
    FILE *unpaid_report = fopen("csv/Bill_Payment.csv", "r");

    if(!unpaid_report){printf("ERROR: 'FILE NOT FOUND\n");}

    printf("\n");
    printf("========YEARLY SALES REPORT========\n");
    printf("\n");
    
    fgets(unpaid_sales.line, sizeof(unpaid_sales.line), unpaid_report);//read first line but skip it

    printf("========UNPAID BILL========\n"
            "%-8s | %-10s | %-10s | %-15s\n"
            "-------------------------------------------------------------------------------------------------------------------\n", 
            "Bill ID", "Patient ID", "Bill date", "Unpaid amount (RM)");

    while (fgets(unpaid_sales.line, sizeof(unpaid_sales.line), unpaid_report))
    {
        //make a copy of data to read, so that when user seach it won't affect the original data
        strcpy(unpaid_sales.temporary_line, unpaid_sales.line);

        //split row of data into pieces by using strtok
        char *unpaid_data_of_row[100];
        int unpaid_number_data_row = 0;

        //compare search admin id to find the exact id
        char *unpaid_search_bill_token = strtok(unpaid_sales.temporary_line, ",");
        while (unpaid_search_bill_token != NULL && unpaid_number_data_row < 100)
        {
            unpaid_data_of_row[unpaid_number_data_row] = unpaid_search_bill_token;
            unpaid_number_data_row++;
            unpaid_search_bill_token = strtok(NULL, ",");
        }

        char *unpaid_bill_id = unpaid_data_of_row[0];
        char *unpaid_patient_id = unpaid_data_of_row[1];
        char *unpaid_bill_date = unpaid_data_of_row[unpaid_number_data_row - 4];
        char *unpaid_amount = unpaid_data_of_row[unpaid_number_data_row - 5];
        char *unpaid_status = unpaid_data_of_row[unpaid_number_data_row - 1];

        //remove "\n" from unpaid_status
        if (unpaid_status != NULL)
        {
            int len = strlen(unpaid_status);
            while (len > 0 && (unpaid_status[len - 1] == '\n' || unpaid_status[len - 1] == '\r'))
            {
                unpaid_status[len - 1] = '\0';
                len--;
            }
        }

        //select year from date
        int day = 0, month = 0, year = 0;
        char unpaid_year[7] = "";
        if(unpaid_bill_date != NULL)
        {
            sscanf(unpaid_bill_date, "%d/%d/%d", &day, &month, &year);
            sprintf(unpaid_year, "%d", year);
        }

        if (search_report_year != NULL && strcmp(unpaid_year, search_report_year) == 0 && strcmp(unpaid_status, "Unpaid") == 0)
        {
            printf("%-8s | %-10s | %-2d/%-2d/%-4d | %-15s\n", unpaid_bill_id, unpaid_patient_id, day, month, year, unpaid_amount);
            total_unpaid_amount += atof(unpaid_amount);
            generate_sales_report = 1;
        }
    }
    fclose(unpaid_report);

    printf("\n");

    FILE *paid_report = fopen("csv/Bill_Payment.csv", "r");

    if(!paid_report){printf("ERROR: 'FILE NOT FOUND\n");}
    
    fgets(paid_sales.line, sizeof(paid_sales.line), paid_report);//read first line but skip it

    printf("========PAID BILL========\n"
            "%-8s | %-10s | %-10s | %-15s\n"
            "-------------------------------------------------------------------------------------------------------------------\n", 
            "Bill ID", "Patient ID", "Bill date", "Paid amount (RM)");

    while (fgets(paid_sales.line, sizeof(paid_sales.line), paid_report))
    {
        //make a copy of data to read, so that when user seach it won't affect the original data
        strcpy(paid_sales.temporary_line, paid_sales.line);

        //split row of data into pieces by using strtok
        char *paid_data_of_row[100];
        int paid_number_data_row = 0;

        //compare search admin id to find the exact id
        char *paid_search_bill_token = strtok(paid_sales.temporary_line, ",");
        while (paid_search_bill_token != NULL && paid_number_data_row < 100)
        {
            paid_data_of_row[paid_number_data_row] = paid_search_bill_token;
            paid_number_data_row++;
            paid_search_bill_token = strtok(NULL, ",");
        }
        
        char *paid_bill_id = paid_data_of_row[0];
        char *paid_patient_id = paid_data_of_row[1];
        char *paid_bill_date = paid_data_of_row[paid_number_data_row - 4];
        char *paid_amount = paid_data_of_row[paid_number_data_row - 5];
        char *paid_status = paid_data_of_row[paid_number_data_row - 1];

        //remove "\n" from unpaid_status
        if (paid_status != NULL)
        {
            int p_len = strlen(paid_status);
            while (p_len > 0 && (paid_status[p_len - 1] == '\n' || paid_status[p_len - 1] == '\r'))
            {
                paid_status[p_len - 1] = '\0';
                p_len--;
            }
        }

        //select month from date
        int p_day = 0, p_month = 0, p_year = 0;
        char paid_year[5] = "";
        if(paid_bill_date != NULL)
        {
            sscanf(paid_bill_date, "%d/%d/%d", &p_day, &p_month, &p_year);
            sprintf(paid_year, "%d", p_year);
        }

        if (search_report_year != NULL && strcmp(paid_year, search_report_year) == 0 && strcmp(paid_status, "Unpaid") != 0)
        {
            printf("%-8s | %-10s | %-2d/%-2d/%-4d | %-15s\n", paid_bill_id, paid_patient_id, p_day, p_month, p_year, paid_amount);
            total_paid_amount += atof(paid_amount);
            generate_sales_report = 1;
        }
    }

    printf("\n");

    printf("======REPORT SUMMARY======\n"
            "Total UNPAID amount: RM%.2f\n"
            "Total PAID amount: RM%.2f\n", total_unpaid_amount, total_paid_amount);

    printf("\n");
    if(generate_sales_report != 1){printf("ERROR: 'THIS YEAR NO SALES'\n\n");}

    fclose(paid_report);
    return;
}