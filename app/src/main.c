#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "lib/parse.h"
#include "lib/chat.h"
#include "lib/dictionary.h"
#include "lib/translator.h"

#define PORT 8080
#define BUFFER_SIZE 8192

enum Mode {RET, CHAT, DIC, TRANS};
enum Mode m = CHAT;

static char *getHelp(void)
{
  char *output = strdup(
    "<h3>Processador de Tupi Antigo - Português:</h3><br>"
    "<p>tpp - exibe essa ajuda</p>"
    "<p>tpa - modo de conversa</p>"
    "<p>tpd - modo dicionário</p>"
    "<p>tpt - modo tradutor</p><br>"
    "<h3>Créditos:</h3>"
    "<p>NAVARRO, Eduardo de A., Método Moderno de Tupi Antigo - A língua do Brasil dos Primeiros Séculos. São Paulo, Editora Global, 2006, 3ª edição.</p>"
    "<p>NAVARRO, Eduardo de A., Dicionário tupi antigo: a língua indígena clássica do Brasil : vocabulário português-tupi e dicionário tupi-português, tupinismos no português do Brasil, etimologias de topônimos e antropônimos de origem tupi. São Paulo: Global.</p><br>"
    "<h3>Versão:</h3>"
    "<p>tpx_www_1.0.0</p>"
  );

  return output;
}

static void urldecode(char *src)
{
    char *dest = src;
    while (*src) {
        if (*src == '%') {
            int code;
            if (sscanf(src + 1, "%2x", &code) == 1) {
                *dest++ = code;
                src += 3;
            }
        } else if (*src == '+') {
            *dest++ = ' ';
            src++;
        } else {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
}

// Função para enviar arquivos estáticos
static void send_file(int client_socket, const char *file_path, const char *content_type)
{
    FILE *file = fopen(file_path, "r");
    if (!file) {
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        write(client_socket, not_found, strlen(not_found));
        close(client_socket);
        return;
    }

    char response[BUFFER_SIZE] = "HTTP/1.1 200 OK\r\nContent-Type: ";
    strcat(response, content_type);
    strcat(response, "\r\nConnection: close\r\n\r\n");
    write(client_socket, response, strlen(response));

    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    fclose(file);
    close(client_socket);
}

// Função para processar o input e retornar resposta
static void process_input(int client_socket, char *input)
{
    char *output = NULL;

    if (strcmp(input, "tpa") == 0) {
        m = CHAT;
        output = strdup("<p class=\"center\">Modo normal ativado.</p>");
    } else if (strcmp(input, "tpd") == 0) {
        m = DIC;
        output = strdup("<p class=\"center\">Modo dicionário ativado.</p>");
    } else if (strcmp(input, "tpt") == 0) {
        m = TRANS;
        output = strdup("<p class=\"center\">Modo tradutor ativado.</p>");
    }else if (strcmp(input, "tpp") == 0) {
        output = getHelp();
    } else {
        switch (m) {
            case CHAT:
                output = chat(input);
                break;
            case DIC:
                output = searchdic(input);
                break;
            case TRANS:
                output = translate(input);
                break;
            default:
                output = strdup("Erro: modo desconhecido.");
        }
    }

    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Connection: close\r\n\r\n"
        "%s", output);

    write(client_socket, response, strlen(response));
    close(client_socket);
    free(output);
}

// Função para processar requisições
static void handle_request(int client_socket)
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer) - 1, 0);

    char method[8], path[256];
    sscanf(buffer, "%s %s", method, path);

    if (strcmp(path, "/") == 0) {
        send_file(client_socket, "app/src/public/index.html", "text/html");
    } else if (strcmp(path, "/style.css") == 0) {
        send_file(client_socket, "app/src/public/style.css", "text/css");
    } else if (strcmp(path, "/script.js") == 0) {
        send_file(client_socket, "app/src/public/script.js", "application/javascript");
    } else if (strcmp(path, "/process") == 0 && strcmp(method, "POST") == 0) {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4; // Pula os "\r\n\r\n"
            char *input = strstr(body, "user_input=");
            if (input) {
                input += 11;
                urldecode(input);
                process_input(client_socket, input);
                return;
            }
        }
    } else {
        const char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
        write(client_socket, not_found, strlen(not_found));
        close(client_socket);
    }
}

// Função principal: inicia o servidor
int main(void)
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Erro ao criar socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Erro ao associar socket");
        exit(1);
    }

    if (listen(server_socket, 10) == -1) {
        perror("Erro ao escutar conexões");
        exit(1);
    }

    printf("Servidor rodando em http://localhost:%d/\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Erro ao aceitar conexão");
            continue;
        }
        handle_request(client_socket);
    }

    close(server_socket);
    return 0;
}

/* cli version
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
*/
