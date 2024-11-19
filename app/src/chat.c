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
    static char result[1024];  // Buffer para armazenar a saída do comando

    // Monta o comando para executar o script Python
    snprintf(command, sizeof(command), "python %s", train_script);

    // Abre um pipe para capturar a saída do comando
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Erro ao executar o script de treino");
        return "Erro ao executar o script de treino";
    }

    // Lê a saída do comando e armazena no buffer 'result'
    result[0] = '\0';  // Garante que o buffer esteja vazio
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strncat(result, buffer, sizeof(result) - strlen(result) - 1);
    }

    // Fecha o pipe
    if (pclose(fp) == -1) {
        perror("Erro ao fechar o pipe");
        return "Erro ao fechar o pipe";
    }

    // Retorna a saída do comando
    return result;
}