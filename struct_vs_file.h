#ifndef STRUCT_VS_FILE_H_INCLUDED
#define STRUCT_VS_FILE_H_INCLUDED

typedef struct members
{
    char first [50]             ;
    char last [50]              ;
    char address [100]          ;
    char mail [50]              ;
    char job [50]               ;
    char returnDate [3][11]     ;
    char borrowedBooks [3][30]  ;
}  Member                       ;

typedef struct books
{
    char title [50]             ;
    char author [50]            ;
    char code[7]                ;
    int totalCopies             ;
    int availableCopies         ;
}  Book                         ;

typedef struct
{
    Member memb                 ;
    Book book[3]                ;
}  Control_date                 ;

void display_database   (Member *m, Book *b,int size_db, int choice);
void update_db_member (FILE *db_memb, Member *m, int nb_member);

#endif // STRUCT_VS_FILE_H_INCLUDED
