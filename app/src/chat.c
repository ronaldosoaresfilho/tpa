#include "common.h"
#include "chat.h"

// Função de conversa
char *chat(const char *text)
{
    const char *input_file = "input.txt";
    const char *output_file = "output.txt";
    const char *chat_script = "app/src/chat.py";
    FILE *fp;

    const char *input_data = (char*) text;

    fp = fopen(input_file, "w");

    fprintf(fp, "%s\n", input_data);
    fclose(fp);

    char command[256];
    snprintf(command, sizeof(command), "python %s", chat_script);
    system(command);

    fp = fopen(output_file, "r");

    char output[256];
    if (fgets(output, sizeof(output), fp) != NULL) {
        //printf("Saída do Python: %s\n", output);
    }else {
        perror("Erro ao ler a saída do arquivo");
    }
    fclose(fp);

    remove(input_file);
    remove(output_file);

    char *t = (char*) malloc(LINESIZE * sizeof(char));
    sprintf(t, "%s", output);

    return t;
}
