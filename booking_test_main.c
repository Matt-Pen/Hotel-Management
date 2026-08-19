#include <stdio.h>
#include "hotel.h"

/* 
    TEMPORARY MAIN FILE FOR TESTING ONLY.
    DELETE BEFORE FINAL MERGE. 
*/

int main(void) {
    while (1) {
        printf("\n===== BOOKING MODULE TEST =====\n");
        printf("1. Check-In Guest\n");
        printf("2. Check-Out Guest\n");
        printf("3. Booking Reports\n");
        printf("4. Exit\n");

        int choice = getValidChoice(1, 4);
        if (choice == 4) break;

        switch (choice) {
            case 1: checkInGuest(); break;
            case 2: checkOutGuest(); break;
            case 3: displayBookingsMenu(); break;
        }
    }
    return 0;
}