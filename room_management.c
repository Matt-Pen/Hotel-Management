#include <stdio.h>
#include <string.h>
#include "hotel.h"
int main(){
	
	
	return 0;
}

void addRoom(void) {
    struct Room r;
    FILE *fp;

    printf("\n--- Add New Room ---\n");

    r.roomId = getValidInt("Enter Room ID: ");
    getValidString("Enter Room Number: ", r.roomNumber, sizeof(r.roomNumber));
    getValidString("Enter Room Type (Single/Double/Deluxe/Suite): ", r.roomType, sizeof(r.roomType));
    getValidString("Enter Category (AC/Non-AC): ", r.category, sizeof(r.category));
    r.floor = getValidInt("Enter Floor Number: ");
    r.pricePerNight = getValidFloat("Enter Price Per Night: ");
    r.capacity = getValidInt("Enter Capacity: ");
    r.status = 0;  

    fp = openFileSafe(ROOM_FILE, "ab");
    if (fp == NULL) return;

    fwrite(&r, sizeof(struct Room), 1, fp);
    fclose(fp);

    printf("Room added successfully!\n");
}

int loadAllRooms(struct Room rooms[], int maxSize) {
    FILE *fp = openFileSafe(ROOM_FILE, "rb");
    if (fp == NULL) return 0;   

    int count = 0;
    while (count < maxSize && fread(&rooms[count], sizeof(struct Room), 1, fp) == 1) {
        count++;
    }

    fclose(fp);
    return count;   
}

int isRoomAvailable(int roomId) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);

    for (int i = 0; i < count; i++) {
        if (rooms[i].roomId == roomId) {
            return (rooms[i].status == STATUS_AVAILABLE);   
        }
    }

    return 0;  
}

int updateRoomStatus(int roomId, int newStatus) {
    FILE *fp = openFileSafe(ROOM_FILE, "rb+");   
    if (fp == NULL) return 0;

    struct Room r;
    int found = 0;

    while (fread(&r, sizeof(struct Room), 1, fp) == 1) {
        if (r.roomId == roomId) {
            r.status = newStatus;

            fseek(fp, -(long)sizeof(struct Room), SEEK_CUR);  
            fwrite(&r, sizeof(struct Room), 1, fp);           
            found = 1;
            break;
        }
    }

    fclose(fp);
    return found;   
}

void displayRooms(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);

    if (count == 0) {
        printf("\nNo rooms found.\n");
        return;
    }

    printf("\n%-6s %-10s %-10s %-8s %-6s %-10s %-10s %-10s\n",
           "ID", "Number", "Type", "Category", "Floor", "Price", "Capacity", "Status");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        printf("%-6d %-10s %-10s %-8s %-6d %-10.2f %-10d %-10s\n",
               rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType,
               rooms[i].category, rooms[i].floor, rooms[i].pricePerNight,
               rooms[i].capacity, rooms[i].status == STATUS_AVAILABLE ? "Available" : "Occupied");
    }
}

void searchRoomById(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);
    int id = getValidInt("Enter Room ID to search: ");

    for (int i = 0; i < count; i++) {
        if (rooms[i].roomId == id) {
            printf("\nFound: Room %d | %s | %s | %s | Floor %d | Rs.%.2f/night | Capacity %d | %s\n",
                   rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType, rooms[i].category,
                   rooms[i].floor, rooms[i].pricePerNight, rooms[i].capacity,
                   rooms[i].status == STATUS_AVAILABLE ? "Available" : "Occupied");
            return;
        }
    }

    printf("\nNo room found with ID %d.\n", id);
}

void searchRoomByType(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);
    char type[STR_LEN];
    int found = 0;

    getValidString("Enter Room Type to search (Single/Double/Deluxe/Suite): ", type, sizeof(type));

    for (int i = 0; i < count; i++) {
        if (strcmp(rooms[i].roomType, type) == 0) {
            printf("Room %d | %s | Floor %d | Rs.%.2f/night | %s\n",
                   rooms[i].roomId, rooms[i].roomNumber, rooms[i].floor,
                   rooms[i].pricePerNight,
                   rooms[i].status == STATUS_AVAILABLE ? "Available" : "Occupied");
            found = 1;
        }
    }

    if (!found) printf("\nNo rooms found of type '%s'.\n", type);
}

void searchRoomByStatus(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);
    int found = 0;

    printf("0 = Available, 1 = Occupied\n");
    int status = getValidChoice(0, 1);

    for (int i = 0; i < count; i++) {
        if (rooms[i].status == status) {
            printf("Room %d | %s | %s | Floor %d\n",
                   rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType, rooms[i].floor);
            found = 1;
        }
    }

    if (!found) printf("\nNo rooms with that status.\n");
}

void searchRoomMenu(void) {
    int choice;

    printf("\n--- Search Room ---\n");
    printf("1. Search by ID\n");
    printf("2. Search by Type\n");
    printf("3. Search by Status\n");
    printf("4. Back\n");

    choice = getValidChoice(1, 4);

    switch (choice) {
        case 1: searchRoomById(); break;
        case 2: searchRoomByType(); break;
        case 3: searchRoomByStatus(); break;
        case 4: return;
    }
}

void sortRoomsByPrice(struct Room rooms[], int n) {
    // Bubble sort ascending by pricePerNight
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (rooms[j].pricePerNight > rooms[j + 1].pricePerNight) {
                struct Room temp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = temp;
            }
        }
    }
}

void sortRoomsByType(struct Room rooms[], int n) {
    // Bubble sort alphabetically by roomType
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (strcmp(rooms[j].roomType, rooms[j + 1].roomType) > 0) {
                struct Room temp = rooms[j];
                rooms[j] = rooms[j + 1];
                rooms[j + 1] = temp;
            }
        }
    }
}

void sortRoomsMenu(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);

    if (count == 0) {
        printf("\nNo rooms to sort.\n");
        return;
    }

    printf("\n--- Sort Rooms ---\n");
    printf("1. Sort by Price\n");
    printf("2. Sort by Type\n");
    int choice = getValidChoice(1, 2);

    if (choice == 1) {
        sortRoomsByPrice(rooms, count);
    } else {
        sortRoomsByType(rooms, count);
    }

    printf("\n%-6s %-10s %-10s %-10s\n", "ID", "Number", "Type", "Price");
    printf("----------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-6d %-10s %-10s %-10.2f\n",
               rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType, rooms[i].pricePerNight);
    }
}



//Helper functions for easy input validation
int getValidInt(const char *prompt) {
    int value;
    char extra;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d%c", &value, &extra) == 2 && extra == '\n') {
            return value;
        }
        printf("Invalid input. Please enter a whole number.\n");
        clearInputBuffer();
    }
}

float getValidFloat(const char *prompt) {
    float value;
    char extra;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f%c", &value, &extra) == 2 && extra == '\n') {
            return value;
        }
        printf("Invalid input. Please enter a valid number.\n");
        clearInputBuffer();
    }
}

void getValidString(const char *prompt, char *value, int size) {
    while (1) {
        printf("%s", prompt);
        fgets(value, size, stdin);
        value[strcspn(value, "\n")] = '\0';
        if (strlen(value) > 0) {
            return;
        }
        printf("Input cannot be empty. Please try again.\n");
    }
}

int getValidChoice(int min, int max) {
    int choice;
    while (1) {
        choice = getValidInt("Enter your choice: ");
        if (choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
    }
}

void pressAnyKeyToContinue(void) {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

void clearInputBuffer(void) {
    int c;
    c = getchar();  
    while (c != '\n' && c != EOF) {
        c = getchar(); 
    }
}

FILE *openFileSafe(const char *filename, const char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        printf("Error: could not open file '%s'.\n", filename);
    }
    return fp;
}
