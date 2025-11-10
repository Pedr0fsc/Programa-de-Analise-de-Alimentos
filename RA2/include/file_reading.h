// *** AJUSTE 3: Troca do HEADER GUARD para evitar conflito ***
#ifndef FILE_READING_H 
#define FILE_READING_H 

#include "file_format.h" // Inclui a definição da struct Alimento e das funções de string

// Protótipos das funções
// A função ignorarPrimeiraLinha foi removida ou você pode atualizar o protótipo
void ignorarPrimeiraLinha(FILE *arquivo_csv); 
int lerCSVpraBIN(const char *arquivoCSV, const char *arquivoBIN);
void verificarArquivoBinario(const char* arquivoBinario);

#endif