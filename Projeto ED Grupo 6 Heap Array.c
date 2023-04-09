#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int *itens;
    int tamanho;
} MinHeap;
typedef struct {
    int* itens;
    int tamanho;
} Heap;

MinHeap* inicializarM() {
    MinHeap* heap = malloc(sizeof(MinHeap));
    heap->itens = malloc(MAX * sizeof(int));
    heap->tamanho = 0;
    return heap;
}
Heap* inicializar() {
    Heap* heap = malloc(sizeof(Heap));
    heap->itens = malloc(MAX * sizeof(int));
    heap->tamanho = 0;
    return heap;
}

void Tamanho(Heap *heap){
    printf("Tamanho: %d\n", heap->tamanho);
}
void TamanhoM(MinHeap *heap){
    printf("Tamanho: %d\n", heap->tamanho);
}

void ajuste(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}
void HeapifyIns(Heap* heap, int i) { // i = indice  Função: Reajustar os elementos da arvore para manter as propriedades pós inserção
    int pai = (i - 1) / 2; // operação para encontrar o indice do pai do nó inserido
    while (i > 0 && heap->itens[i] > heap->itens[pai]) { // Enquanto o item inserido for maior que o pai ele  sobe a arvore com os ajustes
        ajuste(&heap->itens[i], &heap->itens[pai]); // troca o indice do item com o pai
        i = pai;
        pai = (i - 1) / 2; // novo pai do elemento pós troca

    }
}
void heapifyInsM(MinHeap *heap, int i) {
    while (i > 0 && heap->itens[(i - 1) / 2] > heap->itens[i]) {
        ajuste(&heap->itens[(i - 1) / 2], &heap->itens[i]);
        i = (i - 1) / 2;}}

void inserir(Heap* heap, int chave) {
    if (heap->tamanho == MAX) {
        printf("Arvore Cheia!\n");
        return;
    }
    else{
        heap->itens[heap->tamanho++] = chave;
        printf("Inserido: %d\n", chave);
        HeapifyIns(heap, heap->tamanho - 1);
    }

}
void inserirM(MinHeap *heap, int chave) {
    if (heap->tamanho == MAX) {
        printf("Heap Cheia\n");
        return;
    }
    heap->itens[heap->tamanho++] = chave;
    heapifyInsM(heap, heap->tamanho - 1);
    printf("Inserido: %d\n", chave);
}


void HeapifyDel(Heap* heap, int i) { // i = indice  Função: Ajusta os elementos para que as propridades sejam aceitas pós remoção
    int esq = 2 * i + 1; // Operação para descobrir o indice do filho esquerdo
    int dir = 2 * i + 2;// Operação para descobrir o indice do filho direito
    int maior = i;
    if (esq < heap->tamanho && heap->itens[esq] > heap->itens[maior]) { // compara se o elemento esquerdo é maior que o seu indice  superior
        maior = esq;}
    if (dir < heap->tamanho && heap->itens[dir] > heap->itens[maior]) {
        maior = dir;}// compara se o elemento direito é maior que o seu indice  superior
    if (maior != i) { // Se houve alguma alteração o item é ajustado com o seu superior
        ajuste(&heap->itens[i], &heap->itens[maior]);
        HeapifyDel(heap, maior);}
}
void heapifyDelM(MinHeap *heap, int i) {
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
        heapifyDelM(heap, menor);
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
int DeletaMin(MinHeap *heap) {
    if (heap->tamanho == 0) {
        printf("Heap vazia\n");
        return -1;
    }
    int min = heap->itens[0];
    heap->itens[0] = heap->itens[--heap->tamanho];
    heapifyDelM(heap, 0);
    printf("Deletado: %d\n", min);
    return min;
}


void AmostraArvore(Heap* heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->itens[i]);
    }
    printf("\n");
    Tamanho(heap);
}
void AmostarArvoreM(MinHeap *heap) {
    printf("Min Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->itens[i]);
    }
    printf("\n");
    TamanhoM(heap);
}


void AmostraMax(Heap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Max: %d\n", heap->itens[0]);}
    }
void AmostraMin(MinHeap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Min: %d\n", heap->itens[0]);}
    }

void Cauda(Heap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Cauda: %d\n", heap->itens[heap->tamanho-1]);}
    }
void CaudaM(MinHeap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Cauda: %d\n", heap->itens[heap->tamanho-1]);}
    }

void deleteHeap(Heap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->itens);
    free(heap);
}
void deleteHeapM(MinHeap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->itens);
    free(heap);
}

int main() {

    printf("\n\nMAX =======================\n");
    Heap* heap = inicializar();

    inserir(heap, 10);
    inserir(heap, 20);
    inserir(heap, 15);
    inserir(heap, 30);
    inserir(heap, 40);
    printf("\n");
    printf("MAX =======================\n");
    AmostraArvore(heap);
    AmostraMax(heap);
    Cauda(heap);
    inserir(heap, 5);
    AmostraArvore(heap);
    printf("\n");
    printf("MAX =======================\n");
    int max = DeletaMax(heap);
    AmostraMax(heap);
    AmostraArvore(heap);

    deleteHeap(heap);

    MinHeap *heapm = inicializarM();
    printf("\n");
    printf("\n\nMIN =======================\n");
    inserirM(heapm, 10);
    inserirM(heapm, 20);
    inserirM(heapm, 15);
    inserirM(heapm, 30);
    inserirM(heapm, 40);
    printf("\n");
    printf("MIN =======================\n");
    AmostarArvoreM(heapm);
    AmostraMin(heapm);
    CaudaM(heapm);
    inserirM(heapm, 5);
    AmostarArvoreM(heapm);
    printf("\n");
    printf("MIN =======================\n");
    int min = DeletaMin(heapm);

    AmostraMin(heapm);
    AmostarArvoreM(heapm);

    deleteHeapM(heapm);
    return 0;
}
