#include <stdio.h>
#include <ctype.h>
#include "hospital.h"

void director()
{
    char director_feature;//variable of user input
    int director_loop; //variable of looping if user input got error

    do
    {
         printf("====WELCOME TO DIRECTOR MENU====\n" //print director menu
            "1. Create Admin account\n"
            "2. Generate staff report\n"
            "3. Generate sales report\n"
            "4. logout\n"
            "Enter your choice: ");

        scanf(" %c", &director_feature); //get user choice
        while (getchar() != '\n'); //clear '\n' in the buffer and use 'EOF' to prevent the file accidently close due to no new line in the buffer

        if (isdigit(director_feature))// digit validation
        {
            switch (director_feature) //match user choice
            {
            case '1':
                admin new_admin;
                create_admin_account(&new_admin); // if admin account create successful, loop to director menu.
                director_loop = 1;
                break;

            case '2':
                select_staff_type();// if admin account create successful, loop to director menu.
                director_loop = 1;
                break;

            case '3':
                financial_sales_generator();
                director_loop = 1;
                break;

            case '4':
                printf("Loging out......\n\n");
                director_loop = 0;
                break;
            
            default:
                printf("Please enter a correct number ^_^\n\n");
                break;
            }
        }
        else
        {
            director_loop = 1; // invalid input, loop program
        }
        
    } while (director_loop == 1 || director_feature > '4' || director_feature < '1');
    return; // when 'while loop' break, back to main menu
}