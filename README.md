# Programa de Análise de Alimentos - Programação Imperativa

Este projeto implementa em linguagem **C** um programa para leitura e análise de alimentos da **Tabela Brasileira de Composição de Alimentos (TACO)**, conforme enunciado do trabalho de Programação Imperativa.

---

<details>
<summary>📘 Português</summary>

## Descrição
O programa lê um arquivo `alimentos.csv` contendo **100 alimentos** selecionados da TACO e permite ao usuário consultar e ordenar os alimentos por diferentes critérios (energia, proteína, carboidrato, umidade etc.), sem modificar o vetor original.

## Estrutura do Projeto
- `alimentos.csv` → arquivo de entrada com os alimentos.
- `main.c` → código-fonte em C.
- `README.md` → este arquivo de documentação.

## Formato do CSV
O arquivo deve conter os campos:

```csv
Numero;Descricao;Umidade;Energia;Proteina;Carboidrato;Categoria
1;Arroz, integral, cru;12.7;360;7.5;77.9;1
2;Feijão, carioca, cru;12.5;330;20.1;60.5;14
...
```

- **Categoria** é representada por um número (1–15), de acordo com o **enum** no código.

## Funcionalidades
O programa apresenta o seguinte menu interativo:

1. Listar categorias de alimentos  
2. Listar alimentos de uma categoria em ordem alfabética  
3. Listar alimentos de uma categoria em ordem decrescente de energia  
4. Listar N alimentos com maior umidade  
5. Listar N alimentos com maior energia  
6. Listar N alimentos com maior proteína  
7. Listar N alimentos com maior carboidrato  
8. Listar N alimentos com maior relação energia/proteína  
9. Listar N alimentos com maior relação energia/carboidrato  
10. Encerrar o programa  

## Execução
### Compilar:
```bash
gcc main.c -o programa
```

### Executar:
```bash
./programa
```

## Requisitos atendidos
- Uso de **enum** para categorias.  
- Dados armazenados em vetor de struct.  
- Ordenação implementada manualmente (**Bubble Sort**), sem funções de biblioteca.  
- Uso de ponteiros para ordenar sem duplicar o vetor de alimentos.  

---

</details>

---

<details>
<summary>📘 English</summary>

## Description
This project implements in **C language** a program to read and analyze foods from the **Brazilian Food Composition Table (TACO)**, following the assignment of Imperative Programming class.

## Project Structure
- `alimentos.csv` → input file with the food data.  
- `main.c` → C source code.  
- `README.md` → this documentation file.  

## CSV Format
The file must contain the following fields:

```csv
Number;Description;Moisture;Energy;Protein;Carbohydrate;Category
1;Brown rice, raw;12.7;360;7.5;77.9;1
2;Carioca beans, raw;12.5;330;20.1;60.5;14
...
```

- **Category** is represented by a number (1–15), according to the **enum** in the code.

## Features
The program provides the following interactive menu:

1. List food categories  
2. List foods from a category in alphabetical order  
3. List foods from a category in descending order of energy  
4. List N foods with highest moisture  
5. List N foods with highest energy  
6. List N foods with highest protein  
7. List N foods with highest carbohydrate  
8. List N foods with highest energy/protein ratio  
9. List N foods with highest energy/carbohydrate ratio  
10. Exit program  

## How to Run
### Compile:
```bash
gcc main.c -o program
```

### Run:
```bash
./program
```

## Requirements fulfilled
- **Enum** used for categories.  
- Data stored in struct array.  
- Sorting implemented manually (**Bubble Sort**), no library functions used.  
- Use of pointers for sorting without duplicating the food vector.  

---

</details>
