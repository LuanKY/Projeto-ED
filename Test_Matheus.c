#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 100

int heap[MAX_HEAP_SIZE];
int heapSize = 0;

// Função auxiliar para trocar dois elementos em um array
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Função para inserir um elemento na Heap
void insert(int value) {
    if (heapSize >= MAX_HEAP_SIZE) {
        printf("Erro: Heap cheia!\n");
        return;
    }
    heap[heapSize] = value;
    int current = heapSize;
    while (heap[current] > heap[(current-1)/2]) {
        swap(&heap[current], &heap[(current-1)/2]);
        current = (current-1)/2;
    }
    heapSize++;
}

// Função para remover o elemento de maior valor da Heap
int removeMax() {
    if (heapSize == 0) {
        printf("Erro: Heap vazia!\n");
        return -1;
    }
    int max = heap[0];
    heap[0] = heap[heapSize-1];
    heapSize--;
    int current = 0;
    while (current*2+1 < heapSize) {
        int child = current*2+1;
        if (child+1 < heapSize && heap[child+1] > heap[child]) {
            child++;
        }
        if (heap[current] < heap[child]) {
            swap(&heap[current], &heap[child]);
            current = child;
        } else {
            break;
        }
    }
    return max;
}

// Função para imprimir a Heap
void printHeap() {
    printf("Heap: ");
    for (int i = 0; i < heapSize; i++) {
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
    int max = removeMax();
    printf("Elemento removido: %d\n", max);
    printHeap();
    max = removeMax();
    printf("Elemento removido: %d\n", max);
    printHeap();
    insert(25);
    printHeap();
    return 0;
}
