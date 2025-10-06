#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define MAX_ALIMENTOS 200
#define MAX_DESC 100
#define TAMANHO_LINHA 500

/* ENUM para categorias (requisito 2) */
typedef enum {
    CEREAIS = 1, VERDURAS, FRUTAS, GORDURAS, PESCADOS,
    CARNES, LACTEOS, BEBIDAS, OVOS, ACUCARES,
    MISCELANEAS, INDUSTRIALIZADOS, PREPARADOS, LEGUMINOSAS, SEMENTES
} Categoria;

/* Estrutura do alimento */
typedef struct {
    int numero_do_alimento;
    char descricao[MAX_DESC];
    float umidade;
    float energia;
    float proteina;
    float carboidrato;
    Categoria categoria;
} Alimento;

/* Estrutura usada para ordenação eficiente (valor + ponteiro para alimento) */
typedef struct {
    float valor;
    Alimento *ptr_alimento;
} ItemOrdenacao;

/* Vetor global imutável durante execução (requisito 1) */
Alimento alimentos[MAX_ALIMENTOS];
int total_alimentos = 0;

/* --- Funções utilitárias de processamento de strings --- */

/* Remove \n e \r do final da string */
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) { // condicional: se final é quebra
        while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) { // loop: remove todos os finais \n ou \r
            str[len-1] = '\0';
            len--;
        }
    }
}

/* Substitui vírgula por ponto para conversão float */
void substituirVirgulaPorPonto(char *str) {
    for (int i = 0; str[i] != '\0'; i++) { // loop: percorre string
        if (str[i] == ',') { // condicional: se encontrar vírgula
            str[i] = '.';
        }
    }
}

/* Remove aspas duplas no início/fim da string */
void removerAspas(char *str) {
    size_t len = strlen(str);
    if (len == 0) return; // condicional: se string vazia retorna
    if (str[0] == '"') { // condicional: se primeiro caractere é "
        memmove(str, str+1, strlen(str)); // move para frente
    }
    len = strlen(str);
    if (len > 0 && str[len-1] == '"') { // condicional: se último é "
        str[len-1] = '\0';
    }
}

/* Processa porcentagem: remove '%' e converte para float */
float processarPorcentagem(char *str) {
    removerAspas(str); // chama função que remove aspas
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '%') { // condicional: se último caractere é %
        str[len-1] = '\0';
    }
    substituirVirgulaPorPonto(str); // converte vírgula->ponto
    return atof(str); // converte para float
}

/* Converte nome textual da categoria para enum (busca por palavras-chave) */
Categoria nomeParaCategoria(const char *nome) {
    char tmp[200];
    strncpy(tmp, nome, sizeof(tmp)-1);
    tmp[sizeof(tmp)-1] = '\0';
    // condicional em sequência: verifica substrings representativas
    if (strstr(tmp, "Cereais")) return CEREAIS;
    if (strstr(tmp, "Verduras") || strstr(tmp, "hortali")) return VERDURAS;
    if (strstr(tmp, "Frutas")) return FRUTAS;
    if (strstr(tmp, "Gorduras") || strstr(tmp, "oleo")) return GORDURAS;
    if (strstr(tmp, "Pescados") || strstr(tmp, "frutos")) return PESCADOS;
    if (strstr(tmp, "Carnes")) return CARNES;
    if (strstr(tmp, "Leite") || strstr(tmp, "lacteo")) return LACTEOS;
    if (strstr(tmp, "Bebid")) return BEBIDAS;
    if (strstr(tmp, "Ovos")) return OVOS;
    if (strstr(tmp, "acucar") || strstr(tmp, "açuca")) return ACUCARES;
    if (strstr(tmp, "Miscel") || strstr(tmp, "Miscelane")) return MISCELANEAS;
    if (strstr(tmp, "industrial")) return INDUSTRIALIZADOS;
    if (strstr(tmp, "prepar")) return PREPARADOS;
    if (strstr(tmp, "Legumin")) return LEGUMINOSAS;
    if (strstr(tmp, "Noz") || strstr(tmp, "semente")) return SEMENTES;
    return CEREAIS; // padrão caso não reconhecido
}

/* Converte inteiro para Categoria (validação simples) */
Categoria intParaCategoria(int cat_num) {
    if (cat_num >= CEREAIS && cat_num <= SEMENTES) { // condicional: intervalo válido
        return (Categoria)cat_num;
    }
    return CEREAIS; // padrão
}

/* Retorna nome legível da categoria */
const char* obterNomeCategoria(Categoria cat) {
    static const char *nomes[] = {
        "", "Cereais e derivados", "Verduras e hortaliças", "Frutas e derivados",
        "Gorduras e óleos", "Pescados e frutos do mar", "Carnes e derivados",
        "Leite e derivados", "Bebidas", "Ovos e derivados", "Produtos açucarados",
        "Miscelâneas", "Industrializados", "Preparados", "Leguminosas", "Nozes e sementes"
    };
    if (cat >= CEREAIS && cat <= SEMENTES) return nomes[cat];
    return "Desconhecida";
}

/* --- Leitura do CSV (parte A do trabalho) --- */

/* Lê arquivo CSV e preenche o vetor global alimentos[] */
int lerArquivoCSV(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r"); // abre arquivo
    if (!arquivo) return 0; // condicional: se falha ao abrir retorna 0

    total_alimentos = 0;
    char linha[TAMANHO_LINHA];

    /* Pular cabeçalho */
    if (fgets(linha, sizeof(linha), arquivo) == NULL) { // condicional: arquivo vazio
        fclose(arquivo);
        return 0;
    }

    /* Ler linhas até EOF ou até encher o vetor */
    while (fgets(linha, sizeof(linha), arquivo) != NULL && total_alimentos < MAX_ALIMENTOS) { // loop: lê cada linha
        removerQuebraLinha(linha);

        char *token = strtok(linha, ","); // tokeniza por vírgula
        if (token == NULL) continue; // condicional: linha vazia ou inválida

        /* Campo 1: ID */
        alimentos[total_alimentos].numero_do_alimento = atoi(token);

        /* Campo 2: Descrição */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        strncpy(alimentos[total_alimentos].descricao, token, MAX_DESC-1);
        alimentos[total_alimentos].descricao[MAX_DESC-1] = '\0';

        /* Campo 3: Umidade (%) */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        alimentos[total_alimentos].umidade = processarPorcentagem(token);

        /* Campo 4: Energia (kcal) */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].energia = atof(token);

        /* Campo 5: Proteína (g) */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].proteina = atof(token);

        /* Campo 6: Carboidrato (g) */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].carboidrato = atof(token);

        /* Campo 7: Categoria */
        token = strtok(NULL, ",");
        if (token == NULL) {
            alimentos[total_alimentos].categoria = CEREAIS; // condicional: sem categoria -> padrão
        } else {
            alimentos[total_alimentos].categoria = nomeParaCategoria(token);
        }

        total_alimentos++; // incrementa contador
    }

    fclose(arquivo);
    return 1; // sucesso
}

/* --- Funções de impressão de tabela --- */

/* Imprime cabeçalho da tabela */
void imprimirCabecalhoTabela(void) {
    printf("\n+------+------------------------------------+---------+---------+----------+-------------+\n");
    printf("|  ID  |            DESCRICAO               | UMIDADE | ENERGIA | PROTEINA | CARBOIDRATO |\n");
    printf("+------+------------------------------------+---------+---------+----------+-------------+\n");
}

/* Imprime linha de alimento (com corte da descrição) */
void imprimirLinhaAlimento(const Alimento *a) {
    char desc_truncada[37];
    strncpy(desc_truncada, a->descricao, 36);
    desc_truncada[36] = '\0';
    printf("| %4d | %-34s | %7.2f | %7.2f | %8.2f | %11.2f |\n",
           a->numero_do_alimento, desc_truncada, a->umidade,
           a->energia, a->proteina, a->carboidrato);
}

/* Imprime rodapé com total */
void imprimirRodapeTabela(int total) {
    printf("+------+------------------------------------+---------+---------+----------+-------------+\n");
    printf("Total: %d alimento(s) encontrado(s)\n", total);
}

/* --- Funções de filtragem e ordenação (requisito 3: não replicar vetor inteiro) --- */

/* Filtra por categoria e retorna ponteiros para os alimentos filtrados */
int filtrarPorCategoria(Categoria cat, Alimento *resultado[], int max_resultado) {
    int count = 0;
    for (int i = 0; i < total_alimentos && count < max_resultado; i++) { // loop: percorre vetor global
        if (alimentos[i].categoria == cat) { // condicional: se categoria coincide
            resultado[count++] = &alimentos[i]; // armazena ponteiro para elemento original
        }
    }
    return count; // retorna quantidade encontrada
}

/* Ordena alfabeticamente (A-Z) um array de ponteiros para Alimento usando Bubble Sort */
void ordenarPorDescricao(Alimento *arr[], int n) {
    for (int i = 0; i < n - 1; i++) { // loop externo do bubble sort
        for (int j = 0; j < n - i - 1; j++) { // loop interno do bubble sort
            if (strcmp(arr[j]->descricao, arr[j+1]->descricao) > 0) { // condicional: se fora de ordem
                Alimento *tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

/* Prepara vetor ItemOrdenacao com valores extraídos por função passada */
void prepararOrdenacaoNumerica(Alimento *filtrados[], int total, ItemOrdenacao ordenacao[], float (*extrairValor)(const Alimento*)) {
    for (int i = 0; i < total; i++) { // loop: extrai valor para cada filtrado
        ordenacao[i].valor = extrairValor(filtrados[i]);
        ordenacao[i].ptr_alimento = filtrados[i];
    }
}

/* Ordena ItemOrdenacao em ordem decrescente pelo campo valor (Bubble Sort) */
void ordenarDecrescente(ItemOrdenacao arr[], int n) {
    for (int i = 0; i < n - 1; i++) { // loop externo
        for (int j = 0; j < n - i - 1; j++) { // loop interno
            if (arr[j].valor < arr[j+1].valor) { // condicional: se menor que próximo
                ItemOrdenacao tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

/* Funções extratoras para critérios numéricos */
float extrairUmidade(const Alimento *a) { return a->umidade; }
float extrairEnergia(const Alimento *a) { return a->energia; }
float extrairProteina(const Alimento *a) { return a->proteina; }
float extrairCarboidrato(const Alimento *a) { return a->carboidrato; }
float extrairRelacaoEnergiaProteina(const Alimento *a) { return (a->proteina > 0) ? (a->energia / a->proteina) : 0.0f; }
float extrairRelacaoEnergiaCarboidrato(const Alimento *a) { return (a->carboidrato > 0) ? (a->energia / a->carboidrato) : 0.0f; }

/* --- Funções de listagem conforme opções do enunciado --- */

/* Lista categorias (simples) */
void listarCategorias(void) {
    printf("\nCategorias:\n");
    for (int i = CEREAIS; i <= SEMENTES; i++) { // loop: percorre enums
        printf("%2d - %s\n", i, obterNomeCategoria(i));
    }
}

/* Lista todos os alimentos de uma categoria em ordem alfabética */
void listarPorDescricao(Categoria cat) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int t = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS); // filtra
    if (t == 0) { printf("\nNenhum alimento nesta categoria.\n"); return; } // condicional: se vazio
    ordenarPorDescricao(filtrados, t); // ordena alfabeticamente
    imprimirCabecalhoTabela();
    for (int i = 0; i < t; i++) { // loop: imprime cada linha
        imprimirLinhaAlimento(filtrados[i]);
    }
    imprimirRodapeTabela(t);
}

/* Lista todos os alimentos de uma categoria em ordem decrescente por energia */
void listarPorCriterioNumerico(Categoria cat, float (*extrairValor)(const Alimento*), const char *nome_criterio) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int t = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS); // filtra
    if (t == 0) { printf("\nNenhum alimento nesta categoria.\n"); return; } // condicional
    ItemOrdenacao ordenacao[MAX_ALIMENTOS];
    prepararOrdenacaoNumerica(filtrados, t, ordenacao, extrairValor); // prepara vetor de pares
    ordenarDecrescente(ordenacao, t); // ordena por valor decrescente
    imprimirCabecalhoTabela();
    for (int i = 0; i < t; i++) { // loop: imprime cada alimento pela ordem do ordenacao[]
        imprimirLinhaAlimento(ordenacao[i].ptr_alimento);
    }
    imprimirRodapeTabela(t);
}

/* Lista top N por critério (umidade, energia, proteína, carboidrato ou razões) */
void listarTopNPorCriterio(Categoria cat, float (*extrairValor)(const Alimento*), const char *nome_criterio, int n) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int t = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS); // filtra
    if (t == 0) { printf("\nNenhum alimento nesta categoria.\n"); return; } // condicional
    if (n > t) { n = t; } // condicional: limita n ao total disponível
    ItemOrdenacao ordenacao[MAX_ALIMENTOS];
    prepararOrdenacaoNumerica(filtrados, t, ordenacao, extrairValor); // prepara pares
    ordenarDecrescente(ordenacao, t); // ordena decrescente
    imprimirCabecalhoTabela();
    for (int i = 0; i < n; i++) { // loop: imprime apenas os top N
        imprimirLinhaAlimento(ordenacao[i].ptr_alimento);
    }
    imprimirRodapeTabela(n);
}

/* --- Função para pausar (aguardar ENTER) --- */
void pausar(void) {
    int c;
    printf("\nPressione ENTER para continuar...");
    // loop: consome até encontrar newline
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* --- Função principal com menu (parte B do trabalho) --- */
int main(void) {
    setlocale(LC_ALL, ""); // configura localidade

    if (!lerArquivoCSV("lista_de_alimentos.csv")) { // condicional: se não carregou arquivo
        printf("Erro ao carregar arquivo 'lista_de_alimentos.csv'.\n");
        return 1;
    }

    int opcao = 0;
    do {
        /* Menu principal */
        printf("\nSistema de Consulta Nutricional\n");
        printf("1) Listar categorias\n");
        printf("2) Listar alimentos por categoria (ordem alfabetica)\n");
        printf("3) Listar alimentos por categoria (ordem decrescente por energia)\n");
        printf("4) Listar os N alimentos com maior percentual de umidade\n");
        printf("5) Listar os N alimentos com maior capacidade energetica\n");
        printf("6) Listar alimentos por proteina (decrescente)\n");
        printf("7) Listar alimentos por carboidrato (decrescente)\n");
        printf("8) Listar N por relacao energia/proteina (decrescente)\n");
        printf("9) Listar N por relacao energia/carboidrato (decrescente)\n");
        printf("10) Encerrar\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) { // condicional: valida scanf
            while (getchar() != '\n'); // loop: limpa entrada inválida
            printf("Entrada invalida. Digite um numero.\n");
            pausar();
            continue; // volta ao menu
        }

        if (opcao == 1) { // condicional: listar categorias
            listarCategorias();
            pausar();
        }
        else if (opcao >= 2 && opcao <= 9) { // condicional: opções que necessitam escolher categoria
            int categoria_escolhida;
            listarCategorias();
            printf("Escolha uma categoria (1-15): ");
            if (scanf("%d", &categoria_escolhida) != 1 ||
                categoria_escolhida < 1 || categoria_escolhida > 15) { // condicional: valida categoria
                while (getchar() != '\n'); // loop: limpa buffer
                printf("Categoria invalida!\n");
                pausar();
                continue;
            }
            Categoria cat = intParaCategoria(categoria_escolhida);

            switch (opcao) { // condicional múltipla: trata cada opção
                case 2:
                    listarPorDescricao(cat);
                    break;
                case 3:
                    listarPorCriterioNumerico(cat, extrairEnergia, "Energia");
                    break;
                case 4: {
                    int n;
                    printf("Quantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) { // condicional: valida N
                        while (getchar() != '\n');
                        printf("Numero invalido!\n");
                        pausar();
                        break;
                    }
                    listarTopNPorCriterio(cat, extrairUmidade, "Umidade", n);
                    break;
                }
                case 5: {
                    int n;
                    printf("Quantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) {
                        while (getchar() != '\n');
                        printf("Numero invalido!\n");
                        pausar();
                        break;
                    }
                    listarTopNPorCriterio(cat, extrairEnergia, "Energia", n);
                    break;
                }
                case 6:
                    listarPorCriterioNumerico(cat, extrairProteina, "Proteina");
                    break;
                case 7:
                    listarPorCriterioNumerico(cat, extrairCarboidrato, "Carboidrato");
                    break;
                case 8: {
                    int n;
                    printf("Quantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) {
                        while (getchar() != '\n');
                        printf("Numero invalido!\n");
                        pausar();
                        break;
                    }
                    listarTopNPorCriterio(cat, extrairRelacaoEnergiaProteina, "Energia/Proteina", n);
                    break;
                }
                case 9: {
                    int n;
                    printf("Quantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) {
                        while (getchar() != '\n');
                        printf("Numero invalido!\n");
                        pausar();
                        break;
                    }
                    listarTopNPorCriterio(cat, extrairRelacaoEnergiaCarboidrato, "Energia/Carboidrato", n);
                    break;
                }
            }
            pausar();
        }
        else if (opcao != 10) { // condicional: opção inválida fora intervalo
            printf("Opcao invalida! Escolha entre 1 e 10.\n");
            pausar();
        }

        while (getchar() != '\n'); // loop: limpa restante da linha após entradas numéricas

    } while (opcao != 10); // loop: repete até escolher encerrar

    printf("Programa encerrado. Obrigado.\n");
    return 0;
}
