#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_vs_file.h"

int takeInfoBooks (FILE *db_book, Book *b)
{
    char stock[100]="";
    int line=1, nb_book=0;

    fseek(db_book, 0, SEEK_SET);

    //we read information about books, each book is separated by a line break
    while (fgets(stock, 100, db_book)!=NULL)
    {

        if (strcmp(stock, "\n")==0)
        {
            //there is no more information for the current book
            nb_book++;
            line = 0;
        }

        stock[strlen(stock)-1] = '\0';      //we supress the line break

        //we read information
        switch (line)
        {
        case 1: //title
            strcpy (b[nb_book].title, stock);
            break;

        case 2: //author
            strcpy (b[nb_book].author, stock);
            break;

        case 3: //code
            strcpy (b[nb_book].code, stock);
            break;

        case 4: //total number of copy
            b[nb_book].totalCopies = atoi(stock);
            break;

        case 5: //number of available copies
            b[nb_book].availableCopies = atoi(stock);
            break;

        }
        line++;
    }
    return nb_book;
}

int takeInfoMembers (FILE *db_memb, Member *m)
{
    char stock[100]="";
    int line=1, cpt, nb_member=0;

    fseek(db_memb, 0, SEEK_SET);

    //we read information of people, each person is separated by a line break
    for (cpt = 0; cpt <3; cpt++)
    {
        strcpy (m[0].borrowedBooks[cpt], "\0");
        strcpy (m[0].returnDate[cpt], "\0");
    }

    while (fgets(stock, 100, db_memb)!=NULL)
    {

        if (strcmp(stock, "\n")==0)
        {
            //there is no more information for the current person
            nb_member++;
            line = 0;

            for (cpt = 0; cpt <3; cpt++)
            {
                strcpy (m[nb_member].borrowedBooks[cpt], "\0");
                strcpy (m[nb_member].returnDate[cpt], "\0");
            }
        }

        if (stock[strlen(stock)-1] == '\n')
            stock[strlen(stock)-1] = '\0';

        //we read informations
        switch (line)
        {
        case 1: //first name
            strcpy (m[nb_member].first, stock);
            strcat(m[nb_member].first, "\0");
            break;

        case 2: //last name
            strcpy (m[nb_member].last, stock);
            strcat(m[nb_member].last, "\0");
            break;

        case 3: //address
            strcpy (m[nb_member].address, stock);
            strcat(m[nb_member].address, "\0");
            break;

        case 4: //mail
            strcpy (m[nb_member].mail, stock);
            strcat(m[nb_member].mail, "\0");
            break;

        case 5: //job
            strcpy (m[nb_member].job, stock);
            strcat(m[nb_member].job, "\0");
            break;

        case 6: //1st borrowed book and return date
            strncpy(m[nb_member].borrowedBooks[0], stock, 7);
            strcat (m[nb_member].borrowedBooks[0], "\0");
            strncpy(m[nb_member].returnDate[0], stock+8, 10);
            strcat (m[nb_member].returnDate[0], "\0");
            break;

        case 7: //2nd borrowed book and return date
            strncpy(m[nb_member].borrowedBooks[1], stock, 7);
            strcat (m[nb_member].borrowedBooks[1], "\0");
            strncpy(m[nb_member].returnDate[1], stock+8, 10);
            strcat (m[nb_member].returnDate[1], "\0");
            break;

        case 8: //3rd borrowed book and return date
            strncpy(m[nb_member].borrowedBooks[2], stock, 7);
            strcat (m[nb_member].borrowedBooks[2], "\0");
            strncpy(m[nb_member].returnDate[2], stock+8, 10);
            strcat (m[nb_member].returnDate[2], "\0");
            break;

        }
        line++;
    }
    return nb_member;
}

void sort_book (Book *b,int nb_book, int criteria)
{
    int cpt, pos = 0;
    Book min;
    min = b[0];

    for (cpt=1; cpt<nb_book; cpt++)
    {
        switch (criteria)
        {
            case 1:     //sort according to authors
                if (strcmp (min.author, b[cpt].author)>0)
                {
                    min = b[cpt];
                    pos = cpt;
                }
                break;

            case 2:     //sort according to the title
                if (strcmp (min.title, b[cpt].title)>0)
                {
                    min = b[cpt];
                    pos = cpt;
                }
                break;

            case 3:    //sort according to the code
                if (strcmp (min.code, b[cpt].code)>0)
                {
                    min = b[cpt];
                    pos = cpt;
                }
        }

    }

    //we put the minimum in the first position
    b[pos] = b[0];
    b[0]= min;

    //we recall the function
    if (nb_book>1)
        sort_book(b+1, nb_book-1, criteria);
}

void sort_memb (Member *m,int nb_memb)
{
    int cpt, pos = 0;
    Member min;
    min = m[0];

    for (cpt=1; cpt<nb_memb; cpt++)
    {
        if (strcmp (min.last, m[cpt].last)>0)
        {
            min = m[cpt];
            pos = cpt;
        }

        if (strcmp (min.last, m[cpt].last)==0)
        {
            if (strcmp (min.first, m[cpt].first)>0)
            {
                min = m[cpt];
                pos = cpt;
            }

        }
    }

    //we put the minimum in the first position
    m[pos] = m[0];
    m[0]= min;

    //we recall the function
    if (nb_memb>1)
        sort_memb (m+1, nb_memb-1);
}

void sorting(FILE *db, int criteria, int sort_type)
{
    if (sort_type == 1 || sort_type == 3)     //sort books, type 1 for a particular order, type 3 for no order
    {
        Book b[100];
        int nb_book;

        nb_book = takeInfoBooks (db, b);     // we take info of db_book
        if (sort_type == 1)
            sort_book (b, nb_book, criteria);

        // we display the result
        display_database(0, b, nb_book, 2);
    }
    else                                    //sort member
    {
        Member m[100];
        int nb_memb;

        nb_memb = takeInfoMembers(db, m);
        sort_memb (m, nb_memb);

        //we display the result
        display_database(m, 0, nb_memb, 1);
    }

}

void books_acc_theme (FILE *db_book)
{
    char theme[10]="", theme2[4]="", stock[100]="" ;
    Book book[1];
    int line, k=0;

    printf("What is the name of the theme you are looking for ?  ");
    fflush(stdin);
    scanf("%s", theme);
    printf("theme : %s\n\n",theme);

    fseek(db_book, 0, SEEK_SET);

    // We go through the whole file
    while ((fgets(stock, 100, db_book)!=NULL))
    {
        // We read the first line of a record so we immediately store it in the structure
        stock[strlen(stock)-1] = '\0';
        strcpy (book->title, stock);
        line = 2;

        while ((fgets(stock, 100, db_book)!=NULL) && strcmp(stock, "\n") != 0)
        {
            // We store all the other lines into the structure as well
            stock[strlen(stock)-1] = '\0';
            switch (line)
            {
            case 2:     //code
                strcpy (book->author, stock);
                break;
            case 3:     //code
                strcpy (book->code, stock);
                break;

            case 4: //total number of copy
                book->totalCopies = atoi(stock);
                break;

            case 5: //number of available copies
                book->availableCopies = atoi(stock);
                break;

            }
            line++;
        }

        strncpy(theme2, book->code, 3);


        // If it corresponds to the theme we are looking for, we display it
        if (strcmp(theme, theme2) == 0)
        {
            printf("%s\n%s\n%s\n%d\n%d\n\n",book->title, book->author, book->code, book->totalCopies, book->availableCopies);
            k = 1;
        }
    }

    if (k == 0)
    {
        printf("Your research does not match the database\n");
    }
}
