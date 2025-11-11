#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_reading.h"
#include "../include/file_format.h"
#include "../include/classifier.h"
// #include "../include/data_structures.h"

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
    int num_linha_lida = 1;
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
        num_linha_lida++;
        removerQuebraLinha(linha); // Remove quebras de linha no final
        char *token;
        int campo_atual = 0;
        char categoria_str[50] = ""; // Variável auxiliar para a string da categoria

        // Inicializa com valores padrão. Isso preenche os campos ausentes (Carboidratos, Gorduras, Fibras) com 0.0
        alimento.codigo = 0;
        alimento.nome[0] = '\0';
        alimento.calorias = 0.0;
        alimento.proteinas = 0.0;
        alimento.categoria_id = CATEGORIA_DESCONHECIDA;

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
            } else if (campo_atual == 4) {
                strncpy(categoria_str, token, sizeof(categoria_str) - 1);
                categoria_str[sizeof(categoria_str) - 1] = '\0';
                break;
            }
            // Ignora campos 5 em diante

            token = strtok(NULL, delimitador); // *** CORREÇÃO 2.B: Usando o delimitador correto (';') ***
            campo_atual++;
        }

        if (categoria_str[0] != '\0') {
            alimento.categoria_id = inferirCategoriaPelaString(categoria_str);
        }

        if (alimento.codigo > 0 && alimento.categoria_id != CATEGORIA_DESCONHECIDA) {
            printf("--- DEBUG ESCRITA P1 (REG %d) ---\n", contador + 1);
            printf("  Escrevendo Código: %d\n", alimento.codigo);
            printf("  Escrevendo Descrição: %s\n", alimento.nome);
            printf("  Escrevendo Categoria ID: %d\n", alimento.categoria_id);

            if (fwrite(&alimento, sizeof(Alimento), 1, arquivo_binario) != 1) {
                printf("Erro ao escrever dados no arquivo binário\n");
                break;
            }
            contador++;
        } else {
            // Este é o único debug que mostrará as 99 linhas ignoradas.
            printf("\n--- ALERTA: LINHA IGNORADA (CSV Linha #%d) ---\n", num_linha_lida); 
            printf("Motivo: CÓDIGO/CATEGORIA Inválido (Cód: %d, Cat ID: %d)\n", alimento.codigo, alimento.categoria_id);
            printf("Linha RAW:\n>>> %s\n", linha);
            printf("--------------------------------------\n");
        }
    }

    fclose(arquivo_csv);
    fclose(arquivo_binario);

    return contador;
}

void verificarArquivoBinario(const char* arquivoBinario) {
    Alimento alimento;
    FILE *arquivo_bin = fopen(arquivoBinario, "rb");

    if (arquivo_bin == NULL) {
        printf("Aviso: Não foi possível abrir o arquivo binário para verificação.\n");
        return;
    }

    printf("\n--- Conteúdo do arquivo binário (Primeiros 5 alimentos) ---\n");
    printf("| Codigo | Nome (Truncado) | Calorias | Proteinas |\n");
    printf("|--------|-----------------|----------|-----------|\n");

    int i = 0;
    // Tenta ler até 5 registros
    while (i < 5 && fread(&alimento, sizeof(Alimento), 1, arquivo_bin) == 1) {
        char nome_truncado[18];
        strncpy(nome_truncado, alimento.nome, 17);
        nome_truncado[17] = '\0';
        printf("| %6d | %-15s | %8.2f | %9.2f |\n",
               alimento.codigo, nome_truncado, alimento.calorias, alimento.proteinas);
        i++;
    }

    if (i == 0) {
        printf("Não foi possível ler nenhum registro do arquivo binário.\n");
    }

    printf("----------------------------------------------------------\n");
    fclose(arquivo_bin);
}