#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"
#include "Date.h"
#include <string.h>

Book takeInfo_OneBook(FILE *db_book, char *title)
{
    char stock[100]="";
    int line = 1;
    Book book;

    strcpy (book.title, title);

    //we read information about books, each book is separated by a line break
    while ((fgets(stock, 100, db_book)!=NULL) && strcmp(stock, "\n")!=0)
    {
        stock[strlen(stock)-1] = '\0';
        switch (line)
        {
        case 1:     //author
            strcpy (book.author, stock);
            break;

        case 2:     //code
            strcpy (book.code, stock);
            break;

        case 3: //total number of copy
            book.totalCopies = atoi(stock);
            break;

        case 4: //number of available copies
            book.availableCopies = atoi(stock);
            break;

        }
        line++;
    }
    return book;
}

void find_book2 (FILE *db_book, int criteria, char *bookInfo, int *j, Book *found)
{
    char stock[100]="";
    Book current;
    int find=0;


    //we place our cursor at the beginning of the file
    fseek(db_book, 0, SEEK_SET);

    //we read information about books, each book is separated by a line break
    while (find == 0 && fgets(stock, 100, db_book)!=NULL)
    {
        stock[strlen(stock)-1] = '\0';      //we suppress the line break

        //we read the information of the current book
        current = takeInfo_OneBook(db_book, stock);

        switch (criteria)   //we compare its information to bookInfo
        {

        case 1:        // title
            if (strcmp(bookInfo, current.title) == 0)
            {
                found[0] = current;
                find = 1;
            }
            break;

        case 2 :        // author
            if (strcmp(bookInfo, current.author) == 0)
            {
                found[*j]=current;
                (*j)++;
            }

            break;

        case 3:         //code
            if (strcmp(bookInfo, current.code) == 0)
            {
                found[0] = current;
                find = 1;
            }
            break;
        }

    }
}

void search_book (FILE *db_book, int criteria)
{
    char element[50]="";
    int i=0, j=0;
    Book bookFound[40];

    strcpy(bookFound[0].title, "\0");

    switch (criteria)
    {

    case 1:     //title
        printf ("Enter the name of the book you are looking for :  ");
        fflush(stdin);
        gets (element);

        find_book2 (db_book, 1, element, &i, bookFound);       //i is useless but we need to put a variable as parameter (variable useful in case 2)
        break;

    case 2:     // author

        printf ("Enter the name of the author you are looking for :  ");
        fflush(stdin);
        gets (element);

        find_book2 (db_book, 2, element, &j, bookFound);       //here j corresponds to the number of book found
        break;

    case 3:        // code

        printf ("Enter the code of the book you are looking for :  ");
        fflush(stdin);
        gets (element);

        find_book2 (db_book, 3, element, &i, bookFound);      //i is useless but we need to put a variable as parameter (variable useful in case 2)
        break;

    }

    // we display the result
    if (strcmp (bookFound[0].title,"\0")==0)
    {
        printf("\nSorry, but your search does not match the database.\n");
        return;
    }

    printf ("\nHere is the book you are looking for :\n\n");
    if (criteria == 1 || criteria == 3)
        display_database(0, &(bookFound[0]), 0, 2);
    else
        display_database(0, bookFound, j-1, 2);
}

Book find_book (char *book, Book *b, int nb_book)
{
    int i=0;

    while (i<=nb_book && strcmp(book, b[i].code) != 0)
    {
        i++;
    }

    return b[i];
}

void display_list (Control_date *list, int j)
{
    int i;

    for (i = 0; i < j; i ++)
    {
        display_database (&(list[i].memb), 0, 0, 1);        //we display the record of a member

        printf ("\nHas not returned :\n");

        // we display the books this member has not return.
        if (strcmp (list[i].book[0].title, "\0")!= 0)
            display_database (0, &(list[i].book[0]), 0, 2);

        if (strcmp (list[i].book[1].title, "\0")!= 0)
            display_database (0, &(list[i].book[1]), 0, 2);

        if (strcmp (list[i].book[2].title, "\0")!= 0)
            display_database (0, &(list[i].book[2]), 0, 2);
    }
}

void return_date_exceeded (FILE *db_book, FILE *db_memb)
{
    int i, j=0;
    int res, check;
    Control_date list[100];

    // we put information of the files in arrays of structures
    int nb_memb, nb_book;
    Member m[100];
    Book b[100];

    nb_memb = takeInfoMembers(db_memb, m);
    nb_book = takeInfoBooks(db_book, b);

    for (i=0; i<=nb_memb; i++)
    {
        check = 0;
        // we take every return date and compare it to today's date
        if (strcmp (m[i].borrowedBooks[0], "\0")!= 0)
        {
            // we check the return date
            res = check_date (m[i].returnDate[0]);

            if (res == -1)  // the return date has been exceeded
            {
                check = 1;
                list[j].memb = m[i];
                list[j].book[0] = find_book (m[i].borrowedBooks[0], b, nb_book);
            }

            if (strcmp (m[i].borrowedBooks[1],"\0")!= 0)
            {
                res = check_date (m[i].returnDate[1]);

                if (res == -1)  // the return date has been exceeded
                {
                    if (check==0)
                    {
                        list[j].memb = m[i];
                        check = 1;
                    }
                    list[j].book[1] = find_book (m[i].borrowedBooks[1], b, nb_book);
                }


                if (strcmp (m[i].borrowedBooks[2],"\0")!= 0)
                {
                    res = check_date (m[i].returnDate[2]);

                    if (res == -1)  // the return date has been exceeded
                    {
                        if (check==0)
                        {
                            list[j].memb = m[i];
                            check = 1;
                        }
                        list[j].book[2] = find_book (m[i].borrowedBooks[2], b, nb_book);
                    }

                }
            }
            if (check == 1)
                j++;
        }
    }

    display_list (list, j);

}

Member takeInfo_OneMember(FILE *db_memb, char *fname)
{
    char stock[100]="";
    int line = 1;
    Member m;
    int cpt;

    strcpy (m.first, fname);

    for (cpt = 0; cpt <3; cpt++)        //initialization
    {
        strcpy (m.borrowedBooks[cpt], "\0");
        strcpy (m.returnDate[cpt], "\0");
    }

    //we read information about a book until we reach an empty line where there only is a line break
    while ((fgets(stock, 100, db_memb)!=NULL) && strcmp(stock, "\n")!=0)
    {
        stock[strlen(stock)-1] = '\0';
        switch (line)
        {
        case 1:     //last name
            strcpy (m.last, stock);
            break;

        case 2:     //address
            strcpy (m.address, stock);
            break;

        case 3: //mail
            strcpy (m.mail, stock);
            break;

        case 4: //job
            strcpy (m.job, stock);
            break;

        case 5: //books
            strncpy(m.borrowedBooks[0], stock, 7);
            strcat (m.borrowedBooks[0], "\0");
            strcpy(m.returnDate[0], stock+8);
            strcat (m.returnDate[0], "\0");
            break;

        case 6: //2nd borrowed book and return date
            strncpy(m.borrowedBooks[1], stock, 7);
            strcat (m.borrowedBooks[1], "\0");
            strcpy(m.returnDate[1], stock+8);
            strcat (m.returnDate[1], "\0");
            break;

        case 7: //3rd borrowed book and return date
            strncpy(m.borrowedBooks[2], stock, 7);
            strcat (m.borrowedBooks[2], "\0");
            strcpy(m.returnDate[2], stock+8);
            strcat (m.returnDate[2], "\0");
            break;
        }
        line++;
    }

    return m;
}

void find_member (FILE *db_memb, char *info, Member *found, int *j)
{
    char stock[100]="";
    Member current;
    int cpt=0;


    //we place our cursor at the beginning of the file
    fseek(db_memb, 0, SEEK_SET);

    //we read information about members, each member is separated by a line break
    while (fgets(stock, 100, db_memb)!=NULL)
    {
        stock[strlen(stock)-1] = '\0';      //we suppress the line break

        //we read the information of the current book
        current = takeInfo_OneMember(db_memb, stock);

        for (cpt=0; cpt<3; cpt++)
        {
            if (strcmp(info, current.borrowedBooks[cpt]) == 0)
            {
                found[*j] = current;
                (*j)++;
            }
        }

    }
}

void search_borrower(FILE *db_book, FILE *db_memb, int criteria )
{
    char searching [50]="";
    Member found[100];
    int i=0, j=0, cpt=0;

    strcpy (found[0].first, "\0");

    printf("What is the code of the book ?  ");
    fflush(stdin);
    gets(searching);

    //we find the member in the file and put its record in "found"
    find_member (db_memb, searching,found, &j);

    if (strcmp (found[0].first,"\0")==0)
        printf ("this book is not borrowed by anyone");
    else
    {
        printf ("this book is borrowed by : \n");

        for (i=0; i<j; i++)
        {
            cpt=0;

            printf ("--> %s %s\n", found[i].first, found[i].last);
            while (strcmp (found[i].borrowedBooks[cpt], searching)!=0)
                cpt++;
            printf ("This member has to return the book before %s\n\n", found[i].returnDate[cpt]);
        }
    }
}

void available_copies (FILE *db_book)
{
    //display the number of available copies and the total number of copies
    int i;
    Book b;
    char element[100] = "";

    strcpy(b.code, "\0");   // enables us to know if we have found a book or not at the end of the function

    printf ("Enter the name of the book you want to see the number of copies : ");
    fflush(stdin);
    gets(element);

    //we try to find the book in the database and put it in "b"
    find_book2 (db_book, 1, element, &i, &b);

    if (strcmp(b.code, "\0")==0)
        printf("No book corresponds to the title you enter\n");
    else
    {
        printf("Total number of copies : %d\n", b.totalCopies);
        printf("Available copies currently : %d\n", b.availableCopies);
    }
}

void book_most_loans (FILE * db_book)
{
    Book book, max;
    char stock[100] = "";
    int line;

    fseek(db_book, 0, SEEK_SET);

    // We initialize the maximum
    if (fgets(stock, 100, db_book) != NULL)
    {
        stock[strlen(stock)-1] = '\0';
        strcpy (max.title, stock);

        line = 2;

        while ((fgets(stock, 100, db_book)!=NULL) && strcmp(stock, "\n") != 0)
        {
            // We store all the other lines into the structure as well
            stock[strlen(stock)-1] = '\0';
            switch (line)
            {
            case 2:     //code
                strcpy (max.author, stock);
                break;
            case 3:     //code
                strcpy (max.code, stock);
                break;

            case 4: //total number of copy
                max.totalCopies = atoi(stock);
                break;

            case 5: //number of available copies
                max.availableCopies = atoi(stock);
                break;

            }
            line++;
        }
    }

    // We look for the other books
    while (fgets(stock, 100, db_book)!=NULL)
    {
        stock[strlen(stock)-1] = '\0';
        strcpy (book.title, stock);

        line = 2;

        while ((fgets(stock, 100, db_book)!=NULL) && strcmp(stock, "\n") != 0)
        {
            // We store all the other lines into the structure as well
            stock[strlen(stock)-1] = '\0';
            switch (line)
            {
            case 2:     //code
                strcpy (book.author, stock);
                break;
            case 3:     //code
                strcpy (book.code, stock);
                break;

            case 4: //total number of copy
                book.totalCopies = atoi(stock);
                break;

            case 5: //number of available copies
                book.availableCopies = atoi(stock);
                break;

            }
            line++;
        }

        // We check the maximum still is the maximum
        if ((max.totalCopies-max.availableCopies) < (book.totalCopies-book.availableCopies))
        {
            max = book;
        }
    }

    printf("The most wanted book is : \n\n");
    printf("%s \n%s \n%s \n%d \n%d \n\n", max.title, max.author, max.code, max.totalCopies, max.availableCopies);
}


void theme_most_loans (FILE * db_book)
{
    Book book [60];
    int i , j , nb_book , max=0 , loans=0;
    char theme[4] = "",  temp_theme[4] = "", max_theme[4] = "";

    //we decompose everything that is in the file to obtain a book of type book structure
    nb_book = takeInfoBooks(db_book, book);

    for (j = 0 ; j < nb_book; j++)
    {
        loans = 0;
        strncpy(theme, book[j].code, 3);
        loans = book[j].totalCopies - book[j].availableCopies;

        for (i = j+1 ; i < nb_book ; i++)
        {
            strncpy(temp_theme, book[i].code, 3);
            if (strcmp(theme, temp_theme) == 0)
            {
                //we add the loans of the other books on the same theme
                loans = (book[i].totalCopies - book[i].availableCopies) + loans;
            }
        }

        //We compare the number we calculated and the maximum value
        if (loans > max)
        {
            max = loans;
            strcpy(max_theme, theme);
        }
    }

    printf("the theme with the most loans is : %s with %d loans total\n\n", max_theme, max);

}
