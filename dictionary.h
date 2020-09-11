#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#of entries/buckets in hash table
#define TABLE_SIZE 100000

typedef struct word Word;

//Data structure used for entries in the hash table
struct word {
    char * text;
    int frequency;
    Word * next;
};

typedef struct dictionary Dictionary;

//Data structure to represent the hash table
struct dictionary{
    Word ** entries;
};

//Implementation of djb2 hashing function
unsigned long hash(char *str);

//Constructor for Word, data structure
Word * new_word(char * text, int frequency, Word * next);
//Getter for Word->text
char * get_word_text(Word * word);
//Getter for Word->frequency
int get_word_frequency(Word * word);
//Setter for word->frequency
void set_word_frequency(Word * word, int frequency);
//Getter for word->next
Word * get_word_next(Word * word);
//Setter for word->next
void set_word_next(Word * word, Word * next);
//Used to insert a new Word * at the tail of the linked list given by Word * word
void word_insert(Word * word, char * text);
//Used to print the entire provided Word * word linked list with formatted text and frequency
int word_print(Word * word);
//Used to free the memory allocated by the entire Word * word given
void word_free(Word * word);

//Constructor for Dictionary data structure
Dictionary * new_dictionary(void);
//Getter for Dictionary->entries
Word ** get_dictionary_entries(Dictionary * dict);
//Used to insert a new Word * into the given hashtable Dictionary * dict from the provided char * text
void dictionary_insert(Dictionary * dict, char * text);
//Used to print every non-NULL entry in the given hashtable Dictionary * dict
void dictionary_print(Dictionary * dict);
//Used to free the memory allocated by the entire given Dictionary * dict including its entries
void dictionary_free(Dictionary * dict);

#endif