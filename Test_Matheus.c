#include <stdio.h>
#include <stdlib.h>

#define raiz_HEAP_SIZE 100

int heap[raiz_HEAP_SIZE];
int tamanho = 0;

// Função auxiliar para trocar dois elementos em um array
void troca(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para inserir um elemento na Heap
void insert(int value) {
    if (tamanho >= raiz_HEAP_SIZE) {
        printf("Erro: Heap cheia!\n");
        return;
    }
    heap[tamanho] = value;
    int atual = tamanho;
    while (heap[atual] > heap[(atual-1)/2]) {
        troca(&heap[atual], &heap[(atual-1)/2]);
        atual = (atual-1)/2;
    }
    tamanho++;
}

// Função para remover o elemento de maior valor da Heap
int remover() {
    if (tamanho == 0) {
        printf("Erro: Heap vazia!\n");
        return -1;
    }
    int raiz = heap[0];
    heap[0] = heap[tamanho-1];
    tamanho--;
    int atual = 0;
    while (atual*2+1 < tamanho) {
        int filho = atual*2+1;
        if (filho+1 < tamanho && heap[filho+1] > heap[filho]) {
            filho++;
        }
        if (heap[atual] < heap[filho]) {
            troca(&heap[atual], &heap[filho]);
            atual = filho;
        } else {
            break;
        }
    }
    return raiz;
}

// Função para imprimir a Heap
void printHeap() {
    printf("Heap: ");
    for (int i = 0; i < tamanho; i++) {
        printf("%d ", heap[i]);
    }
    printf("\n");
}

int main() {
    insert(10);
    insert(5);
    insert(20);
    insert(8);
    insert(15);
    printHeap();
    int raiz = remover();
    printf("Elemento removido: %d\n", raiz);
    printHeap();
    raiz = remover();
    printf("Elemento removido: %d\n", raiz);
    printHeap();
    insert(25);
    printHeap();
    return 0;
}
