#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 100

typedef struct {
    int* itens;
    int tamanho;
} Heap;

void ajuste(int* a, int* b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void HeapifyIns(Heap* heap, int i) { // i = indice
    int pai = (i - 1) / 2; // operação para encontrar o indice do pai do nó inserido
    while (i > 0 && heap->itens[i] > heap->itens[pai]) { // Enquanto o item inserido for que o pai ele vai subindo a arvore com os ajustes
        ajuste(&heap->itens[i], &heap->itens[pai]); // troca o indice do item com o pai
        i = pai;
        pai = (i - 1) / 2; // novo pai do elemento pós troca

    }
}

void HeapifyDel(Heap* heap, int i) { // i = indice
    int esq = 2 * i + 1; // Operação para descobrir o indice do filho esquerdo
    int dir = 2 * i + 2;// Operação para descobrir o indice do filho direito
    int maior = i;
    if (esq < heap->tamanho && heap->itens[esq] > heap->itens[maior]) {
        maior = esq;}
    if (dir < heap->tamanho && heap->itens[dir] > heap->itens[maior]) {
        maior = dir;}
    if (maior != i) {
        ajuste(&heap->itens[i], &heap->itens[maior]);
        HeapifyDel(heap, maior);}
}

Heap* inicializar() {
    Heap* heap = malloc(sizeof(Heap));
    heap->itens = malloc(MAX_HEAP_SIZE * sizeof(int));
    heap->tamanho = 0;
    return heap;
}

void inserir(Heap* heap, int chave) {
    if (heap->tamanho == MAX_HEAP_SIZE) {
        printf("Arvore Cheia!\n");
        return;
    }
    else{
        heap->itens[heap->tamanho++] = chave;
        printf("Inserido: %d\n", chave);
        HeapifyIns(heap, heap->tamanho - 1);
    }

}

int DeletaMax(Heap* heap) {
    if (heap->tamanho == 0) {
        printf("Arvore Vazia!\n");
        return -1;
    }
    int max = heap->itens[0];
    heap->itens[0] = heap->itens[--heap->tamanho];
    HeapifyDel(heap, 0);
    printf("Deletado: %d\n", max);
    return max;
}

void AmostraArvore(Heap* heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->itens[i]);
    }
    printf("\n");
    printf("Tamanho: %d\n", heap->tamanho);
}

void AmostraMax(Heap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Max: %d\n", heap->itens[0]);}
    }

int main() {
    Heap* heap = inicializar();

    inserir(heap, 10);
    inserir(heap, 20);
    inserir(heap, 15);
    inserir(heap, 30);
    inserir(heap, 40);
    AmostraMax(heap);
    inserir(heap, 5);
    AmostraArvore(heap);
    int max = DeletaMax(heap);
    AmostraMax(heap);
    AmostraArvore(heap);

    free(heap->itens);
    free(heap);

    return 0;
}
