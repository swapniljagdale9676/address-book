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
    Contact temp;
    if(addressBook->contactCount==0)
    {
        printf("File is Empty!\n");
        return;
    }
    printf("Sort by: ");
    printf("\n1.Name\n2.Phone\n3.Email\n");
    reEnterChoice:
    printf("Enter your choice: ");
    scanf("%d", &sortCriteria);
    if(sortCriteria<1|| sortCriteria>3)
    {
        printf("Invalid Input! Enter the valid input\n");
        goto reEnterChoice;
    }
    int strcmpret = 0;
    // To sort the contacts by user's choice by name, phone or email
    for (int i = 0; i < addressBook->contactCount - 1; i++)   // outer loop runs for number of iterations
    {
        for (int j = 0; j < addressBook->contactCount - i - 1;j++)   // inner loop runs for shifting
        {
            // strcasecmp ignore the case of the alphabet (lowercase or)
            if(sortCriteria == 1)
                strcmpret=strcasecmp(addressBook->contacts[j].name, addressBook->contacts[j + 1].name);
            else if(sortCriteria==2)
                strcmpret=strcasecmp(addressBook->contacts[j].phone, addressBook->contacts[j + 1].phone);
            else if(sortCriteria==3)
                strcmpret=strcasecmp(addressBook->contacts[j].email, addressBook->contacts[j + 1].email);
            // else
            //     printf("Invalid choice!\n");
            if(strcmpret > 0)     // if the 'strcmp' returns positive value then it will enter the block and shift (only as '>0')
            {
            temp = addressBook->contacts[j];
            addressBook->contacts[j] = addressBook->contacts[j+1];
            addressBook->contacts[j + 1] = temp;
            }
        }
    }

    printf("\n================================All Contact List=====================================\n");
    printf("\n=====================================================================================\n");
    printf("| %-3s | %-20s | %-20s | %-30s |\n", "No", "Name", "Phone", "Email");
    printf("======================================================================================\n");
    for (int i = 0; i < addressBook->contactCount;i++)
    {
        printf("| %-3d | %-20s | %-20s | %-30s |\n",
            i+1,
            addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
            addressBook->contacts[i].email);
    }
    printf("======================================================================================\n");
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    // populateAddressBook(addressBook);   // optional one
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);   // loading from the file
}

void saveAndExit(AddressBook *addressBook) {
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
            printf("Error : Digits and special characters must not present!\n");
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
            printf("Error: Phone number must be exact 10 digits!\n");
            goto reEnterPhone;
        }
        // phone number must contain only digits
        for (int i = 0; i < len;i++)
        {
            if(!isdigit(new_contact.phone[i]))  
            {
                printf("Error: Phone number must contain only digits!\n");
                goto reEnterPhone;
            }
        }
        // Check for the duplicate phone numbers
        for (int i = 0; i < addressBook->contactCount;i++)
        {
            if(strcmp(new_contact.phone, addressBook->contacts[i].phone) == 0)     // comparing if already phone number present
            {
                printf("Error: This number is already present\n");
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
                printf("Error: This Email is already present!\n");
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
            printf("Error: There must be at least 1 character before @gmail.com!\n");
            goto reEnterEmail;
        }
            // char *end = found + strlen("@gmail.com");  
            if(*(found + strlen("@gmail.com"))!='\0')
            {
                printf("Error : After @gmail.com there must be nothing!\n");
                goto reEnterEmail;
            }
        }    
        else
        {
            printf("Error: Email must end with @gmail.com!\n");
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
                printf("Error: Only lowercase, digits, dots and underscore are allowed before @gmail.com!\n");
                goto reEnterEmail;
            }
        }
        // to save the contact into the structure
        addressBook->contacts[addressBook->contactCount] = new_contact;
        addressBook->contactCount++;

        printf("Contact created successfully!\n");
}

void searchContact(AddressBook *addressBook, int* count,int* foundIndex) 
{
    printf("1.Name\n2.Phone\n3.Email\n");
    printf("Search By (1-3):\n");
    int n;
    char Search_name[100];
    char Search_phone[20];
    char Search_email[50];
    scanf("%d", &n);
    *count = 0;
    switch(n)
    {
        // Search by name
        case 1:
            printf("Search by Name: ");
            scanf(" %[^\n]", Search_name);
            int flag = 0;
            for (int i = 0; i < addressBook->contactCount;i++)
            {
                if(strcasestr(addressBook->contacts[i].name,Search_name) != NULL)    // it ignores the case of the alphabet ('strcasestr')
                {
                    flag = 1;
                    printf("%d.%s\t\t%s\t\t%s\n",(*count+1),
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);
                    foundIndex[*count] = i;
                    (*count)++;
                }
            }
                if(flag==0)
                    printf("Contact not found\n");
                break;
        // Search by phone
        case 2:
            printf("Search by Phone: ");
            scanf(" %[^\n]", Search_phone);
            flag = 0;
            for (int i = 0; i < addressBook->contactCount;i++)
            {
                if(strstr(addressBook->contacts[i].phone,Search_phone) != NULL)
                {
                    flag = 1;
                    printf("%d.%s\t\t%s\t\t%s\n",(*count+1),
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);
                    foundIndex[*count] = i;
                    (*count)++;
                }
            }
                if(flag==0)
                    printf("Contact not found\n");
                    break;
        // Search by Email
        case 3:
            printf("Search by Email: ");
            scanf(" %[^\n]", Search_email);
            flag = 0;
            for (int i = 0; i < addressBook->contactCount;i++)
            {
                if(strstr(addressBook->contacts[i].email,Search_email) != NULL)
                {
                    flag = 1;
                    printf("%d.%s\t\t%s\t\t%s\n",(*count+1),
                    addressBook->contacts[i].name,
                    addressBook->contacts[i].phone,
                    addressBook->contacts[i].email);
                    foundIndex[*count] = i;
                    (*count)++;
                }
            }
                if(flag==0)
                    printf("Contact not found\n");
                break;
        default:
            printf("Invalid input!\n");
            return;
    }
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
            printf("Error: Input index must be within the count!\n");
            printf("Please! Enter the index again!\n");
            goto reEnterIndex;
        }
    }
    else{
        index = 1; // only 1 contact found
    }
    // to get the actual position of the contact
    int position = foundIndex[index - 1];     // gives the exact index in the array
    int option;
    printf("1. Name\n2. Phone\n3. Email\n");
    // Enter which field you want to edit
    printf("Which field you want to edit: ");
    scanf("%d", &option);

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
            printf("Error: Digits and special characters must not present!\n");
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
                printf("Error: Phone number must be exact 10 digits!\n");
                goto reEditPhone;
            }
            // phone must contain only edit
            for (int i = 0; i < phonelen;i++)
            {
                if(!isdigit(addressBook->contacts[position].phone[i]))
                {
                    printf("Error: Phone number must contain only digits!\n");
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
                    printf("Error: This number is already present\n");
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
                    printf("Error: This Email is already present!\n");
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
                    printf("Error: There must be at least 1 character before @gmail.com!\n");
                    goto reEditEmail;
                }   
                // char *end = found + strlen("@gmail.com");  
                if(*(found + strlen("@gmail.com"))!='\0')
                {
                    printf("Error: After @gmail.com there must be nothing!\n");
                    goto reEditEmail;
                }
            }    
            else
            {
                printf("Error: Email must end with @gmail.com!\n");
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
                    printf("Error: Only lowercase, digits, dots and underscore are allowed before @gmail.com!\n");
                    goto reEditEmail;
                }
            }

        break;
    default:
        printf("Invalid Input!");
        return;
    }
    printf("Contact updated successfully!\n");
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
        printf("No contact found to delete!\n");     // if there is no contact in the search list
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
            printf("Error: Input index must be within the count!\n");
            printf("Please! Enter the index again!\n");
            goto reEnterIndex;
        }
    }
    else
    {
        index = 1;
    }

    // reconfirming the deletion decision
    printf("Are you sure you want to delete this contact (y/n) : ");
    scanf(" %c", &choice);
    if(choice != 'y')
    {
        printf("Deletion cancelled!\n");   // if rather than 'y' we enter different input
        return;
    }
    // Performing deletion and shifting remaining contacts
    int position = foundIndex[index - 1];
    for (int i = position; i < addressBook->contactCount - 1;i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1];  
    }
    addressBook->contactCount--;  // Decrementing contactCount 

    printf("Contact Deleted successfully!\n");
}
