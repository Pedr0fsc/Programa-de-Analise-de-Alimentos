#ifndef CLASSIFIER_H 
#define CLASSIFIER_H 
#include "data_structures.h"

// O enum Categoria é importado do data_structures.h

// Protótipo da função de inferência (usada no P1 para determinar a categoria)
Categoria inferirCategoria(const char *nome);
Categoria inferirCategoriaPelaString(const char *categoria_str);

#endif