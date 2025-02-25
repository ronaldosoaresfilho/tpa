#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "parse.h"

#define MAXWORD 64

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
