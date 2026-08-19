#include <stdio.h>
#include <string.h>
#include "hotel.h"

int   isValidDateFormat(const char *date);
void  getValidDate(const char *prompt, char *buffer, int size);
int   isLeapYear(int year);
int   dateToDayNumber(int day, int month, int year);
int   dateDiffInDays(const char *checkIn, const char *checkOut);
float getRoomPrice(int roomId);
void  saveBooking(struct Booking b);
int   rewriteBookingOnCheckout(struct Booking updated);
void  printBookingRow(struct Booking b);


int loadAllBookings(struct Booking bookings[], int maxSize) {
    FILE *fp = fopen(BOOKING_FILE, "rb");
    if (fp == NULL) {
        return 0;
    }

    int count = 0;
    while (count < maxSize && fread(&bookings[count], sizeof(struct Booking), 1, fp) == 1) {
        count++;
    }

    fclose(fp);
    return count;
}

int getNextBookingId(void) {
    struct Booking bookings[MAX_BOOKINGS];
    int count = loadAllBookings(bookings, MAX_BOOKINGS);

    if (count == 0) {
        return 1;
    }

    int maxId = bookings[0].bookingId;
    for (int i = 1; i < count; i++) {
        if (bookings[i].bookingId > maxId) {
            maxId = bookings[i].bookingId;
        }
    }
    return maxId + 1;
}

float calculateBill(int numDays, float pricePerNight) {
    float total = 0.0f;

    for (int day = 1; day <= numDays; day++) {
        if (day <= FREE_DAYS) {
            total += pricePerNight;
        } else if (day <= FINE_TIER1_LIMIT) {
            total += pricePerNight + FINE_TIER1_RATE;
        } else if (day <= FINE_TIER2_LIMIT) {
            total += pricePerNight + FINE_TIER2_RATE;
        } else {
            total += pricePerNight + FINE_TIER3_RATE;
        }
    }

    return total;
}

void checkInGuest(void) {
    printf("\n--- Check-In Guest ---\n");

    int roomId = getValidInt("Enter Room ID: ");
    if (!isRoomAvailable(roomId)) {
        printf("\nCannot check in: Room %d is occupied or does not exist.\n", roomId);
        return;
    }

    int guestId = getValidInt("Enter Guest ID: ");
    if (!doesGuestExist(guestId)) {
        printf("\nCannot check in: Guest %d does not exist. Add the guest first.\n", guestId);
        return;
    }

    struct Booking b;
    b.bookingId = getNextBookingId();
    b.roomId = roomId;
    b.guestId = guestId;

    getValidDate("Enter Check-In Date (DD-MM-YYYY): ", b.checkInDate, sizeof(b.checkInDate));
    strcpy(b.checkOutDate, "-");
    b.numDays = 0;
    b.totalBill = 0.0f;
    b.status = BOOKING_ACTIVE;

    saveBooking(b);
    updateRoomStatus(roomId, STATUS_OCCUPIED);

    printf("\nCheck-in successful. Booking ID: %d\n", b.bookingId);
}

void checkOutGuest(void) {
    printf("\n--- Check-Out Guest ---\n");

    int roomId = getValidInt("Enter Room ID: ");

    struct Booking bookings[MAX_BOOKINGS];
    int count = loadAllBookings(bookings, MAX_BOOKINGS);

    int foundIndex = -1;
    for (int i = 0; i < count; i++) {
        if (bookings[i].roomId == roomId && bookings[i].status == BOOKING_ACTIVE) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("\nNo active booking found for Room %d.\n", roomId);
        return;
    }

    struct Booking b = bookings[foundIndex];

    getValidDate("Enter Check-Out Date (DD-MM-YYYY): ", b.checkOutDate, sizeof(b.checkOutDate));

    int days = dateDiffInDays(b.checkInDate, b.checkOutDate);
    if (days <= 0) {
        printf("\nCheck-out date must be after check-in date (%s). Using 1 day minimum.\n", b.checkInDate);
        days = 1;
    }
    b.numDays = days;

    float price = getRoomPrice(roomId);
    b.totalBill = calculateBill(b.numDays, price);
    b.status = BOOKING_COMPLETED;

    if (!rewriteBookingOnCheckout(b)) {
        printf("\nError: could not update booking record.\n");
        return;
    }

    updateRoomStatus(roomId, STATUS_AVAILABLE);

    printf("\nCheck-out successful.\n");
    printf("Nights stayed: %d\n", b.numDays);
    printf("Total bill: Rs. %.2f\n", b.totalBill);
}

void displayBookingsMenu(void) {
    printf("\n--- Booking Reports ---\n");
    printf("1. Active Bookings\n");
    printf("2. Bookings by Guest\n");
    printf("3. Bookings by Room\n");
    printf("4. Back\n");

    int choice = getValidChoice(1, 4);
    switch (choice) {
        case 1: displayActiveBookings(); break;
        case 2: displayBookingsByGuest(); break;
        case 3: displayBookingsByRoom(); break;
        case 4: return;
    }
}

void displayActiveBookings(void) {
    struct Booking bookings[MAX_BOOKINGS];
    int count = loadAllBookings(bookings, MAX_BOOKINGS);
    int found = 0;

    printf("\n%-6s %-8s %-8s %-14s %-10s\n", "BkgID", "RoomID", "GstID", "CheckIn", "Status");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (bookings[i].status == BOOKING_ACTIVE) {
            printBookingRow(bookings[i]);
            found = 1;
        }
    }

    if (!found) printf("No active bookings.\n");
}

void displayBookingsByGuest(void) {
    struct Booking bookings[MAX_BOOKINGS];
    int count = loadAllBookings(bookings, MAX_BOOKINGS);
    int guestId = getValidInt("Enter Guest ID: ");
    int found = 0;

    printf("\n%-6s %-8s %-8s %-14s %-10s\n", "BkgID", "RoomID", "GstID", "CheckIn", "Status");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (bookings[i].guestId == guestId) {
            printBookingRow(bookings[i]);
            found = 1;
        }
    }

    if (!found) printf("No bookings found for Guest %d.\n", guestId);
}

void displayBookingsByRoom(void) {
    struct Booking bookings[MAX_BOOKINGS];
    int count = loadAllBookings(bookings, MAX_BOOKINGS);
    int roomId = getValidInt("Enter Room ID: ");
    int found = 0;

    printf("\n%-6s %-8s %-8s %-14s %-10s\n", "BkgID", "RoomID", "GstID", "CheckIn", "Status");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        if (bookings[i].roomId == roomId) {
            printBookingRow(bookings[i]);
            found = 1;
        }
    }

    if (!found) printf("No bookings found for Room %d.\n", roomId);
}

void printBookingRow(struct Booking b) {
    printf("%-6d %-8d %-8d %-14s %-10s\n",
           b.bookingId, b.roomId, b.guestId, b.checkInDate,
           b.status == BOOKING_ACTIVE ? "Active" : "Completed");
}

void saveBooking(struct Booking b) {
    FILE *fp = openFileSafe(BOOKING_FILE, "ab");
    if (fp == NULL) return;
    fwrite(&b, sizeof(struct Booking), 1, fp);
    fclose(fp);
}

int rewriteBookingOnCheckout(struct Booking updated) {
    FILE *fp = openFileSafe(BOOKING_FILE, "rb+");
    if (fp == NULL) return 0;

    struct Booking temp;
    int found = 0;

    while (fread(&temp, sizeof(struct Booking), 1, fp) == 1) {
        if (temp.bookingId == updated.bookingId) {
            fseek(fp, -(int)sizeof(struct Booking), SEEK_CUR);
            fwrite(&updated, sizeof(struct Booking), 1, fp);
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;
}

float getRoomPrice(int roomId) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);

    for (int i = 0; i < count; i++) {
        if (rooms[i].roomId == roomId) {
            return rooms[i].pricePerNight;
        }
    }
    return 0.0f;
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

int isLeapYear(int year) {
    if (year % 4 != 0) return 0;
    if (year % 100 == 0 && year % 400 != 0) return 0;
    return 1;
}

int dateToDayNumber(int day, int month, int year) {
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int totalDays = 0;
    int i;

    for (i = 1; i < year; i++) {
        totalDays += isLeapYear(i) ? 366 : 365;
    }

    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    for (i = 0; i < month - 1; i++) {
        totalDays += daysInMonth[i];
    }

    totalDays += day;

    return totalDays;
}

int dateDiffInDays(const char *checkIn, const char *checkOut) {
    int d1, m1, y1, d2, m2, y2;

    sscanf(checkIn, "%d-%d-%d", &d1, &m1, &y1);
    sscanf(checkOut, "%d-%d-%d", &d2, &m2, &y2);

    int day1 = dateToDayNumber(d1, m1, y1);
    int day2 = dateToDayNumber(d2, m2, y2);

    return day2 - day1;
}