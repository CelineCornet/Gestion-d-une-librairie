#ifndef BOOKS_H_INCLUDED
#define BOOKS_H_INCLUDED

void search_book            (FILE *db_book, int criteria);
Book* find_book2            (FILE *db_book, int criteria, char *bookInfo, int *j, Book *found);
Book takeInfo_OneBook       (FILE *db_book, char *info);

void return_date_exceeded   (FILE *db_book, FILE *db_memb);
Book find_book              (char *book, Book *b, int nb_book);
void display_list           (Control_date *list, int j);

void search_borrower        (FILE *db_book, FILE *db_memb, int criteria);
Member takeInfo_OneMember   (FILE *db_memb, char *fname);
void find_member            (FILE *db_memb, char *info, Member *found, int *j);

void available_copies       (FILE *db_book);

void book_most_loans        (FILE *db_book);
void theme_most_loans       (FILE * db_book);

#endif // BOOKS_H_INCLUDED
