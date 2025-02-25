#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <locale.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lib/parse.h"
#include "lib/chat.h"
#include "lib/dictionary.h"
#include "lib/translator.h"

enum Mode {RET, CHAT, DIC, TRANS};

void info(void);
bool quit(char *input);

int main(void)
{
	setlocale(LC_ALL, "Portuguese");
	char *input, *output;
    enum Mode m = CHAT;

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
            m = CHAT;
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
            case CHAT:
                output = chat(input);
                printf("\n| %s\n\n", output);
                break;
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


void info(void)
{
	printf("\n| TPA - Processador de Tupi Antigo - 1.0\n");
	printf("| tpa - entre no modo conversa\n");
	printf("| tpd - entre no modo dicionário\n");
	printf("| tpt - entre no modo tradutor\n");
	printf("| inf - mostre esta informação\n\n");
}

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