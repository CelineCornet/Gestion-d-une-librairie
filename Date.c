#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void PrintDate ()
{
    time_t secondes;
    struct tm timeis;
    time(&secondes);
    timeis = *localtime(&secondes);


    printf("Date : %d/%d/%d",timeis.tm_mday, timeis.tm_mon+1, timeis.tm_year+1900);
}

int compareTheDates (struct tm timeis, struct tm date)
{
    if ( (timeis.tm_year+1900 > date.tm_year) || (timeis.tm_mon+1 > date.tm_mon && timeis.tm_year+1900 == date.tm_year) || (timeis.tm_mday > date.tm_mday && (timeis.tm_year+1900 == date.tm_year && timeis.tm_mon+1 == date.tm_mon)))
        return -1;  //date of today "bigger" than date compared

    else if (timeis.tm_year+1900 == date.tm_year && date.tm_mon == timeis.tm_mon+1 && timeis.tm_mday==date.tm_mday)
        return 0;   //date of today = date compared

    return 1;       //date of today "smaller" than date compared
}

int check_date (char d[10])
{
    char temp[5]="";
    int compare;

    time_t secondes;
    struct tm timeis;
    struct tm date;
    time(&secondes);
    timeis = *localtime(&secondes);

    // we take the date to check
    strncpy(temp, d, 2);
    strcat (temp+2, "\0");
    date.tm_mday = atoi(temp);

    strcpy(temp, "\0");
    strncpy (temp, d+3, 2);
    strcat (temp+2, "\0");
    date.tm_mon = atoi (temp);

    strcpy(temp, "\0");
    strncpy(temp, d+6, 4);
    strcat (temp+4, "\0");
    date.tm_year = atoi(temp);

    // we compare it to today's date
    compare = compareTheDates(timeis, date);


    //we return the result
    if (compare == -1)
        return -1;
    else
        return 1;
}
