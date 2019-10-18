#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct_vs_file.h"

void display_database (Member *m, Book *b,int size_db, int choice)
{
    int i;

    switch (choice)
    {
    case 1:     //display db_member
        for (i=0; i<=size_db; i++)
        {
            printf("%s %s\n", m[i].first, m[i].last);
            printf("%s \n%s \n%s\n", m[i].address, m[i].mail, m[i].job);

            if (strcmp (m[i].borrowedBooks[0], "\0")!= 0)
            {
                printf("%s // %s\n", m[i].borrowedBooks[0], m[i].returnDate[0]);

                if (strcmp (m[i].borrowedBooks[1],"\0")!= 0)
                {
                    printf ("%s // %s\n", m[i].borrowedBooks[1], m[i].returnDate[1]);

                    if (strcmp (m[i].borrowedBooks[2],"\0")!= 0)
                        printf ("%s // %s\n", m[i].borrowedBooks[2], m[i].returnDate[2]);
                }
            }
            if (i<size_db)
                printf ("\n");
        }
        break;

    case 2:     //display db_book
        for (i=0; i<=size_db; i++)
        {
            printf("%s \n%s \n", b[i].title, b[i].author);
            printf("%s \n%d \n%d \n", b[i].code, b[i].totalCopies, b[i].availableCopies);
            printf ("\n");
        }
        break;
    }


}

void update_db_member (FILE *db_memb, Member *m, int nb_member)
{
    fclose(db_memb);
    int i;

    // we open the file in another way to suppress its contents and put into it the updated data base
    db_memb= fopen("DB-members.txt", "w+");

    if (db_memb==NULL)
    {
        printf ("error when trying to open DB-members\n");
        return ;
    }

    for (i=0; i<=nb_member; i++)     //for each structure of the array m[100]
    {
        fprintf (db_memb, "%s\n%s\n%s\n%s\n%s", m[i].first, m[i].last, m[i].address, m[i].mail, m[i].job);
        // If there are books to write, we go on
        if (i < nb_member || strcmp (m[i].borrowedBooks[0], "\0")!= 0)
            fputc('\n', db_memb);

        if (strcmp (m[i].borrowedBooks[0], "\0")!= 0)
        {
            fprintf (db_memb, "%s %s", m[i].borrowedBooks[0], m[i].returnDate[0]);
            if (i<nb_member || strcmp (m[i].borrowedBooks[1],"\0")!= 0)
                fputc('\n', db_memb);

            if (strcmp (m[i].borrowedBooks[1],"\0")!= 0)
            {
                fprintf (db_memb, "%s %s", m[i].borrowedBooks[1], m[i].returnDate[1]);

                if (i<nb_member || strcmp (m[i].borrowedBooks[2],"\0")!= 0)
                    fputc('\n', db_memb);

                if (strcmp (m[i].borrowedBooks[2],"\0")!= 0)
                {
                    fprintf (db_memb, "%s %s", m[i].borrowedBooks[2], m[i].returnDate[2]);

                    if (i<nb_member)
                        fputc('\n', db_memb);
                }

            }
        }
        if (i<nb_member)
            fputc('\n', db_memb);
    }
    fclose (db_memb);
}

