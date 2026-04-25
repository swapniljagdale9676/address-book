#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contact.txt", "w");    // Opening the file in the write mode
    if(fp == NULL)
    {
        printf("Error: Could not open the file for writing\n");
        return;
    }
    // save total contact first
    fprintf(fp, "%d\n", addressBook->contactCount);
    // save each contact in the new line
    for (int i = 0; i < addressBook->contactCount;i++)
    {
        fprintf(fp, "%s,%s,%s\n",
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }
    fclose(fp);    // close the file
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fp = fopen("contact.txt", "r");     // opening the file in the read mode
    if(fp == NULL)    // if it's empty
    {
        printf("No contacts file found, starting with an empty address book\n");
        addressBook->contactCount = 0;   // make the count of the total contacts as '0'
        return;
    }
    fscanf(fp, "%d\n", &addressBook->contactCount);    // To have the total count of contacts
    for (int i = 0; i < addressBook->contactCount;i++) 
    {
        fscanf(fp, "%[^,],%[^,],%[^\n]\n",
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    fclose(fp);   // Close the file
}
