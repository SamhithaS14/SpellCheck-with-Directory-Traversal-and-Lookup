#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include "sb.h"
#include "comparison.h"

#ifndef DEBUG
#define DEBUG 0
#endif
#ifndef BUFLENGTH
#define BUFLENGTH 32
#endif

#define INITIAL_SIZE 10


char **dictionary;
int dictionary_size = 0;


void print_error(const char *filename, int line_number, int column_number, const char *word) {
    char relative_path[512];
    char cwd[512];

    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("Error getting current working directory");
        exit(EXIT_FAILURE);
    }

    // Find the relative path by removing the common prefix
    if (strncmp(filename, cwd, strlen(cwd)) == 0) {
        strcpy(relative_path, filename + strlen(cwd));
    } else {
        strcpy(relative_path, filename);
    }

    // Print the relative path, line number, column number, and word
    printf("%s (%d,%d): %s\n", relative_path, line_number, column_number, word);
}


void read_dictionary(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }

    dictionary = malloc(sizeof(char *) * INITIAL_SIZE);
    if (dictionary == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    strbuf_t dictionary_buffer;
    sb_init(&dictionary_buffer, 0);

    char* buffer = malloc(BUFLENGTH);
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, BUFLENGTH)) > 0) {
        sb_ensure(&dictionary_buffer, dictionary_buffer.length + bytes_read + 1);
        memcpy(dictionary_buffer.data + dictionary_buffer.length, buffer, bytes_read); // Append buffer to dictionary buffer
        dictionary_buffer.length += bytes_read;
        dictionary_buffer.data[dictionary_buffer.length] = '\0'; // Null-terminate the buffer
    }

    if (bytes_read == -1) {
        perror("Error reading from dictionary file");
        exit(EXIT_FAILURE);
    }

    close(fd); // Close the file descriptor


    char *start = dictionary_buffer.data;
    char *end = dictionary_buffer.data;
    while (*end != '\0') {
        if (*end == '\n' || *end == ' ' || *end == '\t') {
            if (start < end) {
                *end = '\0'; // Null-terminate the word                
                dictionary[dictionary_size] = strdup(start);
                if (dictionary[dictionary_size] == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                //printf("Dictionary word: %s\n", dictionary[dictionary_size]);
                dictionary_size++;
                if (dictionary_size % INITIAL_SIZE == 0) {
                    char **temp = realloc(dictionary, sizeof(char *) * (dictionary_size + INITIAL_SIZE));
                    if (temp == NULL) {
                        perror("Memory allocation failed");
                        exit(EXIT_FAILURE);
                    }
                    dictionary = temp;
                }
                start = end + 1;
            }
        }
        end++;
    }
    if (start < end) {
        dictionary[dictionary_size] = strdup(start);
        if (dictionary[dictionary_size] == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        dictionary_size++;
    }

    free(buffer);

    sb_destroy(&dictionary_buffer);
    
}

char *filter_word(char* word) {
    //printf("filter_word called!\n");
    int indexStart = 0;
    int indexEnd = strlen(word)-1;

    while (indexStart < (int)strlen(word)) {
        if (word[indexStart] == '[' || word[indexStart] == '(' || word[indexStart] == '{'  || word[indexStart] == '\'' \
            || word[indexStart] == '"'){
            indexStart++;
            //printf("index start: %d\n", indexStart);
            continue;
        }
        break;
    }

    while (indexEnd > indexStart) {
        if (!isalpha(word[indexEnd])){
            indexEnd--;
            if(indexEnd <= indexStart) {
                return NULL;
            }
            //printf("index end: %d\n", indexEnd);
            continue;
        }
        break;
    }

    if(indexEnd<indexStart) {
        //printf("Null returned\n");
        return NULL;
    }

    if (DEBUG)  printf("Filter malloc: %d\n", indexEnd-indexStart+2);
    char* filter = malloc(indexEnd-indexStart+2);
     if (filter == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(filter, word+indexStart, indexEnd-indexStart+1);
    filter[indexEnd - indexStart + 1] = '\0'; // Null-terminate the string

    if(DEBUG)   printf("Filter word: %s\n", filter);
    if(DEBUG)   printf("Word Given: %s, Filtered: %s\n", word, filter);
    return filter;
    //word[indexEnd] = '\0';
    //return word+=indexStart; 
    
}


void process_file(const char *filename, int *error) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }

    int buflength = BUFLENGTH;
    char *buf = malloc(BUFLENGTH);
    if (buf == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int pos = 0;
    int bytes;
    int word_start = -1;
    int line_number = 1;
    int column_number = 1;


    while ((bytes = read(fd, buf + pos, buflength - pos)) > 0) {
        int bufend = pos + bytes;
        for (int i = pos; i < bufend; i++) {
            column_number++;
            
            if (isspace(buf[i])) {
                // if (buf[i] == '\n') { //CHANGE: Moved where I increment new line after checking word Line 230
                //     line_number++;
                //     column_number = 1;
                // }
                if (word_start != -1) {
                    // Process the word
                    char *word = malloc(i - word_start + 1);
                if (word == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                strncpy(word, buf + word_start, i - word_start);
                word[i - word_start] = '\0'; // Null-terminate the word
                if (DEBUG) printf("Processing word: '%s'\n", word); // Debug print
                char* filter = filter_word(word);
                if (filter == NULL) {
                    free(word);
                } else {
                    if (!check_hyphenated_word(filter, (const char **)dictionary)) {
                        *error = 1; //CHANGE: Change this int to 1 if error exists
                        if(column_number!=1) {
                            print_error(filename, line_number, column_number-strlen(word)-1, filter);
                        } else {
                            //Change: did line_number-1 here to fix line_number reporting!
                            print_error(filename, line_number-1, column_number, filter);
                        }
                    }
                    free(filter);
                    free(word);
                }
                    word_start = -1;
                }
                if (buf[i] == '\n') { //CHANGE: Moved where I increment new line here
                    line_number++;
                    column_number = 1;
                }
                
                //column_number++;
            } else {
                if (word_start == -1) {
                    if(DEBUG)    printf("word start -1 to i: %d\n", i);
                    word_start = i;
                }
            }
            if (DEBUG) printf("pos %d, char '%c'\n", i, buf[i]); // Debug print
        }

        // Shift the remaining data to the start of the buffer
        if(word_start == pos) {
            pos = 0;
        } else if(bufend == buflength) {
            buflength *= 2;
            buf = realloc(buf, buflength);
        } 
        if(word_start != -1) {
            int remaining_length = bufend - word_start;
            memmove(buf, buf + word_start, remaining_length);
            pos = remaining_length;
            word_start = 0;
        }
        

        //printf("Line: %d, Column: %d\n", line_number, column_number);
    }

    if (bytes == -1) {
        perror("Error reading from file");
        exit(EXIT_FAILURE);
    }

    // Process the last word if it exists
    if (word_start != -1) {
        // Process the last word
        if(DEBUG)   printf("buff length: %d, word start: %d, bytes: %d, pos: %d\n", buflength, word_start, bytes, pos);
        int remaining_length = pos + bytes;
        char *word = malloc(remaining_length + 2);
        if (word == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        strncpy(word, buf + word_start, remaining_length);
        word[remaining_length] = '\0'; // Null-terminate the word
        if (DEBUG) printf("Processing word: '%s'\n", word); // Debug print
        char* filter = filter_word(word);
        if (filter == NULL) {
            free(word);
        } else {
            if (!check_hyphenated_word(filter, (const char **)dictionary)) {
                *error = 1; //CHANGE: Change this int to 1 if error exists
                if(column_number!=1) {
                    print_error(filename, line_number, column_number-remaining_length, filter);
                } else {
                    print_error(filename, line_number, column_number, filter);
                }
            }
            free(filter);
            free(word);
        }
    }

    free(buf);
    close(fd);
}


void process_directory(const char *dirname, int *error) {
    DIR *dir;
    struct dirent *entry;
    char filepath[512];

    dir = opendir(dirname);
    if (!dir) {
        perror("Error opening directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        //printf("Name: %s\n", entry->d_name);
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.') {
            continue;
        } 

        snprintf(filepath, sizeof(filepath), "%s/%s", dirname, entry->d_name);

        if (entry->d_type == DT_DIR) {
            process_directory(filepath, error);
        } else if (entry->d_type == DT_REG && strstr(entry->d_name, ".txt") != NULL) {
            process_file(filepath, error);
        }
    }

    closedir(dir);
}

int main(int argc, char **argv) {
    int error = 0; //CHANGE: int that check if there was an error
    if (argc < 3) {
        fprintf(stderr, "Usage: %s dictionary_file file_or_directory ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    read_dictionary(argv[1]);


    for (int i = 2; i < argc; i++) {
        struct stat statbuf;
        if (stat(argv[i], &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                process_directory(argv[i], &error);
            } else {
                process_file(argv[i], &error);
            }
        } else {
            perror("Error getting file status");
            exit(EXIT_FAILURE);
        }
    }

    // Cleanup dictionary
    for (int i = 0; i < dictionary_size; i++) {
        free(dictionary[i]);
    }
    free(dictionary);

    //CHANGE: Check if if error existed to reutn EXIT_SUCCESS or EXIT_FAILURE
    if (DEBUG)printf("Error: %i\n", error);
    return error;
}