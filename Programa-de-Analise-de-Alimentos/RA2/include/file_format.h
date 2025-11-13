#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

typedef struct {
    int codigo;
    char nome[50];
    float calorias;
    float proteinas;
    char categoria[100];
} Alimento;

// Protótipos
void removerQuebraLinha(char *str);
void substituirVirgula(char *str);
void removerAspas(char *str);

#endif