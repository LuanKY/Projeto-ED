#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

typedef struct {
    int item[MAX];
    int tamanho;
} ArvHeap;

void inicializar(ArvHeap *heap) {
    heap->tamanho = 0;
}

bool vazia(ArvHeap *heap){
    return heap == NULL;
}

bool cheia(ArvHeap *heap){
    return heap->tamanho == MAX;
}
/*
    Serve para ajustar os ponteiros pós inserção ou pos remoção
*/
void ajuste(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void inserir(ArvHeap *heap, int chave) {
    if (cheia(heap)) {
        return false;
    }

    heap->item[heap->tamanho] = chave;
    int i = heap->tamanho; // i representa o indice
    heap->tamanho++;

    while (i > 0 && heap->item[i] > heap->item[(i - 1) / 2]){ // (i - 1) / 2 representa o indice do nó pai da arvore em questão
                                                             // o Loop só para quando o indice for > 0 ou quando o valor a ser inserido for menor que o pai
        ajuste(&heap->item[i], &heap->item[(i - 1) / 2]);

        i = (i - 1) / 2;
    }
}

int RemoveMax(ArvHeap *heap) {
    if (vazia(heap)) {
        return;}

    int max = heap->item[0]; // Sempre vai excluir o indice 0 que representa a raiz, em arvore heap as remocoes acontecem sempre na raiz
    heap->tamanho--;         // seja a arvore min heap ou max heap
    heap->item[0] = heap->item[heap->tamanho]; // o nó raiz passa a ser o indice

    int i = 0;
    while (i < heap->tamanho) {
        int NoEsq = i * 2 + 1; // indica o indice do nó esquerdo do pai, sendo i o indice do pai se o pai for tiver o indice 0 então
                               // 0 * 2 + 1 = 1 na arvore o filho esquerdo tem o indice 1


        int NoDir = i * 2 + 2; // indica o indice do nó direto do pai, sendo i o indice do pai se o pai for tiver o indice 0 então
                               // 0 * 2 + 2 = 2 na arvore o filho direito tem o indice 2
        int NoPai = -1;

        if (NoEsq < heap->tamanho) {
            NoPai = NoEsq;
        }
        if (NoDir < heap->tamanho && heap->item[NoDir] > heap->item[NoEsq]) {
            NoPai = NoDir;
        }
        if (heap->item[NoPai] > heap->item[i]) {
            ajuste(&heap->item[NoPai], &heap->item[i]);
            i = NoPai;
        } else {
            break;
        }
    }

    printf("No Removido %d\n", max);
}
/*
    Printa a arvore
*/
void MostrarArv(ArvHeap *heap) {
    printf("Max Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->item[i]);
    }
  printf("\nTamanho: %d\n", heap->tamanho);
  printf("\n");
}

int main() {
    ArvHeap heap;
    inicializar(&heap);

    inserir(&heap, 88);
    inserir(&heap, 87);
    inserir(&heap, 73);
    inserir(&heap, 47);
    inserir(&heap, 54);
    inserir(&heap, 6);
    inserir(&heap, 0);
    inserir(&heap, 43);
    inserir(&heap, 100);
    MostrarArv(&heap);


    int HeapMax = RemoveMax(&heap);

    MostrarArv(&heap);

    return 0;
}
