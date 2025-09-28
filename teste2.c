#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <locale.h> // biblioteca para definir a leitura do sistema para UTF-8
#include <string.h>


const int TBUFFER = 1024 * 1024; // 1MB // Tamanho do array buffer para armazenar os dados do arquivo


int lerArquivo() {

    FILE *fptr;
    char buffer[TBUFFER];
    int coluna = 0;
    int linha_num = 0;

    fptr = fopen("lista_de_alimentos.csv", "r");

    if (fptr == NULL) {
        perror("Erro ao abrir o arquivo: { lista_de_alimentos.csv }\n"); 
        return 1;
    }
    else {
        printf("Arquivo aberto com sucesso... 🚀\n");

        while (fgets(buffer,TBUFFER,fptr) != NULL) {

            // Remove o caractere de nova linha (\n) do final do buffer
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0'; 
            }

            coluna = 0; 
            linha_num++;

            if (linha_num == 1) { // Se for a primeira linha (cabeçalho), eh pra ignorar e continuar
                continue;
            }
            
            char* valor = strtok(buffer, ",");

            printf("Linha %d | ", linha_num); // Mostra a Linha atual

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
                        printf("Categoria: %s", valor); // Sem '\n' aqui!
                        break;
                    default:
                        // Não faz nada em colunas extras
                        break; 
                }
                //Espaço em branco após cada valor
                printf(" ");
                valor = strtok(NULL, ","); 
                coluna++;
            }
            printf("\n"); 
            
        }
        fclose(fptr); 
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