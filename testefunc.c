#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h> // biblioteca para definir a leitura do sistema para UTF-8
#include <string.h>


const int TBUFFER = 1024 * 1024; // 1MB // Tamanho do array buffer para armazenar os dados do arquivo

int lerArquivo() {

    FILE *fptr; // ponteiro para armazenar o valor retornado pelo fopen()
    
    //---VARIAVEIS DE CONTROLE---
    char buffer[TBUFFER]; // buffer para armazenar os dados do arquivo
    int coluna = 0; // variavel para controlar a coluna do arquivo
    int linha_num = 0; // variavel para controlar a linha do arquivo

    // abrindo o arquivo no modo de leitura
    fptr = fopen("lista_de_alimentos.csv", "r");

    // verificando se o arquivo foi aberto com sucesso
    if (fptr == NULL) {
        perror("Erro ao abrir o arquivo: { lista_de_alimentos.csv }\n"); // mensagem de erro pro usuario
        return 1; // retornando 1 para indicar erro
    }
    else {
        printf("Arquivo aberto com sucesso... 🚀\n"); // mensagem de sucesso pro usuario

        // ler os dados do arquivo
        // usando o metodo fgets() para ler uma linha do arquivo
        while (fgets(buffer,TBUFFER,fptr) != NULL) {

            coluna = 0; // estamos no inicio da linha, entao a coluna eh 0
            linha_num++; // incrementa a linha

            // usando o metodo strtok() para separar os dados da linha
            char* valor = strtok(buffer, ","); // separa os dados da linha a cada virgula

            while (valor){
                switch (coluna) {
                    case 0:
                        printf("Numero do alimento: %s | ", valor);
                        break;
                    case 1:
                        printf("Descricao do alimento: %s | ", valor);
                        break;
                    case 2:
                        printf("Umidade: %s | ", valor);
                        break;
                    case 3:
                        printf("Energia (kcal): %s | ", valor);
                        break;
                    case 4:
                        printf("Proteina (g): %s | ", valor);
                        break;
                    case 5:
                        printf("Carboidrato (g): %s | ", valor);
                        break;
                    case 6:
                        printf("Categoria: %s \n", valor);
                        break;
                    default:
                        printf("Coluna invalida\n");
                        break;
                }
            }
            
        }
        fclose(fptr); // fechando o arquivo
        printf("\nArquivo fechado com sucesso... ✅\n");
    }
    return 0;
}
// fim da funcao lerArquivo

int main() {

    setlocale(LC_ALL, "C.UTF-8"); // define a localidade do sistema para UTF-8

    lerArquivo(); // chamando a funcao lerArquivo
    return 0;
}   