#include "../include/data_structures.h"

#define ARQUIVO_BINARIO "dados.bin"
#define MIN_ID_CATEGORIA 1
#define MAX_ID_CATEGORIA 15

int main() {
    // Cabeça da lista ligada de Categorias (Requisito C.a.1)
    NoCategoria *lista_categorias_head = NULL;
    int alteracao_dados = 0; // Flag para indicar se houve remoção (Requisito C.c.9)

    printf("=== Programa de Gerenciamento de Dados de Alimentos (P2) ===\n");

    // Etapa C.a: Lê dados do binário e monta estruturas dinâmicas
    int total_lido = lerDadosBinario(ARQUIVO_BINARIO, &lista_categorias_head);
    
    if (total_lido < 0) {
        printf("ERRO FATAL: Falha ao abrir ou ler o arquivo binário %s.\n", ARQUIVO_BINARIO);
        exit(EXIT_FAILURE); // Requisito 10: Usar exit() para falhas excepcionais
    }

    if (lista_categorias_head == NULL || total_lido == 0) {
        printf("Aviso: O arquivo %s estava vazio. Iniciando com dados vazios.\n", ARQUIVO_BINARIO);
        pausar();
    } else {
        printf("Sucesso! %d alimentos lidos de %s e carregados nas estruturas dinâmicas.\n", total_lido, ARQUIVO_BINARIO);
    }
    
    int opcao = 0;
    
    // Etapa C.c: Fornece interface de menu em loop (Requisito 9: sem break/continue para controlar loop)
    while (opcao != 9) {
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) { 
            limparBufferEntrada();
            opcao = 0; // Opção inválida
        } else {
            limparBufferEntrada(); // Limpa o restante da linha (ENTER)
        }

        if (opcao < 1 || opcao > 9) {
            printf("\nOpção inválida. Por favor, escolha uma opção entre 1 e 9.\n");
        } else if (opcao == 1) { // 1. Liste todas as categorias
            listarCategorias(lista_categorias_head);
        } else if (opcao == 7) { // 7. Remova uma categoria
            removerCategoria(&lista_categorias_head, &alteracao_dados);
        } else if (opcao != 9) { // Opções 2, 3, 4, 5, 6, 8 (necessitam de categoria válida)
            
            int id_categoria = 0;
            NoCategoria *cat_escolhida = NULL;
            
            listarCategorias(lista_categorias_head);
            printf("\nEscolha uma categoria pelo ID (%d a %d): ", MIN_ID_CATEGORIA, MAX_ID_CATEGORIA);
            
            if (scanf("%d", &id_categoria) == 1) {
                limparBufferEntrada();
                Categoria cat_enum = intParaCategoria(id_categoria);

                if (cat_enum != CATEGORIA_DESCONHECIDA) {
                    NoCategoria *anterior = NULL;
                    cat_escolhida = buscarCategoriaPorID(lista_categorias_head, cat_enum, &anterior);
                }
            } else {
                limparBufferEntrada();
            }

            if (cat_escolhida == NULL) {
                printf("Aviso: Categoria inválida ou sem alimentos cadastrados (ID: %d).\n", id_categoria);
            } else {
                // Chama a função correspondente
                if (opcao == 2) { // 2. Liste todos os alimentos na ordem da lista (alfabética)
                    listarAlimentosPorLista(cat_escolhida);
                } else if (opcao == 3) { // 3. Liste por Energia (decrescente, via árvore)
                    listarAlimentosPorEnergia(cat_escolhida);
                } else if (opcao == 4) { // 4. Liste por Proteína (decrescente, via árvore)
                    listarAlimentosPorProteina(cat_escolhida);
                } else if (opcao == 5) { // 5. Liste por Energia em Intervalo (via árvore)
                    listarAlimentosPorEnergiaIntervalo(cat_escolhida);
                } else if (opcao == 6) { // 6. Liste por Proteína em Intervalo (via árvore)
                    listarAlimentosPorProteinaIntervalo(cat_escolhida);
                } else if (opcao == 8) { // 8. Remova um alimento específico
                    removerAlimentoEspecifico(lista_categorias_head, &alteracao_dados);
                }
            }
        }
        
        if (opcao != 9) {
            pausar();
        }
    }

    // Etapa C.c.9: Salvar a versão atualizada de dados.bin (se houve alteração)
    if (alteracao_dados) {
        printf("Houve alteração nos dados. Salvando a nova versão em %s...\n", ARQUIVO_BINARIO);
        if (salvarDadosBinario(ARQUIVO_BINARIO, lista_categorias_head) > 0) {
            printf("Dados salvos com sucesso!\n");
        } else {
            printf("ERRO: Falha ao salvar os dados atualizados em %s.\n", ARQUIVO_BINARIO);
            // Requisito 10: Se for uma falha grave, poderia usar exit(EXIT_FAILURE) aqui.
        }
    } else {
        printf("Nenhuma alteração nos dados. O arquivo %s não foi modificado.\n", ARQUIVO_BINARIO);
    }
    
    // Requisito 5: Liberar toda a memória alocada dinamicamente
    liberarListaCategorias(lista_categorias_head);

    printf("Memória liberada. Programa encerrado.\n");
    return 0; // Requisito 10: Retorno único
}