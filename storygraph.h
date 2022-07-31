/*
 * Header file for storygraph
 * made by Jacqueline Mei
*/

// books will be stored in a hash table
typedef struct book book;

// a linked list will store the names of all the shelves
typedef struct shelf shelf;

// the aforementioned hash table
typedef struct book_bucket bin;
typedef struct hash_table htbl;

struct book {
    char *title;
    char *author;
    char *review;
    int rating;
    shelf *shelf;
};

struct shelf {
    shelf *prev;
    shelf *next;
    char *name;
};

struct book_bucket {
    bin *prev;
    bin *next;
    book *title;
};

struct hash_table {
    unsigned int (*hash) (char *);
    unsigned int n_buckets;
    bin **buckets; // an array of pointers to bins
};

// add to book bucket
bin *bin_new(book *title, bin *orig);

// search bin for a book with given title and author
book *search_bin(char *title, char *author, bin *searchbin);

// searches through a bucket for books in a given shelf
book **search_bin_byshelf(shelf *shelfname, bin *searchbin);

// the hash function
unsigned int hash(char *title);

// making the hash table
htbl *htbl_new(unsigned int (*hash)(char *), unsigned int n);

// adds a new book to a given shelf
book *new_book(htbl *tbl, shelf *shelfname, char *title, char *author, char *review, int rating);

// adds new shelf to the end of the linked list, returns new shelf
shelf *new_shelf(char *shelfname, shelf *curr);

// changes the shelf a book is on
void shelve_book(book *book, shelf *newshelf);

// changes the rating of a book
void set_rating(book *book, int newrating);

// changes the review of a book
void set_review(book *book, char *newreview);

// find a book by title and author
book *find_book(char *title, char *author, htbl *tbl);

// show a singular book
void show_book(book *title);

// display a shelf
void show_shelf(shelf *shelf, htbl *tbl);

// display names of all shelves
void show_shelves(shelf *start);

// lets you choose a shelf based on its position in the linked list
// to-read is first, and has #1
shelf *shelf_choice(shelf *start, int num);

// free a book bucket from its bucket
void free_bin(bin *remove);

// remove a book from existence
void remove_book(book *book, htbl *tbl);




