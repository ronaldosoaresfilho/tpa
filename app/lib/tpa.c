#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "tpa.h"

#define MAXLINE 4096
#define MAXLEN  32768
#define MAXWORD 64

// dictionary path
const char *dicpath = "./app/data/dic.txt";

bool empty(char *str) 
{
	int i = 0;
	int len = strlen(str);

	for (i = 0; i < len; i++) {
		if (!str[i] != ' ' || !str[i] != '\n' || !str[i] != '\t') {
			return false;
		}
	}

	return true;
}

// compare an input with a sanitized a token
bool islike(char *token, char *input)
{
	char tmp[MAXWORD];
	int i = 0;

	while (*token) {
		if (*token == '-' || *token == '?' || *token == '!') {
			token++;
			continue;
		}
		tmp[i] = *token;
		i++;
		token++;
	}
	tmp[i] = '\0';

	if (strcmp(tmp, input) == 0) {
		return true;
	}

	return false;
}

void info(void)
{
	printf("\n| TPA - Processador de Tupi Antigo - 1.0\n");
	printf("| tpa - entre no modo normal\n");
	printf("| tpd - entre no modo dicionário\n");
	printf("| tpt - entre no modo tradutor\n");
	printf("| inf - mostre esta informação\n\n");
}

// tranlate input
char *translate(char *input)
{
	char *output = (char*) malloc(MAXLEN * sizeof(char));
	
	strcpy(output, input);
	
	return output;
}

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
		strcpy(output, "Nenhuma definição encontrada no dicionário!\n\n");
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
			sprintf(line, "%s\n", line);
			strcat(output, line);
		}
		
		free(first);
	}

	fclose(fp);
	free(line);

    return output;
}

// exit application
bool quit(char *input)
{
	if (strcmp(input, "sem") == 0 ||
		strcmp(input, "sair") == 0 ||
		strcmp(input, "s") == 0) {
		free(input);
		return  true;
	}	
	return false;
}