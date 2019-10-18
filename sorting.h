#ifndef SORTING_H_INCLUDED
#define SORTING_H_INCLUDED
#include "struct_vs_file.h"

void sorting            (FILE *db, int criteria, int sort_type);
void sort_book          (Book *b,int nb_book, int criteria);
void sort_memb          (Member *m,int nb_memb);

void books_acc_theme    (FILE *db_book);

int takeInfoBooks       (FILE *db_book, Book *b);
int takeInfoMembers     (FILE *db_memb, Member *m);

#endif // SORTING_H_INCLUDED
