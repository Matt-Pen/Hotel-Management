#include "hotel.h"

void addGuest(void)
{
    struct Guest g;
    FILE *fp;

    printf("\n--- Add New Guest ---\n");

    g.guestId = getValidInt("Enter Guest ID: ");

    if (doesGuestExist(g.guestId))
    {
        printf("Guest ID already exists.\n");
        return;
    }

    getValidString("Enter Name: ", g.name, sizeof(g.name));
    getValidString("Enter Address: ", g.address, sizeof(g.address));
    getValidString("Enter Phone: ", g.phone, sizeof(g.phone));
    getValidString("Enter Email: ", g.email, sizeof(g.email));
    getValidString("Enter ID Proof Number: ", g.idProofNumber, sizeof(g.idProofNumber));

    fp = openFileSafe(GUEST_FILE, "ab");

    if (fp == NULL)
        return;

    fwrite(&g, sizeof(struct Guest), 1, fp);

    fclose(fp);

    printf("Guest added successfully!\n");
}

int loadAllGuests(struct Guest guests[], int maxSize)
{
    FILE *fp;
    int count = 0;

    fp = openFileSafe(GUEST_FILE, "rb");

    if (fp == NULL)
        return 0;

    while (count < maxSize &&
           fread(&guests[count], sizeof(struct Guest), 1, fp) == 1)
    {
        count++;
    }

    fclose(fp);

    return count;
}

void displayGuests(void)
{
    struct Guest guests[100];
    int count;
    int i;

    count = loadAllGuests(guests, 100);

    if (count == 0)
    {
        printf("\nNo guests found.\n");
        return;
    }

    printf("\n==================== GUEST LIST ====================\n");

    printf("%-6s %-20s %-25s %-15s %-25s %-20s\n",
           "ID",
           "Name",
           "Address",
           "Phone",
           "Email",
           "ID Proof");

    printf("--------------------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < count; i++)
    {
        printf("%-6d %-20s %-25s %-15s %-25s %-20s\n",
               guests[i].guestId,
               guests[i].name,
               guests[i].address,
               guests[i].phone,
               guests[i].email,
               guests[i].idProofNumber);
    }

    printf("--------------------------------------------------------------------------------------------------------------\n");
}

int doesGuestExist(int guestId)
{
    struct Guest guests[100];
    int count;
    int i;

    count = loadAllGuests(guests, 100);

    for (i = 0; i < count; i++)
    {
        if (guests[i].guestId == guestId)
        {
            return 1;
        }
    }

    return 0;
}

void searchGuestById(void)
{
    struct Guest guests[100];
    int count;
    int id;
    int i;

    count = loadAllGuests(guests, 100);

    if (count == 0)
    {
        printf("\nNo guests found.\n");
        return;
    }

    id = getValidInt("Enter Guest ID to search: ");

    for (i = 0; i < count; i++)
    {
        if (guests[i].guestId == id)
        {
            printf("\nGuest Found!\n");
            printf("----------------------------------------\n");
            printf("Guest ID       : %d\n", guests[i].guestId);
            printf("Name           : %s\n", guests[i].name);
            printf("Address        : %s\n", guests[i].address);
            printf("Phone          : %s\n", guests[i].phone);
            printf("Email          : %s\n", guests[i].email);
            printf("ID Proof       : %s\n", guests[i].idProofNumber);
            printf("----------------------------------------\n");

            return;
        }
    }

    printf("\nNo guest found with ID %d.\n", id);
}

void searchGuestByName(void)
{
    struct Guest guests[100];
    int count;
    int i;
    int found = 0;
    char searchName[NAME_LEN];

    count = loadAllGuests(guests, 100);

    if (count == 0)
    {
        printf("\nNo guests found.\n");
        return;
    }

    getValidString("Enter Guest Name to search: ",
                   searchName,
                   sizeof(searchName));

    tolowercase(searchName);

    for (i = 0; i < count; i++)
    {
        char tempName[NAME_LEN];

        strcpy(tempName, guests[i].name);
        tolowercase(tempName);

        if (strcmp(tempName, searchName) == 0)
        {
            printf("\nGuest Found!\n");
            printf("----------------------------------------\n");
            printf("Guest ID       : %d\n", guests[i].guestId);
            printf("Name           : %s\n", guests[i].name);
            printf("Address        : %s\n", guests[i].address);
            printf("Phone          : %s\n", guests[i].phone);
            printf("Email          : %s\n", guests[i].email);
            printf("ID Proof       : %s\n", guests[i].idProofNumber);
            printf("----------------------------------------\n");

            found = 1;
        }
    }

    if (!found)
    {
        printf("\nNo guest found with name '%s'.\n", searchName);
    }
}

void searchGuestMenu(void)
{
    int choice;

    printf("\n--- Search Guest ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("3. Back\n");

    choice = getValidChoice(1, 3);

    switch (choice)
    {
        case 1:
            searchGuestById();
            break;

        case 2:
            searchGuestByName();
            break;

        case 3:
            return;
    }
}