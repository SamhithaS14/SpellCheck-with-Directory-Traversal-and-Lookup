#ifndef COMPARISON_H
#define COMPARISON_H

char *capitalize_initial(const char *word);
char *capitalize_all(const char *word);
int is_capitalized(const char *word);
int is_all_lowercase(const char *word);
int is_first_letter_uppercase(const char *word);
int is_exact_capitalization(const char *word, const char *dict_word);
int is_in_dictionary(const char *word);
void make_lowercase(char *word);
int has_apostrophe(const char *word);
int check_capitalization(const char *word, const char **dictionary);
int check_hyphenated_word(const char *word, const char **dictionary);

#endif /* COMPARISON_H */
