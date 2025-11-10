#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_reading.h"
#include "../include/file_format.h"


//  A função ignorarPrimeiraLinha não deve reabrir/declarar FILE* 
void ignorarPrimeiraLinha(FILE *arquivo_csv) {
    char linha[512];
    // A função apenas lê uma linha, avançando o ponteiro de arquivo já aberto.
    if (fgets(linha, sizeof(linha), arquivo_csv) == NULL) {
        printf("Aviso: O arquivo CSV está vazio ou o cabeçalho não foi lido.\n");
    }
}

int lerCSVpraBIN(const char *arquivoCSV, const char *arquivoBIN) {
    Alimento alimento;
    int contador = 0;
    FILE *arquivo_csv, *arquivo_binario;
    char linha[512];
    const char *delimitador = ";"; // Delimiter do csv

    arquivo_csv = fopen(arquivoCSV, "r");
    if (arquivo_csv == NULL) {
        printf("Erro: Não foi possível abrir o arquivo %s\n", arquivoCSV);
        return -1;
    }
    
    ignorarPrimeiraLinha(arquivo_csv);

    arquivo_binario = fopen(arquivoBIN, "wb");
    if (arquivo_binario == NULL) {
        printf("Erro: Não foi possível criar o arquivo %s\n", arquivoBIN);
        fclose(arquivo_csv);
        return -1;
    }

    // Leitura e processamento das linhas do CSV
    while (fgets(linha, sizeof(linha), arquivo_csv) != NULL) {
        removerQuebraLinha(linha); // Remove quebras de linha no final
        char *token;
        int campo_atual = 0;

        // Inicializa com valores padrão. Isso preenche os campos ausentes (Carboidratos, Gorduras, Fibras) com 0.0
        alimento.codigo = 0;
        alimento.nome[0] = '\0';
        alimento.calorias = 0.0;
        alimento.proteinas = 0.0;

        token = strtok(linha, delimitador); // Primeira chamada com ';'
        while (token) {
            removerQuebraLinha(token);
            removerAspas(token);
            substituirVirgula(token);

            // Processa APENAS os campos existentes (0, 1, 2, 3 e a 'Categoria' 4)
            if (campo_atual == 0) {
                alimento.codigo = atoi(token);
            } else if (campo_atual == 1) {
                strncpy(alimento.nome, token, sizeof(alimento.nome) - 1);
                alimento.nome[sizeof(alimento.nome) - 1] = '\0';
            } else if (campo_atual == 2) {
                alimento.calorias = atof(token);
            } else if (campo_atual == 3) {
                alimento.proteinas = atof(token);
            }
            // O campo 4 (Categoria) e os seguintes (5 e 6) são ignorados ou permanecem 0.0/vazio.

            token = strtok(NULL, delimitador); // *** CORREÇÃO 2.B: Usando o delimitador correto (';') ***
            campo_atual++;
        }

        // *** CORREÇÃO 1: Ajusta a condição para o número REAL de campos (5) ***
        // O CSV tem 5 campos. Se o campo_atual for 5, a linha foi lida corretamente.
        if (campo_atual >= 4) { // Pelo menos 4 campos úteis (código, nome, calorias, proteínas)
            if (fwrite(&alimento, sizeof(Alimento), 1, arquivo_binario) != 1) {
                printf("Erro ao escrever dados no arquivo binário\n");
                break;
            }
            contador++;
        } else {
             // Imprime o erro apenas para linhas que não conseguiram 4 campos
             // (o que indica uma linha vazia ou muito mal formatada)
             // A Categoria (campo 4) é ignorada mas a linha é válida.
             if (campo_atual > 0) { 
                 printf("Erro ao processar linha (campos insuficientes/formato incorreto): %s\n", linha);
             }
        }
    }

    fclose(arquivo_csv);
    fclose(arquivo_binario);

    return contador;
}

// ... (verificarArquivoBinario não precisa de correção)