#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#include "dictionary.h"

typedef struct partial_word Partial_Word;

//Data structure used for extracing words by parts
struct partial_word{
    char * text;
    int index;
};

//Constructor for Partial_Word data structure
Partial_Word * new_partial_word(void);
//Getter for Partial_Word->text
char * get_partial_text(Partial_Word * partial);
//Setter for Partial_Word->text
void set_partial_text(Partial_Word * partial, char * text);
//Getter for Partial_Word->index
int get_partial_index(Partial_Word * partial);
//Setter for Partial_Word->index
void set_partial_index(Partial_Word * partial, int index);
//Appends the given char c to the end of the given Partial_Word * partial
void partial_put_char(Partial_Word * partial, char c);
//Increases the size of the given Partial_Word * partial text field by 1 byte
void partial_expand(Partial_Word * partial);
//Used to free the memory allocated by the given Partial_Word * partial
void partial_free(Partial_Word * partial);
//Used to check if the given Partial_Word * partial has index == 0
_Bool partial_is_empty(Partial_Word * partial);
//Used to process and extract words from the given char buf[] of length int buf_length, and insert those extracted words
//into the given Dicationary * dict
void extract_words(char buf[], int buf_length, Partial_Word * partial, Dictionary * dict);
//Used to read and then call extract_words() from a given char * argv[] containing 1 or more file paths
void read_from_fd(char * argv[], char buf[], int buf_size, Dictionary * dict);
//Used to read and then call extract_words() from a piped input
void read_from_pipe(char buf[], int buf_size, Dictionary * dict);
//Used to read from the given char * env_input and then call extract_words() from WORD_FREAK environment variable given
void read_from_env(char * env_input, char buf[], int buf_size, Dictionary * dict);

#endif