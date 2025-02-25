#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "chat.h"

#define MAXLEN  32768

// chat with user
char *chat(char *input)
{
	char *output = (char*) malloc(MAXLEN * sizeof(char));
	
	strcpy(output, input);
	
	return output;
}
