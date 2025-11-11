#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CATEGORIA_DESCONHECIDA = 0, // ID 0 para desconhecida
    CEREAIS = 1, VERDURAS, FRUTAS, GORDURAS, PESCADOS,
    CARNES, LACTEOS, BEBIDAS, OVOS, ACUCARES,
    MISCELANEAS, INDUSTRIALIZADOS, PREPARADOS, LEGUMINOSAS, SEMENTES // sementes = 15
} Categoria;

#define MAX_DESC 50 
#define NUM_CATEGORIAS 15

/* Estrutura Auxiliar para Leitura Binária (Exatamente igual ao struct Alimento do P1) */
typedef struct {
    int codigo;
    char descricao[50]; 
    float energia; 
    float proteina;
    int categoria_id;
} __attribute__((packed)) AlimentoBin;

/* Estrutura de dados completa para uso no P2 */
typedef struct {
    int codigo;
    char descricao[50]; 
    float energia; 
    float proteina; 
    int categoria_id; 
    Categoria categoria; // Campo extra: calculado ou convertido APÓS A LEITURA.
} AlimentoDados;


/* --- Estrutura de Lista Ligada de Alimentos (Nó de Alimento) --- */
typedef struct NoAlimento {
    AlimentoDados dados;
    struct NoAlimento *prox; 
    void *no_arvore_energia; 
    void *no_arvore_proteina;
} NoAlimento;

/* --- Estrutura de Nó de Árvore Binária de Indexação --- */
typedef struct NoArvore {
    float chave; 
    NoAlimento *ptr_alimento; 
    struct NoArvore *esq;
    struct NoArvore *dir;
} NoArvore;

/* --- Estrutura de Nó de Lista Ligada de Categorias --- */
typedef struct NoCategoria {
    Categoria id;
    char nome[MAX_DESC]; 

    NoAlimento *lista_alimentos_head; 
    int total_alimentos;

    NoArvore *arvore_energia_root; 
    NoArvore *arvore_proteina_root; 

    struct NoCategoria *prox; 
} NoCategoria;


/* --- Protótipos de Funções --- */

/* Funções de Utilidade */
const char* obterNomeCategoria(Categoria cat);
Categoria intParaCategoria(int cat_num);
void limparBufferEntrada(void);
void pausar(void);
void exibirMenu(void);
// ...

/* I/O e Construção de Estruturas */
int lerDadosBinario(const char *nome_arquivo, NoCategoria **lista_categorias_head);
int salvarDadosBinario(const char *nome_arquivo, NoCategoria *lista_categorias_head);
void liberarListaCategorias(NoCategoria *head);
void reconstruirArvoresCategoria(NoCategoria *no_cat); // Para Opção 8

/* Funções de Busca/Inserção/Remoção nas Estruturas */
NoCategoria* buscarCategoriaPorID(NoCategoria *head, Categoria id, NoCategoria **anterior);
// ... demais protótipos de manipulação de listas e árvores (seriam implementados em outros .c)

/* Funções de Menu (Implementadas em outro .c) */
void listarCategorias(NoCategoria *head);
void listarAlimentosPorLista(NoCategoria *no_cat);
void listarAlimentosPorEnergia(NoCategoria *no_cat);
void listarAlimentosPorProteina(NoCategoria *no_cat);
void listarAlimentosPorEnergiaIntervalo(NoCategoria *no_cat);
void listarAlimentosPorProteinaIntervalo(NoCategoria *no_cat);
void removerCategoria(NoCategoria **head, int *alteracao_dados);
void removerAlimentoEspecifico(NoCategoria *head, int *alteracao_dados);

#endif /* DATA_STRUCTURES_H */