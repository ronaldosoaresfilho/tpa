#ifndef DICTIONARY_H
#define DICTIONARY_H

#define FILESIZE       64 
#define LETTERSIZE      9

char *get_dictionary_filename(const char *word);
bool parse_dictionary(json_t *root, const char *word);

#endif // DICTIONARY_H
