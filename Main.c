#include <stdio.h>
#include <ctype.h>
#include "hospital.h"
#include "admin_menu.h"
char current_logged_in_id[20] = "";

int main()
{

    char role; //variable of user input
    int menu_loop; //variable of looping if user input got error
    do
    {
        printf("====MAIN MENU====\n"
                "1. Director\n"
                "2. Administrator\n"
                "3. Doctor\n"   
                "4. Nurse\n"
                "5. Patient\n"
                "6. Exit\n");

        printf("Enter your choice: ");
        scanf(" %c", &role);
        while(getchar() != '\n'); // clear buffer '\n'

        if (isdigit(role))
        {
            switch (role)
            {
            case '1':
                // staff_authentication();
                if(login_page() == 1)
                {
                    director();
                }
                menu_loop = 1;
                break;

            case '2':
                if(login_page() == 1)
                {
                    admin_menu();
                }
                menu_loop = 1;
                break;

            case '3':
                if(login_page() == 1)
                {
                    doctor();
                }
                menu_loop = 1;
                break;

            case '4':
                if(login_page() == 1)
                {
                    nurse();
                }
                menu_loop = 1;
                break;

            case '5':
                if(login_page() == 1)
                {
                    patient_menu();
                }
                menu_loop = 1;
                break;

            case '6':
                printf("^_^ Thank You for visiting Hospital ^_^");
                menu_loop = 0;
                break;
            
            default:
                printf("ERROR: 'INPUT OUT OF RANGE' PLEASE ENTER AGAIN\n");
                break;
            }
        }
        else
        {
            menu_loop = 1;
        }
    } while (menu_loop == 1 || role > '6' || role < '1');
}