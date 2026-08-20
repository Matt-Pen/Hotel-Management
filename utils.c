#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "hotel.h"

// Helper functions for easy input validation
int getValidInt(const char *prompt)
{
    int value;
    char extra;
    while (1)
    {
        printf("%s", prompt);
        if (scanf("%d%c", &value, &extra) == 2 && extra == '\n')
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
        if (scanf("%f%c", &value, &extra) == 2 && extra == '\n')
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
        fgets(value, size, stdin);
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
        printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
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
    FILE *fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("Error: could not open file '%s'.\n", filename);
    }
    return fp;
}

void tolowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

int isValidDateFormat(const char *date)
{
    int d, m, y;
    if (sscanf(date, "%d-%d-%d", &d, &m, &y) != 3)
        return 0;
    if (d < 1 || d > 31)
        return 0;
    if (m < 1 || m > 12)
        return 0;
    if (y < 2000 || y > 2100)
        return 0;
    return 1;
}

void getValidDate(const char *prompt, char *buffer, int size)
{
    while (1)
    {
        getValidString(prompt, buffer, size);
        if (isValidDateFormat(buffer))
        {
            return;
        }
        printf("Invalid date. Use format DD-MM-YYYY (e.g. 18-08-2026).\n");
    }
}

int isLeapYear(int year)
{
    if (year % 4 != 0)
        return 0;
    if (year % 100 == 0 && year % 400 != 0)
        return 0;
    return 1;
}

int dateToDayNumber(int day, int month, int year)
{
    int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int totalDays = 0;
    int i;

    for (i = 1; i < year; i++)
    {
        totalDays += isLeapYear(i) ? 366 : 365;
    }

    if (isLeapYear(year))
    {
        daysInMonth[1] = 29;
    }

    for (i = 0; i < month - 1; i++)
    {
        totalDays += daysInMonth[i];
    }

    totalDays += day;

    return totalDays;
}

int dateDiffInDays(const char *checkIn, const char *checkOut)
{
    int d1, m1, y1, d2, m2, y2;

    sscanf(checkIn, "%d-%d-%d", &d1, &m1, &y1);
    sscanf(checkOut, "%d-%d-%d", &d2, &m2, &y2);

    int day1 = dateToDayNumber(d1, m1, y1);
    int day2 = dateToDayNumber(d2, m2, y2);

    return day2 - day1;
}
