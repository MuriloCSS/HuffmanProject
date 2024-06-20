
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman.h"

void inicializar_lista(Lista *lista) {
    lista->cabeca = NULL;
    lista->tamanho = 0;
}

void adicionar_ou_atualizar(Lista *lista, char caractere, int frequencia) {
    ListNode *atual = lista->cabeca;
    while (atual != NULL) {
        if (atual->caractere == caractere) {
            atual->frequencia = frequencia;
            return;
        }
        atual = atual->proximo;
    }

    ListNode *novo_no = (ListNode *)malloc(sizeof(ListNode));
    novo_no->caractere = caractere;
    novo_no->frequencia = frequencia;
    novo_no->proximo = lista->cabeca;
    lista->cabeca = novo_no;
    lista->tamanho++;
}

void limpar_lista(Lista *lista) {
    ListNode *atual = lista->cabeca;
    while (atual != NULL) {
        ListNode *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    lista->cabeca = NULL;
    lista->tamanho = 0;
}

typedef struct MinHeapNode {
    char caractere;
    int frequencia;
    struct MinHeapNode *esquerda;
    struct MinHeapNode *direita;
} MinHeapNode;

typedef struct MinHeap {
    int tamanho;
    int capacidade;
    MinHeapNode **array;
} MinHeap;

MinHeapNode* novo_no(char caractere, int frequencia) {
    MinHeapNode* temp = (MinHeapNode*) malloc(sizeof(MinHeapNode));
    temp->caractere = caractere;
    temp->frequencia = frequencia;
    temp->esquerda = temp->direita = NULL;
    return temp;
}

MinHeap* criar_min_heap(int capacidade) {
    MinHeap* min_heap = (MinHeap*) malloc(sizeof(MinHeap));
    min_heap->tamanho = 0;
    min_heap->capacidade = capacidade;
    min_heap->array = (MinHeapNode**) malloc(min_heap->capacidade * sizeof(MinHeapNode*));
    return min_heap;
}

void trocar_min_heap_node(MinHeapNode** a, MinHeapNode** b) {
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

void min_heapify(MinHeap* min_heap, int idx) {
    int menor = idx;
    int esquerda = 2 * idx + 1;
    int direita = 2 * idx + 2;

    if (esquerda < min_heap->tamanho && min_heap->array[esquerda]->frequencia < min_heap->array[menor]->frequencia)
        menor = esquerda;

    if (direita < min_heap->tamanho && min_heap->array[direita]->frequencia < min_heap->array[menor]->frequencia)
        menor = direita;

    if (menor != idx) {
        trocar_min_heap_node(&min_heap->array[menor], &min_heap->array[idx]);
        min_heapify(min_heap, menor);
    }
}

MinHeapNode* extrair_minimo(MinHeap* min_heap) {
    MinHeapNode* temp = min_heap->array[0];
    min_heap->array[0] = min_heap->array[min_heap->tamanho - 1];
    --min_heap->tamanho;
    min_heapify(min_heap, 0);
    return temp;
}

void inserir_min_heap(MinHeap* min_heap, MinHeapNode* min_heap_node) {
    ++min_heap->tamanho;
    int i = min_heap->tamanho - 1;
    while (i && min_heap_node->frequencia < min_heap->array[(i - 1) / 2]->frequencia) {
        min_heap->array[i] = min_heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    min_heap->array[i] = min_heap_node;
}

void construir_min_heap(MinHeap* min_heap) {
    int n = min_heap->tamanho - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        min_heapify(min_heap, i);
}

int eh_folha(MinHeapNode* raiz) {
    return !(raiz->esquerda) && !(raiz->direita);
}

MinHeap* criar_e_construir_min_heap(Lista *lista) {
    ListNode *atual = lista->cabeca;
    MinHeap* min_heap = criar_min_heap(lista->tamanho);
    int i;
    for (i = 0; atual != NULL; ++i, atual = atual->proximo)
        min_heap->array[i] = novo_no(atual->caractere, atual->frequencia);
    min_heap->tamanho = lista->tamanho;
    construir_min_heap(min_heap);
    return min_heap;
}

MinHeapNode* construir_arvore_huffman(Lista *lista) {
    MinHeapNode *esquerda, *direita, *topo;
    MinHeap* min_heap = criar_e_construir_min_heap(lista);
    while (min_heap->tamanho != 1) {
        esquerda = extrair_minimo(min_heap);
        direita = extrair_minimo(min_heap);
        topo = novo_no('$', esquerda->frequencia + direita->frequencia);
        topo->esquerda = esquerda;
        topo->direita = direita;
        inserir_min_heap(min_heap, topo);
    }
    return extrair_minimo(min_heap);
}

void construir_codigos_huffman(MinHeapNode* raiz, int arr[], int topo, TabelaHuffman *tabela) {
    if (raiz->esquerda) {
        arr[topo] = 0;
        construir_codigos_huffman(raiz->esquerda, arr, topo + 1, tabela);
    }
    if (raiz->direita) {
        arr[topo] = 1;
        construir_codigos_huffman(raiz->direita, arr, topo + 1, tabela);
    }
    if (eh_folha(raiz)) {
        tabela->codigos[tabela->tamanho].caractere = raiz->caractere;
        int i;
        for (i = 0; i < topo; ++i)
            tabela->codigos[tabela->tamanho].codigo[i] = arr[i] + '0';
        tabela->codigos[tabela->tamanho].codigo[topo] = '\0';
        tabela->tamanho++;
    }
}

void calcular_codigos_huffman(Lista *lista, TabelaHuffman *tabela) {
    MinHeapNode* raiz = construir_arvore_huffman(lista);
    int arr[100], topo = 0;
    tabela->tamanho = 0;
    tabela->codigos = (CodigoHuffman*) malloc(lista->tamanho * sizeof(CodigoHuffman));
    construir_codigos_huffman(raiz, arr, topo, tabela);
}

void imprimir_tabela(TabelaHuffman *tabela) {
    for (int i = 0; i < tabela->tamanho; ++i)
        printf("%c codificado como %s\n", tabela->codigos[i].caractere, tabela->codigos[i].codigo);
}

void codificar_string(TabelaHuffman *tabela, char *str) {
    char codigo[1000] = "";
    for (int i = 0; str[i] != '\0'; ++i) {
        for (int j = 0; j < tabela->tamanho; ++j) {
            if (tabela->codigos[j].caractere == str[i]) {
                strcat(codigo, tabela->codigos[j].codigo);
                break;
            }
        }
    }
    printf("%s\n", codigo);
    int tamanho_original = strlen(str) * 8;
    int tamanho_huffman = strlen(codigo);
    int tamanho_minimo = 0;
    while ((1 << tamanho_minimo) < tabela->tamanho) tamanho_minimo++;
    tamanho_minimo *= strlen(str);
    printf("%f%% %f%%\n", (double)tamanho_huffman / tamanho_original * 100, (double)tamanho_huffman / tamanho_minimo * 100);
}

void decodificar_bits(TabelaHuffman *tabela, char *bits) {
    char decodificado[1000] = "";
    char buffer[256] = "";
    int buffer_idx = 0;
    for (int i = 0; bits[i] != '\0'; ++i) {
        buffer[buffer_idx++] = bits[i];
        buffer[buffer_idx] = '\0';
        for (int j = 0; j < tabela->tamanho; ++j) {
            if (strcmp(buffer, tabela->codigos[j].codigo) == 0) {
                strncat(decodificado, &tabela->codigos[j].caractere, 1);
                buffer_idx = 0;
                buffer[0] = '\0';
                break;
            }
        }
    }
    printf("%s\n", decodificado);
}

void limpar_tabela(TabelaHuffman *tabela) {
    free(tabela->codigos);
    tabela->tamanho = 0;
}
