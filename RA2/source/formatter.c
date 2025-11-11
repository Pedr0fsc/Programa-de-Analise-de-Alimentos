#include <stdio.h>
#include <string.h>
#include "../include/file_format.h"

// nao usar STATIC como o escopo da funcao;


// Funções para manipulação de strings
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
            str[len - 1] = '\0';
            len--;
        }
    }
}

void substituirVirgula(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            str[i] = '.';
        }
    }
}

void removerAspas(char *str) {
    size_t len = strlen(str);
    if (len == 0) 
        return;
    if (str[0] == '"') { 
        memmove(str, str + 1, strlen(str));
    }
    len = strlen(str);
    if (len > 0 && str[len - 1] == '"') {
        str[len - 1] = '\0';
    }
}
