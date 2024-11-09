#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <gtk/gtk.h>
#include <jansson.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define FILESIZE       64 
#define LETTERSIZE      9
#define LINESIZE     2048

char *get_dictionary_filename(const char *word);
bool parse_dictionary(json_t *root, const char *word);

#endif // DICTIONARY_H
