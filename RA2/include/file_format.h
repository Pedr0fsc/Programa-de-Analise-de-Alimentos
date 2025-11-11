// Isto é o HEADER GUARD
// Basicamente garante que o arquivo .h nao pode ser expandido multiplas vezes
#ifndef file_format_H // If not defined?
#define file_format_H // then it defines...

typedef struct {
    int codigo;
    char nome[50];
    float calorias;
    float proteinas;
    int categoria_id;
} __attribute__ ((packed)) Alimento;

void removerQuebraLinha(char *str);
void substituirVirgula(char *str);
void removerAspas(char *str);

#endif