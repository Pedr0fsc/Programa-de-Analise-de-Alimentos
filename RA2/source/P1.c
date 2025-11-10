#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/file_format.h"
#include "./include/file_reading.h"

// Função principal para chamar os métodos
int main() {
    const char* arquivo_entrada = "trabalho03-teste.csv"; // Nome do arquivo CSV
    const char* arquivo_saida = "dados.bin"; // Nome do arquivo binário

    printf("=== Programa de Conversão de Dados de Alimentos ===\n");
    printf("Convertendo dados de %s para %s...\n", arquivo_entrada, arquivo_saida);

    int resultado = lerCSVpraBIN(arquivo_entrada, arquivo_saida);

    if (resultado > 0) {
        printf("Conversão realizada com sucesso!\n");
        printf(" %d alimentos foram convertidos e salvos em %s\n", resultado, arquivo_saida);
        verificarArquivoBinario(arquivo_saida);
    } else {
        printf("Nenhum dado foi convertido. Verifique o arquivo de entrada.\n");
    }

    return 0;
}

    