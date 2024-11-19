/* TPA: Processamento do Tupi Antigo */

#include "interface.h"
#include "locale.h"

/* 
 * Função principal que inicia a aplicação
 */
int main(int argc, char **argv) 
{
    setlocale(LC_ALL, "Portuguese");

    int status;

    status = initApp(argc, argv);

    return status;
}

