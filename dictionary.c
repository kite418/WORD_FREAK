#include "dictionary.h"

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ( (c = *str++) )
        hash = ((hash << 5) + hash) + c;
        
    return (hash % TABLE_SIZE);
}

Word * new_word(char * text, int frequency, Word * next) {
    Word * new_word = (Word *) malloc(sizeof(Word));

    //Create a copy of the given text and set it to the struct
    new_word->text = strdup(text);

    //Make sure memory allocation is successful
    if (new_word == NULL || new_word->text == NULL) {
        printf("Failed to allcolate memory\n");
        exit(1);
    }

    new_word->frequency = frequency;
    new_word->next = next;

    return new_word;
}

char * get_word_text(Word * word) {
    return word->text;
}

int get_word_frequency(Word * word) {
    return word->frequency;
}

void set_word_frequency(Word * word, int frequency) {
    word->frequency = frequency;
}

Word * get_word_next(Word * word) {
    return word-> next;
}

void set_word_next(Word * word, Word * next) {
    word->next = next;
}

void word_insert(Word * word, char * text) {

    Word * previous = word;
    Word * current = word;

    //Iterate through linked list
    while (current != NULL) {
        char * word_text = get_word_text(current);
        //If we find a matching word, increase the frequency of that word
        if (strcmp(text, word_text) == 0) {
            set_word_frequency(current, get_word_frequency(current) + 1);
            return;
        }

        previous = current;
        current = current->next;
    }

    //If we didn't find a matching word, add it to the linked list
    previous->next = new_word(text, 1, NULL);

}

int word_print(Word * word) {
    Word * current = word;

    int count = 0;

    //Iterate through linked list and output the text and frequency
    while (current != NULL) {
        char output_buf[75] = {0};
        sprintf(output_buf, "%-20s%-1c %2i\n", get_word_text(current), ':', get_word_frequency(current));
        write(1, output_buf, 75);
        current = get_word_next(current);
    }

    return count;

}

void word_free(Word * word) {
    Word * previous = word;
    Word * current = word;

    //Iterate through linked list
    while (current != NULL) {
        current = get_word_next(current);
        set_word_next(previous, NULL);

        //Free word->text
        free(get_word_text(previous));
        free(previous);
        previous = current;
    }


}

Dictionary * new_dictionary(void) {

    Dictionary * dict = (Dictionary *) malloc(sizeof(Dictionary *));
    dict->entries = malloc(sizeof(Word *) * TABLE_SIZE);
    
    //Ensure memory allocation was successful
    if (dict == NULL || dict->entries == NULL) {
        printf("Failed to allcolate memory\n");
        exit(1);
    }

    //Itiilize all of the entires to NULL
    for (int i = 0; i < TABLE_SIZE; i++) {
        dict->entries[i] = NULL;
    }

    return dict;
}

Word ** get_dictionary_entries(Dictionary * dict) {
    return dict->entries;
}

void dictionary_insert(Dictionary * dict, char * text) {
    //Get the has for the key which is char * text
    unsigned long hashed = hash(text);
    
    //Copy the text
    char * text_cpy = strdup(text);

    //Get the bucket in the table to insert the new entry
    Word ** entries = get_dictionary_entries(dict);
    Word * entry = entries[hashed];

    //If the bucket is empty, insert it directly otherwise walk through the linked list and search
    //for an equal word or insert at the tail
    if (entry == NULL) {
        entries[hashed] = new_word(text_cpy, 1, NULL);
    } else {
        word_insert(entry, text_cpy);
    }

    //Prevent memory leak by freeing the text_cpy once we don't need it anymore
    free(text_cpy);
}

void dictionary_print(Dictionary * dict) {

    Word ** entries = get_dictionary_entries(dict);

    //Loop through every entry in the hashtable and print each one
    for (int i = 0; i < TABLE_SIZE; i++) {
        word_print(entries[i]);
    }
    
}

void dictionary_free(Dictionary * dict) {
    Word ** entries = get_dictionary_entries(dict);
    //Free the linked list of each entry
    for (int i = 0; i < TABLE_SIZE; i++) {
        Word * entry = entries[i];
        word_free(entry);
    }

    //Free the entire entries list, and the dictionary itself
    free(entries);
    free(dict);
    
}

