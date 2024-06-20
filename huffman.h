
#ifndef LIST_H
#define LIST_H

typedef struct ListNode {
    char caractere;
    int frequencia;
    struct ListNode *proximo;
} ListNode;

typedef struct {
    ListNode *cabeca;
    int tamanho;
} Lista;

typedef struct {
    char caractere;
    char codigo[256];
} CodigoHuffman;

typedef struct {
    CodigoHuffman *codigos;
    int tamanho;
} TabelaHuffman;

void inicializar_lista(Lista *lista);
void adicionar_ou_atualizar(Lista *lista, char caractere, int frequencia);
void limpar_lista(Lista *lista);

void calcular_codigos_huffman(Lista *lista, TabelaHuffman *tabela);
void imprimir_tabela(TabelaHuffman *tabela);
void codificar_string(TabelaHuffman *tabela, char *str);
void decodificar_bits(TabelaHuffman *tabela, char *bits);
void limpar_tabela(TabelaHuffman *tabela);

#endif // LIST_H
