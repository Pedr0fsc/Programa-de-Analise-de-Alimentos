#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/p2.h"
#include "../include/file_format.h"

/*
 Implementation notes:
 - Categories are stored in a singly-linked list ordered alphabetically by name.
 - Each category contains a singly-linked list of FoodNode ordered alphabetically by food name.
 - Two binary search trees index foods by energy (calorias) and protein (proteinas).
 - All dynamic memory is allocated with malloc/calloc and freed before program exit.
 - Loops are controlled without using `break` or `continue`.
*/

/* ----- Helper constructors / destructors ----- */
static CategoryNode *createCategoryNode(const char *name) {
    CategoryNode *node = (CategoryNode *)calloc(1, sizeof(CategoryNode));
    if (node == NULL) {
        fprintf(stderr, "Falha de alocacao para categoria\n");
        exit(EXIT_FAILURE);
    }
    node->tag = CAT_DYNAMIC;
    strncpy(node->name, name, sizeof(node->name) - 1);
    node->name[sizeof(node->name) - 1] = '\0';
    node->foods = NULL;
    node->energyRoot = NULL;
    node->proteinRoot = NULL;
    node->next = NULL;
    return node;
}

static FoodNode *createFoodNode(const Alimento *a) {
    FoodNode *node = (FoodNode *)malloc(sizeof(FoodNode));
    if (node == NULL) {
        fprintf(stderr, "Falha de alocacao para alimento\n");
        exit(EXIT_FAILURE);
    }
    node->data = *a;
    node->next = NULL;
    return node;
}

static IndexNode *createIndexNode(float key, FoodNode *food) {
    IndexNode *node = (IndexNode *)malloc(sizeof(IndexNode));
    if (node == NULL) {
        fprintf(stderr, "Falha de alocacao para no de indice\n");
        exit(EXIT_FAILURE);
    }
    node->key = key;
    node->food = food;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* ----- Sorted insert for categories and foods ----- */
static void insertCategorySorted(CategoryNode **head, CategoryNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    CategoryNode *prev = NULL;
    CategoryNode *cur = *head;
    while (cur != NULL && strcmp(cur->name, newNode->name) < 0) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        newNode->next = *head;
        *head = newNode;
    } else {
        prev->next = newNode;
        newNode->next = cur;
    }
}

static void insertFoodSorted(FoodNode **head, FoodNode *newNode) {
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    FoodNode *prev = NULL;
    FoodNode *cur = *head;
    while (cur != NULL && strcmp(cur->data.nome, newNode->data.nome) < 0) {
        prev = cur;
        cur = cur->next;
    }
    if (prev == NULL) {
        newNode->next = *head;
        *head = newNode;
    } else {
        prev->next = newNode;
        newNode->next = cur;
    }
}

/* ----- Index tree operations ----- */
static void insertIndexNode(IndexNode **root, float key, FoodNode *food) {
    if (*root == NULL) {
        *root = createIndexNode(key, food);
        return;
    }
    IndexNode *cur = *root;
    while (1) {
        if (key > cur->key) {
            if (cur->right == NULL) {
                cur->right = createIndexNode(key, food);
                return;
            }
            cur = cur->right;
        } else {
            if (cur->left == NULL) {
                cur->left = createIndexNode(key, food);
                return;
            }
            cur = cur->left;
        }
    }
}

static void freeIndexTree(IndexNode *root) {
    if (root == NULL) {
        return;
    }
    freeIndexTree(root->left);
    freeIndexTree(root->right);
    free(root);
}

/* Build index trees from the foods list for a category */
static void buildIndexesForCategory(CategoryNode *category) {
    if (category == NULL) {
        return;
    }
    freeIndexTree(category->energyRoot);
    freeIndexTree(category->proteinRoot);
    category->energyRoot = NULL;
    category->proteinRoot = NULL;

    FoodNode *it = category->foods;
    while (it != NULL) {
        insertIndexNode(&category->energyRoot, it->data.calorias, it);
        insertIndexNode(&category->proteinRoot, it->data.proteinas, it);
        it = it->next;
    }
}

/* ---- (printing) ----- */
static void printFoodNodeShort(FoodNode *fn) {
    if (fn == NULL) return;
    printf("%d; %s; %.2f Kcal; %.2f g proteinas\n", fn->data.codigo, fn->data.nome, fn->data.calorias, fn->data.proteinas);
}

static void traverseIndexDesc(IndexNode *root) {
    if (root == NULL) return;
    traverseIndexDesc(root->right);
    printFoodNodeShort(root->food);
    traverseIndexDesc(root->left);
}

static void traverseIndexRangeDesc(IndexNode *root, float min, float max) {
    if (root == NULL) return;
    traverseIndexRangeDesc(root->right, min, max);
    if (root->key >= min && root->key <= max) {
        printFoodNodeShort(root->food);
    }
    traverseIndexRangeDesc(root->left, min, max);
}

/* ----- Find helpers ----- */
static int isNumberString(const char *s) {
    if (s == NULL || *s == '\0') return 0;
    for (const char *p = s; *p != '\0'; p++) {
        if (*p < '0' || *p > '9') return 0;
    }
    return 1;
}

static int strCaseEqual(const char *a, const char *b) {
    if (a == NULL || b == NULL) return 0;
    while (*a != '\0' && *b != '\0') {
        char ca = *a;
        char cb = *b;
            ca = (char)tolower((unsigned char)ca);
            cb = (char)tolower((unsigned char)cb);
            if (ca != cb) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}

/* Find by exact name (used when loading from file) */
static CategoryNode *findCategoryExact(CategoryNode *head, const char *name) {
    CategoryNode *cur = head;
    while (cur != NULL) {
        if (strcmp(cur->name, name) == 0) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

/* Find by user input: accepts either an index number ("3") or a case-insensitive name.
   Indexes start at 1 as shown to the user when listing categories. */
static CategoryNode *findCategoryByInput(CategoryNode *head, const char *input) {
    if (head == NULL || input == NULL) return NULL;
    if (isNumberString(input)) {
        int idx = atoi(input);
        if (idx <= 0) return NULL;
        int i = 1;
        CategoryNode *cur = head;
        while (cur != NULL) {
            if (i == idx) return cur;
            i++;
            cur = cur->next;
        }
        return NULL;
    }
    CategoryNode *cur = head;
    while (cur != NULL) {
        if (strCaseEqual(cur->name, input)) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ----- Public API implementations ----- */
void carregarDadosBinario(const char *arquivoBin, CategoryNode **categories) {
    FILE *f = fopen(arquivoBin, "rb");
    if (f == NULL) {
        fprintf(stderr, "Aviso: arquivo '%s' nao encontrado. Lista vazia iniciada.\n", arquivoBin);
        return;
    }
    Alimento a;
    while (fread(&a, sizeof(Alimento), 1, f) == 1) {
        CategoryNode *cat = findCategoryExact(*categories, a.categoria);
        if (cat == NULL) {
            CategoryNode *newCat = createCategoryNode(a.categoria);
            insertCategorySorted(categories, newCat);
            cat = findCategoryExact(*categories, a.categoria);
        }
        FoodNode *foodNode = createFoodNode(&a);
        insertFoodSorted(&cat->foods, foodNode);
    }
    fclose(f);

    /* Build index trees for every category */
    CategoryNode *it = *categories;
    while (it != NULL) {
        buildIndexesForCategory(it);
        it = it->next;
    }
}

void liberarTudo(CategoryNode **categories) {
    CategoryNode *curCat = *categories;
    while (curCat != NULL) {
        CategoryNode *nextCat = curCat->next;
        freeIndexTree(curCat->energyRoot);
        freeIndexTree(curCat->proteinRoot);
        FoodNode *curFood = curCat->foods;
        while (curFood != NULL) {
            FoodNode *nextFood = curFood->next;
            free(curFood);
            curFood = nextFood;
        }
        free(curCat);
        curCat = nextCat;
    }
    *categories = NULL;
}

void salvarDadosBinario(const char *arquivoBin, CategoryNode *categories) {
    FILE *f = fopen(arquivoBin, "wb");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir '%s' para escrita\n", arquivoBin);
        return;
    }
    CategoryNode *cat = categories;
    while (cat != NULL) {
        FoodNode *food = cat->foods;
        while (food != NULL) {
            if (fwrite(&food->data, sizeof(Alimento), 1, f) != 1) {
                fprintf(stderr, "Erro ao escrever registro no arquivo binario\n");
                fclose(f);
                return;
            }
            food = food->next;
        }
        cat = cat->next;
    }
    fclose(f);
}

/* Input helpers --------------------------------------------------------- */
static void readLine(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
        buffer[len - 1] = '\0';
        len--;
    }
}

/* Menu and operations --------------------------------------------------- */
void executarInterface(CategoryNode **categories) {
    char line[256];
    int running = 1;
    int modified = 0;
    while (running) {
        printf("\n--- Menu P2 ---\n");
        printf("1. Listar todas as categorias\n");
        printf("2. Listar alimentos de uma categoria\n");
        printf("3. Listar alimentos por energia (decrescente)\n");
        printf("4. Listar alimentos por proteina (decrescente)\n");
        printf("5. Listar alimentos por energia entre min e max\n");
        printf("6. Listar alimentos por proteina entre min e max\n");
        printf("7. Remover uma categoria\n");
        printf("8. Remover um alimento\n");
        printf("9. Sair\n");
        printf("Escolha: ");
        readLine(line, sizeof(line));

        int option = atoi(line);
        if (option == 1) {
            printf("Categorias:\n");
            CategoryNode *it = *categories;
            int idx = 1;
            while (it != NULL) {
                printf("%d. %s\n", idx, it->name);
                it = it->next;
                idx++;
            }
        }
            if (option == 2) {
                printf("Nome ou numero da categoria: ");
                readLine(line, sizeof(line));
                CategoryNode *cat = findCategoryByInput(*categories, line);
                if (cat == NULL) {
                    printf("Categoria nao encontrada\n");
                } else {
                    FoodNode *f = cat->foods;
                    while (f != NULL) {
                        printFoodNodeShort(f);
                        f = f->next;
                    }
                }
            }
        if (option == 3) {
            printf("Nome ou numero da categoria: ");
            readLine(line, sizeof(line));
            CategoryNode *cat = findCategoryByInput(*categories, line);
            if (cat == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                traverseIndexDesc(cat->energyRoot);
                fflush(stdout);
            }
        }
        if (option == 4) {
            printf("Nome ou numero da categoria: ");
            readLine(line, sizeof(line));
            CategoryNode *cat = findCategoryByInput(*categories, line);
            if (cat == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                traverseIndexDesc(cat->proteinRoot);
                fflush(stdout);
            }
        }
        if (option == 5) {
            printf("Nome ou numero da categoria: ");
            readLine(line, sizeof(line));
            CategoryNode *cat = findCategoryByInput(*categories, line);
            if (cat == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                char minStr[64], maxStr[64];
                float minv = 0.0f, maxv = 0.0f;
                printf("Valor minimo de energia: ");
                readLine(minStr, sizeof(minStr));
                printf("Valor maximo de energia: ");
                readLine(maxStr, sizeof(maxStr));
                minv = (float)atof(minStr);
                maxv = (float)atof(maxStr);
                if (minv > maxv) {
                    float tmp = minv; minv = maxv; maxv = tmp;
                }
                traverseIndexRangeDesc(cat->energyRoot, minv, maxv);
                fflush(stdout);
            }
        }
        if (option == 6) {
            printf("Nome ou numero da categoria: ");
            readLine(line, sizeof(line));
            CategoryNode *cat = findCategoryByInput(*categories, line);
            if (cat == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                char minStr[64], maxStr[64];
                float minv = 0.0f, maxv = 0.0f;
                printf("Valor minimo de proteina: ");
                readLine(minStr, sizeof(minStr));
                printf("Valor maximo de proteina: ");
                readLine(maxStr, sizeof(maxStr));
                minv = (float)atof(minStr);
                maxv = (float)atof(maxStr);
                if (minv > maxv) {
                    float tmp = minv; minv = maxv; maxv = tmp;
                }
                traverseIndexRangeDesc(cat->proteinRoot, minv, maxv);
                fflush(stdout);
            }
        }
        if (option == 7) {
            printf("Nome ou numero da categoria a remover: ");
            readLine(line, sizeof(line));
            CategoryNode *target = findCategoryByInput(*categories, line);
            if (target == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                CategoryNode *prev = NULL;
                CategoryNode *cur = *categories;
                while (cur != NULL && cur != target) {
                    prev = cur;
                    cur = cur->next;
                }
                if (cur == NULL) {
                    printf("Categoria nao encontrada\n");
                } else {
                    if (prev == NULL) {
                        *categories = cur->next;
                    } else {
                        prev->next = cur->next;
                    }
                    freeIndexTree(cur->energyRoot);
                    freeIndexTree(cur->proteinRoot);
                    FoodNode *ff = cur->foods;
                    while (ff != NULL) {
                        FoodNode *nxt = ff->next;
                        free(ff);
                        ff = nxt;
                    }
                    free(cur);
                    modified = 1;
                    printf("Categoria removida\n");
                }
            }
        }
        if (option == 8) {
            printf("Nome ou numero da categoria do alimento: ");
            readLine(line, sizeof(line));
            CategoryNode *cat = findCategoryByInput(*categories, line);
            if (cat == NULL) {
                printf("Categoria nao encontrada\n");
            } else {
                char foodName[128];
                printf("Nome do alimento a remover: ");
                readLine(foodName, sizeof(foodName));
                FoodNode *prevF = NULL;
                FoodNode *curF = cat->foods;
                while (curF != NULL && strcmp(curF->data.nome, foodName) != 0) {
                    prevF = curF;
                    curF = curF->next;
                }
                if (curF == NULL) {
                    printf("Alimento nao encontrado\n");
                } else {
                    if (prevF == NULL) {
                        cat->foods = curF->next;
                    } else {
                        prevF->next = curF->next;
                    }
                    free(curF);
                    /* rebuild indexes for the category */
                    buildIndexesForCategory(cat);
                    modified = 1;
                    printf("Alimento removido\n");
                }
            }
        }
        if (option == 9) {
            running = 0;
            if (modified) {
                printf("Alteracoes detectadas. O arquivo sera atualizado ao sair.\n");
            }
        }
    }
}
