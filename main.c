// main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "huffman.h"

void add(Lista *lista, char caractere, int frequencia) {
    adicionar_ou_atualizar(lista, caractere, frequencia);
}

void calc(Lista *lista, TabelaHuffman *tabela) {
    calcular_codigos_huffman(lista, tabela);
}

void print(TabelaHuffman *tabela) {
    imprimir_tabela(tabela);
}

void cod(TabelaHuffman *tabela, char *str) {
    codificar_string(tabela, str);
}

void dec(TabelaHuffman *tabela, char *bits) {
    decodificar_bits(tabela, bits);
}

void clear(Lista *lista, TabelaHuffman *tabela) {
    limpar_lista(lista);
    limpar_tabela(tabela);
}

int main() {
    Lista lista;
    TabelaHuffman tabela;
    inicializar_lista(&lista);

    char comando[10];
    char argumento[100];
    int frequencia;

    while (1) {
        printf("->");
        scanf("%s", comando);

        if (strcmp(comando, "add") == 0) {
            scanf(" %c %d", argumento, &frequencia);
            add(&lista, argumento[0], frequencia);
        } else if (strcmp(comando, "calc") == 0) {
            calc(&lista, &tabela);
        } else if (strcmp(comando, "print") == 0) {
            print(&tabela);
        } else if (strcmp(comando, "cod") == 0) {
            scanf("%s", argumento);
            cod(&tabela, argumento);
        } else if (strcmp(comando, "dec") == 0) {
            scanf("%s", argumento);
            dec(&tabela, argumento);
        } else if (strcmp(comando, "clear") == 0) {
            clear(&lista, &tabela);
        } else if (strcmp(comando, "exit") == 0) {
            break;
        } else {
            printf("Comando inválido!\n");
        }
    }

    limpar_lista(&lista);
    limpar_tabela(&tabela);
    return 0;
}
