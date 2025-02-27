#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"
#include "parse.h"

#define MAXLINE 4096
#define MAXLEN  32768

const char *dicpath = "./app/data/dic.txt";

// search input in the dictionary
char *searchdic(char *input)
{
    char *output = (char*) malloc(MAXLEN * sizeof(char));
    char *line = (char*) malloc(MAXLINE * sizeof(char));
    FILE *fp;

    if (!output || !line) {
	    free(output);
	    free(line);
	    return NULL;
    }

    output[0] = '\0';

    if (empty(input)) {
	    strcpy(output, "Entrada vazia!\n\n");
	    return output;
    }

    fp = fopen(dicpath, "r");
    if (!fp) {
	    free(output);
	    free(line);
	    return NULL;
    }

    while (fgets(line, MAXLINE, fp) != NULL) {
	    size_t token_len = strcspn(line, " \t\n");
	    char *first = (char*) malloc((token_len + 1) * sizeof(char));

	    strncpy(first, line, token_len);
	    first[token_len] = '\0';

	    if (islike(first, input)) {
		    sprintf(line, "%s\n\n", line);
		    strcat(output, line);
	    }

	    free(first);
    }

    fclose(fp);
    free(line);


    if (empty(output)) {
	    strcpy(output, "Nehuma definição encontrada para o termo pesquisado.\n\n");
    }

    return output;
}
