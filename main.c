#include <stdio.h>
#include <stdlib.h>
#include "menu.h"

int main()
{
    FILE *db_memb = NULL;
    FILE *db_book = NULL;

    //we open files
    db_memb= fopen("DB-members.txt", "r+");

    if (db_memb==NULL)
    {
        printf ("error when trying to open DB-members\n");
        return 0;
    }

    db_book = fopen("DB-books.txt", "r+");
    if (db_book==NULL)
    {
        printf ("error when trying to open DB-books\n");
        return 0;
    }

    greetings (0, db_memb, db_book);

    return 0;
}
