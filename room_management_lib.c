/*
    TEMPORARY COPY OF room_management.c FILE FOR TESTING.
    DELETE BEFORE MERGING.
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hotel.h"


// int main(){
// 	while(1){
// 		printf("\n\n================ROOM MANAGEMENT SYSTEM================\n");
// 		printf("\n1.Add Rooms");
// 		printf("\n2.Check availability");
// 		printf("\n3.Display all rooms");
// 		printf("\n4.Search Rooms");
// 		printf("\n5.Sort Rooms");
// 		printf("\n6.Update Room Status");
// 		printf("\n7.Delete Room");
// 		printf("\n8.Exit\n\n");
// 		int ch=getValidChoice(1,8);
// 		if(ch==8){
// 			break;
// 		}
// 		switch(ch){
// 			case 1:{
// 				addRoom();
// 				break;
// 			}
// 			case 2:{
// 				int id;
// 				id=getValidInt("\nEnter room id to check for availability: ");
// 				if(isRoomAvailable(id)){
// 					printf("\nRoom %d is Available\n",id);
// 					searchRoomById(id);
// 				}
// 				else {
//        				 printf("\nRoom %d is Occupied or does not exist.\n", id);
//     			}
// 				break;
// 			}
// 			case 3:{
// 				displayRooms();
// 				break;
// 			}
// 			case 4:{
// 				searchRoomMenu();
// 				break;
// 			}
// 			case 5:{
// 				sortRoomsMenu();
// 				break;
// 			}
// 			case 6:{
// 				int rid=getValidInt("\nEnter room id: ");
// 				int status=getValidInt("\nEnter status (0-Available / 1-Occupied)");
// 				updateRoomStatus(rid,status);
// 				break;
// 			}
// 			case 7:{
// 				deleteRoom();
// 				break;
// 			}
// 		}
// 	}
	
// 	return 0;
// }

void addRoom(void) {
    struct Room r;
    FILE *fp;

    printf("\n--- Add New Room ---\n");

    r.roomId = getValidInt("Enter Room ID: ");
    getValidString("Enter Room Number: ", r.roomNumber, sizeof(r.roomNumber));
	while(1){
		char type[STR_LEN];
		printf("\nChoose Room Type (Single/Double/Deluxe/Suite)\n");
		getValidString("Enter choice: ",type,sizeof(type));
		tolowercase(type);
		if(strcmp(type,"single")==0 || strcmp(type,"double")==0 || strcmp(type,"deluxe")==0 || strcmp(type,"suite")==0){
			strcpy(r.roomType, type);
			break;
		}else{
			printf("\nEnter a Valid choice.\n");
		}
		
	}

	while(1){
		char type[STR_LEN];
		printf("\nChoose Room Category (AC/Non-AC):\n");
		getValidString("Enter choice: ",type,sizeof(type));
		tolowercase(type);
		if(strcmp(type,"ac")==0 || strcmp(type,"non-ac")==0){
			strcpy(r.category, type);
			break;
		}else{
			printf("\nEnter a Valid choice.\n");
		}
	}
		
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
void deleteRoom(void) {
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);

    if (count == 0) {
        printf("\nNo rooms to delete.\n");
        return;
    }

    int id = getValidInt("Enter Room ID to delete: ");
    int found = 0;

    FILE *fp = openFileSafe(ROOM_FILE, "wb");   
    if (fp == NULL) return;

    for (int i = 0; i < count; i++) {
        if (rooms[i].roomId == id) {
            found = 1;
            continue;  
        }
        fwrite(&rooms[i], sizeof(struct Room), 1, fp);
    }

    fclose(fp);

    if (found) {
        printf("\nRoom %d deleted successfully.\n", id);
    } else {
        printf("\nNo room found with ID %d.\n", id);
    }
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

void searchRoomById(int n) {
	int id;
    struct Room rooms[MAX_ROOMS];
    int count = loadAllRooms(rooms, MAX_ROOMS);
    if(n==0){
		id = getValidInt("Enter Room ID to search: ");		
	}
	else{
		id=n;
	}
    for (int i = 0; i < count; i++) {
        if (rooms[i].roomId == id) {
        	printf("\n%-6s %-10s %-10s %-8s %-6s %-10s %-10s %-10s\n",
           "ID", "Number", "Type", "Category", "Floor", "Price", "Capacity", "Status");
    printf("--------------------------------------------------------------------------\n");
            printf("%-6d %-10s %-10s %-8s %-6d %-10.2f %-10d %-10s\n",
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
           printf("\n%-6s %-10s %-10s %-8s %-6s %-10s %-10s %-10s\n",
           "ID", "Number", "Type", "Category", "Floor", "Price", "Capacity", "Status");
    printf("--------------------------------------------------------------------------\n");
            printf("%-6d %-10s %-10s %-8s %-6d %-10.2f %-10d %-10s\n",
                   rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType, rooms[i].category,
                   rooms[i].floor, rooms[i].pricePerNight, rooms[i].capacity,
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
          printf("%-6s %-10s %-10s %-8s %-6s %-10s %-10s %-10s\n",
           "ID", "Number", "Type", "Category", "Floor", "Price", "Capacity", "Status");
    printf("--------------------------------------------------------------------------\n");
            printf("\n%-6d %-10s %-10s %-8s %-6d %-10.2f %-10d %-10s\n",
                   rooms[i].roomId, rooms[i].roomNumber, rooms[i].roomType, rooms[i].category,
                   rooms[i].floor, rooms[i].pricePerNight, rooms[i].capacity,
                   rooms[i].status == STATUS_AVAILABLE ? "Available" : "Occupied");
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
        case 1: searchRoomById(0); break;
        case 2: searchRoomByType(); break;
        case 3: searchRoomByStatus(); break;
        case 4: return;
    }
}

void sortRoomsByPrice(struct Room rooms[], int n) {
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
    	printf("Choose between %d and %d\n",min,max);
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

void tolowercase(char *str){
	for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}
