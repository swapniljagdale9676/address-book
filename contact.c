/* 
Description: This file includes the function definition and the logic needed to execute this program 
             Address Book project allows user to store edit, delete and list contacts efficiently.
             All the contacts can be saved to file for permanent storage and can be sorted alphabetically by the name, phone or email.          
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "contact.h"
#include "file.h"


void listContacts(AddressBook *addressBook, int sortCriteria)
{
    Contact temp;          // Temporary variable for swapping contacts
    int strcmpret = 0;    // Stores result of string comparison

    // Check if contact list is empty
    if (addressBook->contactCount == 0)
    {
        printf("File is Empty!\n");
        return;
    }

    // Get valid sorting choice from user
    while (1)
    {
        printf("Sort by:\n");
        printf("1.Name\n2.Phone\n3.Email\n");
        printf("Enter your choice: ");

        if (scanf("%d", &sortCriteria) != 1)
        {
            printf(RED"Error : Enter integers only (1 - 3)!\n"RESET);

            char ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Clear buffer
            continue;
        }

        if (sortCriteria >= 1 && sortCriteria <= 3)
            break;
        else
            printf(RED"Error : Enter only required input!\n"RESET);
    }

    // Bubble sort based on selected field
    for (int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for (int j = 0; j < addressBook->contactCount - i - 1; j++)
        {
            if (sortCriteria == 1) // Sort by name
                strcmpret = strcasecmp(addressBook->contacts[j].name,
                                       addressBook->contacts[j + 1].name);

            else if (sortCriteria == 2) // Sort by phone
                strcmpret = strcasecmp(addressBook->contacts[j].phone,
                                       addressBook->contacts[j + 1].phone);

            else // Sort by email
                strcmpret = strcasecmp(addressBook->contacts[j].email,
                                       addressBook->contacts[j + 1].email);

            // Swap if required
            if (strcmpret > 0)
            {
                temp = addressBook->contacts[j];
                addressBook->contacts[j] = addressBook->contacts[j + 1];
                addressBook->contacts[j + 1] = temp;
            }
        }
    }

    // Display contacts in table format (more space for Email)
    printf(CYAN"\n+----+----------------------+--------------+------------------------------------------+\n"RESET);
    printf(CYAN"| No | Name                 | Phone        | Email                                    |\n"RESET);
    printf(CYAN"+----+----------------------+--------------+------------------------------------------+\n"RESET);

    for (int i = 0; i < addressBook->contactCount; i++)
    {
        printf(CYAN"| %-2d | %-20s | %-12s | %-40s |\n"RESET,
               i + 1,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }

    printf(CYAN"+----+----------------------+--------------+------------------------------------------+\n"RESET);
}

void initialize(AddressBook *addressBook) 
{
    addressBook->contactCount = 0;
    // populateAddressBook(addressBook);   // optional one
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);   // loading from the file
}

void saveAndExit(AddressBook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    Contact new_contact;
    // validating the name of the contact
    reEnterName:
    printf("Enter Name: ");
    scanf(" %[^\n]", new_contact.name); // space will clear the input buffer


    // Check if contacts contains only alphabets and spaces
    for (int i = 0; new_contact.name[i] != '\0'; i++) 
    {
        if (!isalpha(new_contact.name[i]))
        {
            if(new_contact.name[i]== ' ')  
            {
                continue;   // if space will be there it will continue
            }
            printf(RED"Error : Digits and special characters must not present!\n"RESET);
            goto reEnterName;
        }
    }
        // validating the phone number of contact
    reEnterPhone:
        printf("Enter Phone: ");
        scanf(" %[^\n]", new_contact.phone);
        int len = strlen(new_contact.phone);
        // phone number must have exactly 10 digits
        if(len != 10)
        {
            printf(RED"Error: Phone number must be exact 10 digits!\n"RESET);
            goto reEnterPhone;
        }
        // phone number must contain only digits
        for (int i = 0; i < len;i++)
        {
            if(!isdigit(new_contact.phone[i]))  
            {
                printf(RED"Error: Phone number must contain only digits!\n"RESET);
                goto reEnterPhone;
            }
        }
        // Check for the duplicate phone numbers
        for (int i = 0; i < addressBook->contactCount;i++)
        {
            if(strcmp(new_contact.phone, addressBook->contacts[i].phone) == 0)     // comparing if already phone number present
            {
                printf(RED"Error: This number is already present\n"RESET);
                goto reEnterPhone;
            }
        }

        // to validating the email of the contact
    reEnterEmail:
        printf("Enter Email: ");
        scanf(" %[^\n]", new_contact.email);
        
        // Check for the duplicate email address
        for (int i = 0; i < addressBook->contactCount;i++)
        {
            if(strcmp(new_contact.email, addressBook->contacts[i].email) == 0)     // Comparing if already email present
            {
                printf(RED"Error: This Email is already present!\n"RESET);
                goto reEnterEmail;
            }
        }
        int lenofmail = strlen(new_contact.email);
        // Check if email address ends with '@gmail.com'
        char *found;
        found = strstr(new_contact.email, "@gmail.com");
        if(found != NULL)
        {   
        // at least 1 character must exists before '@gmail.com'
        if(lenofmail<=10)
        {
            printf(RED"Error: There must be at least 1 character before @gmail.com!\n"RESET);
            goto reEnterEmail;
        }
            // char *end = found + strlen("@gmail.com");  
            if(*(found + strlen("@gmail.com"))!='\0')
            {
                printf(RED"Error : After @gmail.com there must be nothing!\n"RESET);
                goto reEnterEmail;
            }
        }    
        else
        {
            printf(RED"Error: Email must end with @gmail.com!\n"RESET);
            goto reEnterEmail;
        }
        
        // To validating all characters before '@gmail.com'
        for (int i = 0; i < lenofmail - strlen("@gmail.com");i++)   
        {
            if(!islower(new_contact.email[i]) &&
            !isdigit(new_contact.email[i]) &&
            new_contact.email[i] != '.' &&
            new_contact.email[i] != '_')
            {
                printf(RED"Error: Only lowercase, digits, dots and underscore are allowed before @gmail.com!\n"RESET);
                goto reEnterEmail;
            }
        }
        // to save the contact into the structure
        addressBook->contacts[addressBook->contactCount] = new_contact;
        addressBook->contactCount++;

        printf(GREEN"Contact created successfully!\n"RESET);
}

void searchContact(AddressBook *addressBook, int *count, int *foundIndex)
{
    int n;
    char Search_name[100];
    char Search_phone[20];
    char Search_email[50];
    int flag = 0;

    while (1)
    {
        printf("1.Name\n2.Phone\n3.Email\n");
        printf("Search By (1-3): ");

        if (scanf("%d", &n) != 1)
        {
            printf(RED "Error : Enter only integers (1 - 3)!\n" RESET);

            char ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if (n == 1 || n == 2 || n == 3)
            break;
        else
            printf(RED "Error : Enter required input only!\n" RESET);
    }

    *count = 0;

    switch (n)
    {
        case 1:
            printf("Search by Name: ");
            scanf(" %[^\n]", Search_name);
            break;

        case 2:
            printf("Search by Phone: ");
            scanf(" %[^\n]", Search_phone);
            break;

        case 3:
            printf("Search by Email: ");
            scanf(" %[^\n]", Search_email);
            break;
    }

    /* Search first */
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        if ((n == 1 && strcasestr(addressBook->contacts[i].name, Search_name) != NULL) ||
            (n == 2 && strstr(addressBook->contacts[i].phone, Search_phone) != NULL) ||
            (n == 3 && strstr(addressBook->contacts[i].email, Search_email) != NULL))
        {
            foundIndex[*count] = i;
            (*count)++;
            flag = 1;
        }
    }

    /* If not found, don't print table */
    if (flag == 0)
    {
        printf(RED "Contact not found\n" RESET);
        return;
    }

    /* Print table only when found */
    printf(BLUE "\n================ SEARCHED CONTACTS ================\n" RESET);

    printf(CYAN "+----+----------------------+--------------+------------------------------------------+\n" RESET);
    printf(CYAN "| No | Name                 | Phone        | Email                                    |\n" RESET);
    printf(CYAN "+----+----------------------+--------------+------------------------------------------+\n" RESET);

    for (int i = 0; i < *count; i++)
    {
        int pos = foundIndex[i];

        printf(BLUE "| %-2d | %-20s | %-12s | %-40s |\n" RESET,
               i + 1,
               addressBook->contacts[pos].name,
               addressBook->contacts[pos].phone,
               addressBook->contacts[pos].email);
    }

    printf(CYAN "+----+----------------------+--------------+------------------------------------------+\n" RESET);
}

void editContact(AddressBook *addressBook,int *count,int *foundIndex)
{
    // first we have to search the contacts
    searchContact(addressBook, count, foundIndex);   // function call to search the contact we want to edit

    int index;
    if(*count>1)
    {
        // if multiple contacts present ask which to edit 
        printf("Multiple contacts found.\n");
        reEnterIndex:
        printf("Which contact you want to edit: ");
        scanf("%d", &index);
        // if the index is not within the valid range
        if(index < 1 || index > *count)
        {
            printf(RED"Error: Input index must be within the count!\n"RESET);
            printf(RED"Please, Enter the index again!\n"RESET);
            goto reEnterIndex;
        }
    }
    else
    {
        index = 1; // only 1 contact found
    }
    // to get the actual position of the contact
    int position = foundIndex[index - 1];     // gives the exact index in the array
    int option;
    while(1)
    {
        printf("1. Name\n2. Phone\n3. Email\n");
        // Enter which field you want to edit
        printf("Which field you want to edit: ");
        if(scanf("%d", &option) != 1)
        {
            printf(RED"Error : Enter only integers!\n"RESET);
            char ch;
            while((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }
        if(option == 1 || option == 2 || option == 3)
            break;
        else
            printf(RED"Error : Enter required input only!\n"RESET);
    }

    switch (option)
    {
    // Edit name
    case 1:
       reEditName:
    printf("Enter New Name: ");
    scanf(" %[^\n]", addressBook->contacts[position].name); //  space clears input buffer ('\n')


    // Validation of contact name must contain only alphabets and spaces
    for (int i = 0; addressBook->contacts[position].name[i] != '\0'; i++) 
    {
        if (!isalpha(addressBook->contacts[position].name[i]))
        {
            if(addressBook->contacts[position].name[i]== ' ')  
            {
                continue;   // if space will be there it will continue
            }
            printf(RED"Error: Digits and special characters must not present!\n"RESET);
            goto reEditName;
        }
    }
    break;
    case 2:
        // Edit phone
        reEditPhone:
            printf("Enter New Phone: ");
            scanf(" %[^\n]", addressBook->contacts[position].phone);
            int phonelen = strlen(addressBook->contacts[position].phone);
            // Phone length must be exact 10
            if(phonelen != 10)
            {
                printf(RED"Error: Phone number must be exact 10 digits!\n"RESET);
                goto reEditPhone;
            }
            // phone must contain only edit
            for (int i = 0; i < phonelen;i++)
            {
                if(!isdigit(addressBook->contacts[position].phone[i]))
                {
                    printf(RED"Error: Phone number must contain only digits!\n"RESET);
                    goto reEditPhone;
                }
            }
            // checking for duplicate phone
            for (int i = 0; i < addressBook->contactCount;i++)
            {
                if(i==position)   // skipping the same index which edited
                    continue;
                if(strcmp(addressBook->contacts[position].phone, addressBook->contacts[i].phone) == 0)
                {
                    printf(RED"Error: This number is already present\n"RESET);
                    goto reEditPhone;
                }
            }
            break;
    case 3:
    // Edit Email
        reEditEmail:
            printf("Enter New Email: ");
            scanf(" %[^\n]", addressBook->contacts[position].email);
            int lenofmail = strlen(addressBook->contacts[position].email);   // len of email

            // Check for duplicate Email address
            for (int i = 0; i < addressBook->contactCount;i++)
            {
                if(i == position)   // skipping the index which edited
                continue;
                if(strcmp(addressBook->contacts[position].email, addressBook->contacts[i].email) == 0)
                {
                    printf(RED"Error: This Email is already present!\n"RESET);
                    goto reEditEmail;
                }
            }
            // Check Email Address exactly ends with @gmail.com
            char *found;
            found = strstr(addressBook->contacts[position].email, "@gmail.com");
            if(found != NULL)
            {   
            // at least 1 character must exists before '@gmail.com'
                if(lenofmail<=10)
                {
                    printf(RED"Error: There must be at least 1 character before @gmail.com!\n"RESET);
                    goto reEditEmail;
                }   
                // char *end = found + strlen("@gmail.com");  
                if(*(found + strlen("@gmail.com"))!='\0')
                {
                    printf(RED"Error: After @gmail.com there must be nothing!\n"RESET);
                    goto reEditEmail;
                }
            }    
            else
            {
                printf(RED"Error: Email must end with @gmail.com!\n"RESET);
                goto reEditEmail;
            }
            // To validate all the characters before '@gmail.com'
            for (int i = 0; i < lenofmail - strlen("@gmail.com");i++)
            {
                if(!islower(addressBook->contacts[position].email[i]) &&
                !isdigit(addressBook->contacts[position].email[i]) &&
                addressBook->contacts[position].email[i] != '.' &&
                addressBook->contacts[position].email[i] != '_')
                {
                    printf(RED"Error: Only lowercase, digits, dots and underscore are allowed before @gmail.com!\n"RESET);
                    goto reEditEmail;
                }
            }

        break;
    default:
        printf(RED"Invalid Input!"RESET);
        return;
    }
    printf(GREEN"Contact updated successfully!\n"RESET);
}

void deleteContact(AddressBook * addressBook)
{
    int count = 0;
    int foundIndex[100];
    char choice;
    searchContact(addressBook, &count, foundIndex);    // function call to search the contacts
    int index;
    if(count==0)
    {
        printf(RED"Error : No contact found to delete!\n"RESET);     // if there is no contact in the search list
        return;
    }
    // if the multiple contacts present and ask which to edit
    if(count>1)
    {
        printf("Multiple Contacts found\n"); 
        reEnterIndex:
        printf("Which contact you want to delete : ");
        scanf("%d", &index);   // to read which index we have to delete
        // if the index is not within the valid range
        if(index < 1 || index > count)
        {
            printf(RED"Error: Input index must be within the count!\n"RESET);
            printf(RED"Please! Enter the index again!\n"RESET);
            goto reEnterIndex;
        }
    }
    else
    {
        index = 1;
    }

    // reconfirming the deletion decision
    printf(CYAN"Are you sure you want to delete this contact (y/n) : "RESET);
    scanf(" %c", &choice);
    if(choice != 'y')
    {
        printf(GREEN"Deletion cancelled!\n"RESET);   // if rather than 'y' we enter different input
        return;
    }
    // Performing deletion and shifting remaining contacts
    int position = foundIndex[index - 1];
    for (int i = position; i < addressBook->contactCount - 1;i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];  
    }
    addressBook->contactCount--;  // Decrementing contactCount 

    printf(GREEN"Contact Deleted successfully!\n"RESET);
}
