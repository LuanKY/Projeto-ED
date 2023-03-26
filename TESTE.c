#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct HEAP {
  int chave;
  struct HEAP *esq;
  struct HEAP *dir;
} HEAP;

HEAP *criarNo(int chave) {
  HEAP *novoNo = (HEAP*) malloc(sizeof(HEAP));
  novoNo->chave = chave;
  novoNo->esq = NULL;
  novoNo->dir = NULL;
  return novoNo;
}

void inserir(HEAP **arvore, int chave) {
    HEAP *novoNo = criarNo(chave);
    HEAP *noTemp = *arvore;
    HEAP *noPai = NULL;

    while (noTemp != NULL) {
      noPai = noTemp;
      if (chave < noTemp->chave) {
        noTemp = noTemp->esq;
      } else {
        noTemp = noTemp->dir;
      }
    }

    if (noPai == NULL) {
      *arvore = novoNo;
    } else if (chave < noPai->chave) {
      noPai->esq = novoNo;
    } else {
      noPai->dir = novoNo;
    }
}

void trocar(int *a, int *b) {
    int noTemp = *a;
    *a = *b;
    *b = noTemp;
}

void heapify(HEAP *arvore) {
  if (arvore == NULL) {
    return;
  }

  HEAP *noTemp = arvore;

  if (arvore->esq != NULL && arvore->esq->chave > noTemp->chave) {
    noTemp = arvore->esq;
  }

  if (arvore->dir != NULL && arvore->dir->chave > noTemp->chave) {
    noTemp = arvore->dir;
  }

  if (noTemp != arvore) {
    trocar(&arvore->chave, &noTemp->chave);
    heapify(noTemp);
  }
}

void delete(HEAP **arvore) {
  if (*arvore == NULL) {
    return;
  }

  if ((*arvore)->esq == NULL && (*arvore)->dir == NULL) {
    free(*arvore);
    *arvore = NULL;
    return;
  }

  HEAP *noTemp = *arvore;
  HEAP *noPai = NULL;

  while (noTemp->dir != NULL) {
    noPai = noTemp;
    noTemp = noTemp->dir;
  }

  (*arvore)->chave = noTemp->chave;

  if (noPai != NULL) {
    noPai->dir = NULL;
  } else {
    (*arvore)->esq = NULL;
  }

  free(noTemp);

  heapify(*arvore);
}

void printHeap(HEAP *arvore) {
  if (arvore == NULL) {
    return;
  }

  printf("%d ", arvore->chave);
  printHeap(arvore->esq);
  printHeap(arvore->dir);
}

int main() {
  HEAP *arvore = NULL;

  inserir(&arvore, 8);
  inserir(&arvore, 5);
  inserir(&arvore, 12);
  inserir(&arvore, 3);
  inserir(&arvore, 7);
  inserir(&arvore, 10);
  inserir(&arvore, 15);

  printf("Heap: ");
  printHeap(arvore);
  printf("\n");

  delete(&arvore);

  printf("Heap apos a exclusao: ");
  printHeap(arvore);
  printf("\n");

  return 0;
}
