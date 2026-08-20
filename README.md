# Hotel Management System

A simple **Hotel Management System written in C**. This is a basic console-based project designed to demonstrate fundamental C programming concepts such as **structures, functions, file handling, searching, sorting, and menu-driven programs**.

## Features

### Room Management

* Add and delete rooms
* Display available rooms
* Search rooms by:

  * Room ID
  * Room type
  * Availability status
* Sort rooms by:

  * Price
  * Room type
* Store room information using file handling

### Guest Management

* Add guests
* Display guest details
* Search guests by:

  * Guest ID
  * Guest name
* Store guest information in a file

### Booking Management

* Check-in guests
* Check-out guests
* Check room availability
* Calculate the total bill based on the number of days and room price
* View active bookings
* View bookings by guest
* View bookings by room

### Data Storage

The program uses binary `.dat` files to store data:

* `rooms.dat` – Room information
* `guests.dat` – Guest information
* `bookings.dat` – Booking information

## Technologies Used

* **Language:** C
* **Storage:** Binary files (`.dat`)
* **Interface:** Command-line / Console

## Project Structure

```text
Hotel-Management/
│
├── main.c
├── hotel.h
├── room_management.c
├── guest.c
├── booking_management.c
├── utils.c
│
├── rooms.dat
├── guests.dat
├── bookings.dat
│
└── hotel.exe
```

The project is divided into separate C files for room management, guest management, booking management, and utility functions.

## Concepts Demonstrated

This project was created to practice basic C programming concepts, including:

* Structures
* Functions
* Header files
* File handling
* Binary file operations
* Arrays
* Searching
* Sorting
* Input validation
* Menu-driven programming
* Basic modular programming

## How to Run

### Using GCC

Compile the source files together:

```bash
gcc main.c room_management.c guest.c booking_management.c utils.c -o hotel
```

Run the program:

```bash
./hotel
```

On Windows:

```bash
hotel.exe
```

A precompiled `hotel.exe` is also included in the repository.

## Main Menu

The program starts with a simple menu:

```text
========== HOTEL MANAGEMENT SYSTEM ==========

1. Room Management
2. Guest Management
3. Booking Management
4. Exit
```

The application then provides separate menus for managing rooms, guests, and bookings.

## Note

This is a **basic academic C project** and is not intended to represent a complete real-world hotel management system. It focuses primarily on demonstrating C programming fundamentals and file-based data storage.
