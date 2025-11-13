#include <stdio.h>
#include "../include/file_reading.h"
#include "../include/file_format.h"

void verificarArquivoBinario(const char* arquivoBinario) {
    FILE *f = fopen(arquivoBinario, "rb");
    if (!f) {
        printf("Erro: não foi possível abrir %s\n", arquivoBinario);
        return;
    }

    Alimento a;
    int i = 0;
    while (fread(&a, sizeof(Alimento), 1, f) == 1) {
        printf("Registro %d: codigo=%d nome=\"%s\" calorias=%.2f proteinas=%.2f categoria=\"%s\"\n",
               i + 1, a.codigo, a.nome, a.calorias, a.proteinas, a.categoria);
        i++;
    }
    printf("Total de registros no binário: %d\n", i);
    fclose(f);
}




