#include <stdio.h>
#include "admin_menu.h"


void admin_menu() 
{
   int options;

    do
    {
     printf("\n=====Admin main menu=====\n");
     printf("1. Acoount management.\n");
     printf("2. Patient complaints.\n");
     printf("3. Bill management.\n");
     printf("4. Back to main menu.\n");
     for (int i = 0; i<26; i++)
     {
        printf("=");
     }
     printf("\n");

     printf(" Enter the option: ");

     scanf("%d", &options);
            switch(options)
            {
                case 1:
                    account_management_sys(); // call function
                    break;
                case 2:
                    patient_complaint_sys();
                    break;
                case 3:
                    bill_management_sys();
                    break;
                case 4:
                    printf("Back to main menu...\n");
                    break;
               default :
               printf("Invalid option. Please try again!\n");
            }
        } while(options != 4);
        return;
     
}
