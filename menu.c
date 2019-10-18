#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <myconio.h>
#include "struct_vs_file.h"
#include "Date.h"
#include "sorting.h"
#include "Books.h"
#include "Members.h"

int TrackingCursor (int numberOfAnswers)
{
    // We picture the following scheme as being our screen :
    POINT pt;
    int xmin, xmax, ymax, ymin, y5min, y5max;                           //        1         3
    // Variables for the placements of zones
    xmin = 600;                                                         //        2         4
    xmax = 680;
    ymin = 320;                                                         //             5
    ymax = 400;
    y5min = 550;
    y5max = 800;

    getch();
    GetCursorPos(&pt);

    switch (numberOfAnswers)
    {
    case 5 :
        if (pt.x < xmax && pt.x > xmin && pt.y < y5max && pt.y > y5min)           //bottom
        {
            return 5;
        }
    case 4 :
        if (pt.x > xmax && pt.y > ymax)                                       //bottom right
        {
            return 4;
        }
    case 3 :
        if (pt.x < xmin && pt.y > ymax)                                       //bottom left
        {
            return 2;
        }
    case 2 :
        if (pt.x < xmin && pt.y <  ymin)                                       //upper left
        {
            return 1;
        }

        if (pt.x > xmax && pt.y < ymin)                                       //upper right
        {
            return 3;
        }
    }
    return -1;
}


void greetings (int step,FILE *db_memb,FILE *db_book)
{
    int trackingC, x13, y12, x24, y34, x5, y5, pox, poy;

    // Variables for the placement of text
    x13 = 33;
    y12 = 12;
    x24 = 122;
    y34 = 30;
    x5 = 70;
    y5 = 38;
    // Variables for the placement of the cursor
    pox = 640;
    poy = 360;


    switch (step)
    {

    case 0:      //First step

        printf("    Please put this window in your big screen then press any key\n\n");
        printf("You will have in this programme to use your cursor to show what you want to do\n");
        printf("When you are ready, press any key on the keyboard to confirm your choice\n\n\n");
        printf("            Enjoy!\n");
        getch();
        clrscr();       //clear the screen

    case 1:     //Greetings

        SetCursorPos(pox,poy);      //we put the cursor in the middle
        printf("\n\t\tHello and Welcome, what do you want to do ? Just move your cursor to any of the cross and press any key to confirm\n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy( x13, y12);
        printf("managing books   x");
        gotoxy( x24, y12);
        printf("x   managing members");
        gotoxy( x13, y34 );
        printf("exit   x");
        trackingC = TrackingCursor(3);      // We check where the cursor is with 3 different options for answers

        if (trackingC == 1)             //books
        {
            clrscr();
            greetings(4, db_memb, db_book);
        }
        else if (trackingC == 3)            //members
        {
            clrscr();
            greetings(2, db_memb, db_book);
        }
        else if (trackingC == 2)            //exit
        {
            clrscr();
            greetings(19, db_memb, db_book);
        }
        else
        {
            clrscr();
            greetings (1, db_memb, db_book);
        }

        break;

    case 2 :            //Dealing with Members part 1/2

        SetCursorPos(pox,poy);
        printf("\n\t\t\t\t\tHandling the members : \n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13-1, y12);
        printf("add a new member   x");
        gotoxy(x24, y12);
        printf("x   delete a member");
        gotoxy(x13, y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("more options -->");

        trackingC = TrackingCursor(4);

        if (trackingC == 1)              // Add new member
        {
            clrscr();
            printf("\t\t\tYou want to add a new member\n\n\n");

            add_new_member (db_memb);
            getch();

            greetings(19, db_memb, db_book);
        }
        else if (trackingC == 3)            // Delete member
        {
            clrscr();
            printf("\t\t\tYou want to delete a member\n\n\n");

            delete_member (db_memb);
            getch();

            greetings(19, db_memb, db_book);
        }
        else if (trackingC == 4)        // More options
        {
            clrscr();
            greetings(3, db_memb, db_book);


        }
        else if (trackingC == 2)        //return
        {
            clrscr();
            greetings(1, db_memb, db_book);
        }
        else
        {
            clrscr ();
            greetings (2, db_memb, db_book);
        }

        break;

    case 3:            //Dealing with Members part 2/2
        SetCursorPos(pox,poy);
        printf("\n\t\t\t\t\tHandling the members : \n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13-3,y12);
        printf("see the list of all members   x");
        gotoxy(x24,y12);
        printf("x   new loan");
        gotoxy(x13+10,y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("x   return a book");

        trackingC = TrackingCursor(3);

        if (trackingC == 1)        // List of the members
        {
            clrscr();
            printf("\t\t\tList of all members ordered alphabetically\n\n\n");

            sorting(db_memb, -1, 2);
            getch();

            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 3)         // New loan
        {
            clrscr();
            printf("\t\t\tAdding a new loan");
            menu_loan(db_memb, db_book, 2);
            getch();
            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 2)        // Return
        {
            clrscr();
            greetings(2, db_memb, db_book);
        }

        else if (trackingC == 3)         // New loan
        {
            clrscr();
            printf("\t\t\tReturn a book\n");
            menu_loan(db_memb, db_book, 1);
            getch();
            greetings(19, db_memb, db_book);
        }

        else
        {
            clrscr();
            greetings(3, db_memb, db_book);
        }
        break;

    case 4:         //dealing with books part 1/2

        SetCursorPos(pox,poy);
        printf("\n\t\t\t\t\tHandling the books : \n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13-7,y12);
        printf("check return date of books   x");
        gotoxy(x24,y12);
        printf("x   search books");
        gotoxy(x13+1,y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("more options -->");
        gotoxy(x5, y5);
        printf("exit");

        trackingC = TrackingCursor(5);

        if (trackingC == 1)         // Return dates
        {
            clrscr();
            printf("\there is the list of members who has not returned a book in time and the information about this book\n\n\n");
            return_date_exceeded (db_book, db_memb);
            getch();
            greetings(19, db_memb, db_book);
        }
        else if (trackingC == 3)      // Search books
        {
            clrscr();
            printf("\t\t\t\tYou want to search a book\n\n\n");
            greetings(6, db_memb, db_book);
        }
        else if (trackingC == 4)        // More options
        {
            clrscr();
            greetings(5, db_memb, db_book);
        }
        else if (trackingC == 2)        //return
        {
            clrscr();
            greetings(1, db_memb, db_book);

        }
        else if (trackingC == 5)        //exit
        {
            clrscr();
            greetings(19, db_memb, db_book);
        }
        else
        {
            clrscr();
            greetings (4, db_memb, db_book);
        }

        break;

    case 5 :            //Dealing with book part 2/2

        SetCursorPos(pox,poy);
        printf("\n\t\t\t\t\tHandling the books : \n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13-4, y12);
        printf("search the borrower of a book  x");
        gotoxy(x24, y12);
        printf("x   display the books");
        gotoxy(x13, y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("number of copies of a book   x");
        gotoxy(x5, y5);
        printf("exit");

        trackingC = TrackingCursor(5);

        if (trackingC == 1)     // Search the borrower of a book
        {
            clrscr();
            search_borrower(db_book, db_memb, 1);
            getch();
            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 3)        // Display list of books
        {
            clrscr();
            greetings(7, db_memb, db_book);
        }
        else if (trackingC == 4)        //number of copies of a book
        {
            clrscr();
            available_copies(db_book);
            getch();
            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 2)        // Return
        {
            clrscr();
            greetings(4, db_memb, db_book);
        }
        else if (trackingC == 5)             // Exit
            greetings (19, db_memb, db_book);
        else
        {
            clrscr();
            greetings(5, db_memb, db_book);
        }
        break ;

    case 6 :        //dealing with books : search

        clrscr();
        SetCursorPos(pox,poy);
        printf("\n\t\t\t\t\tWhat are your criteria to search the book ? \n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13, y12);
        printf("title   x");
        gotoxy(x24, y12);
        printf("x   author");
        gotoxy(x13, y34);
        printf("code    x");
        gotoxy(x24, y34);
        printf("x   exit");

        trackingC = TrackingCursor(4);

        if (trackingC == 1)             //title
        {
            clrscr();
            search_book(db_book, 1);
            getch();
            greetings(19, db_memb, db_book);
        }
        else if (trackingC == 3)            //author
        {
            clrscr();
            search_book(db_book, 2);
            getch();
            greetings(19,db_memb, db_book);
        }
        else if (trackingC == 2)            //code
        {
            clrscr();
            search_book(db_book, 3);
            getch();
            greetings(19, db_memb, db_book);
        }
        else if (trackingC == 4)        //exit
        {
            clrscr();
            greetings(19, db_memb, db_book);
        }
        else
        {
            clrscr();
            greetings (6, db_memb, db_book);
        }

        break;

    case 7 :            //Dealing with books : display  1/3

        SetCursorPos(pox, poy);
        printf("\n\t\t\t\t\tDisplay the books according to what ? 1/3\n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13, y12);
        printf("no order   x");
        gotoxy(x24, y12);
        printf("x   author");
        gotoxy(x13, y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("more options -->");

        trackingC = TrackingCursor(4);

        if (trackingC == 1)             // No order
        {
            clrscr();
            sorting(db_book, 0, 3);
            getch();
            greetings(19, db_memb, db_book);
        }
        if (trackingC == 3)                 // Author
        {
            clrscr();
            sorting(db_book, 1, 1);
            getch();

            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 4)            // More options
        {
            clrscr();
            greetings(8, db_memb, db_book);
        }

        else if (trackingC == 2)                // Return
        {
            clrscr();
            greetings(4, db_memb, db_book);
        }
        else
        {
            clrscr();
            greetings (7, db_memb, db_book);
        }

        break;

    case 8 :          //Dealing with books : display 2/3

        SetCursorPos(pox, poy);
        printf("\n\t\t\t\t\tDisplay the books according to what ? 2/3\n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13, y12);
        printf("code   x");
        gotoxy(x24-5, y12);
        printf("x   all the books according to a theme");
        gotoxy(x13, y34);
        printf("<-- return");
        gotoxy(x24, y34);
        printf("more options -->");

        trackingC = TrackingCursor(4);

        if (trackingC == 1)                // Code
        {
            clrscr();
            sorting(db_book, 3, 1);

            getch();
            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 3)                // Books according to a theme
        {
            clrscr();
            books_acc_theme (db_book);
            getch();
            greetings(19, db_memb, db_book);
        }

        else if (trackingC == 4)             // More options
        {
            clrscr();
            greetings(9,db_memb, db_book);
        }

        else if (trackingC == 2)                // Return
        {
            clrscr();
            greetings(7, db_memb, db_book);
        }

        else
        {
            clrscr();
            greetings (8, db_memb, db_book);
        }

        break;


    case 9 : //Dealing with books : display 3/3

        SetCursorPos(pox, poy);
        printf("\n\t\t\t\t\tDisplay the books according to what ? 3/3\n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13, y12);
        printf("title   x");
        gotoxy(x24-4, y12);
        printf("x   book with the most loans");
        gotoxy(x13, y34);
        printf("<-- return");
        gotoxy(x24-4, y34);
        printf("x   theme with the most loans");
        gotoxy(x5, y5);
        printf("x   exit");

        trackingC = TrackingCursor(5);


        if (trackingC == 1)             // Title
        {
            clrscr();
            sorting(db_book, 2, 1);

            getch();
            greetings(19,db_memb, db_book);
        }

        else if (trackingC == 3)             // Books with the most loans
        {
            clrscr();
            book_most_loans (db_book);
            getch();
            greetings(19,db_memb, db_book);
        }

        else if (trackingC == 2)            // Return
        {
            clrscr();
            greetings(8, db_memb, db_book);
        }

        else if (trackingC == 4)             // Theme with the most loans
        {
            clrscr();
            theme_most_loans (db_book);
            getch();
            greetings(19,db_memb, db_book);
        }

        else if (trackingC == 5)            // Exit
        {
            clrscr();
            greetings(19, db_memb, db_book);
        }

        else
        {
            clrscr();
            greetings(9, db_memb, db_book);
        }

        break;



    case 19 :           // continue or stop the program ?
        clrscr();
        SetCursorPos(pox, poy);
        printf("\t\t\t\tDo you want to go on?\n\n\n");
        gotoxy(x24+10, y12-8);
        PrintDate();
        gotoxy(x13+10, y12);
        printf("Yes   x");
        gotoxy(x24, y12);
        printf("x   No");

        trackingC = TrackingCursor(2);

        if (trackingC == 1)
        {
            clrscr();
            greetings(1, db_memb, db_book);
        }
        else if (trackingC == 3)
        {
            greetings(20, db_memb, db_book);
        }
        else
        {
            clrscr();
            greetings (19, db_memb, db_book);
        }

        break;

    case 20 :           // end of the program
        clrscr();
        printf("\n\n\n\n\n\t\t\t\t\tThank you, come back another time !\n\n\n\n\n\n\n\n\n\n");
        break;
    }
}
