#include "comparison.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef DEBUG
#define DEBUG 0
#endif

char *capitalize_initial(const char *word) {
    char *capitalized = strdup(word);
    if (capitalized == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    if (strlen(capitalized) > 0) {
        capitalized[0] = toupper(capitalized[0]);
    }
    return capitalized;
}

char *capitalize_all(const char *word) {
    char *capitalized = strdup(word);
    if (capitalized == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < strlen(capitalized); i++) {
        capitalized[i] = toupper(capitalized[i]);
    }
    return capitalized;
}

int is_capitalized(const char *word) {
    return isupper(word[0]);
}

int is_all_lowercase(const char *word) {
    for (const char *ptr = word; *ptr != '\0'; ptr++) {
        if (!islower(*ptr)) {
            return 0;
        }
    }
    return 1;
}

int is_first_letter_uppercase(const char *word) {
    return isupper(word[0]) && is_all_lowercase(word + 1);
}

int is_exact_capitalization(const char *word, const char *dict_word) {
    return strcmp(word, dict_word) == 0;
}

int is_in_dictionary(const char *word) {
    for (int i = 0; i < dictionary_size; i++) {
        if (strcasecmp(word, dictionary[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void make_lowercase(char *word) {
    for (char *ptr = word; *ptr != '\0'; ptr++) {
        *ptr = tolower(*ptr);
    }
}

int has_apostrophe(const char *word) {
    while (*word) {
        if (*word == '\'') {
            return 1;
        }
        word++;
    }
    return 0;
}

int check_capitalization(const char *word, const char **dictionary) {
    for (int i = 0; i < dictionary_size; i++) {
        char* capital_init = capitalize_initial(dictionary[i]);
        char* capital_all = capitalize_all(dictionary[i]);
        //printf("Capitalized initial: '%s'\n", capital_init);
        //printf("Capitalized all: '%s'\n", capital_all);
        if (strcmp(word, dictionary[i]) == 0) {
            if(DEBUG)   printf("Found: %s\n", word);
            free(capital_init);
            free(capital_all);
            return 1;
        } else if (strcmp(word, capital_init) == 0 && (is_first_letter_uppercase(dictionary[i]) || is_all_lowercase(dictionary[i]))) {
            if(DEBUG)   printf("Found (Capitalized initial): %s (Dictionary: %s)\n", word, dictionary[i]);
            free(capital_init);
            free(capital_all);
            return 1;
        } else if (strcmp(word, capital_all) == 0 && (is_capitalized(dictionary[i]) || is_all_lowercase(dictionary[i]) || is_first_letter_uppercase(dictionary[i]))) {
            if(DEBUG)   printf("Found (All capitalized): %s (Dictionary: %s)\n", word, dictionary[i]);
            free(capital_init);
            free(capital_all);
            return 1;
        } else if (strcmp(word, capital_init) == 0 && (has_apostrophe(dictionary[i]))) {
            if(DEBUG)   printf("Found (Initial Apostrophe capitalized): %s (Dictionary: %s)\n", word, dictionary[i]);
            free(capital_init);
            free(capital_all);
            return 1;
        } else if(strcasecmp(word, dictionary[i]) == 0 && (strcmp(word, dictionary[i]) == 0 || strcmp(word, capital_all) == 0)) {
            if(DEBUG)   printf("Found (Case insensitive): %s (Dictionary: %s)\n", word, dictionary[i]);
            free(capital_init);
            free(capital_all);
            return 1;
        }
        free(capital_init);
        free(capital_all);
    }

    if(DEBUG)   printf("Not found: %s\n", word);
    return 0;
}


int check_hyphenated_word(const char *word, const char **dictionary) {
    char *part = strdup(word);
    if (part == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(part, "-");
    while (token != NULL) {
        if (!check_capitalization(token, dictionary)) {
            free(part);
            return 0;
        }
        token = strtok(NULL, "-");
    }
    free(part);
    return 1;
}
