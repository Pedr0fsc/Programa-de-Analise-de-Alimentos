#include <stdio.h>
#include <stdlib.h>

#include "../include/p2.h"
#include "../include/p2_impl.h"

// Main do programa P2 — fica somente aqui
int main(void) {
    const char *arquivo_bin = "dados.bin";
    CategoryNode *categories = NULL;

    carregarDadosBinario(arquivo_bin, &categories);

    executarInterface(&categories);

    salvarDadosBinario(arquivo_bin, categories);

    liberarTudo(&categories);

    return 0;
}
  