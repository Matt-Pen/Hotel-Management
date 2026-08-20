#include <stdio.h>
#include "hotel.h"

int main(void)
{
    int choice;
    int running = 1;

    while (running)
    {
        printf("\n\n========== HOTEL MANAGEMENT SYSTEM ==========\n");
        printf("1. Room Management\n");
        printf("2. Guest Management\n");
        printf("3. Booking Management\n");
        printf("4. Exit\n");

        choice = getValidChoice(1, 4);
        switch (choice)
        {
        case 1:
            roomManagementMenu(); 
            break;
        case 2:
            guestManagementMenu(); 
            break;
        case 3:
            displayBookingsMenu(); 
            break;
        case 4:
            printf("Exiting program...\n");
            running = 0;
            break;
        }
    }
    return 0;
}
