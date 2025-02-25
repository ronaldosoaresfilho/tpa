#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lib/tpa.h"

enum Mode {RET, NORM, DIC, TRANS};

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	char *input, *output;
    enum Mode m = NORM;

    info();

    while (true) {
        input = readline(">>> "); 

        if (!input) {  
            printf("\n...\n");
            break;
        }else { 
            add_history(input);
        }

		if (quit(input)) {
            printf("\n| Tiá!\n\n");
            m = RET;
            break;
        }

        if (islike("inf", input)) {
            info();
            free(input);
            continue;
        }else if (islike("tpa", input)) {
            m = NORM;
            printf("\n| Modo normal\n\n");
            free(input);
            continue;
        }else if (islike("tpd", input)) {
            m = DIC;
            printf("\n| Modo dicionário\n\n");
            free(input);
            continue;
        }else if (islike("tpt", input)) {
            m = TRANS;
            printf("\n| Modo tradutor\n\n");
            free(input);
            continue;
        }

        switch (m) {
            case RET:
                break;
            case NORM:
                printf("\n| %s\n\n", input);
                free(input);
                continue;
            case DIC:
                output = searchdic(input);
                printf("\n%s", output);
                break;
            case TRANS:
                output = translate(input);
                printf("\n| %s\n\n", output);
                break;
        }

		free(input);
		free(output);
    }

    return m;
}