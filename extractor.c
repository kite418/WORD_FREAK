#include "extractor.h"

Partial_Word * new_partial_word(void) {
    Partial_Word * new_partial_word2 = (Partial_Word *) malloc(sizeof(Partial_Word));
    char * text = (char *) malloc(sizeof(char));
    //Make sure memory is allocated successfully
    if (text == NULL || new_partial_word2 == NULL) {
        printf("Failed to allcolate memory\n");
        exit(1);
    }

    //Append null character to only available byte
    text[0] = '\0';
    new_partial_word2->text = text;
    new_partial_word2->index =  0;

    return new_partial_word2;
}

char * get_partial_text(Partial_Word * partial) {
    return partial->text;
}

void set_partial_text(Partial_Word * partial, char * text) {
    partial->text = text;
}

int get_partial_index(Partial_Word * partial) {
    return partial->index;
}

void set_partial_index(Partial_Word * partial, int index) {
    partial->index = index;
}

void partial_put_char(Partial_Word * partial, char c) {
   
    //Get text from and index from partial_word
    char * text = get_partial_text(partial);
    int index = get_partial_index(partial);

    //Append the given char to the current index
    text[index] = c;

    //Set the newly altered text to the partial
    set_partial_text(partial, text);

    //Expand the char * text
    partial_expand(partial);

    //Get text after expansion, and append null character
    text = get_partial_text(partial);
    set_partial_index(partial, ++index);
    text[index] = '\0';


}

void partial_expand(Partial_Word * partial) {
    //Get text and index from partial_word
    char * text = get_partial_text(partial);
    int partial_index = get_partial_index(partial);

    //Expand the size of the char * text
    char * temp = realloc(text, sizeof(char) * (partial_index + 2));
    
    //Make sure memory was allocated successfully
    if (temp == NULL) {
        printf("Failed to allcolate memory\n");
        exit(1);
    }

    //Set the old text to the reallocated text, then set the text to the partial_word
    text = temp;
    set_partial_text(partial, text);
}

void partial_reset(Partial_Word * partial) {

    //Free the char * text, from the partial_word
    char * text = get_partial_text(partial);
    free(text);

    //Malloc a new char * of size 1 byte for the partial_word
    text = (char *) malloc(sizeof(char));
    
    //Make sure memory was allocated successfully
    if (text == NULL) {
        printf("Failed to allcolate memory\n");
        exit(1);
    }

    //Append null character to only available byte
    text[0] = '\0';
    set_partial_text(partial, text);

    //Set the now reset text to the partial_word
    set_partial_index(partial, 0);
}

void partial_free(Partial_Word * partial) {
    //Free the text, and Partial_Word
    free(get_partial_text(partial));
    free(partial);
}

_Bool partial_is_empty(Partial_Word * partial) {
    //If index == 0, we know it has only 1 char a null character and therefore is "empty"
    return (get_partial_index(partial) == 0);
}

void extract_words(char buf[], int buf_length, Partial_Word * partial, Dictionary * dict) {

    //Loop through the entire given buffer
    for (int i = 0; i < buf_length; i++) {
        //Get char at index i
        char buf_char = buf[i];

        //Make sure the char is alphabetical
        if (isalpha(buf_char)) {
            //Append char to the partial_word we're working on
            partial_put_char(partial, buf_char);
        } else if (get_partial_index(partial) > 0) { //If the char isn't alphabetical and our partial_Word is not empty
            //Insert the partial_word text into the hash table
            char * text = get_partial_text(partial);
            dictionary_insert(dict, text);
            partial_reset(partial); 
        } else {
            //If the partial_word is empty and we hit a non alphabetical char, reset it
            partial_reset(partial);
        }

    }
    
}


void read_from_fd(char * argv[], char buf[], int buf_size, Dictionary * dict) {

    int fdin, bytes_read;

    int i = 1;
    //Point to first path in argv
    char * cursor = argv[i];

    //Create a partial word to work with
    Partial_Word * partial = new_partial_word();
    while (cursor != NULL) {
        //Loop through every path
        
        //Get file descriptor from path
        if ((fdin = open(cursor, O_RDONLY)) < 0) {
            printf("Failed to open file\n");
            exit(2);
        }

        //Read the entire file and extract words after every read
        while ((bytes_read = read(fdin, buf, buf_size - 1)) > 0) {
            buf[bytes_read] = '\0';
            extract_words(buf, bytes_read, partial, dict);
        }

        //Attempt to close file
        if (close(fdin) < 0) {
            printf("Failed to close file\n");
            exit(3);
        }

        //If we have a word left over, make sure to add it to the hash table
        if (!partial_is_empty(partial)) {
            dictionary_insert(dict, get_partial_text(partial));
        }
        
        //Advance to the next file path
        cursor = argv[++i];
    }

    //Free the partial_word once we're done
    free(get_partial_text(partial));
    free(partial);
}

void read_from_pipe(char buf[], int buf_size, Dictionary * dict) {

    int bytes_read;

    //Read everything directly from STDIN_FILENO 
    Partial_Word * partial = new_partial_word();
    while ((bytes_read = read(STDIN_FILENO, buf, buf_size - 1)) > 0) {
        buf[bytes_read] = '\0';
        extract_words(buf, bytes_read, partial, dict);
    }

    //If word left over, add it to the hash table
    if (!partial_is_empty(partial)) {
        dictionary_insert(dict, get_partial_text(partial));
    }

    //Free the partial_word once we're done
    partial_free(partial);
}

void read_from_env(char * env_input, char buf[], int buf_size, Dictionary * dict) {
    int fdin, bytes_read;

    int i = 1;
    char * cursor = env_input;

    Partial_Word * partial = new_partial_word();
        
        //Attempt to open file via path provided by env_input
        if ((fdin = open(cursor, O_RDONLY)) < 0) {
            printf("Failed to open file\n");
            exit(2);
        }

        //Read the entire file, extract words from each buffer
        while ((bytes_read = read(fdin, buf, buf_size - 1)) > 0) {
            buf[bytes_read] = '\0';
            extract_words(buf, bytes_read, partial, dict);
        }

        //Attempt to close the file
        if (close(fdin) < 0) {
            printf("Failed to close file\n");
            exit(3);
        }

        //If we have a word left over, add it to the hashtable
        if (!partial_is_empty(partial)) {
            dictionary_insert(dict, get_partial_text(partial));
        }
        
    //Free the partial_word once we're done
    free(get_partial_text(partial));
    free(partial);
}

