#ifndef MEMBERS_H_INCLUDED
#define MEMBERS_H_INCLUDED

void    add_new_member          (FILE *db_memb);
void    delete_member           (FILE *db_memb);

int     find_memb               (FILE *db_memb, char *name, char *last, int *b, int *end_memb);
void    menu_loan               (FILE *db_memb, FILE *db_book, int criteria);
int     find_lines_in_files     (FILE *db_memb, char first [30],  char last [30], int b);
void    display_result          (FILE *db_memb, FILE *db_book, int check, char *name, char *last, int b, int *end_memb);
void    suggest_option          (FILE *db_memb, FILE *db_book, int criteria, char *name, char *last, int b, int *end_memb);

int     new_loan                (FILE *db_memb, FILE *db_book,int beg,int *end_memb);
int     check_db_book           (FILE *db_book, char title[40], Book book[1], int *end_book);
int     add_book_to_memb        (FILE *db, int end, Book book[1], int criteria);
void    compute_return_date     (struct tm *timeis);


int     return_loan             (FILE *db_memb, FILE *db_book,int beg,int *end_memb);
int     check_db_memb           (FILE *db_memb, char *code, Member *m, int beg);
void    supress_from_db         (FILE *db, int stop, Member *m, Book *b, int criteria, int loan);


#endif // MEMBERS_H_INCLUDED
