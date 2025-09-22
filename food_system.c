#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define MAX_DESC 100
#define MAX_CATEGORIAS_SELECIONADAS 10
#define MAX_ALIMENTOS_CATEGORIA 20
#define MIN_ALIMENTOS_CATEGORIA 5
#define MAX_ALIMENTOS 100

// Estrutura das categorias
typedef enum
{
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
    SEMENTES,
} Categoria;

// Estrutra dos alimentos
typedef struct
{
    int numero_do_alimento;
    char descricao[MAX_DESC];
    float umidade;
    float energia;
    float proteina;
    float carboidrato;
    Categoria categoria;
} Alimento;

//funcao para ler o csv 
int ler_csv(const char *nome_do_arquivo, Alimento alimentos_lidos[]){
    FILE *arquivo = fopen(nome_do_arquivo, "r");


    // verifica se o arquivo foi aberto
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s'. Verifique se o arquivo existe e o caminho esta correto.\n", nome_do_arquivo);
        return 0; 
    }

    char linha[256];
    int contador_alimentos = 0;


    fgets(linha, sizeof(linha), arquivo); 

    // le as linhas ate o final ou EOF (end of file)
    while (fgets(linha, sizeof(linha), arquivo) != EOF)
    {

        // preencher a struct usando scanf
        scanf(linha, "%d,%[^,],%f,%f,%f,%f,%d",
                &alimentos_lidos[contador_alimentos].numero_do_alimento,
                alimentos_lidos[contador_alimentos].descricao,
                &alimentos_lidos[contador_alimentos].umidade,
                &alimentos_lidos[contador_alimentos].energia,
                &alimentos_lidos[contador_alimentos].proteina,
                &alimentos_lidos[contador_alimentos].carboidrato,
                (int *)&alimentos_lidos[contador_alimentos].categoria);

        contador_alimentos++;

        if (contador_alimentos >= MAX_ALIMENTOS)
        {
            break;
        }
    }

    fclose(arquivo);
    return contador_alimentos;
    }




void listarCategorias()
{
    printf("\n1. Cereais e derivados");
    printf("\n2. Verduras, hortaliças e derivados");
    printf("\n3. Frutas e derivados");
    printf("\n4. Gorduras e óleos");
    printf("\n5. Pescados e frutos do mar");
    printf("\n6. Carnes e derivados");
    printf("\n7. Leite e derivados");
    printf("\n8. Bebidas(alcoólicas e não alcoólicas)");
    printf("\n9. Ovos e derivados");
    printf("\n10. Produtos açucarados");
    printf("\n11. Miscelâneas");
    printf("\n12. Outros alimentos industrializados");
    printf("\n13. Alimentos preparados");
    printf("\n14. Leguminosas e derivados");
    printf("\n15. Nozes e sementes\n");
}

int main()
{
    int opcao;
    do
    {
        printf("\n--- MENU ---\n");
        printf("\n(1) - Listar as categorias de alimentos");
        printf("\n(2) - Listar todos os alimentos de certa categoria em ordem alfabética de acordo com a descrição");
        printf("\n(3) - Listar todos os alimentos de certa categoria em ordem decrescente de acordo com a energia(Kcal)");
        printf("\n(4) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a umidade(%%)");
        printf("\n(5) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a energia(Kcal)");
        printf("\n(6) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a proteína(g)");
        printf("\n(7) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a carboidrato(g)");
        printf("\n(8) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a relação de energia(Kcal) e proteína(g)");
        printf("\n(9) - Listar os alimentos de certa categoria em ordem decrescente de acordo com a relação de energia(Kcal) e carboidrato(g)");
        printf("\n(10) - Encerrar o programa");
        printf("\nOpção: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            listarCategorias();
        }
        else if (opcao >= 2 && opcao <= 9)
        {
            int cat, N;
            listarCategorias();
            printf("\nEscolha uma categoria: ");
            scanf("%d", &cat);
        }

    } while (opcao != 10);

    return 0;
}