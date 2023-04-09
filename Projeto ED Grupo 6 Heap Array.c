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

int Tamanho(Heap *heap){
    printf("Tamanho: %d\n", heap->tamanho);
}

/*Função: troca de ponteiros para percorrer a arvore*/
void ajuste(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

/*reoganiza a arvore de forma que obedeça as propriedades de uma heap*/

void HeapifyIns(Heap* heap, int i) {
    int pai = (i - 1) / 2;
    while (i > 0 && heap->itens[i] > heap->itens[pai]) {
        ajuste(&heap->itens[i], &heap->itens[pai]);
        pai = (i - 1) / 2; // novo pai do elemento pós troca

    }
}
void heapifyInsM(MinHeap *heap, int i) {
    while (i > 0 && heap->itens[(i - 1) / 2] > heap->itens[i]) {
        ajuste(&heap->itens[(i - 1) / 2], &heap->itens[i]);
        i = (i - 1) / 2;}}

/*Dado um valor chave o item é inserido no final da arvore*/

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

/*Função: Ajusta os elementos para que as propridades sejam aceitas pós remoção*/

void HeapifyDel(Heap* heap, int i) { // i = indice
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;
    int maior = i;
    if (esq < heap->tamanho && heap->itens[esq] > heap->itens[maior]) { // compara se o elemento esquerdo é maior que o seu indice  superior
        maior = esq;}
    if (dir < heap->tamanho && heap->itens[dir] > heap->itens[maior]) {// compara se o elemento direito é maior que o seu indice  superior
        maior = dir;}
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
/*Deleta a raiz da arvore e reogarniza com o heapify*/
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
    if(heap->tamanho == 0){
        printf("Arvore Vazia!\n");
    }
    else{
    printf("Heap: ");
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d ", heap->itens[i]);
    }
    printf("\n");
    }

    Tamanho(heap);
}


void AmostraRaiz(Heap *heap){
    if(heap->tamanho == 0){
        printf("Arvore Vazia!");}
    else{
        printf("Raiz: %d\n", heap->itens[0]);}
    }

void Cauda(Heap *heap){
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
    AmostraRaiz(heap);
    Cauda(heap);
    inserir(heap, 5);
    AmostraArvore(heap);
    printf("\n");
    printf("MAX =======================\n");
    DeletaMax(heap);
    AmostraRaiz(heap);
    AmostraArvore(heap);

    for(int i = 0; heap->tamanho != 0; i++){
        printf("\nMAX =======================\n");
        DeletaMax(heap);
        AmostraArvore(heap);
    }


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
    AmostraArvore(heapm);
    AmostraRaiz(heapm);
    Cauda(heapm);
    inserirM(heapm, 5);
    AmostraArvore(heapm);
    printf("\n");
    printf("MIN =======================\n");
    DeletaMin(heapm);

    AmostraRaiz(heapm);
    AmostraArvore(heapm);

    for(int i = 0; heap->tamanho != 0; i++){
        printf("\nMIN =======================\n");
        DeletaMin(heapm);
        AmostraArvore(heapm);
    }
    deleteHeap(heapm);
    return 0;
}
