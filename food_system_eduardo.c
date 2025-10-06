#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Códigos de cor ANSI
#define RESET_COLOR   "\033[0m"
#define RED_COLOR     "\033[31m"
#define GREEN_COLOR   "\033[32m"
#define YELLOW_COLOR  "\033[33m"
#define BLUE_COLOR    "\033[34m"
#define MAGENTA_COLOR "\033[35m"
#define CYAN_COLOR    "\033[36m"
#define BOLD          "\033[1m"

// Símbolos ASCII
#define CHECKMARK     "OK"
#define WARNING       "!!"
#define ERROR_SYMBOL  "XX"
#define BULLET        "*"
#define ARROW         "->"

#define MAX_ALIMENTOS 200
#define MAX_DESC 100
#define TAMANHO_LINHA 500

/* 
 * Enumeração das categorias de alimentos
 * Requisito 2: Representação por meio de enumerado
 */
typedef enum {
    CEREAIS = 1,
    VERDURAS,
    FRUTAS,
    GORDURAS,
    PESCADOS,
    CARNES,
    LACTEOS,
    BEBIDAS,
    OVOS,
    AÇUCARES,
    MISCELANEAS,
    INDUSTRIALIZADOS,
    PREPARADOS,
    LEGUMINOSAS,
    SEMENTES
} Categoria;

/* 
 * Estrutura principal de alimento
 */
typedef struct {
    int numero_do_alimento;
    char descricao[MAX_DESC];
    float umidade;
    float energia;
    float proteina;
    float carboidrato;
    Categoria categoria;
} Alimento;

/*
 * Estrutura auxiliar para ordenação
 * Requisito 3: Evita replicar todo o vetor, usa apenas ponteiros
 */
typedef struct {
    float valor;              // Valor usado para comparação
    Alimento *ptr_alimento;   // Ponteiro para o alimento original
} ItemOrdenacao;

/* Array global para armazenar os alimentos (requisito 1: não modificável) */
Alimento alimentos[MAX_ALIMENTOS];
int total_alimentos = 0;

/* ========================================================================
 * FUNÇÕES DE DESIGN DO CLI
 * ======================================================================== */

/*
 * Barrinha de progresso 
 */
void mostrarProgresso(int atual, int total, const char* operacao) {
    int porcentagem = (atual * 100) / total;
    printf("\r%s %s [%d%%]%s", CYAN_COLOR, operacao, porcentagem, RESET_COLOR);
    fflush(stdout);
}

/*
 * Limpar a tela do cmd
 */
void limparTela() {
    #ifdef _WIN32 // se for windows, usa CLS
        system("cls");
    #else
        system("clear"); // qualquer outro sistema usa CLEAR
    #endif
}

/*
 * Aguardar o usuario apretar ENTER pra continuar
 */
void pausar() {
    printf("\n%sPressione ENTER para continuar...%s", YELLOW_COLOR, RESET_COLOR);
    while (getchar() != '\n');
    getchar();
}

/* ========================================================================
 * FUNÇÕES DE FORMATACAO
 * ======================================================================== */

/*
 * Remove caracteres de quebra de linha (\n e \r)
 */
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }
    if (len > 0 && str[len - 1] == '\r') {
        str[len - 1] = '\0';
    }
}

/*
 * Substitui vírgula por ponto para conversão numérica
 */
void substituirVirgulaPorPonto(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
}

/*
 * Remove aspas duplas de uma string
 */
void removerAspas(char *str) {
    if (str[0] == '"') {
        memmove(str, str + 1, strlen(str));
    }
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '"') {
        str[len - 1] = '\0';
    }
}

/*
 * Remove símbolo de porcentagem e converte para float
 */
float processarPorcentagem(char *str) {
    // Remove aspas se existirem
    removerAspas(str);
    
    // Remove o símbolo % se existir
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '%') {
        str[len - 1] = '\0';
    }
    
    // Substitui vírgula por ponto
    substituirVirgulaPorPonto(str);
    
    return atof(str);
}

/*
 * Converte nome da categoria em enum
 */
Categoria nomeParaCategoria(const char *nome) {
    // Remove aspas se existirem
    char nome_limpo[200];
    strncpy(nome_limpo, nome, sizeof(nome_limpo) - 1);
    nome_limpo[sizeof(nome_limpo) - 1] = '\0';
    
    if (strstr(nome_limpo, "Cereais")) return CEREAIS;
    if (strstr(nome_limpo, "Verduras") || strstr(nome_limpo, "hortalicas")) return VERDURAS;
    if (strstr(nome_limpo, "Frutas")) return FRUTAS;
    if (strstr(nome_limpo, "Gorduras") || strstr(nome_limpo, "oleos")) return GORDURAS;
    if (strstr(nome_limpo, "Pescados") || strstr(nome_limpo, "frutos do mar")) return PESCADOS;
    if (strstr(nome_limpo, "Carnes")) return CARNES;
    if (strstr(nome_limpo, "Leite") || strstr(nome_limpo, "lacteos")) return LACTEOS;
    if (strstr(nome_limpo, "Bebidas") || strstr(nome_limpo, "alcoolicas")) return BEBIDAS;
    if (strstr(nome_limpo, "Ovos")) return OVOS;
    if (strstr(nome_limpo, "acucarados") || strstr(nome_limpo, "Produtos acucarados")) return AÇUCARES;
    if (strstr(nome_limpo, "Miscelaneas")) return MISCELANEAS;
    if (strstr(nome_limpo, "industrializados")) return INDUSTRIALIZADOS;
    if (strstr(nome_limpo, "preparados")) return PREPARADOS;
    if (strstr(nome_limpo, "Leguminosas")) return LEGUMINOSAS;
    if (strstr(nome_limpo, "sementes") || strstr(nome_limpo, "Nozes")) return SEMENTES;
    
    return CEREAIS; // padrão
}

/*
 * Converte número inteiro para seu respectivo no ENUM
 */
Categoria intParaCategoria(int cat_num) {
    if (cat_num >= CEREAIS && cat_num <= SEMENTES) {
        return (Categoria)cat_num;
    }
    return CEREAIS;
}

/*
 * Categorias
 */
const char* obterNomeCategoria(Categoria cat) {
    static const char *nomes[] = {
        "",
        "Cereais e derivados",
        "Verduras, hortalicas e derivados", 
        "Frutas e derivados",
        "Gorduras e oleos",
        "Pescados e frutos do mar",
        "Carnes e derivados",
        "Leite e derivados",
        "Bebidas",
        "Ovos e derivados",
        "Produtos acucarados",
        "Miscelaneas",
        "Outros alimentos industrializados",
        "Alimentos preparados",
        "Leguminosas e derivados",
        "Nozes e sementes"
    };
    
    if (cat >= 1 && cat <= 15) {
        return nomes[cat];
    }
    return "Desconhecida";
}

/* ========================================================================
 * FUNÇÕES DE LEITURA DE DADOS 
 * ======================================================================== */

/*
 * Lê arquivo CSV, ja fazendo o processamento dos dados
 */
int lerArquivoCSV(const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("%s %s Arquivo '%s' nao encontrado.%s\n", 
               RED_COLOR, ERROR_SYMBOL, nome_arquivo, RESET_COLOR);
        return 0;
    }

    printf("%s %s Iniciando leitura do arquivo...%s\n", 
           BLUE_COLOR, BULLET, RESET_COLOR);
    
    total_alimentos = 0;
    char linha[TAMANHO_LINHA];
    
    /* Pular titulo */
    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        printf("%s %s Arquivo vazio.%s\n", 
               RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
        fclose(arquivo);
        return 0;
    }
    
    printf("DEBUG: Cabecalho: %s\n", linha);

    /* Ler cada linha com debug inicial */
    while (fgets(linha, sizeof(linha), arquivo) != NULL && 
           total_alimentos < MAX_ALIMENTOS) {
        
        removerQuebraLinha(linha);
        
        if (total_alimentos < 3) {
            printf("DEBUG: Linha %d: '%s'\n", total_alimentos + 1, linha);
        }
        
        // Processar linha usando strtok
        char *token = strtok(linha, ",");
        if (token == NULL) continue;
        
        /* Campo 1: ID */
        alimentos[total_alimentos].numero_do_alimento = atoi(token);
        if (total_alimentos < 3) {
            printf("DEBUG: ID = %d\n", alimentos[total_alimentos].numero_do_alimento);
        }
        
        /* Campo 2: Descrição */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        strncpy(alimentos[total_alimentos].descricao, token, MAX_DESC - 1);
        alimentos[total_alimentos].descricao[MAX_DESC - 1] = '\0';
        if (total_alimentos < 3) {
            printf("DEBUG: Descricao = '%s'\n", alimentos[total_alimentos].descricao);
        }
        
        /* Campo 3: Umidade (com %) */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        alimentos[total_alimentos].umidade = processarPorcentagem(token);
        if (total_alimentos < 3) {
            printf("DEBUG: Umidade = %.2f%%\n", alimentos[total_alimentos].umidade);
        }
        
        /* Campo 4: Energias */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].energia = atof(token);
        if (total_alimentos < 3) {
            printf("DEBUG: Energia = %.2f kcal\n", alimentos[total_alimentos].energia);
        }
        
        /* Campo 5: Proteínas */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].proteina = atof(token);
        if (total_alimentos < 3) {
            printf("DEBUG: Proteina = %.2f g\n", alimentos[total_alimentos].proteina);
        }
        
        /* Campo 6: Carboidratos */
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        removerAspas(token);
        substituirVirgulaPorPonto(token);
        alimentos[total_alimentos].carboidrato = atof(token);
        if (total_alimentos < 3) {
            printf("DEBUG: Carboidrato = %.2f g\n", alimentos[total_alimentos].carboidrato);
        }
        
        /* Campo 7: Categoria */
        token = strtok(NULL, ",");
        if (token == NULL) {
            alimentos[total_alimentos].categoria = CEREAIS;
        } else {
            alimentos[total_alimentos].categoria = nomeParaCategoria(token);
            if (total_alimentos < 3) {
                printf("DEBUG: Categoria = '%s' -> enum %d (%s)\n", 
                       token, alimentos[total_alimentos].categoria,
                       obterNomeCategoria(alimentos[total_alimentos].categoria));
            }
        }
        
        total_alimentos++;
        
        if (total_alimentos == 3) {
            printf("DEBUG: Continuando leitura sem debug...\n\n");
        }
    }

    fclose(arquivo);
    printf("\n%s %s Arquivo carregado: %d alimentos lidos com sucesso!%s\n", 
           GREEN_COLOR, CHECKMARK, total_alimentos, RESET_COLOR);
    
    // Debug: Mostrar estatísticas por categoria
    printf("\nDEBUG: Estatisticas por categoria:\n");
    for (int cat = CEREAIS; cat <= SEMENTES; cat++) {
        int count = 0;
        for (int i = 0; i < total_alimentos; i++) {
            if (alimentos[i].categoria == cat) {
                count++;
            }
        }
        printf("Categoria %d (%s): %d alimentos\n", cat, obterNomeCategoria(cat), count);
    }
    
    return 1;
}

/* ========================================================================
 * INTERFACE 
 * ======================================================================== */

/*
 * Exibe lista de categorias
 */
void listarCategorias(void) {
    printf("\n===============================================\n");
    printf("              CATEGORIAS DE ALIMENTOS\n");
    printf("===============================================\n");
    
    const char* categorias[] = {
        "", // índice 0 não usado
        "Cereais e derivados",
        "Verduras, hortalicas e derivados",
        "Frutas e derivados", 
        "Gorduras e oleos",
        "Pescados e frutos do mar",
        "Carnes e derivados",
        "Leite e derivados",
        "Bebidas (alcoolicas e nao alcoolicas)",
        "Ovos e derivados",
        "Produtos acucarados",
        "Miscelaneas", 
        "Outros alimentos industrializados",
        "Alimentos preparados",
        "Leguminosas e derivados",
        "Nozes e sementes"
    };
    
    for (int i = 1; i <= 15; i++) {
        printf(" %s %2d. %s\n", ARROW, i, categorias[i]);
    }
    printf("===============================================\n");
}

/*
 * header de tabela
 */
void imprimirCabecalhoTabela(void) {
    printf("\n+------+------------------------------------+---------+---------+----------+-------------+\n");
    printf("|  ID  |            DESCRICAO               | UMIDADE | ENERGIA | PROTEINA | CARBOIDRATO |\n");
    printf("|      |                                    |   (%%)   |  (kcal) |    (g)   |     (g)     |\n");
    printf("+------+------------------------------------+---------+---------+----------+-------------+\n");
}

/*
 * Footer de tabela
 */
void imprimirRodapeTabela(int total) {
    printf("+------+------------------------------------+---------+---------+----------+-------------+\n");
    printf("%s %s Total: %d alimento(s) encontrado(s)%s\n", 
           GREEN_COLOR, CHECKMARK, total, RESET_COLOR);
}

/*
 * Linha de alimento
 */
void imprimirLinhaAlimento(const Alimento *a) {
    char desc_truncada[37];
    strncpy(desc_truncada, a->descricao, 36);
    desc_truncada[36] = '\0';
    
    printf("| %4d | %-34s | %7.2f | %7.2f | %8.2f | %11.2f |\n",
           a->numero_do_alimento, desc_truncada, a->umidade, 
           a->energia, a->proteina, a->carboidrato);
}

/* ========================================================================
 * FUNÇÕES DE ORDENAÇÃO 
 * ======================================================================== */

int filtrarPorCategoria(Categoria cat, Alimento *resultado[], int max_resultado) {
    int count = 0;
    
    for (int i = 0; i < total_alimentos && count < max_resultado; i++) {
        if (alimentos[i].categoria == cat) {
            resultado[count] = &alimentos[i];
            count++;
        }
    }
    
    return count;
}

void ordenarPorDescricao(Alimento *arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j]->descricao, arr[j + 1]->descricao) > 0) {
                Alimento *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void ordenarDecrescente(ItemOrdenacao arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j].valor < arr[j + 1].valor) {
                ItemOrdenacao temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void prepararOrdenacaoNumerica(Alimento *filtrados[], int total, 
                                ItemOrdenacao ordenacao[], 
                                float (*extrairValor)(const Alimento*)) {
    for (int i = 0; i < total; i++) {
        ordenacao[i].valor = extrairValor(filtrados[i]);
        ordenacao[i].ptr_alimento = filtrados[i];
    }
}

/* Funções auxiliares para extração */
float extrairUmidade(const Alimento *a) { return a->umidade; }
float extrairEnergia(const Alimento *a) { return a->energia; }
float extrairProteina(const Alimento *a) { return a->proteina; }
float extrairCarboidrato(const Alimento *a) { return a->carboidrato; }
float extrairRelacaoEnergiaProteina(const Alimento *a) {
    return (a->proteina > 0) ? (a->energia / a->proteina) : 0;
}
float extrairRelacaoEnergiaCarboidrato(const Alimento *a) {
    return (a->carboidrato > 0) ? (a->energia / a->carboidrato) : 0;
}

/* ========================================================================
 * FUNÇÕES DE LISTAGEM
 * ======================================================================== */

void listarPorDescricao(Categoria cat) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int total = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS);
    
    if (total == 0) {
        printf("\n%s %s Nenhum alimento encontrado nesta categoria.%s\n", 
               YELLOW_COLOR, WARNING, RESET_COLOR);
        return;
    }
    
    ordenarPorDescricao(filtrados, total);
    
    printf("\n%s=== CATEGORIA: %s ===%s\n", MAGENTA_COLOR, obterNomeCategoria(cat), RESET_COLOR);
    printf("%sORDENACAO: Alfabetica (A-Z)%s\n", MAGENTA_COLOR, RESET_COLOR);
    
    imprimirCabecalhoTabela();
    for (int i = 0; i < total; i++) {
        imprimirLinhaAlimento(filtrados[i]);
    }
    imprimirRodapeTabela(total);
}

void listarPorCriterioNumerico(Categoria cat, 
                                float (*extrairValor)(const Alimento*),
                                const char *nome_criterio) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int total = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS);
    
    if (total == 0) {
        printf("\n%s %s Nenhum alimento encontrado nesta categoria.%s\n", 
               YELLOW_COLOR, WARNING, RESET_COLOR);
        return;
    }
    
    ItemOrdenacao ordenacao[MAX_ALIMENTOS];
    prepararOrdenacaoNumerica(filtrados, total, ordenacao, extrairValor);
    ordenarDecrescente(ordenacao, total);
    
    printf("\n%s=== CATEGORIA: %s ===%s\n", MAGENTA_COLOR, obterNomeCategoria(cat), RESET_COLOR);
    printf("%sORDENACAO: %s (decrescente)%s\n", MAGENTA_COLOR, nome_criterio, RESET_COLOR);
    
    imprimirCabecalhoTabela();
    for (int i = 0; i < total; i++) {
        imprimirLinhaAlimento(ordenacao[i].ptr_alimento);
    }
    imprimirRodapeTabela(total);
}

/*
 * Lista os N alimentos de uma categoria com maior valor de acordo com critério
 * Requisito específico: N definido pelo usuário
 */
void listarTopNPorCriterio(Categoria cat, 
                           float (*extrairValor)(const Alimento*),
                           const char *nome_criterio,
                           int n) {
    Alimento *filtrados[MAX_ALIMENTOS];
    int total = filtrarPorCategoria(cat, filtrados, MAX_ALIMENTOS);
    
    if (total == 0) {
        printf("\n%s %s Nenhum alimento encontrado nesta categoria.%s\n", 
               YELLOW_COLOR, WARNING, RESET_COLOR);
        return;
    }
    
    // Limitar N ao total disponível
    if (n > total) {
        n = total;
        printf("\n%s %s Apenas %d alimentos disponiveis nesta categoria. Mostrando todos.%s\n", 
               YELLOW_COLOR, WARNING, total, RESET_COLOR);
    }
    
    ItemOrdenacao ordenacao[MAX_ALIMENTOS];
    prepararOrdenacaoNumerica(filtrados, total, ordenacao, extrairValor);
    ordenarDecrescente(ordenacao, total);
    
    printf("\n%s=== CATEGORIA: %s ===%s\n", MAGENTA_COLOR, obterNomeCategoria(cat), RESET_COLOR);
    printf("%sORDENACAO: Top %d por %s (decrescente)%s\n", MAGENTA_COLOR, n, nome_criterio, RESET_COLOR);
    
    imprimirCabecalhoTabela();
    for (int i = 0; i < n; i++) {
        imprimirLinhaAlimento(ordenacao[i].ptr_alimento);
    }
    imprimirRodapeTabela(n);
}

/* ========================================================================
 * FUNÇÃO PRINCIPAL
 * ======================================================================== */

int main(void) {
    setlocale(LC_ALL, "Portuguese");
    
    // Banner inicial
    limparTela();
    printf("===============================================\n");
    printf("     SISTEMA DE CONSULTA NUTRICIONAL\n");  
    printf("           Versao 2.1 Corrigida\n");
    printf("===============================================\n");
    
    /* Carregar dados */
    if (!lerArquivoCSV("lista_de_alimentos.csv")) {
        printf("\n%s %s Erro ao carregar arquivo. Encerrando.%s\n", 
               RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
        pausar();
        return 1;
    }
    
    pausar();
    
    int opcao, categoria_escolhida;
    
    do {
        limparTela();
        
        printf("===============================================\n");
        printf("     SISTEMA DE CONSULTA NUTRICIONAL\n");
        printf("===============================================\n");
        
        printf("\n%s 1%s Listar categorias de alimentos\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 2%s Listar alimentos por categoria (ordem alfabetica)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 3%s Listar alimentos por categoria (ordem decrescente por energia)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 4%s Listar os N alimentos com maior percentual de umidade\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 5%s Listar os N alimentos com maior capacidade energetica\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 6%s Listar alimentos por categoria (ordem por proteina)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 7%s Listar alimentos por categoria (ordem por carboidrato)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 8%s Listar alimentos por categoria (relacao energia/proteina)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s 9%s Listar alimentos por categoria (relacao energia/carboidrato)\n", YELLOW_COLOR, RESET_COLOR);
        printf("%s10%s Encerrar programa\n", RED_COLOR, RESET_COLOR);
        
        printf("\n===============================================\n");
        printf("Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n');
            printf("\n%s %s Entrada invalida! Digite um numero.%s\n", 
                   RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
            pausar();
            continue;
        }
        
        if (opcao == 1) {
            listarCategorias();
            pausar();
        }
        else if (opcao >= 2 && opcao <= 9) {
            listarCategorias();
            printf("\nEscolha uma categoria (1-15): ");
            
            if (scanf("%d", &categoria_escolhida) != 1 || 
                categoria_escolhida < 1 || categoria_escolhida > 15) {
                while (getchar() != '\n');
                printf("\n%s %s Categoria invalida!%s\n", 
                       RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
                pausar();
                continue;
            }
            
            Categoria cat = intParaCategoria(categoria_escolhida);
            
            switch(opcao) {
                case 2:
                    listarPorDescricao(cat);
                    break;
                case 3:
                    listarPorCriterioNumerico(cat, extrairEnergia, "Energia (kcal)");
                    break;
                case 4: {
                    int n;
                    printf("\nQuantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) {
                        while (getchar() != '\n');
                        printf("\n%s %s Numero invalido! Digite um valor positivo.%s\n", 
                               RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
                        pausar();
                        continue;
                    }
                    listarTopNPorCriterio(cat, extrairUmidade, "Umidade (%)", n);
                    break;
                }
                case 5: {
                    int n;
                    printf("\nQuantos alimentos deseja listar (N): ");
                    if (scanf("%d", &n) != 1 || n <= 0) {
                        while (getchar() != '\n');
                        printf("\n%s %s Numero invalido! Digite um valor positivo.%s\n", 
                               RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
                        pausar();
                        continue;
                    }
                    listarTopNPorCriterio(cat, extrairEnergia, "Energia (kcal)", n);
                    break;
                }
                case 6:
                    listarPorCriterioNumerico(cat, extrairProteina, "Proteina (g)");
                    break;
                case 7:
                    listarPorCriterioNumerico(cat, extrairCarboidrato, "Carboidrato (g)");
                    break;
                case 8:
                    listarPorCriterioNumerico(cat, extrairRelacaoEnergiaProteina, 
                                             "Relacao Energia/Proteina");
                    break;
                case 9:
                    listarPorCriterioNumerico(cat, extrairRelacaoEnergiaCarboidrato, 
                                             "Relacao Energia/Carboidrato");
                    break;
            }
            
            pausar();
        }
        else if (opcao != 10) {
            printf("\n%s %s Opcao invalida! Escolha entre 1 e 10.%s\n", 
                   RED_COLOR, ERROR_SYMBOL, RESET_COLOR);
            pausar();
        }
        
    } while (opcao != 10);
    
    limparTela();
    printf("===============================================\n");
    printf("        Programa encerrado. Ate logo!\n");
    printf("     Obrigado por usar nosso sistema!\n");
    printf("===============================================\n");
    
    return 0;
}