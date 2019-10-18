#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sorting.h"
#include "struct_vs_file.h"
#include "Members.h"
#include "Books.h"

void add_new_member (FILE *db_memb)
{
    char temp [100]="";

    fseek(db_memb, 0, SEEK_END);

    printf("Surname of the new member ?  ");
    gets (temp);
    fprintf (db_memb,"\n\n%s\n", temp);
    strcpy (temp, "\0");

    printf("Last name ?  ");
    gets (temp);
    fprintf (db_memb,"%s\n", temp);
    strcpy (temp, "\0");

    printf("Address ?  ");
    gets (temp);
    fprintf (db_memb,"%s\n", temp);
    strcpy (temp, "\0");

    printf("Mail ?  ");
    gets (temp);
    fprintf (db_memb,"%s\n", temp);
    strcpy (temp, "\0");

    printf("Job ?  ");
    gets (temp);
    fprintf (db_memb,"%s", temp);
    strcpy (temp, "\0");

    printf("the new member has been added to the database\n");
}

void delete_member (FILE *db_memb)
{
    int cpt=1, check=1;
    int beg, end_memb;
    char stock [100]="", name[50]="", last[50]="";
    Member m[1];

    //we find the member in the file saving the line corresponding to the beginning and to the end of its record
    check = find_memb(db_memb, name, last, &beg, &end_memb);

    // if the member exists
    if (check == 1)
    {
        fseek(db_memb, 0, SEEK_SET);

        //we go to the line we are interested in
        while (cpt<=beg)
        {
            fgets(stock, 100, db_memb);
            cpt++;
        }

        //we take the information of this member
        stock[strlen(stock)-1] = '\0';
        m[0] = takeInfo_OneMember(db_memb,stock);

        if (strcmp(m[0].borrowedBooks[0], "\0")!=0)
        {
            printf("The member has some books to return, it cannot be deleted from the database\n");
            return;
        }

        //we suppress its record
        supress_from_db (db_memb, beg, m, 0, 3, 0);
        printf("The member has been deleted\n");
    }

}


int find_memb(FILE *db_memb, char *name, char *last, int *b, int *end_memb)
{
    char stock [100]="";

    printf("\n\nFirst name of the member ?  ");
    fflush(stdin);
    gets(name);
    printf("Last name of the member ? ");
    fflush(stdin);
    gets(last);

    fseek(db_memb, 0, SEEK_SET);

    //we find the line corresponding to the beginning of its file
    *b = find_lines_in_files (db_memb, name, last, 0) ;

    *end_memb = *b + 1 ;        // we add 1 because the last line read in "find_lines_in_files" is the second line of the record

    if (*b == -1)
    {
        printf("There is no user with this name");
        return -1;
    }

    // We find the n° of the line which corresponds to the end of the description of the book
    while ((fgets(stock, 100, db_memb)!=NULL) && strcmp(stock, "\n")!=0)
    {
        (*end_memb)++;
    }
    return 1;
}

void menu_loan (FILE *db_memb, FILE *db_book, int criteria)
{
    int check;
    char name[30]="", last[30]="";
    // b for the first line of the description of a book or member, end_memb for the last one
    int b = 0, end_memb = 0;

    //we find the member in the file saving the line corresponding to the beginning and to the end of its record
    check = find_memb(db_memb, name, last, &b, &end_memb);

    if (check == -1)        //this person is not in the file
        return;

    if (criteria == 1 )      //we make a new loan
        check = new_loan (db_memb, db_book, b, &end_memb);

    else if (criteria == 2)      //we return a book
        check = return_loan (db_memb, db_book, b, &end_memb);

    display_result (db_memb, db_book, check, name, last, b, &(end_memb));

}

void display_result (FILE *db_memb, FILE *db_book, int check, char *name, char *last, int b, int *end_memb)
{
    char stock [100]="";
    int cpt;

    switch (check)
    {

    case 1:         //everything worked fine
        fseek(db_memb, 0,SEEK_SET);
        printf("\nHere is now the new record of %s %s :\n\n", name, last);

        for (cpt = 1; cpt<b; cpt++) //we skip b number of lines
            fgets(stock, 100, db_memb);

        while ((fgets(stock, 100, db_memb)!=NULL) && strcmp(stock, "\n")!=0)
            printf("%s", stock);
        break;

    case -1:
        printf("\nCannot borrow any more books the member has to return one before\n");
        suggest_option (db_memb, db_book,-1, name, last, b, &(*end_memb));
        break;

    case -2:
        printf("\nSorry, but your search does not match the database.\n");
        suggest_option (db_memb, db_book,-2, name, last, b, &(*end_memb));
        break;

    case -3:
        printf("\nNo copies available \n");
        break;

    case -4:
        printf("\nThis member has no loan\n");
        break;

    case -5:
        printf("\nThe book hasn't been borrowed by this member\n");
        suggest_option (db_memb, db_book,-3, name, last, b, &(*end_memb));
        break;
    }
}

int find_lines_in_files (FILE *db, char info_1 [30],  char info_2 [30], int b)
{
    char stock[100]="";
    int check = 0;

    //while the info we have don't correspond to a line in the database, we continue to search until we reach the end of the file
    while ((fgets(stock, 100, db)!=NULL) && check == 0)
    {
        stock [strlen(stock)-1] = '\0';
        b++;
        if (strcmp(stock, info_1)==0)
        {
            check = 1;
        }
    }

    //we check the second info
    stock[strlen(stock)-1] = '\0';
    if (strcmp(stock, info_2) == 0)
    {
        return b;                       //if it matches, we have found what we were looking for
    }

    else if (fgets(stock, 100, db) == NULL)         //it is the end of the file, no match with the database
        return -1;

    else b +=2;

    find_lines_in_files(db, info_1, info_2, b);         // the second information doesn't correspond so we restart beginning from the position b+2
    return 0;
}


int new_loan (FILE *db_memb, FILE *db_book,int beg,int *end_memb)
{
    Book book [1];
    int end_book = 0, copies;
    char title [40]="";

    if (*end_memb - beg == 7)
    {
        //3 books already borrowed
        return -1;
    }

    printf("What is the title of the book?  ");
    fflush(stdin);
    gets (title);

    // we check if the book is available
    copies = check_db_book (db_book, title, book, &end_book);

    if (copies == -1)   //the book is not in the database
        return -2;

    else if (copies == 0)   //no copy available
        return -3;

    // else we update the database
    add_book_to_memb (db_memb, *end_memb, book, 1);
    add_book_to_memb (db_book, end_book-1, book, 2);
    (*end_memb)++;

    return 1;
}

int check_db_book (FILE *db_book, char title[40], Book book [1], int *end_book)
{
    int b;
    char stock[100]="";

    // we find the book in the database and take its characteristics
    strcpy (book[0].title, "\0");
    find_book2(db_book, 1, title, 0, book);

    if (strcmp (book[0].title,"\0")==0)         //no books found
    {
        return -1;
    }

    //We find the lines corresponding to this book in the database to use them in the function "add_book_to_memb"
    fseek(db_book, 0, SEEK_SET);

    b = find_lines_in_files (db_book, book[0].title, book[0].author, 0);
    *end_book = b+1;

    //we find the last line of this book description
    while ((fgets(stock, 100, db_book)!=NULL) && strcmp(stock, "\n")!=0)
    {
        (*end_book)++;
    }
    return book[0].availableCopies;
}

int add_book_to_memb (FILE *db, int end, Book book[1], int criteria)
{
    FILE *temp = NULL;
    struct tm timeis;
    int cpt=0, copie;
    char stock[100]="";

    temp= fopen("temp.txt", "w+");

    if (temp==NULL)
    {
        printf ("error when trying to open temp\n");
        return 0;
    }

    fseek(db, 0, SEEK_SET);

    //we copy in a temporary file the data until we reach the line were we have to add a loan
    while ( cpt < end)
    {
        fgets(stock, 100, db);
        fprintf(temp, "%s", stock);
        cpt++;
    }

    if (criteria==1)        //dealing with db_memb
    {
        //we add the new loan
        compute_return_date (&timeis);
        fprintf (temp, "%s ", book[0].code);

        if (timeis.tm_mday <10)         //we add 0 if needed
            fprintf (temp, "0%d.", timeis.tm_mday);
        else
            fprintf (temp, "%d.", timeis.tm_mday);

        if (timeis.tm_mon+1 <10)         //we add 0 if needed
            fprintf (temp, "0%d.", timeis.tm_mon+1);
        else
            fprintf (temp, "%d.", timeis.tm_mon+1);

        fprintf (temp, "%d\n", timeis.tm_year+1900);
    }
    else                    //dealing with db_books
    {
        fgets(stock, 100, db);
        copie = atoi(stock)-1;
        fprintf(temp, "%d\n", copie);
    }

    //we finih to copy the data in the temporary file
    while ( fgets(stock, 100, db) != NULL)
    {
        fprintf(temp, "%s", stock);
    }

    fclose(db);

    //we copy the temporary file in the database

    if (criteria==1)                            //dealing with db_memb
        db = fopen("DB-members.txt", "w+");
    else                                        //dealing with db_book
        db = fopen("DB-books.txt", "w+");

    fseek(db, 0, SEEK_SET);
    fseek(temp, 0, SEEK_SET);

    while ( fgets(stock, 100, temp) != NULL)
    {
        fprintf(db, "%s", stock);
    }

    fclose(temp);
    remove("temp.txt");
    return 1;
}

void compute_return_date (struct tm *timeis)
{
    time_t secondes;
    time(&secondes);
    *timeis = *localtime(&secondes);

    //we compute the date in 15 days
    timeis->tm_mday += 15;
    mktime(timeis);
}


int return_loan (FILE *db_memb, FILE *db_book,int beg,int *end_memb)
{
    char code [10]="";
    int loan;
    Member m[1];
    Book b[1];


    if (*end_memb - beg == 4)    // we check the member has borrowed a book
        return -4;

    printf ("What is the code of the book you want to return ?    ");
    fflush(stdin);
    gets (code);

    // we check that the member has borrowed a book
    loan = check_db_memb (db_memb, code, m, beg);

    if (loan == -1)     // the book hasn't been borrowed by this member
        return -5;

    //we look for the book thanks to the code and copy it in the structure b
    find_book2(db_book, 3, code, 0, b);

    //we change the number of available copies
    (b->availableCopies)++;

    //we update the databases
    supress_from_db (db_memb, beg, m, 0, 1, loan);
    supress_from_db (db_book, 0, 0, b, 2, loan);
    (*end_memb)--;

    return 1;
}

int check_db_memb (FILE *db_memb, char *code, Member *m, int beg)
{
    int cpt=1;
    char stock [100]="";
    int check=0;

    fseek(db_memb, 0, SEEK_SET);

    //we go to the line we are interested in
    while (cpt<=beg)
    {
        fgets(stock, 100, db_memb);
        cpt++;
    }

    //we take the information of this member
    stock[strlen(stock)-1] = '\0';
    m[0] = takeInfo_OneMember(db_memb,stock);

    //we check he borrowed the book he wants to return
    if (strcmp (code, m->borrowedBooks[0])!=0)
    {
        check=1;
        if (strcmp (code, m->borrowedBooks[1])!=0)
        {
            check=2;
            if (strcmp (code, m->borrowedBooks[2])!=0)
                return -1;
        }
    }
    return check;

}

void supress_from_db (FILE *db, int stop, Member *m, Book *b, int criteria, int loan)
{

    FILE *temp = NULL;
    int cpt=1;
    char *check=" ";
    char stock[100]="";

    temp= fopen("temp.txt", "w+");

    if (temp==NULL)
    {
        printf ("error when trying to open temp\n");
        return;
    }

    fseek(db, 0, SEEK_SET);



    if (criteria==1 || criteria==3)        //dealing with db_memb
    {
        //we copy in a temporary file the data until we reach the line
        //were we have to change something
        while (cpt < stop)
        {
            fgets(stock, 100, db);
            fprintf(temp, "%s", stock);
            cpt++;
        }

        if (criteria==1)
        {
            //we suppress the record of the member but the info of the book he or she just returned
            fprintf(temp, "%s\n%s\n%s\n%s\n%s", m->first, m->last, m->address, m->mail, m->job);

            if (strcmp(m->borrowedBooks[0], "\0")!=0)
            {
                if (0 != loan)
                    fprintf(temp, "\n%s %s", m->borrowedBooks[0], m->returnDate[0]);

                if (strcmp(m->borrowedBooks[1], "\0")!=0)
                {
                    if (1 != loan)
                        fprintf(temp, "\n%s %s", m->borrowedBooks[1], m->returnDate[1]);

                    if (strcmp(m->borrowedBooks[2], "\0")!=0)
                    {
                        if (2 != loan)
                            fprintf(temp, "\n%s %s", m->borrowedBooks[2], m->returnDate[2]);
                    }
                }
            }

        }
    }
    else                    //dealing with db_books
    {
        //we copy in a temporary file the data until we reach the line
        //were we have to change something
        fgets(stock, 100, db);
        stock[strlen(stock)-1] = '\0';

        //we change the number of available copies
        while (strcmp(stock, b->title)!=0 )
        {
            fprintf(temp, "%s\n", stock);
            fgets(stock, 100, db);
            stock[strlen(stock)-1] = '\0';
        }
        fprintf(temp, "%s\n%s\n%s\n%d\n%d", b->title, b->author, b->code, b-> totalCopies, (b->availableCopies));
    }

    //we go to the next member if any
    check = fgets(stock, 100, db);

    while (check!=NULL && strcmp (stock, "\n")!=0)
        check = fgets(stock, 100, db);

    if (check!=NULL)
    {
        if (criteria!=3)                        // to delete a member
            fprintf(temp, "\n%s", stock);

        //we finish to copy the data in the temporary file
        while ( fgets(stock, 100, db) != NULL)
        {
            fprintf(temp, "%s", stock);
        }
    }
    fclose(db);

    //we copy the temporary file in the database
    if (criteria==1 || criteria==3)             //dealing with db_memb
        db = fopen("DB-members.txt", "w+");
    else                                        //dealing with db_book
        db = fopen("DB-books.txt", "w+");

    fseek(db, 0, SEEK_SET);
    fseek(temp, 0, SEEK_SET);

    while (fgets(stock, 100, temp) != NULL)
    {
        fprintf(db, "%s", stock);
    }
    fclose(temp);
    remove("temp.txt");

}


void suggest_option (FILE *db_memb, FILE *db_book, int criteria, char *name, char *last, int b, int *end_memb)
{
    char choice;
    int check;


    switch (criteria)
    {

    case -1 :   //too many books already borrowed
        printf("Do you want to return a book to borrow the new one?\nEnter Y for yes and N for no:   ");
        do
        {
            fflush(stdin);
        }
        while (scanf("%c", &choice)!=1);

        if (choice == 'Y' || choice == 'y')
        {
            system("@cls||clear");
            check = return_loan (db_memb, db_book, b, &(*end_memb));
            printf ("\n");
            display_result (db_memb, db_book, check, name, last, b, &(*end_memb));

            printf ("\n");
            check = new_loan (db_memb, db_book, b, &(*end_memb));
            printf ("\n");
            display_result (db_memb, db_book, check, name, last, b, &(*end_memb));
        }

        break;

    case -2 :  //mistake in the title ?
        printf("Maybe there is a mistake in the title of the book, enter a new title ?\nEnter Y for yes and N for no:   ");
        do
        {
            fflush(stdin);
        }
        while (scanf("%c", &choice)!=1);

        if (choice == 'Y' || choice == 'y')
        {
            system("@cls||clear");
            check = new_loan (db_memb, db_book, b, &(*end_memb));
            printf ("\n");
            display_result (db_memb, db_book, check, name, last, b, &(*end_memb));
        }
        break;

    case -3 :   //mistake in the code ?
        printf("Maybe there is a mistake in the code entered, Let's start again ?\nEnter Y for yes and N for no:   ");
        do
        {
            fflush(stdin);
        }
        while (scanf("%c", &choice)!=1);

        if (choice == 'Y' || choice == 'y')
        {
            system("@cls||clear");
            check = return_loan (db_memb, db_book, b, &(*end_memb));
            printf ("\n");
            display_result (db_memb, db_book, check, name, last, b, &(*end_memb));
        }

        break;
    }
}
