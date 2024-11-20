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

char *train(void)
{
    char command[256];
    const char *train_script = "app/src/train.py";
 
    snprintf(command, sizeof(command), "python %s", train_script);
    system(command);

    // se há o arquivo train_end.txt e se ele contem "ok" 
    FILE *fp;
    char *train_end = "train_end.txt";
    char *t = (char*) malloc(LINESIZE * sizeof(char));

    fp = fopen(train_end, "r");

    if (fp != NULL) {   
        sprintf(t, "%s", "Treino finalizado!");
        remove(train_end);
        fclose(fp);
    }else {
        sprintf(t, "%s", "Não houve treino!");
    }
    
    return t;
}