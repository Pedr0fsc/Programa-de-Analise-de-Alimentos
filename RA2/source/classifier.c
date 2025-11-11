#include <ctype.h>
#include <string.h>
#include "../include/classifier.h"
#include "../include/data_structures.h"
#include <stdio.h>


Categoria inferirCategoriaPelaString(const char *categoria_str) {
    if (strcmp(categoria_str, "Cereais e derivados") == 0) return CEREAIS;
    if (strcmp(categoria_str, "Verduras, hortalicas e derivados") == 0) return VERDURAS;
    if (strcmp(categoria_str, "Frutas e derivados") == 0) return FRUTAS;
    if (strcmp(categoria_str, "Gorduras e Oleos") == 0) return GORDURAS;
    if (strcmp(categoria_str, "Pescados e frutos do mar") == 0) return PESCADOS;
    if (strcmp(categoria_str, "Carnes e derivados") == 0) return CARNES;
    if (strcmp(categoria_str, "Leite e derivados") == 0) return LACTEOS;
    if (strcmp(categoria_str, "Bebidas (alcoolicas e nao alcoolicas)") == 0) return BEBIDAS;
    if (strcmp(categoria_str, "Ovos e derivados") == 0) return OVOS;
    if (strcmp(categoria_str, "Produtos acucarados") == 0) return ACUCARES;
    if (strcmp(categoria_str, "Miscelaneas") == 0) return MISCELANEAS;
    if (strcmp(categoria_str, "Outros alimentos industrializados") == 0) return INDUSTRIALIZADOS;
    if (strcmp(categoria_str, "Alimentos preparados") == 0) return PREPARADOS;
    if (strcmp(categoria_str, "Leguminosas e derivados") == 0) return LEGUMINOSAS;
    if (strcmp(categoria_str, "Nozes e sementes") == 0) return SEMENTES;
    return CATEGORIA_DESCONHECIDA;
}

Categoria inferirCategoria(const char *nome) {
    char temp_desc[MAX_DESC * 2]; 
    int i, j = 0;
    
    // 1. Copia, converte para minúsculas E remove acentos (simplificado)
    for (i = 0; nome[i] && i < sizeof(temp_desc) - 1; i++) {
        char c = tolower((unsigned char)nome[i]);
        
        // Simples substituição de acentos (tentando cobrir os comuns do português)
        if (c == 225 || c == 227 || c == 226) c = 'a';     // á, ã, â
        else if (c == 233 || c == 234) c = 'e';            // é, ê
        else if (c == 237) c = 'i';                        // í
        else if (c == 243 || c == 245 || c == 244) c = 'o'; // ó, õ, ô
        else if (c == 250) c = 'u';                        // ú
        else if (c == 231) c = 'c';                        // ç
        
        // Adiciona apenas se for alfanumérico ou espaço
        if (isalnum((unsigned char)c) || c == ' ' || c == '-') {
            temp_desc[j++] = c;
        }
    }
    temp_desc[j] = '\0';
        
    // 13. Alimentos preparados (PREPARADOS) - Prioridade para itens compostos
    if (strstr(temp_desc, "acaraje") || strstr(temp_desc, "feijoada") || strstr(temp_desc, "vatapa") ||
        strstr(temp_desc, "cuscuz") || strstr(temp_desc, "bolognesa") || strstr(temp_desc, "salada") ||
        strstr(temp_desc, "omelete") || strstr(temp_desc, "estrogonofe") || strstr(temp_desc, "sanduiche") ||
        strstr(temp_desc, "pastel") || strstr(temp_desc, "pizza") || strstr(temp_desc, "torta") ||
        strstr(temp_desc, "sopao") || strstr(temp_desc, "massa com") || strstr(temp_desc, "prato")) { 
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
        strstr(temp_desc, "mussarela") || strstr(temp_desc, "coalhada") ||
        strstr(temp_desc, "requeijao") || strstr(temp_desc, "creme de leite") || strstr(temp_desc, "parmesao") || 
        strstr(temp_desc, "sorvete")) { 
        return LACTEOS;
    }

    // 6. Carnes e derivados (CARNES) 
    if (strstr(temp_desc, "carne") || strstr(temp_desc, "frango") || strstr(temp_desc, "bovina") || 
        strstr(temp_desc, "suina") || strstr(temp_desc, "presunto") || strstr(temp_desc, "linguica") || 
        strstr(temp_desc, "mortadela") || strstr(temp_desc, "salame") || strstr(temp_desc, "peru") || 
        strstr(temp_desc, "hamburguer") || strstr(temp_desc, "picanha") || strstr(temp_desc, "charque")) { 
        return CARNES;
    }

    // 5. Pescados e frutos do mar (PESCADOS)
    if (strstr(temp_desc, "peixe") || strstr(temp_desc, "pescado") || strstr(temp_desc, "camarao") || 
        strstr(temp_desc, "sardinha") || strstr(temp_desc, "atum") || strstr(temp_desc, "bacalhau") || 
        strstr(temp_desc, "salmao") || strstr(temp_desc, "marisco") || strstr(temp_desc, "tilapia") ||
        strstr(temp_desc, "molusco") || strstr(temp_desc, "ostra") || strstr(temp_desc, "lula") ||
        strstr(temp_desc, "mexilhao")) {
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
        (strstr(temp_desc, "coco") && !strstr(temp_desc, "agua")) || strstr(temp_desc, "linhaca")) { 
        return SEMENTES;
    }

    // 14. Leguminosas e derivados (LEGUMINOSAS)
    if (strstr(temp_desc, "feijao") || strstr(temp_desc, "soja") || strstr(temp_desc, "lentilha") || 
        strstr(temp_desc, "grao de bico") || strstr(temp_desc, "ervilha")) {
        return LEGUMINOSAS;
    }

    // 8. Bebidas (BEBIDAS) - *** PRIORIDADE ALTA ***
    if (strstr(temp_desc, "bebida") || strstr(temp_desc, "cerveja") || strstr(temp_desc, "vinho") || 
        strstr(temp_desc, "cafe") || strstr(temp_desc, "cha") || strstr(temp_desc, "refrigerante") ||
        strstr(temp_desc, "aguardente") || strstr(temp_desc, "licor") || strstr(temp_desc, "caldo de cana") ||
        strstr(temp_desc, "destilado") || strstr(temp_desc, "agua") || strstr(temp_desc, "isotonica") ||
        strstr(temp_desc, "refri") || strstr(temp_desc, "cola") || strstr(temp_desc, "tonica")) {
        return BEBIDAS;
    }

    // 3. Frutas e derivados (FRUTAS) - AGORA VEM DEPOIS DE BEBIDAS
    if (strstr(temp_desc, "fruta") || strstr(temp_desc, "suco") || strstr(temp_desc, "polpa") ||
        strstr(temp_desc, "laranja") || strstr(temp_desc, "maca") || strstr(temp_desc, "banana") || 
        strstr(temp_desc, "uva") || strstr(temp_desc, "mamao") || strstr(temp_desc, "morango") || 
        (strstr(temp_desc, "coco") && strstr(temp_desc, "agua")) || strstr(temp_desc, "limao") || 
        strstr(temp_desc, "abacaxi") || strstr(temp_desc, "abacate")) { 
        return FRUTAS;
    }
    
    // 2. Verduras, hortaliças e derivados (VERDURAS) 
    if (strstr(temp_desc, "alface") || strstr(temp_desc, "couve") || strstr(temp_desc, "tomate") || 
        strstr(temp_desc, "cebola") || strstr(temp_desc, "cenoura") || strstr(temp_desc, "pimentao") || 
        strstr(temp_desc, "batata") || strstr(temp_desc, "mandioca") || strstr(temp_desc, "abobora") || 
        strstr(temp_desc, "alho") || strstr(temp_desc, "palmito") || strstr(temp_desc, "chuchu") || 
        strstr(temp_desc, "pepino") || strstr(temp_desc, "quiabo") || strstr(temp_desc, "brocolis") ||
        strstr(temp_desc, "farofa") || strstr(temp_desc, "fecula")) { 
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

    // Se nada for encontrado, retorna a categoria padrão
    return CATEGORIA_DESCONHECIDA;
}