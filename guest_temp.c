/*
    TEMPORARY TESTING FILE WITH DUMMY CODE FOR guest_management.c FILE.
    DELETE THIS FILE AFTER ABOVE MENTIONED FILE IS MERGED!!!
*/

#include "hotel.h"

int doesGuestExist(int guestId) {
    return (guestId >= 1 && guestId <= 500);
}

int loadAllGuests(struct Guest guests[], int maxSize) {
    (void)guests;
    (void)maxSize;
    return 0;
}