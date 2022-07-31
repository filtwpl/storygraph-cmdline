/*
 * Details for everything in storygraph.h
 * made by Jacqueline Mei
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storygraph.h"

bin *bin_new(book *title, bin *orig) {
    // assumes orig bin is beginning of bucket
    bin *newbin = (bin *) malloc (sizeof(bin));
    newbin->prev = NULL;
    newbin->next = orig;
    newbin->title = title;
    if (orig)
        orig->prev = newbin;
    return newbin;
}

book *search_bin(char *title, char *author, bin *searchbin) {
    // assumes search bin is the beginning of bucket
    bin *curr = searchbin;
    while (curr->title != NULL) {
        if (strcmp(curr->title->title, title) == 0 && strcmp(curr->title->author, author) == 0) {
            return curr->title;
        }
    }
    return NULL;
}

book **search_bin_byshelf(shelf *shelfname, bin *searchbin) {
    // assumes search bin is the beginning of bucket
    bin *curr = searchbin;
    int shelfcnt = 0;
    while (curr != NULL) {
        if (curr->title->shelf == shelfname) {
            shelfcnt++;
        }
        curr = curr->next;
    }
    book **shelvedbooks = (book **) malloc (sizeof(book *) * shelfcnt);
    curr = searchbin;
    int i=0;
    while (curr != NULL) {
        if (curr->title->shelf == shelfname) {
            shelvedbooks[i] = curr->title;
            i++;
        }
        curr = curr->next;
    }
    return shelvedbooks;
}

unsigned int hash(char *title) {
    char *p;
    unsigned int hashval = 0;
    for (p=title; *p; p++)
        hashval = (*p) + 31 * hashval;
    return hashval;
}

htbl *htbl_new(unsigned int (*hash)(char *), unsigned int n) {
    htbl *table = (htbl *) malloc (sizeof(htbl));
    table->hash = hash;
    table->n_buckets = n;
    bin **buckets = (bin **) malloc (sizeof(bin *) * n);
    for (int i=0; i<n; i++)
        buckets[i] = NULL;
    table->buckets = buckets;
    return table;
}

book *new_book(htbl *tbl, shelf *shelfname, char *title, char *author, char *review, int rating) {
    book *newbook = (book *) malloc (sizeof(book));
    newbook->title = strdup(title);
    newbook->author = strdup(author);
    newbook->review = strdup(review);
    newbook->rating = rating;
    newbook->shelf = shelfname;
    unsigned long int hashval = (tbl->hash)(newbook->title) % tbl->n_buckets;
    tbl->buckets[hashval] = bin_new(newbook, tbl->buckets[hashval]);
    return newbook;
}

shelf *new_shelf(char *shelfname, shelf *curr) {
    shelf *c = curr;
    shelf *end;
    while (c != NULL) {
        end = c;
        c = c->next;
    }
    shelf *newshelf = (shelf *) malloc (sizeof(shelf));
    end->next = newshelf;
    newshelf->prev = end;
    newshelf->next = NULL;
    newshelf->name = strdup(shelfname);
    return newshelf;
}

void shelve_book(book *book, shelf *newshelf) {
    book->shelf = newshelf;
}

void set_rating(book *book, int newrating) {
    book->rating = newrating;
}

void set_review(book *book, char *newreview) {
    book->review = strdup(newreview);
}

book *find_book(char *title, char *author, htbl *tbl) {
    unsigned long int hashval = (tbl->hash)(title) % tbl->n_buckets;
    return search_bin(title, author, tbl->buckets[hashval]);
}

void show_book(book *title) {
    printf("%s by %s\nRating: %d\nReview: %s\n", title->title, title->author, title->rating, title->review);
}

void show_shelf(shelf *shelf, htbl *tbl) {
    printf("%s\n", shelf->name);
    for (int i=0; i<tbl->n_buckets; i++) {
        book **array;
        if ((tbl->buckets)[i] && (array = search_bin_byshelf(shelf, (tbl->buckets)[i])) != NULL) {
            int j=0;
            book *curr;
            while ((curr = array[j]) != NULL) {
                show_book(curr);
                j++;
            }
        }
    }
    printf("End of shelf. Taking you back to the home page!\n\n");
}

void show_shelves(shelf *start) {
    shelf *curr = start;
    int i=1;
    while (curr) {
        printf("%d) %s\n", i, curr->name);
        i++;
        curr = curr->next;
    }
}

shelf *shelf_choice(shelf *start, int num) {
    shelf *curr = start;
    int i=1;
    while (curr) {
        if (i == num)
            return curr;
        curr = curr->next;
        i++;
    }
    return NULL;
}

void free_bin(bin *remove) {
    free(remove->title->title);
    free(remove->title->author);
    free(remove->title->review);
    free(remove->title);
}

void remove_book(book *book, htbl *tbl) {
    unsigned long int hashval = (tbl->hash)(book->title) % tbl->n_buckets;
    bin *curr = tbl->buckets[hashval];
    bin *prev = curr->prev;
    bin *next = curr->next;
    if (prev)
        prev->next = next;
    if (next)
        next->prev = prev;
}
