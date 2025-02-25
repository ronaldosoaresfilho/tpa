#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "translator.h"

#define MAXLEN  32768

// tranlate input
char *translate(char *input)
{
	char *output = (char*) malloc(MAXLEN * sizeof(char));
	
	strcpy(output, input);
	
	return output;
}
