#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lib/tpa.h"

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	char *input, *output;

    while (true) {
        input = readline(">>> "); 

        if (!input) {  
            printf("\n...\n");
            break;
        }else { 
            add_history(input);
        }

		if (quit(input)) {
            printf("Tiá!\n");
            break;
        }
		
		output = process(input);
        printf("\n%s", output);
        
		free(input);
		free(output);
    }

    return 0;
}