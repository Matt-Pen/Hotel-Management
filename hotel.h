#ifndef HOTEL_H
#define HOTEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NAME_LEN   100
#define STR_LEN    50
#define DATE_LEN   15
#define PHONE_LEN  20
#define IDPROOF_LEN 30
#define MAX_ROOMS 100

#define ROOM_FILE    "rooms.dat"
#define GUEST_FILE   "guests.dat"
#define BOOKING_FILE "bookings.dat"

#define ROOM_BACKUP_FILE    "backup_rooms.dat"
#define GUEST_BACKUP_FILE   "backup_guests.dat"
#define BOOKING_BACKUP_FILE "backup_bookings.dat"

#define STATUS_AVAILABLE 0
#define STATUS_OCCUPIED  1

#define BOOKING_ACTIVE    1
#define BOOKING_COMPLETED 0


#define FREE_DAYS        7      
#define FINE_TIER1_LIMIT 14
#define FINE_TIER2_LIMIT 30
#define FINE_TIER1_RATE  2.0
#define FINE_TIER2_RATE  5.0
#define FINE_TIER3_RATE  10.0

/* ---------------- Structures ---------------- */

struct Room {
    int   roomId;
    char  roomNumber[10];      
    char  roomType[STR_LEN];   
    char  category[STR_LEN];   
    int   floor;
    float pricePerNight;
    int   capacity;            
    int   status;             
};

struct Guest {
    int  guestId;
    char name[NAME_LEN];
    char address[NAME_LEN];
    char phone[PHONE_LEN];
    char email[NAME_LEN];
    char idProofNumber[IDPROOF_LEN];  
};

struct Booking {
    int   bookingId;
    int   roomId;
    int   guestId;
    char  checkInDate[DATE_LEN];
    char  checkOutDate[DATE_LEN];      
    int   numDays;                     
    float totalBill;                  
    int   status;                      
};



void addRoom(void);
void displayRooms(void);

void searchRoomMenu(void);
void searchRoomById(void);
void searchRoomByType(void);
void searchRoomByStatus(void);

void sortRoomsMenu(void);
void sortRoomsByPrice(struct Room rooms[], int n);
void sortRoomsByType(struct Room rooms[], int n);

int  loadAllRooms(struct Room rooms[], int maxSize);   
int  isRoomAvailable(int roomId);                     
int  updateRoomStatus(int roomId, int newStatus);       



void addGuest(void);
void displayGuests(void);

void searchGuestMenu(void);
void searchGuestById(void);
void searchGuestByName(void);

int  loadAllGuests(struct Guest guests[], int maxSize);  
int  doesGuestExist(int guestId);                        


void checkInGuest(void);
void checkOutGuest(void);

float calculateBill(int numDays, float pricePerNight);

void displayBookingsMenu(void);
void displayActiveBookings(void);
void displayBookingsByGuest(void);
void displayBookingsByRoom(void);

int  loadAllBookings(struct Booking bookings[], int maxSize);
int  getNextBookingId(void);


void displayMainMenu(void);
void roomManagementMenu(void);
void guestManagementMenu(void);
void bookingManagementMenu(void);
void reportsMenu(void);


int  getValidInt(const char *prompt);
float getValidFloat(const char *prompt);
void getValidString(const char *prompt, char *value, int size);
int  getValidChoice(int min, int max);
void pressAnyKeyToContinue(void);
void clearInputBuffer(void);


FILE *openFileSafe(const char *filename, const char *mode);


#endif
