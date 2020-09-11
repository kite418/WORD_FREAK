#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "extractor.h"
#include "dictionary.h"

extern char ** environ;

//Entry point for program
int main(int argc, char * argv[], char *envp[]) {
    
    //Create buffer for read() of size BUF_SIZE
    int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    //Get WORD_FREAK env
    int count = 0;
    char * WORD_FREAK = NULL;
    while (environ[count] != NULL) {
        if (strncmp(environ[count], "WORD_FREAK", 10) == 0) {
            WORD_FREAK = strtok (environ[count]," =");
            WORD_FREAK = strtok(NULL, " =");
        }
        count++;
    }

    //Create new hash table
    Dictionary * dict = new_dictionary();

    //Determine which method of input is provided to the program, filepath, pipe, or env
    if (argv[1] != NULL) {
        read_from_fd(argv, buf, BUF_SIZE, dict);
    } else if (WORD_FREAK != NULL) {
        read_from_env(WORD_FREAK, buf, BUF_SIZE, dict);
    } else {
        read_from_pipe(buf, BUF_SIZE, dict);
    }

    //Print and free hash table
    dictionary_print(dict);
    dictionary_free(dict);

    return 0;
}