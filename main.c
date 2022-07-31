/*
 * A program that tracks the books you read, like Goodreads or Storygraph, but no nice GUI yet.
 * Just on the command line.
 *
 * Features include:
 * Default shelves: read, to-read, and currently reading
 * A home screen displaying currently reading
 * Adding books and sorting by author, title, etc.
 * Ratings and reviews
 *
 * Made for fun by Jacqueline Mei
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storygraph.h"

#define BUFSIZE 514

void remove_newline(char *str) {
    int len = strlen(str);
    str[len-1] = '\0';
}

void add_new(htbl *table, shelf *shelves) {
    char *name = (char *) malloc (sizeof(char) * BUFSIZE);
    char *author = (char *) malloc (sizeof(char) * BUFSIZE);
    char *review = (char *) malloc (sizeof(char) * BUFSIZE);
    char *ratingstr = (char *) malloc (sizeof(char) * BUFSIZE);
    int rating;
    char *thevoid;
    int shelfnum;
    printf("Name of book: ");
    fgets(name, BUFSIZE, stdin);
    remove_newline(name);
    printf("Author of book: ");
    fgets(author, BUFSIZE, stdin);
    remove_newline(author);
    printf("Rating out of 5 stars: ");
    fgets(ratingstr, BUFSIZE, stdin);
    remove_newline(ratingstr);
    rating = strtol(ratingstr, &thevoid, 10);
    printf("Review: ");
    fgets(review, BUFSIZE, stdin);
    remove_newline(review);
    int stat = 1;
    while (stat) {
        printf("What shelf would you like to add this book to? Enter its number.\n");
        show_shelves(shelves);
        char *shelfstr = (char *) malloc (sizeof(char) *BUFSIZE);
        fgets(shelfstr, BUFSIZE, stdin);
        shelfnum = strtol(shelfstr, &thevoid, 10);
        printf("%d\n", shelfnum);
        shelf *chosen = shelf_choice(shelves, shelfnum);
        if (chosen) {
            new_book(table, chosen, name, author, review, rating);
            stat = 0;
            printf("Great! We've added %s by %s to your %s shelf. Taking you back to the home page.\n\n",
                   name, author, chosen->name);
        } else
            printf("Invalid shelf choice.\n");
        free(shelfstr);
    }
    free(name);
    free(author);
    free(ratingstr);
}

void create_shelf(shelf *shelves) {
    printf("What would you like to name your new shelf?\n");
    char *new_name = (char *) malloc (sizeof(char) * BUFSIZE);
    fgets(new_name, BUFSIZE, stdin);
    new_shelf(new_name, shelves);
    printf("Your shelf has been created. Here is a list of your current shelves.\n");
    show_shelves(shelves);
    free(new_name);
}

void reshelve(htbl *table, shelf *shelves) {
    char *name = (char *) malloc (sizeof(char) * BUFSIZE);
    printf("What's the name of the book you want to reshelve? ");
    fgets(name, BUFSIZE, stdin);
    remove_newline(name);
    char *author = (char *) malloc (sizeof(char) * BUFSIZE);
    printf("What's the name of the author? ");
    fgets(author, BUFSIZE, stdin);
    printf("first confirm %s", author);
    remove_newline(author);
    printf("confirming: %s\n", author);
    book *found = find_book(name, author, table);
    if (found) {
        int stat = 1;
        while (stat) {
            printf("What shelf would you like it on? Here are your shelves: \n");
            show_shelves(shelves);
            char *choicestr = (char *) malloc (sizeof(char) * BUFSIZE);
            char *thevoid;
            int choice;
            fgets(choicestr, BUFSIZE, stdin);
            choice = strtol(choicestr, &thevoid, 10);
            shelf *chosen = shelf_choice(shelves, choice);
            if (chosen) {
                shelve_book(found, chosen);
                stat = 0;
            } else
                printf("Invalid shelf choice.\n");
            free(choicestr);
        }
    } else {
        printf("Sorry, we couldn't find that book in your shelves. Add it or check for spelling errors.\n");
    }
    free(name);
    free(author);
}

void view_book(htbl *table) {
    int status = 1;
    while (status) {
        char *name = (char *) malloc(sizeof(char) * BUFSIZE);
        printf("What is the title of the book you want to view?\n");
        fgets(name, BUFSIZE, stdin);
        remove_newline(name);
        char *author = (char *) malloc(sizeof(char) * BUFSIZE);
        printf("What's the name of the author?\n");
        fgets(author, BUFSIZE, stdin);
        remove_newline(author);
        book *found = find_book(name, author, table);
        if (found) {
            printf("We've found your book! Here are the details:\n");
            printf("%s by %s\n%d stars\n%s\n\n5", found->title, found->author, found->rating, found->review);
            printf("Is there anything you'd like to modify about this book's info?\n You can 1) change rating or 2) "
                   "change review or 3) remove it, or 4) do nothing.\n");
            char *choicestr = (char *) malloc (sizeof(char) * BUFSIZE);
            char *thevoid;
            int choice;
            fgets(choicestr, BUFSIZE, stdin);
            choice = strtol(choicestr, &thevoid, 10);
            if (choice == 1) {
                printf("What is your new rating out of 5?\n");
                char *ratingstr = (char *) malloc (sizeof(char) * BUFSIZE);
                int rating;
                fgets(ratingstr, BUFSIZE, stdin);
                remove_newline(ratingstr);
                rating = strtol(ratingstr, &thevoid, 10);
                set_rating(found, rating);
                free(ratingstr);
                printf("We've changed the rating. Taking you back to the main menu.\n\n");
            } else if (choice == 2) {
                printf("What is your new review?\n");
                char *review = (char *) malloc(sizeof(char) * BUFSIZE);
                fgets(review, BUFSIZE, stdin);
                remove_newline(review);
                set_review(found, review);
                free(review);
                printf("We've changed the review. Taking you back to the main menu.\n\n");
            } else if (choice == 3) {
                remove_book(found, table);
                printf("We have removed the book. Taking you back to the main menu.\n\n");
            } else {
                printf("Okay! Taking you back to the main menu.\n\n");
            }
            free(choicestr);
            status = 0;
        } else {
            printf("Sorry, we can't find that book. Would you like to 1) search again or 2) return to the "
                   "main menu?");
            char *choicestr = (char *) malloc(sizeof(char) * BUFSIZE);
            char *thevoid;
            int choice;
            fgets(choicestr, BUFSIZE, stdin);
            choice = strtol(choicestr, &thevoid, 10);
            if (choice == 2) { status = 0; }
        }
        free(name);
        free(author);

    }

}

void view_shelf(htbl *table, shelf *shelves) {
    printf("Which shelf would you like to see?\n");
    show_shelves(shelves);
    char *choicestr = (char *) malloc (sizeof(char) * BUFSIZE);
    char *thevoid;
    int choice;
    fgets(choicestr, BUFSIZE, stdin);
    choice = strtol(choicestr, &thevoid, 10);
    shelf *chosen = shelf_choice(shelves, choice);
    show_shelf(chosen, table);
    free(choicestr);
}

int main() {
    // setting up hash table and shelves
    htbl *table = htbl_new(&hash, 10);
    shelf *shelves = (shelf *) malloc (sizeof(shelf));
    shelves->prev = NULL;
    shelves->name = strdup("to-read");
    shelves->next = NULL;
    new_shelf("currently reading", shelves);
    new_shelf("read", shelves);

    while (1) {
        printf("Welcome to Storygraph Offline!\nHere's what you're currently reading:\n");
        //show_shelf(shelves->next, table);
        printf("What would you like to do?\n1) Add new book\n2) Create a new shelf\n3) Reshelve a book\n");
        printf("4) View a book's details\n5) View a shelf's contents\n");
        printf("Please enter the number corresponding to your choice.\n");
        char *choicestr = (char *) malloc (sizeof(char) * BUFSIZE);
        char *thevoid;
        fgets(choicestr, BUFSIZE, stdin);
        int opt = strtol(choicestr, &thevoid, 10);
        switch (opt) {
            case 1:
                add_new(table, shelves);
                break;
            case 2:
                create_shelf(shelves);
                break;
            case 3:
                reshelve(table, shelves);
                break;
            case 4:
                view_book(table);
                break;
            case 5:
                view_shelf(table, shelves);
                break;
            default:
                printf("Sorry, we don't recognize that option. Try entering the number again.\n");
                break;
        }
    }
    return 0;
}

