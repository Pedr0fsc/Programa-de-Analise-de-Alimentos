// p2_implementation.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/data_structures.h"
#include "file_format.h"

#define MAX_DESC 50 

const char* obterNomeCategoria(Categoria cat) {
    switch (cat) {
        case CEREAIS: return "Cereais e derivados";
        case VERDURAS: return "Verduras e hortaliças";
        case FRUTAS: return "Frutas e derivados";
        case GORDURAS: return "Gorduras e óleos";
        case PESCADOS: return "Pescados e frutos do mar";
        case CARNES: return "Carnes e derivados";
        case LACTEOS: return "Leite e derivados";
        case BEBIDAS: return "Bebidas (alcoólicas e não alcoólicas)";
        case OVOS: return "Ovos e derivados";
        case ACUCARES: return "Produtos açucarados";
        case MISCELANEAS: return "Miscelâneas";
        case PREPARADOS: return "Alimentos preparados";
        case LEGUMINOSAS: return "Leguminosas e derivados";
        case SEMENTES: return "Nozes e sementes";
        case INDUSTRIALIZADOS: return "Outros industrializados";
        default: return "Categoria Desconhecida"; // Categoria 0 (se for o caso)
    }
}

Categoria intParaCategoria(int cat_num) {
    if (cat_num >= CEREAIS && cat_num <= SEMENTES) {
        return (Categoria)cat_num;
    }
    return CATEGORIA_DESCONHECIDA;
}

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar(void) {
    printf("\nPressione ENTER para continuar...");
    limparBufferEntrada();
}

void exibirMenu(void) {
    printf("\n=== Menu de Gerenciamento ===\n");
    printf("1) Listar todas as categorias\n");
    printf("2) Listar alimentos de certa categoria (Ordem Alfabética)\n");
    printf("3) Listar alimentos por Energia (Decrescente)\n");
    printf("4) Listar alimentos por Proteína (Decrescente)\n");
    printf("5) Listar alimentos por Energia em Intervalo\n");
    printf("6) Listar alimentos por Proteína em Intervalo\n");
    printf("7) Remover Categoria\n");
    printf("8) Remover Alimento Específico\n");
    printf("9) Sair (salvar se houver remoção)\n");
    printf("Opção: ");
}

void imprimirCabecalhoTabela(void) {
    printf("\n+------+------------------------------------+---------+----------+-----------------------+\n");
    printf("| ID   | DESCRICAO                          | ENERGIA | PROTEINA | CATEGORIA             |\n");
    printf("+------+------------------------------------+---------+----------+-----------------------+\n");
}

Categoria inferirCategoria(const char *descricao) {
    char temp_desc[MAX_DESC];
    for (int i = 0; descricao[i] && i < MAX_DESC - 1; i++) {
        char c = tolower((unsigned char)descricao[i]);
        
        if (c == 'a' || c == 225 || c == 227 || c == 226) temp_desc[i] = 'a'; // á, ã, â (225, 227, 226 em Latin-1)
        else if (c == 'e' || c == 233 || c == 234) temp_desc[i] = 'e';        // é, ê (233, 234 em Latin-1)
        else if (c == 'i' || c == 237) temp_desc[i] = 'i';                    // í (237 em Latin-1)
        else if (c == 'o' || c == 243 || c == 245 || c == 244) temp_desc[i] = 'o'; // ó, õ, ô (243, 245, 244 em Latin-1)
        else if (c == 'u' || c == 250) temp_desc[i] = 'u';                    // ú (250 em Latin-1)
        else if (c == 231) temp_desc[i] = 'c';                                // ç (231 em Latin-1)
        else temp_desc[i] = c;
    }
    temp_desc[MAX_DESC - 1] = '\0';

    // 13. Alimentos preparados (PREPARADOS) 
    if (strstr(temp_desc, "acaraje") || strstr(temp_desc, "carreteiro") || strstr(temp_desc, "baiao") || 
        strstr(temp_desc, "feijoada") || strstr(temp_desc, "vatapa") || strstr(temp_desc, "cuscuz") || 
        strstr(temp_desc, "bolognesa") || strstr(temp_desc, "salada de") || strstr(temp_desc, "omelete") ||
        strstr(temp_desc, "estrogonofe") || strstr(temp_desc, "misto quente") || strstr(temp_desc, "torta")) {
        return PREPARADOS;
    }
    
    // 10. Produtos açucarados (ACUCARES)
    if (strstr(temp_desc, "acucar") || strstr(temp_desc, "doce") || strstr(temp_desc, "geleia") || 
        strstr(temp_desc, "mel") || strstr(temp_desc, "rapadura") || strstr(temp_desc, "chocolate") ||
        strstr(temp_desc, "condensado") || strstr(temp_desc, "quindim") || strstr(temp_desc, "pacoca")) { 
        return ACUCARES;
    }
    
    // 7. Leite e derivados (LACTEOS)
    if (strstr(temp_desc, "leite") || strstr(temp_desc, "queijo") || strstr(temp_desc, "iogurte") || 
        strstr(temp_desc, "mussarela") || strstr(temp_desc, "ricota") || strstr(temp_desc, "coalhada") ||
        strstr(temp_desc, "requeijao") || strstr(temp_desc, "creme de leite") || strstr(temp_desc, "lactea") || 
        strstr(temp_desc, "parmesao") || strstr(temp_desc, "sorvete")) { 
        return LACTEOS;
    }

    // 6. Carnes e derivados (CARNES) 
    if (strstr(temp_desc, "carne") || strstr(temp_desc, "frango") || strstr(temp_desc, "bovina") || 
        strstr(temp_desc, "suina") || strstr(temp_desc, "porco") || strstr(temp_desc, "presunto") || 
        strstr(temp_desc, "linguica") || strstr(temp_desc, "mortadela") || strstr(temp_desc, "salame") ||
        strstr(temp_desc, "peru") || strstr(temp_desc, "hamburguer") || strstr(temp_desc, "costela") || 
        strstr(temp_desc, "picanha") || strstr(temp_desc, "toucinho") || strstr(temp_desc, "charque")) { 
        return CARNES;
    }

    // 5. Pescados e frutos do mar (PESCADOS) 
    if (strstr(temp_desc, "peixe") || strstr(temp_desc, "pescado") || strstr(temp_desc, "camarao") || 
        strstr(temp_desc, "sardinha") || strstr(temp_desc, "atum") || strstr(temp_desc, "bacalhau") || 
        strstr(temp_desc, "salmao") || strstr(temp_desc, "marisco") || strstr(temp_desc, "tilapia")) {
        return PESCADOS;
    }
    
    // 4. Gorduras e óleos (GORDURAS)
    if (strstr(temp_desc, "oleo") || strstr(temp_desc, "margarina") || strstr(temp_desc, "azeite") || 
        strstr(temp_desc, "manteiga") || strstr(temp_desc, "gordura") || strstr(temp_desc, "banha") ||
        strstr(temp_desc, "dende") || strstr(temp_desc, "sebo")) {
        return GORDURAS;
    }

    // 15. Nozes e sementes (SEMENTES)
    if (strstr(temp_desc, "castanha") || strstr(temp_desc, "noz") || strstr(temp_desc, "amendoa") || 
        strstr(temp_desc, "semente") || strstr(temp_desc, "pinhao") || strstr(temp_desc, "gergelim") || 
        (strstr(temp_desc, "coco") && !strstr(temp_desc, "agua")) || strstr(temp_desc, "linhaca") || 
        strstr(temp_desc, "pistache")) { 
        return SEMENTES;
    }

    // 14. Leguminosas e derivados (LEGUMINOSAS)
    if (strstr(temp_desc, "feijao") || strstr(temp_desc, "soja") || strstr(temp_desc, "lentilha") || 
        strstr(temp_desc, "grao de bico") || strstr(temp_desc, "ervilha") || strstr(temp_desc, "fuba de soja")) {
        return LEGUMINOSAS;
    }

    // 8. Bebidas (BEBIDAS)
    if (strstr(temp_desc, "bebida") || strstr(temp_desc, "cerveja") || strstr(temp_desc, "vinho") || 
        strstr(temp_desc, "cafe") || strstr(temp_desc, "cha") || strstr(temp_desc, "refrigerante") ||
        strstr(temp_desc, "aguardente") || strstr(temp_desc, "licor") || strstr(temp_desc, "caldo de cana") || 
        strstr(temp_desc, "refri") || strstr(temp_desc, "cola")) {
        return BEBIDAS;
    }

    // 3. Frutas e derivados (FRUTAS)
    if (strstr(temp_desc, "fruta") || strstr(temp_desc, "suco") || strstr(temp_desc, "polpa") ||
        strstr(temp_desc, "laranja") || strstr(temp_desc, "maca") || strstr(temp_desc, "banana") || 
        strstr(temp_desc, "uva") || strstr(temp_desc, "mamao") || strstr(temp_desc, "morango") || 
        (strstr(temp_desc, "coco") && strstr(temp_desc, "agua")) || strstr(temp_desc, "limao") || 
        strstr(temp_desc, "manga") || strstr(temp_desc, "abacaxi") || strstr(temp_desc, "abacate")) { 
        return FRUTAS;
    }
    
    // 2. Verduras, hortaliças e derivados (VERDURAS) 
    if (strstr(temp_desc, "alface") || strstr(temp_desc, "couve") || strstr(temp_desc, "tomate") || 
        strstr(temp_desc, "cebola") || strstr(temp_desc, "cenoura") || strstr(temp_desc, "pimentao") || 
        strstr(temp_desc, "batata") || strstr(temp_desc, "mandioca") || strstr(temp_desc, "abobora") || 
        strstr(temp_desc, "alho") || strstr(temp_desc, "palmito") || strstr(temp_desc, "chuchu") || 
        strstr(temp_desc, "pepino") || strstr(temp_desc, "quiabo") || strstr(temp_desc, "brocolis") ||
        strstr(temp_desc, "farofa") || strstr(temp_desc, "fecula") || strstr(temp_desc, "broto")) { 
        return VERDURAS;
    }
    
    // 1. Cereais e derivados (CEREAIS)
    if (strstr(temp_desc, "arroz") || strstr(temp_desc, "trigo") || strstr(temp_desc, "aveia") || 
        strstr(temp_desc, "macarrao") || strstr(temp_desc, "milho") || strstr(temp_desc, "pao") || 
        strstr(temp_desc, "biscoito") || strstr(temp_desc, "massa") || strstr(temp_desc, "cereal") || 
        strstr(temp_desc, "amido") || strstr(temp_desc, "fuba") || strstr(temp_desc, "torrada") || 
        strstr(temp_desc, "pipoca") || strstr(temp_desc, "farinha de")) { 
        return CEREAIS;
    }

    // 9. Ovos e derivados (OVOS)
    if (strstr(temp_desc, "ovo") || strstr(temp_desc, "gema") || strstr(temp_desc, "clara")) {
        return OVOS;
    }

    // 11. Miscelâneas (MISCELANEAS)
    if (strstr(temp_desc, "fermento") || strstr(temp_desc, "sal") || strstr(temp_desc, "gelatina") ||
        strstr(temp_desc, "shoyu") || strstr(temp_desc, "tempero") || strstr(temp_desc, "pimenta") ||
        strstr(temp_desc, "maionese") || strstr(temp_desc, "mostarda")) {
        return MISCELANEAS;
    }
    
    // 12. Outros alimentos industrializados (INDUSTRIALIZADOS) 
    if (strstr(temp_desc, "industrializado") || strstr(temp_desc, "enlatado") || strstr(temp_desc, "conserva") || 
        strstr(temp_desc, "azeitona") || strstr(temp_desc, "chantilly") || strstr(temp_desc, "pate")) {
        return INDUSTRIALIZADOS;
    }

    return CATEGORIA_DESCONHECIDA;
}

void imprimirLinhaAlimento(const AlimentoDados *a) {
    char desc_truncada[MAX_DESC];
    strncpy(desc_truncada, a->descricao, 34);
    desc_truncada[34] = '\0';
    printf("| %4d | %-34s | %7.2f | %8.2f | %-21s |\n",
           a->codigo, desc_truncada, a->energia, a->proteina,
           obterNomeCategoria(a->categoria));
}

void imprimirRodapeTabela(int total) {
    printf("+------+------------------------------------+---------+----------+-----------------------+\n");
    printf("Total: %d alimento(s) listado(s)\n", total);
}

NoAlimento* criarNoAlimento(const AlimentoDados *dados) {
    NoAlimento *novo = (NoAlimento*)malloc(sizeof(NoAlimento));
    if (!novo) { perror("Erro alocacao NoAlimento"); exit(EXIT_FAILURE); }
    novo->dados = *dados;
    novo->prox = NULL;
    novo->no_arvore_energia = NULL;
    novo->no_arvore_proteina = NULL;
    return novo;
}

NoAlimento* inserirAlimentoOrdenado(NoAlimento **head, const AlimentoDados *dados, int *sucesso) {
    *sucesso = 0;
    NoAlimento *novo = criarNoAlimento(dados);

    if (*head == NULL || strcmp(dados->descricao, (*head)->dados.descricao) < 0) {
        novo->prox = *head;
        *head = novo;
    } else {
        NoAlimento *atual = *head;
        while (atual->prox != NULL && strcmp(dados->descricao, atual->prox->dados.descricao) > 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
    *sucesso = 1;
    return novo;
}

NoCategoria* criarNoCategoria(Categoria id) {
    NoCategoria *novo = (NoCategoria*)malloc(sizeof(NoCategoria));
    if (!novo) { perror("Erro alocacao NoCategoria"); exit(EXIT_FAILURE); }
    novo->id = id;
    strncpy(novo->nome, obterNomeCategoria(id), MAX_DESC - 1);
    novo->nome[MAX_DESC - 1] = '\0';
    novo->lista_alimentos_head = NULL;
    novo->total_alimentos = 0;
    novo->arvore_energia_root = NULL;
    novo->arvore_proteina_root = NULL;
    novo->prox = NULL;
    return novo;
}

NoCategoria* inserirCategoriaOrdenada(NoCategoria **head, Categoria id, int *sucesso) {
    *sucesso = 0;
    NoCategoria *atual_check = *head;
    while(atual_check != NULL) {
        if (atual_check->id == id) {
            *sucesso = 1;
            return atual_check;
        }
        atual_check = atual_check->prox;
    }

    NoCategoria *novo = criarNoCategoria(id);

    if (*head == NULL || id < (*head)->id) {
        novo->prox = *head;
        *head = novo;
    } else {
        NoCategoria *atual = *head;
        while (atual->prox != NULL && id > atual->prox->id) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
    *sucesso = 1;
    return novo;
}

NoCategoria* buscarCategoriaPorID(NoCategoria *head, Categoria id, NoCategoria **anterior) {
    *anterior = NULL;
    NoCategoria *atual = head;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        *anterior = atual;
        atual = atual->prox;
    }
    return NULL;
}

NoArvore* criarNoArvore(float chave, NoAlimento *ptr_alimento) {
    NoArvore *novo = (NoArvore*)malloc(sizeof(NoArvore));
    if (!novo) { perror("Erro alocacao NoArvore"); exit(EXIT_FAILURE); }
    novo->chave = chave;
    novo->ptr_alimento = ptr_alimento;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

NoArvore* inserirNaArvore(NoArvore *root, float chave, NoAlimento *ptr_alimento) {
    if (root == NULL) {
        return criarNoArvore(chave, ptr_alimento);
    }
    
    if (chave <= root->chave) {
        root->esq = inserirNaArvore(root->esq, chave, ptr_alimento);
    } else {
        root->dir = inserirNaArvore(root->dir, chave, ptr_alimento);
    }
    return root;
}

void liberarArvore(NoArvore *root) {
    if (root != NULL) {
        liberarArvore(root->esq);
        liberarArvore(root->dir);
        free(root);
    }
}

NoArvore* encontrarMinimo(NoArvore* node) {
    NoArvore* atual = node;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

NoArvore* removerNoArvoreEspecifico(NoArvore* root, float chave, NoAlimento* ptr_alimento_remover) {
    if (root == NULL) return root;

    if (chave < root->chave) {
        root->esq = removerNoArvoreEspecifico(root->esq, chave, ptr_alimento_remover);
    } else if (chave > root->chave) {
        root->dir = removerNoArvoreEspecifico(root->dir, chave, ptr_alimento_remover);
    } else {
        if (root->ptr_alimento == ptr_alimento_remover) {
            if (root->esq == NULL) {
                NoArvore *temp = root->dir;
                free(root);
                return temp;
            } else if (root->dir == NULL) {
                NoArvore *temp = root->esq;
                free(root);
                return temp;
            }

            NoArvore* temp = encontrarMinimo(root->dir);
            
            root->chave = temp->chave;
            root->ptr_alimento = temp->ptr_alimento;
            
            root->dir = removerNoArvoreEspecifico(root->dir, temp->chave, temp->ptr_alimento);
            
        } else {
            root->esq = removerNoArvoreEspecifico(root->esq, chave, ptr_alimento_remover);
            if (root->ptr_alimento != ptr_alimento_remover) {
                root->dir = removerNoArvoreEspecifico(root->dir, chave, ptr_alimento_remover);
            }
        }
    }
    return root;
}

void percorrerInversoCentral(NoArvore *root, int *contador) {
    if (root != NULL) {
        percorrerInversoCentral(root->dir, contador);
        imprimirLinhaAlimento(&(root->ptr_alimento->dados));
        (*contador)++;
        percorrerInversoCentral(root->esq, contador);
    }
}

void buscarIntervalo(NoArvore *root, float min_chave, float max_chave, int *contador) {
    if (root == NULL) return;
    if (min_chave < root->chave) {
        buscarIntervalo(root->esq, min_chave, max_chave, contador);
    }
    
    if (root->chave >= min_chave && root->chave <= max_chave) {
        imprimirLinhaAlimento(&(root->ptr_alimento->dados));
        (*contador)++;
    }

    if (max_chave > root->chave) {
        buscarIntervalo(root->dir, min_chave, max_chave, contador);
    }
}

int lerDadosBinario(const char *nome_arquivo, NoCategoria **lista_categorias_head) {
    FILE *f = fopen(nome_arquivo, "rb");
    if (!f) {
        printf("Aviso: Arquivo binário %s não encontrado ou vazio. Iniciando do zero.\n", nome_arquivo);
        return 0;
    }

    AlimentoBin alimento_bin_lido;
    AlimentoDados alimento_lido;
    int total_lido = 0;
    
    for (int i = CEREAIS; i <= SEMENTES; i++) {
        int sucesso;
        inserirCategoriaOrdenada(lista_categorias_head, (Categoria)i, &sucesso);
    }
    
    size_t tamanho_leitura = 68;

    int debug_count = 0;

    while (fread(&alimento_bin_lido, tamanho_leitura, 1, f) == 1) {
        debug_count++;
        printf("--- DEBUG LEITURA P2 (REG %d) ---\n", debug_count);
        printf("  Código lido: %d\n", alimento_bin_lido.codigo);

        char desc_debug[51]; 
        strncpy(desc_debug, alimento_bin_lido.descricao, 50);
        desc_debug[50] = '\0';
        printf("  Descrição lida: %s\n", desc_debug); 
        
        printf("  Energia lida: %.2f\n", alimento_bin_lido.energia);
        printf("  Categoria ID lida: %d\n", alimento_bin_lido.categoria_id);

        alimento_lido.codigo = alimento_bin_lido.codigo;
        strncpy(alimento_lido.descricao, alimento_bin_lido.descricao, sizeof(alimento_lido.descricao) - 1);
        alimento_lido.descricao[sizeof(alimento_lido.descricao) - 1] = '\0'; 
        
        alimento_lido.energia = alimento_bin_lido.energia;
        alimento_lido.proteina = alimento_bin_lido.proteina;
        alimento_lido.categoria_id = alimento_bin_lido.categoria_id;
        
        Categoria cat_enum = intParaCategoria(alimento_lido.categoria_id);
        
        alimento_lido.categoria = cat_enum;

        NoCategoria *anterior = NULL;
        NoCategoria *cat_destino = buscarCategoriaPorID(*lista_categorias_head, alimento_lido.categoria, &anterior);

        if (cat_destino) {
            int sucesso_alimento;
            NoAlimento *novo_alimento = inserirAlimentoOrdenado(&(cat_destino->lista_alimentos_head), &alimento_lido, &sucesso_alimento);
            
            if (novo_alimento && sucesso_alimento) {
                cat_destino->arvore_energia_root = inserirNaArvore(
                    cat_destino->arvore_energia_root, 
                    novo_alimento->dados.energia, 
                    novo_alimento
                );
                cat_destino->arvore_proteina_root = inserirNaArvore(
                    cat_destino->arvore_proteina_root, 
                    novo_alimento->dados.proteina, 
                    novo_alimento
                );
                cat_destino->total_alimentos++;
                total_lido++;
            }
        }
    }
    
    fclose(f);
    printf("Total de %d alimentos carregados e organizados.\n", total_lido);
    return total_lido;
}

void listarCategorias(NoCategoria *head) {
    printf("\n--- Categorias Cadastradas ---\n");
    NoCategoria *atual = head;
    while (atual != NULL) {
        printf("ID %2d: %-30s (Total: %d alimentos)\n", 
            atual->id, atual->nome, atual->total_alimentos);
        atual = atual->prox;
    }
    printf("------------------------------\n");
    pausar();
}

void listarAlimentosPorLista(NoCategoria *no_cat) {
    if (no_cat == NULL) return;
    
    printf("\n--- Alimentos da Categoria: %s (Ordem Alfabética) ---\n", no_cat->nome);
    imprimirCabecalhoTabela();
    
    NoAlimento *atual = no_cat->lista_alimentos_head;
    int contador = 0;
    
    while (atual != NULL) {
        imprimirLinhaAlimento(&(atual->dados));
        contador++;
        atual = atual->prox;
    }
    
    imprimirRodapeTabela(contador);
    pausar();
}

void listarAlimentosPorEnergia(NoCategoria *no_cat) {
    if (no_cat == NULL) return;
    
    printf("\n--- Alimentos da Categoria: %s (Ordem Decrescente de ENERGIA) ---\n", no_cat->nome);
    imprimirCabecalhoTabela();
    
    int contador = 0;
    percorrerInversoCentral(no_cat->arvore_energia_root, &contador);
    
    imprimirRodapeTabela(contador);
    pausar();
}

void listarAlimentosPorProteina(NoCategoria *no_cat) {
    if (no_cat == NULL) return;
    
    printf("\n--- Alimentos da Categoria: %s (Ordem Decrescente de PROTEÍNA) ---\n", no_cat->nome);
    imprimirCabecalhoTabela();
    
    int contador = 0;
    percorrerInversoCentral(no_cat->arvore_proteina_root, &contador);
    
    imprimirRodapeTabela(contador);
    pausar();
}

void listarAlimentosPorEnergiaIntervalo(NoCategoria *no_cat) {
    if (no_cat == NULL) return;
    
    float min_e, max_e;
    printf("\n[5] LISTAR POR ENERGIA EM INTERVALO\n");
    printf("Digite a Energia Mínima (kcal): ");
    if (scanf("%f", &min_e) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    printf("Digite a Energia Máxima (kcal): ");
    if (scanf("%f", &max_e) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    limparBufferEntrada();

    if (min_e > max_e) { 
        float temp = min_e; min_e = max_e; max_e = temp; 
        printf("(Intervalo corrigido: %.2f a %.2f kcal)\n", min_e, max_e);
    }

    printf("\n--- Alimentos da Categoria: %s (Energia entre %.2f e %.2f kcal) ---\n", 
           no_cat->nome, min_e, max_e);
    imprimirCabecalhoTabela();
    
    int contador = 0;
    buscarIntervalo(no_cat->arvore_energia_root, min_e, max_e, &contador);
    
    imprimirRodapeTabela(contador);
    pausar();
}

void listarAlimentosPorProteinaIntervalo(NoCategoria *no_cat) {
    if (no_cat == NULL) return;
    
    float min_p, max_p;
    printf("\n[6] LISTAR POR PROTEÍNA EM INTERVALO\n");
    printf("Digite a Proteína Mínima (g): ");
    if (scanf("%f", &min_p) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    printf("Digite a Proteína Máxima (g): ");
    if (scanf("%f", &max_p) != 1) { limparBufferEntrada(); printf("Entrada inválida.\n"); return; }
    limparBufferEntrada();

    if (min_p > max_p) { 
        float temp = min_p; min_p = max_p; max_p = temp; 
        printf("(Intervalo corrigido: %.2f a %.2f g)\n", min_p, max_p);
    }
    
    printf("\n--- Alimentos da Categoria: %s (Proteína entre %.2f e %.2f g) ---\n", 
           no_cat->nome, min_p, max_p);
    imprimirCabecalhoTabela();
    
    int contador = 0;
    buscarIntervalo(no_cat->arvore_proteina_root, min_p, max_p, &contador);
    
    imprimirRodapeTabela(contador);
    pausar();
}

void removerCategoria(NoCategoria **head, int *alteracao_dados) {
    int id_remover;
    printf("\n[7] REMOVER CATEGORIA\n");
    printf("Digite o ID da categoria a ser removida: ");
    if (scanf("%d", &id_remover) != 1) {
        limparBufferEntrada();
        printf("ID inválido.\n");
        pausar();
        return;
    }
    limparBufferEntrada();

    NoCategoria *anterior = NULL;
    NoCategoria *remover = buscarCategoriaPorID(*head, (Categoria)id_remover, &anterior);

    if (remover == NULL) {
        printf("Erro: Categoria com ID %d não encontrada.\n", id_remover);
        pausar();
        return;
    }

    if (anterior == NULL) {
        *head = remover->prox; // É o head
    } else {
        anterior->prox = remover->prox;
    }

    NoAlimento *atual = remover->lista_alimentos_head;
    NoAlimento *temp_alimento;
    int alimentos_removidos = 0;
    
    while (atual != NULL) {
        temp_alimento = atual;
        atual = atual->prox;
        free(temp_alimento);
        alimentos_removidos++;
    }

    liberarArvore(remover->arvore_energia_root);
    liberarArvore(remover->arvore_proteina_root);

    printf("Categoria '%s' (ID %d) removida com sucesso. (%d alimentos liberados)\n", 
           remover->nome, remover->id, alimentos_removidos);
    free(remover);

    *alteracao_dados = 1;
    pausar();
}

void removerAlimentoEspecifico(NoCategoria *head, int *alteracao_dados) {
    int id_remover;
    int id_cat;
    printf("\n[8] REMOVER ALIMENTO ESPECÍFICO\n");
    printf("Digite o ID da Categoria do alimento: ");
    if (scanf("%d", &id_cat) != 1) {
        limparBufferEntrada();
        printf("ID de Categoria inválido.\n");
        pausar();
        return;
    }
    printf("Digite o CÓDIGO do alimento a ser removido: ");
    if (scanf("%d", &id_remover) != 1) {
        limparBufferEntrada();
        printf("CÓDIGO de Alimento inválido.\n");
        pausar();
        return;
    }
    limparBufferEntrada();

    NoCategoria *anterior_cat = NULL;
    NoCategoria *cat = buscarCategoriaPorID(head, (Categoria)id_cat, &anterior_cat);

    if (cat == NULL) {
        printf("Erro: Categoria com ID %d não encontrada.\n", id_cat);
        pausar();
        return;
    }
    NoAlimento *atual = cat->lista_alimentos_head;
    NoAlimento *anterior = NULL;
    
    while (atual != NULL) {
        if (atual->dados.codigo == id_remover) {
            
            // Remove da lista
            if (anterior == NULL) {
                cat->lista_alimentos_head = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            cat->arvore_energia_root = removerNoArvoreEspecifico(
                cat->arvore_energia_root, 
                atual->dados.energia, 
                atual
            );
            
            cat->arvore_proteina_root = removerNoArvoreEspecifico(
                cat->arvore_proteina_root, 
                atual->dados.proteina, 
                atual
            );

            printf("Alimento '%s' (Código %d) da Categoria '%s' removido com sucesso.\n", 
                   atual->dados.descricao, atual->dados.codigo, cat->nome);
            cat->total_alimentos--;
            free(atual);
            *alteracao_dados = 1;
            pausar();
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    
    printf("Erro: Alimento com Código %d não encontrado na categoria '%s'.\n", id_remover, cat->nome);
    pausar();
}

int salvarDadosBinario(const char *nome_arquivo, NoCategoria *lista_categorias_head) {
    FILE *f = fopen(nome_arquivo, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo para escrita");
        return 0;
    }
    
    NoCategoria *cat_atual = lista_categorias_head;
    int total_salvo = 0;
    
    while (cat_atual != NULL) {
        NoAlimento *alimento_atual = cat_atual->lista_alimentos_head;
        while (alimento_atual != NULL) {
            AlimentoDados *dados = &(alimento_atual->dados);

            fwrite(&(dados->codigo), sizeof(dados->codigo), 1, f);
            fwrite(dados->descricao, sizeof(dados->descricao), 1, f);
            fwrite(&(dados->energia), sizeof(dados->energia), 1, f);
            fwrite(&(dados->proteina), sizeof(dados->proteina), 1, f);
            fwrite(&(dados->categoria_id), sizeof(dados->categoria_id), 1, f);
            
            total_salvo++;
            alimento_atual = alimento_atual->prox;
        }
        cat_atual = cat_atual->prox;
    }
    
    fclose(f);
    printf("Sucesso: %d alimentos salvos em %s.\n", total_salvo, nome_arquivo);
    return 1;
}

void liberarListaCategorias(NoCategoria *head) {
    printf("\nIniciando liberação de memória...\n");
    NoCategoria *cat_atual = head;
    NoCategoria *cat_temp;

    while (cat_atual != NULL) {
        NoAlimento *alimento_atual = cat_atual->lista_alimentos_head;
        NoAlimento *alimento_temp;
        while (alimento_atual != NULL) {
            alimento_temp = alimento_atual;
            alimento_atual = alimento_atual->prox;
            free(alimento_temp);
        }
        
        liberarArvore(cat_atual->arvore_energia_root);
        liberarArvore(cat_atual->arvore_proteina_root);
        
        cat_temp = cat_atual;
        cat_atual = cat_atual->prox;
        free(cat_temp);
    }
    printf("Memória de todas as estruturas liberada com sucesso.\n");
}

void reconstruirArvoresCategoria(NoCategoria *no_cat) {
    printf("Função reconstruirArvoresCategoria não é necessária, remoção específica lida com a BST.\n");
}