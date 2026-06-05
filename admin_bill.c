// 3.bill management system
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include "admin_menu.h"

const char *medIDMap[] = {"", "M001", "M002", "M003", "M004", "M005", "M006", "M007", "M008", "M009", "M010"};

// typedef struct{
//     char bill_category[20];
//     int bill_sub;
//     float bill_sub_price; 
// }BillItem;


// struct bill{
//     char bill_id[8];
//     char patient_id[8];
//     char admin_id[8];

//     BillItem * items;
//     int count;

//     float bill_total;
//     char bill_date[10];
//     char payment_date[10];
//     char payment_time[5];
//     char payment_method[15];
// };

//protect for skipping

void trim(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == ' ')) {
        str[len-1] = '\0';
        len--;
    }
}
//find the name
char *get_name(const char *filepath, const char *searchID) {
    static char name[30];
    char line[256];
    FILE *file = fopen(filepath, "r");
    if (!file) return "Unknown";

    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char temp[256];
        strcpy(temp, line);

        char *id   = strtok(temp, ",");
        char *nm   = strtok(NULL, ",");  // name is second column

        if (id && strcmp(id, searchID) == 0) {
            strcpy(name, nm);
            fclose(file);
            return name;
        }
    }
    fclose(file);
    return "Unknown";
}

//search invoice management
void displayinvoice(const char *invoicefile) {
    FILE *display = fopen(invoicefile, "r");
    char line[2048];

    if (display == NULL) {
        perror("Error opening the file");
        return;
    }

    printf("\n================================ HOSPITAL BILLING LEDGER ==================================\n");
    printf("%-10s %-12s %-10s %-15s %-10s %-10s %-15s %-15s\n",
           "Bill ID", "Patient ID", "Total (RM)", "Date", "Items", "Status", "Bill date", "AdminID");
    printf("--------------------------------------------------------------------------------\n");

    fgets(line, sizeof(line), display); // Skip header

    while (fgets(line, sizeof(line), display)) {
        char temp[2048];
        strcpy(temp, line);

        // 1. Extract IDs
        char *billID    = strtok(temp, ",");
        char *patientID = strtok(NULL, ",");
        char *adminID   = strtok(NULL, ",");

        // 2. Loop through all slots

        char *countStr = strtok(NULL, ",");
        int itemCount  = countStr ? atoi(countStr) : 0;

            // 3. Skip past all item columns
            for (int i = 0; i < itemCount; i++) {
                strtok(NULL, ","); // skip category
                strtok(NULL, ","); // skip qty
                strtok(NULL, ","); // skip subprice
            }
                // 3. Extract footer fields
                char *billTotal = strtok(NULL, ",");
                char *billDate  = strtok(NULL, ",");
                char *payDate   = strtok(NULL, ",");
                char *payTime   = strtok(NULL, ",");
                char *payMethod = strtok(NULL, ",\r\n");

                if (payMethod) trim(payMethod);

                // 4. Print main summary line
                printf("%-10s %-12s %-10s %-15s %-10d %-10s %-15s %-15s\n",
                    billID,
                    patientID,
                    (billTotal ? billTotal : "0.00"),
                    (payDate  ? payDate  : "N/A"),
                    itemCount,               // ← already the correct count!
                    (payMethod ? payMethod : "Unpaid"),
                     billDate,
                    adminID);
    }

    printf("--------------------------------------------------------------------------------\n");
    fclose(display);
}

void display_bill_sections(char catIDs[][20], int catQtys[], float catPrices[], 
                           int count, char *billID, char *patientID, 
                           char *adminID, char *billDate, 
                           char *billTotal, char *payMethod) 
{

    printf("\n==================================================\n");
    printf(" Bill ID: %-10s | Date: %s\n", billID, billDate);
    printf(" Patient ID: %-10s | Admin: %s\n", patientID, adminID);
    printf("--------------------------------------------------\n");

    printf(" Medicine:\n");
    for (int j = 0; j < count; j++) {
        if (catIDs[j][0] == 'M') {
            char *name = get_name("csv/Inventory.csv", catIDs[j]);
            printf("  %-20s | Qty: %-5d | Subtotal: RM%.2f\n",
                   name, catQtys[j], catPrices[j]);
        }
    }

    printf("\n Surgery:\n");
    for (int j = 0; j < count; j++) {
        if (catIDs[j][0] == 'S') {
            char *name = get_name(".csv/category.csv", catIDs[j]);
            printf("  %-20s | Qty: %-5d | Subtotal: RM%.2f\n",
                   name, catQtys[j], catPrices[j]);
        }
    }

    printf("\n Room:\n");
    for (int j = 0; j < count; j++) {
        if (catIDs[j][0] == 'R') {
            char *name = get_name("csv/category.csv", catIDs[j]);
            printf("  %-20s | Days: %-4d | Subtotal: RM%.2f\n",
                   name, catQtys[j], catPrices[j]);
        }
    }

    printf("\n Consultation:\n");
    for (int j = 0; j < count; j++) {
        if (catIDs[j][0] == 'C') {
            char *name = get_name("csv/category.csv", catIDs[j]);
            printf("  %-20s | Qty: %-5d | Subtotal: RM%.2f\n",
                   name, catQtys[j], catPrices[j]);
        }
    }

    printf("--------------------------------------------------\n");
    printf(" GRAND TOTAL: RM%-10s | STATUS: %s\n",
           billTotal ? billTotal : "?",
           payMethod ? payMethod : "Unpaid");
    printf("==================================================\n");
}
//search invoice
void searchBillByPatientIDAndDate(const char *filepath, const char *targetPID, const char *targetDate) {
    FILE *file = fopen(filepath, "r");
    if (!file) return;

    char line[2048];
    int found = 0;
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        char temp[2048];
        strcpy(temp, line);

        char *billID    = strtok(temp, ",");
        char *patientID = strtok(NULL, ",");
        char *adminID   = strtok(NULL, ",");

        char *countStr  = strtok(NULL, ",");
        int count       = countStr ? atoi(countStr) : 0;

        // declare separate arrays — matches display_bill_sections parameters
        char  catIDs[count][20];
        int   catQtys[count];
        float catPrices[count];

        for (int i = 0; i < count; i++) {
            char *id       = strtok(NULL, ",");
            char *qty      = strtok(NULL, ",");
            char *subprice = strtok(NULL, ",");

            strcpy(catIDs[i], id       ? id       : "N/A");
            catQtys[i]   = qty      ? atoi(qty)      : 0;
            catPrices[i] = subprice ? atof(subprice) : 0.0;
        }

        char *billTotal = strtok(NULL, ",");
        char *billDate  = strtok(NULL, ",");
        char *payDate   = strtok(NULL, ",");
        char *payTime   = strtok(NULL, ",");
        char *payMethod = strtok(NULL, ",\r\n");

        if (billDate) billDate[strcspn(billDate, " \r\n")] = 0;

        if (billID && billDate &&
            strcmp(billID, targetPID) == 0 &&
            strcmp(billDate,  targetDate) == 0)
        {
            found = 1;
            display_bill_sections(catIDs, catQtys, catPrices, count,
                                  billID, patientID, adminID, billDate,
                                  billTotal, payMethod);
        }  // ← missing closing brace fixed!
    }

    fclose(file);
    if (!found) printf("No record found for %s on %s.\n", targetPID, targetDate);
}

/*int getmedinfo(const char *medicine_id, float *medicine_price)
{
    char line[256];
    FILE *med =fopenf("..file/Inventory.csv","r");
        if(med == NULL)
    {
        printf("Error opening inventory file!\n");
        return 0;
    }
    fgets(line, sizeof(line), med); // skip header

    while(fgets(line, sizeof(line), med))
    {
        char temp[256];
        strcpy(temp, line);

        char *med_id =strtok(temp, ",");
        char * medicine_name = strtok(NULL, ",");
        char *price = strtok(NULL, ",");
        if(strcmp(med_id, medicine_id) == 0)
        {
            *medicine_price = atof(price);
            fclose(med);
            return 1; // found
        }
    }
    fclose(med);
    return 0;
}*/

float getcategoryprice(const char *category){
    char line[256];
    FILE *cat = fopen("csv/category.csv", "r");
    if (cat == NULL)
    {
        printf("Error opening bill category file!\n");
        return -1; // indicate error
    }
    fgets(line, sizeof(line), cat); // skip header

    while(fgets(line, sizeof(line), cat))
    {
        char temp[256];
        strcpy(temp, line);

        char *cat_id = strtok(temp, ",");
        char *cat_price = strtok(NULL, ",");

        if (cat_id && strcmp(cat_id, category) == 0)
        {
            float price = atof(cat_price);
            fclose(cat);
            return price;
        }
    }
    fclose(cat);
    return -1; // indicate error
}

float getmedprice(const char *medicine_id){
   char line[256];
   FILE *med = fopen("csv/Inventory.csv", "r");
   if (med == NULL)
   {
    perror("Error opening inventory file!");
    return -1; // indicate error // or could exit program// make sure to handle this in the calling function
   }
   fgets(line, sizeof(line), med); // skip header

   while(fgets(line, sizeof(line), med))
   {
    char temp[200];
    strcpy( temp, line);

    char *med_id = strtok(temp,",");
    strtok(NULL,","); // skip medicine name
    char *price = strtok(NULL, ",");

    if (med_id &&strcmp(med_id, medicine_id) == 0)
    {
        float med_price = atof(price); // convert price string to float, atof returns 0.0 if conversion fails, so we can check for that
        fclose(med);
        return med_price;
    }
   }
   fclose(med);
   return -1; // indicate error
}

int idcheck(const char *filepath, const char *id, int columnIndex)
 {
    char line[256];
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 0; // treat as not found if file can't be opened
    }

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        char temp[256];
        strcpy(temp, line);

        // Tokenize by commas
        char *token = strtok(temp, ",");
        int col = 0;

        while (token != NULL) {
            if (col == columnIndex) {
                if (strcmp(token, id) == 0) {
                    fclose(file);
                    return 1; // ID found in the specified column
                }
                break; // stop checking this line once we reach the target column
            }
            token = strtok(NULL, ",");
            col++;
        }
    }

    fclose(file);
    return 0; // ID not found
  }

int validatePrefixedID(const char *id, char prefix) {
    int len = strlen(id);

    // 1. Check if the ID is at least a prefix + 1 digit (e.g., A1)
    if (len < 2) return 0;

    // 2. Check the Prefix
    if (id[0] != prefix) return 0;

    // 3. Check the Length based on the prefix
    if (prefix == 'P' && len != 5) return 0; // P0001 (5 chars)
    if (prefix == 'A' && len != 4) return 0; // A001  (4 chars)

    // 4. Ensure all remaining characters are digits
    for (int i = 1; i < len; i++) {
        if (!isdigit(id[i])) return 0;
    }

    return 1;
}

void save_bill(struct bill *f) {
    FILE *fptr = fopen("csv/Bill_Payment.csv", "a");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }

    // 1. ID, Patient, Admin
    fprintf(fptr, "%s,%s,%s", f->bill_id, f->patient_id, f->admin_id);

    // 2. Only actual items — no N/A padding!
    fprintf(fptr, ",%d", f->count);  // save count first so we know how many to read back
    for (int i = 0; i < f->count; i++) {
        fprintf(fptr, ",%s,%d,%.2f",
            f->items[i].bill_category,
            f->items[i].bill_sub,
            f->items[i].bill_sub_price);
    }

    // 3. Total, dates, payment
    fprintf(fptr, ",%.2f,%s,%s,%s,%s\n",
        f->bill_total,
        f->bill_date,
        "Pending", "Pending", "Unpaid");

    fclose(fptr);
    printf("%s saved.\n", f->bill_id);
}

float get_price(const char *filepath, const char *searchID) {
    char line[256];
    FILE *file = fopen(filepath, "r");
    
    if (file == NULL) {
        perror("Error opening bill category file!");
        return -1.0; 
    }

    fgets(line, sizeof(line), file); // Skip the CSV header

    while (fgets(line, sizeof(line), file)) {
        char temp[256];
        strcpy(temp, line);

        char *id = strtok(temp, ",");
        strtok(NULL, ","); // Skip the description/name column
        char *priceStr = strtok(NULL, ",");

        if (id && strcmp(id, searchID) == 0) {
            float price = atof(priceStr);
            fclose(file);
            return price; // Found it!
        }
    }

    fclose(file);
    return -1.0; // ID not found
}

void add_item(struct bill *b, const char *id, int qty, float unit_price) {
    b->items = realloc(b->items, (b->count + 1) * sizeof(BillItems));
    
    strcpy(b->items[b->count].bill_category, id);
    b->items[b->count].bill_sub       = qty;
    b->items[b->count].bill_sub_price = unit_price * qty;
    
    b->bill_total += unit_price * qty;
    b->count++;
}

void calculate_billing(struct bill *b) {
    // initialize
    b->items      = NULL;
    b->count      = 0;
    b->bill_total = 0.0;

    int choice, qty, days;
    float price;
    int med_types;
do{
    // --- Medicine ---
    printf("How many medicines? (0-10): ");
    scanf("%d", &med_types);
    buffer();
    if(med_types > 0 && med_types <= 10)
    {
    for (int i = 0; i < med_types; i++) {
        printf("\n=======HOSPITAL MEDICINE INVENTORY======\n");
        printf("1. Amoxicillin       6. Gadobenate\n");
        printf("2. Fluticasone       7. Propofol\n");
        printf("3. Epinephrine       8. Cefazolin\n");
        printf("4. Naloxone          9. Levetiracetam\n");
        printf("5. Iohexol          10. Levodopa\n");
        printf("========================================\n");
        printf("Select Med #%d (1-10): ", i + 1);
        scanf("%d", &choice);
        buffer();

        if (choice >= 1 && choice <= 10) {
            const char *id = medIDMap[choice];
            price = get_price("csv/Inventory.csv", id);

            if (price != -1.0) {
                printf("Enter quantity: ");
                scanf("%d", &qty);
                buffer();
                add_item(b, id, qty, price);
                printf("Added %s | Subtotal: RM%.2f\n", id, price * qty);
            } else {
                printf("Medicine not found!\n");
                i--;
            }
          
        } else {
            printf("Invalid choice.\n");
            i--;
        }
    }  break;
}else if(med_types == 0)
{
    printf("No need for nedicine.Skipped to surgery.\n");
    break;
}
else
{
    printf("Invalid input. Please enter again!\n");
}
}while(1);

    // --- Surgery ---
    printf("\n1. General Surgery\n2. Emergency Surgery\n0. Skip\nEnter surgery: ");
    scanf("%d", &choice);
    buffer();

    if (choice == 1 || choice == 2) {
        const char *id = (choice == 1) ? "S001" : "S002";
        price = get_price("csv/category.csv", id);
        if (price != -1.0) {
            add_item(b, id, 1, price);
            printf("Surgery added: RM%.2f\n", price);
        }
    }

    // --- Room ---
    printf("\n1. Normal Ward\n2. VIP\n3. ICU\n0. Skip\nEnter room: ");
    scanf("%d", &choice);
    buffer();

    if (choice >= 1 && choice <= 3) {
        const char *id = (choice == 1) ? "R001" : (choice == 2) ? "R002" : "R003";
        printf("Enter number of days: ");
        scanf("%d", &days);
        buffer();
        price = get_price("csv/category.csv", id);
        if (price != -1.0) {
            add_item(b, id, days, price);
            printf("Room added: RM%.2f\n", price * days);
        }
    }

    // --- Consultation ---
    printf("\nConsultation? (0: skip, 1: yes): ");
    scanf("%d", &choice);
    buffer();

    if (choice == 1) {
        price = get_price("csv/category.csv", "C001");
        if (price != -1.0) {
            add_item(b, "C001", 1, price);
            printf("Consultation added: RM%.2f\n", price);
        }
    }
}

void CreateBillRecord(struct bill *b)

{
    FILE *admin = fopen("csv/Admin.csv", "r");
    if (admin == NULL) 
    {
        printf("Error opening admin file!\n");
        return;
    }
  FILE *patient = fopen("csv/Patient.csv", "r");
    if (patient == NULL)
    {
        printf("Error opening patient file!\n");
        return;
    }

   // generate unique bill ID
    generateID(b->bill_id,'B', 3);
    printf("Assigned Bill ID: %s\n", b->bill_id);

    // Patient ID //confirm the patient ID exist in the patient.csv file
    while(1){
    printf("Enter the Patient ID (format P0001): ");
    scanf("%s", b->patient_id);
    buffer();
    for (int i = 0; b->patient_id[i]; i++) {
        b->patient_id[i] = toupper(b->patient_id[i]);
    }

    if (!validatePrefixedID(b->patient_id, 'P')) {
          printf("Invalid Patient ID format! Must be P followed by 4 digits.\n");
    } else if (!idcheck("csv/Patient.csv", b->patient_id, 0)) {
          printf("Patient ID not found in patient.csv! Please enter a valid ID.\n");
    } else {
          printf("Patient ID found! Proceeding...\n");
          break;
    }
    };


    // admin ID //confirm the admin ID exist in the admin.csv file
    while(1){
    printf("Enter the Admin ID (format A001): ");
    scanf("%s", b->admin_id);
    buffer();
    for (int i = 0; b->admin_id[i]; i++) {
        b->admin_id[i] = toupper(b->admin_id[i]);
    }

      if (!validatePrefixedID(b->admin_id, 'A')) {
          printf("Invalid Admin ID format! Must be A followed by 3 digits.\n");
      } else if (!idcheck("csv/Admin.csv", b->admin_id, 0)) {
          printf("Admin ID not found in admin.csv! Please enter a valid ID.\n");
      } else {
          printf("Admin ID found! Proceeding...\n");
          break;
      }
    };

    calculate_billing(b);
    
    printf("Enter bill date (DD/MM/YYYY): ");
    while (1) {
        scanf("%10s", b->bill_date);

        if (strlen(b->bill_date) != 10 || b->bill_date[2] != '/' || b->bill_date[5] != '/') {
            printf("Invalid format! Please enter in DD/MM/YYYY format: ");
            continue;
        }

        int day   = (b->bill_date[0] - '0') * 10 + (b->bill_date[1] - '0');
        int month = (b->bill_date[3] - '0') * 10 + (b->bill_date[4] - '0');
        int year  = (b->bill_date[6] - '0') * 1000 +
                    (b->bill_date[7] - '0') * 100 +
                    (b->bill_date[8] - '0') * 10 +
                    (b->bill_date[9] - '0');

        if (day < 1 || day > 31 || month < 1 || month > 12) {
            printf("Invalid date! Please enter a valid date in DD/MM/YYYY format: ");
            continue;
        }
        break;
    };
        
    save_bill(b);

}

void bill_management_sys()
{

    int choice;
    int decision;
    do{
          printf("1. View/ Search bill management\n");
          printf("2. Add new bill \n");
          printf("3. Back to admin main menu\n");
          printf(" Enter the option: ");

          scanf("%d", &choice);

       
            if (choice == 1)
            {
              do{
                
                printf("1.View bill\n");
                printf("2.Search bill\n");
                printf("3.Back to invoice management menu\n");
                printf(" Enter the option: ");
                scanf("%d", &decision);
                if(decision == 1)
                {
                  displayinvoice("csv/Bill_Payment.csv"); // point to bill
                }
                else if(decision == 2)
                {
                  char billID[10], date[11];
                  printf("Enter Bill ID (e.g., B001): ");
                  while(1)
                  {
                  scanf("%s", billID);
                  for (int i = 0;billID[i]; i++) 
                  {
                  billID[i] = toupper(billID[i]);
                   }
                  if (!validatePrefixedID(billID, 'B')) {
                      printf("Invalid Patient ID format! Must be P followed by 4 digits.\n");
                      printf("Enter Bill ID (e.g., B001): ");
                  } else if (!idcheck("csv/Bill_Payment.csv", billID, 0)) {
                      printf("Bill ID not found in bill_Payment.csv! Please enter a valid ID.\n");
                      printf("Enter Bill ID (e.g., B001): ");
                  } else {
                      printf("Bill ID found! Proceeding...\n");
                      break;
                  }
                  };
                  printf("Enter add date (DD/MM/YYYY): ");
                    while (1) {
                        scanf("%10s", date);
  
                        if (strlen(date) != 10 || date[2] != '/' || date[5] != '/') {
                            printf("Invalid format! Please enter in DD/MM/YYYY format: ");
                            continue;
                        }
  
                        int day   = (date[0] - '0') * 10 + (date[1] - '0');
                        int month = (date[3] - '0') * 10 + (date[4] - '0');
                        int year  = (date[6] - '0') * 1000 +
                                    (date[7] - '0') * 100 +
                                    (date[8] - '0') * 10 +
                                    (date[9] - '0');
  
                        if (day < 1 || day > 31 || month < 1 || month > 12) {
                            printf("Invalid date! Please enter a valid date in DD/MM/YYYY format: ");
                            continue;
                        }
                        break;
                    } 

                  searchBillByPatientIDAndDate("csv/Bill_Payment.csv", billID, date);
                }
                else if(decision == 3)
                {
                  printf("Back to invoice management menu...\n");
                }
                else
                {
                  printf("Invalid option. please try again!\n");
                }

              }while(decision != 3);
            } 
            else if (choice == 2)
            {
              srand(time(NULL));
              struct bill b;
              CreateBillRecord(&b); 
            } 
            else if (choice == 3)
            {
              printf("Back to admin main menu...\n");
            } 
            else
            {
               printf("Invalid option. please try again!\n");
            }
         }while (choice != 3);
}