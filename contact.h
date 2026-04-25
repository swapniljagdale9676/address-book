#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100

typedef struct {
    char name[50];
    char phone[20];
    char email[50];
} Contact;

typedef struct {
    Contact contacts[100];
    int contactCount;
} AddressBook;
// function prototypes
void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook,int* count,int* foundIndex);
void editContact(AddressBook *addressBook,int* count,int* foundIndex);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook, int sortCriteria);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *AddressBook);
void saveAndExit(AddressBook *addressBook);

#endif
