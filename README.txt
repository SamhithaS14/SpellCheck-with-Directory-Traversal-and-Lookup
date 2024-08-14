Names:  Krithish Ayyappan        NetIDs: ka759
        Samhitha Sangaraju               ss4043

Code Design and Requirements:
        Process Overview: 
                The code begins by parsing command-line arguments, expecting a dictionary file followed by one or 
                more files or directories to process. It then reads the dictionary file into memory. Afterward, it 
                iterates through each specified file or directory, recursively traversing directories and processing each 
                text file it encounters. During processing, it checks the spelling, non-alphabetical symbols, and capitalization 
                of words against the loaded dictionary, reporting any errors encountered.

        Dictionary: 
                The dictionary is read from a file, with each word stored in memory for efficient comparison 
                during spell checking. Words are stored in a dynamic array, allowing for flexibility in dictionary size.

        Directory and File Traversal: 
                The program traverses directories recursively using the dirent.h library, 
                processing each subdirectory and text file found within them. It distinguishes between directories and 
                regular files, processing only text files with a ".txt" extension.

        Helper Functions: (brief overview)

                is_punctuation: Checks if a character is a punctuation mark based on its ASCII value.
                is_hyphen: Checks if a character is a hyphen.
                capitalize_initial: Capitalizes the first letter of a word and returns the modified string.
                capitalize_all: Capitalizes all letters in a word and returns the modified string.
                is_capitalized: Checks if the first letter of a word is capitalized.
                is_all_lowercase: Checks if all letters in a word are lowercase.
                is_first_letter_uppercase: Checks if the first letter of a word is uppercase, and the rest are lowercase.
                is_exact_capitalization: Checks if a word matches exactly the capitalization of a dictionary word.
                is_in_dictionary: Checks if a word is present in the dictionary.
                make_lowercase: Converts all letters in a word to lowercase.
                has_apostrophe: Checks if a word contains an apostrophe.
                check_capitalization: Checks if a word matches any capitalization variations of a dictionary word, considering different cases and apostrophes.
                is_hyphenated_word: Checks if a word contains a hyphen.
                check_hyphenated_word: Checks the capitalization of each part of a hyphenated word against the dictionary.
                print_error: Prints an error message indicating a misspelled word, along with the file name, line number, column number, and the misspelled word.
                sb_init: Initializes a string buffer with a specified initial size.
                sb_destroy: Frees the memory allocated for a string buffer.
                sb_ensure: Ensures that a string buffer has enough capacity to accommodate additional data.
                sb_append: Appends a string to a string buffer, dynamically resizing it if necessary.


        Checking Words Spelling + If They're in the Dictionary: 
                The code checks each word's spelling and whether it exists 
                in the loaded dictionary. It considers various cases, such as initial capitalization, all caps, lower case,
                mixed case, hyphenated words, symbols (non alpha characters) after, before, and within words. Additionally, it ignores extraneous 
                punctuation and symbols at the end. It only ignores certain non-alphabetic symbols at the beginning, as 
                specified in the write-up.

        Error Reporting: 
                Errors, such as misspelled words or improper capitalization, are reported with detailed information, 
                including the filename, line number, column number, and the misspelled word. This ensures users can easily identify 
                and correct errors in their text files.

        Recursive Traversal: 
                The code recursively traverses directories using a depth-first search approach, ensuring all text files 
                within the directory structure are processed.

        Properly Reads Dictionary: 
                The code correctly reads the dictionary from a file, parsing each word and 
                storing it in memory. It handles dynamic memory allocation efficiently to accommodate dictionaries of varying sizes.

        Properly Processes and Compares Word: 
                Words are processed and compared against the dictionary using robust logic, 
                accounting for different capitalization styles, hyphenation, and symbols/punctuation. This ensures accurate spell 
                checking under diverse scenarios.

        Check Capitalization: 
                The code verifies the proper capitalization of words, considering initial capitalization, all lower case, 
                all caps, and mixed case scenarios. It distinguishes between intentional capitalization and potential errors.

        Check Punctuation at the End: 
                The program works to remove all trailing punctuation before checking if a word is spelled correctly,
                according to the write-up directions. If the word is misspelled and returned as an error, only the word should 
                print, without the trailing punctuation.

        Check Punctuation and Symbols (non alpha characters) at the Beginning: 
                The program works to remove only some symbols at the beginning, specifically
                those discussed in the write-up. So, if a word has "{", "(", "[", "'", or """, they should all be removed 
                before printing. If there are other allowable symbols still at the beginning, those can remain when the word 
                is printed as an error. In our design, if we encounter an acceptable symbol, we simply print all other symbols 
                after that along with the word itself in the error, even if one of those symbols after is one of 
                these: "{", "(", "[", "'", or """. We also test for all non-alpha characters as a "word".

        Check Symbols (non alpha characters) in the Middle of a Word: 
                In the case that there are symbols in the middle of a word, the word must print as an error. In our code, 
                to parse the word, we use a pointer system, in which one pointer marks the first letter of a word, and the other 
                moves to the last letter. This should encapsualte all the other letters and existing symbols/puncutation 
                in the middle of the word.

        Check Hyphens: 
                To check hyphenated words, we use strtok(), and use the hyphen as where the word should be split (our delimitor). 
                We then check if each individual word is spelled correctly, accounting for all other situations too such as its 
                capitalization case. If one or more of the words within the larger hyphenated word are wrong, the entire word with the 
                hyphen is returned as an error.
                If there are hyphens before a word, we treat it as leading punctuation and print the whole word out as an error.

        Buffer Checking:
                The code dynamically manages a buffer for reading text files, reallocating memory as needed to accommodate data 
                growth. When the buffer reaches its capacity, memory is dynamically reallocated, typically doubling the buffer 
                size to ensure efficient memory utilization. This process continues until all data from the file has been 
                processed, after which the dynamically allocated memory is freed to prevent memory leaks.

Our Test Scenarios and Test Cases:
        1. testCase1.txt : 
        This case covers many different instances which the spchk should catch. We test out initial capitalization, all lowercase, 
        and all uppercase. We also test random capitalization within a word, which should return an error. We also check 
        to make sure words like MacDonald, where there are capital words within the word, check out. We test a couple variations 
        to check capitalization against this. We also test out hyphens, to see if our code will individually check the 
        words, and print the entire hypenated word if one or more of those "sub" words are wrong. Within the hyphenated words, 
        we also test different capitalization cases. the We also test out different symbols in different places in the word. 
        We also test the situations where a symbol is right before a word, right after a word, or inbetween letters of a word, 
        such as "!", "{", or even double and single quotes/apostrophes, to name a few. The write up gave us specific rules on 
        starting and ending punctuation, so we had many variations of this. To be safe, we also test only symbols and numbers.

        2. testCase2.txt :
        This case has the lyrics of a song as input. The song has many words/onomatopoeia that are not in the dictionary, so we 
        used it mainly to catch spelling based errors.

        3. small_dict.txt:
        Made a small dictionary consisting of fruit names just to see if our read_dictionary function depended on size and was variable
        to different sizes. Also, tested how our buffer works with different sizes.

Some Things we Learned from our Assignment:
        1.      File I/O Operations: file input/output operations in C,  reading text files and directories using system calls and library functions 
                like open(), read(), readdir(), etc.

        2.      Error Handling: Error reporting through POSIX perror() and fprintf(stderr, ...), and termination within
                with exit().

        3.      System Calls: We used calls like lstat() to retrieve file status information and getcwd() to obtain the current 
                working directory

        4.      Buffer Allocation/Reallocation and Management: We used functions like malloc() and realloc(). Buffers are allocated to store 
                data read from files,  for efficient processing of large datasets without knowing the exact size beforehand. We also covered reallocation of 
                buffers to accommodate data growth, and management of buffer capacity.

        5.      String Manipulation: We use different string manipulation techniques like tokenization to filter and compare our text file's words to dictionary words. 
                This is necessary to process and validate words read from files.