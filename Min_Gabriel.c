#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int *itens;
    int tamanho;
} MinHeap;

MinHeap* inicializar() {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->itens = malloc(MAX * sizeof(int));
    heap->tamanho = 0;
    return heap;
}

void ajuste(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void heapifyIns(MinHeap *heap, int i) {
    while (i > 0 && heap->itens[(i - 1) / 2] > heap->itens[i]) {
        ajuste(&heap->itens[(i - 1) / 2], &heap->itens[i]);
        i = (i - 1) / 2;}
}

void heapifyDel(MinHeap *heap, int i) {
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    if (esq < heap->tamanho && heap->itens[esq] < heap->itens[menor]) {
        menor = esq;
    }
    if (dir < heap->tamanho && heap->itens[dir] < heap->itens[menor]) {
        menor = dir;
    }
    if (menor != i) {
        ajuste(&heap->itens[i], &heap->itens[menor]);
        heapifyDel(heap, menor);
    }
}

void inserir(MinHeap *heap, int chave) {
    if (heap->tamanho == MAX) {
        printf("Heap Cheia\n");
        return;
    }
    heap->itens[heap->tamanho++] = chave;
    heapifyIns(heap, heap->tamanho - 1);
    printf("Inserido: %d\n", chave);
}

int DeletaMin(MinHeap *heap) {
    if (heap->tamanho == 0) {
        printf("Heap vazia\n");
        return -1;
    }
    int min = heap->itens[0];
    heap->itens[0] = heap->itens[--heap->tamanho];
    heapifyDel(heap, 0);
    printf("Deletado: %d\n", min);
    return min;
}

void AmostarArvore(MinHeap *heap) {
    printf("Min Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->itens[i]);
    }
    printf("\n");
}
void AmostraMin(MinHeap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Min: %d\n", heap->itens[0]);}
    }
void Cauda(MinHeap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Cauda: %d\n", heap->itens[heap->tamanho - 1]);}
    }
int main() {
    MinHeap *heap = inicializar();


    inserir(heap, 10);
    inserir(heap, 20);
    inserir(heap, 15);
    inserir(heap, 30);
    inserir(heap, 40);
    AmostarArvore(heap);
    AmostraMin(heap);
    inserir(heap, 5);
    AmostarArvore(heap);
    Cauda(heap);
    int min = DeletaMin(heap);

    AmostraMin(heap);
    AmostarArvore(heap);

    free(heap->itens);
    free(heap);
    return 0;
}
