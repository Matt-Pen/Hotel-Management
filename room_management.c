#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotel.h"


int main(void)
{
    int choice;

    while (1)
    {
        printf("\n\n========== HOTEL MANAGEMENT SYSTEM ==========\n");
        printf("1. Room Management\n");
        printf("2. Guest Management\n");
        printf("3. Exit\n");

        choice = getValidChoice(1, 3);

        switch (choice)
        {
            case 1:
                roomManagementMenu();
                break;

            case 2:
                guestManagementMenu();
                break;

            case 3:
                printf("Exiting program...\n");
                return 0;
        }
    }

    return 0;
}

void roomManagementMenu(void)
{
    int choice;
    int roomId;
    int status;

    while (1)
    {
        printf("\n\n========== ROOM MANAGEMENT ==========\n");
        printf("1. Add Room\n");
        printf("2. Check Availability\n");
        printf("3. Display All Rooms\n");
        printf("4. Search Rooms\n");
        printf("5. Sort Rooms\n");
        printf("6. Update Room Status\n");
        printf("7. Delete Room\n");
        printf("8. Back\n");

        choice = getValidChoice(1, 8);

        switch (choice)
        {
            case 1:
                addRoom();
                break;

            case 2:
                roomId = getValidInt(
                    "Enter room ID to check availability: ");

                if (isRoomAvailable(roomId))
                {
                    printf("Room is available.\n");
                }
                else
                {
                    printf("Room is not available or does not exist.\n");
                }
                break;

            case 3:
                displayRooms();
                break;

            case 4:
                searchRoomMenu();
                break;

            case 5:
                sortRoomsMenu();
                break;

            case 6:
                roomId = getValidInt("Enter room ID: ");

                printf("0 = Available, 1 = Occupied\n");

                status = getValidChoice(0, 1);

                if (updateRoomStatus(roomId, status))
                {
                    printf("Room status updated successfully.\n");
                }
                else
                {
                    printf("Room not found.\n");
                }
                break;

            case 7:
                deleteRoom();
                break;

            case 8:
                return;
        }

        pressAnyKeyToContinue();
    }
}

void guestManagementMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n\n========== GUEST MANAGEMENT ==========\n");
        printf("1. Add Guest\n");
        printf("2. Display Guests\n");
        printf("3. Search Guest\n");
        printf("4. Back\n");

        choice = getValidChoice(1, 4);

        switch (choice)
        {
            case 1:
                addGuest();
                break;

            case 2:
                displayGuests();
                break;

            case 3:
                searchGuestMenu();
                break;

            case 4:
                return;
        }

        pressAnyKeyToContinue();
    }
}


//THESE ARE HELPER FUNCTIONS COMMON TO ALL
//MAYBE WE CAN CREATE A SEPARATE UTILS.C

int getValidInt(const char *prompt)
{
    int value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (scanf("%d%c", &value, &extra) == 2 &&
            extra == '\n')
        {
            return value;
        }

        printf("Invalid input. Please enter a whole number.\n");

        clearInputBuffer();
    }
}

float getValidFloat(const char *prompt)
{
    float value;
    char extra;

    while (1)
    {
        printf("%s", prompt);

        if (scanf("%f%c", &value, &extra) == 2 &&
            extra == '\n')
        {
            return value;
        }

        printf("Invalid input. Please enter a valid number.\n");

        clearInputBuffer();
    }
}

void getValidString(const char *prompt, char *value, int size)
{
    while (1)
    {
        printf("%s", prompt);

        if (fgets(value, size, stdin) == NULL)
        {
            continue;
        }

        value[strcspn(value, "\n")] = '\0';

        if (strlen(value) > 0)
        {
            return;
        }

        printf("Input cannot be empty. Please try again.\n");
    }
}

int getValidChoice(int min, int max)
{
    int choice;

    while (1)
    {
        printf("Choose between %d and %d\n", min, max);

        choice = getValidInt("Enter your choice: ");

        if (choice >= min && choice <= max)
        {
            return choice;
        }

        printf("Invalid choice. Please enter a number between %d and %d.\n",
               min,
               max);
    }
}

void pressAnyKeyToContinue(void)
{
    printf("\nPress Enter to continue...");

    clearInputBuffer();
}

void clearInputBuffer(void)
{
    int c;

    c = getchar();

    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

FILE *openFileSafe(const char *filename, const char *mode)
{
    FILE *fp;

    fp = fopen(filename, mode);

    if (fp == NULL)
    {
        printf("Error: could not open file '%s'.\n",
               filename);
    }

    return fp;
}

void tolowercase(char *str)
{
    int i;

    for (i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}


int isValidDateFormat(const char *date) {
    int d, m, y;
    if (sscanf(date, "%d-%d-%d", &d, &m, &y) != 3) return 0;
    if (d < 1 || d > 31) return 0;
    if (m < 1 || m > 12) return 0;
    if (y < 2000 || y > 2100) return 0;
    return 1;
}

void getValidDate(const char *prompt, char *buffer, int size) {
    while (1) {
        getValidString(prompt, buffer, size);
        if (isValidDateFormat(buffer)) {
            return;
        }
        printf("Invalid date. Use format DD-MM-YYYY (e.g. 18-08-2026).\n");
    }
}
