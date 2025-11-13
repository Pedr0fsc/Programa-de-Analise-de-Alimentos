#ifndef P2_H
#define P2_H

#include <stdio.h>
#include "file_format.h"

/* Requisito 1: uso de um enumerado associado às categorias.
   Como o número e os nomes das categorias são dinâmicos, o enum
   serve aqui como rótulo/flag e é mantido em cada nó de categoria. */
typedef enum {
    CAT_DYNAMIC = 0
} CategoryEnum;

/* Nó de lista de alimentos (ordenada alfabeticamente por nome) */
typedef struct FoodNode {
    Alimento data;
    struct FoodNode *next;
} FoodNode;

/* Nó de índice (árvore binária) para energia/proteína.
   Cada nó aponta para o FoodNode correspondente. */
typedef struct IndexNode {
    float key;
    FoodNode *food;
    struct IndexNode *left;
    struct IndexNode *right;
} IndexNode;

/* Nó de lista de categorias (ordenada alfabeticamente por nome).
   Cada categoria possui lista ligada de alimentos e duas árvores
   de indexação (energia e proteína). */
typedef struct CategoryNode {
    CategoryEnum tag;       // ENUM associado (requisito 1)
    char name[100];
    FoodNode *foods;        // lista ligada de alimentos (ordenada por nome)
    IndexNode *energyRoot;  // árvore indexando por calorias
    IndexNode *proteinRoot; // árvore indexando por proteínas
    struct CategoryNode *next;
} CategoryNode;

/* Protótipos principais usados por P2 */
void carregarDadosBinario(const char *arquivoBin, CategoryNode **categories);
void liberarTudo(CategoryNode **categories);
void salvarDadosBinario(const char *arquivoBin, CategoryNode *categories);
void executarInterface(CategoryNode **categories);

#endif