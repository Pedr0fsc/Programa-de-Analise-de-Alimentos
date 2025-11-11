#ifndef FILE_READING_H
#define FILE_READING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_format.h"
#include "formatter.h"

// Lê e ignora a primeira linha (cabeçalho) do CSV
void ignorarPrimeiraLinha(FILE *arquivo_csv) {
    char linha[512];
    if (fgets(linha, sizeof(linha), arquivo_csv) == NULL) {
        printf("Aviso: O arquivo CSV está vazio ou o cabeçalho não foi lido.\n");
    }
}

// Converte CSV → BIN
int lerCSVpraBIN(const char *arquivoCSV, const char *arquivoBIN) {
    Alimento alimento;
    int contador = 0;
    FILE *arquivo_csv, *arquivo_binario;
    char linha[512];
    const char *delimitador = ";";

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

    while (fgets(linha, sizeof(linha), arquivo_csv) != NULL) {
        removerQuebraLinha(linha);
        char *token;
        int campo_atual = 0;

        alimento.codigo = 0;
        alimento.nome[0] = '\0';
        alimento.calorias = 0.0;
        alimento.proteinas = 0.0;

        token = strtok(linha, delimitador);
        while (token) {
            removerQuebraLinha(token);
            removerAspas(token);
            substituirVirgula(token);

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

            token = strtok(NULL, delimitador);
            campo_atual++;
        }

        if (campo_atual >= 4) {
            if (fwrite(&alimento, sizeof(Alimento), 1, arquivo_binario) != 1) {
                printf("Erro ao escrever dados no arquivo binário\n");
                break;
            }
            contador++;
        } else if (campo_atual > 0) {
            printf("Erro ao processar linha (campos insuficientes): %s\n", linha);
        }
    }

    fclose(arquivo_csv);
    fclose(arquivo_binario);

    return contador;
}

// Verifica o conteúdo do arquivo binário gerado
void verificarArquivoBinario(const char* arquivoBinario) {
    FILE *arquivo = fopen(arquivoBinario, "rb");
    if (arquivo == NULL) {
        printf("Erro: Não foi possível abrir o arquivo binário %s\n", arquivoBinario);
        return;
    }

    Alimento alimento;
    int contador = 0;

    printf("\n=== Conteúdo do arquivo binário ===\n");

    while (fread(&alimento, sizeof(Alimento), 1, arquivo) == 1) {
        printf("Código: %d | Nome: %s | Calorias: %.2f | Proteínas: %.2f\n",
               alimento.codigo, alimento.nome, alimento.calorias, alimento.proteinas);
        contador++;
    }

    printf("\nTotal de registros lidos: %d\n", contador);
    fclose(arquivo);
}

#endif