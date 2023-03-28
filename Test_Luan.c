#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int TIPOCHAVE;

typedef enum {NoEsquerdo, NoDireito, NoPai, NoRaiz} DIRECAO;

typedef struct {
  TIPOCHAVE chave;
} ITEM;

typedef struct estrutura {
  ITEM item;
  int nivel;
  struct estrutura *esq;
  struct estrutura *dir;
  struct estrutura *pai;
} HEAP;


// Retorna true se a arvore esta vazia (igual a NULL)
bool vazia(HEAP *arvore) {
  return arvore == NULL;
}


// Cria um novo no usando o apontador arvore passado contendo o item,
// os apontadores para o pai e para os filhos contendo NULL
void criarNo(ITEM item, HEAP **arvore) {
  if (!vazia(*arvore)) {
    printf("ERRO: O no deve estar vazio para ser criado.");
    exit(EXIT_FAILURE);
  }

  *arvore = (HEAP*) malloc(sizeof(HEAP));
  (*arvore)->item = item;
  (*arvore)->nivel = 0;
  (*arvore)->pai = NULL;
  (*arvore)->esq = NULL;
  (*arvore)->dir = NULL;
}


// Testa se o No indicado por Direcao a partir de arv existe
bool existeNo(DIRECAO direcao, HEAP *arvore) {
  if (vazia(arvore))
    return false;

  if (direcao == NoRaiz)
    return true;
  
  if (direcao == NoPai)
    return !vazia(arvore->pai);

  if (direcao == NoEsquerdo)
    return !vazia(arvore->esq);

  if (direcao == NoDireito)
    return !vazia(arvore->dir);

  return false;
}


// Deslocar o apontador Arvore para o No indicado por Direcao
void deslocar(DIRECAO direcao, HEAP **arvore) {
  if (direcao == NoRaiz) {
    while (existeNo(NoPai, *arvore)) {
      *arvore = (*arvore)->pai;
    }
  }
  if (direcao == NoPai)
    *arvore = (*arvore)->pai;

  if (direcao == NoEsquerdo)
    *arvore = (*arvore)->esq;

  if (direcao == NoDireito)
    *arvore = (*arvore)->dir;
}


/* Verifica se a arvore possui filho a esquerda e a direita */
bool arvoreCompleta(HEAP *arvore) {
  if (vazia(arvore))
    return false;
  if (!vazia(arvore->esq) && !vazia(arvore->dir)) {
    return true;
  } else
    return false;
}


/* Verifica se a arvore possui filho a esquerda */
bool arvoreQuaseCompleta(HEAP *arvore) {
  if (vazia(arvore))
    return false;
  if (!vazia(arvore->esq) && vazia(arvore->dir)) {
    return true;
  } else
    return false;
}

/* Verifica se a arvore possui nao possui filhos */
bool arvoreImcompleta(HEAP *arvore) {
  if (!existeNo(NoEsquerdo, arvore) && !existeNo(NoDireito, arvore))
    return true;
  else
    return false;
}


// Cria um filho no NO apontado por Arvore na direcao informada
bool adicionarFilho(ITEM item, DIRECAO direcao, HEAP *arvore) {
  if (vazia(arvore) || (direcao == NoPai) || (direcao == NoRaiz) || existeNo(direcao, arvore))
    return false;  // Criacao ilegal de um filho

  if (direcao == NoEsquerdo) {
    criarNo(item, &(arvore->esq));
    arvore->esq->pai = arvore;
    arvore->esq->nivel = arvore->esq->pai->nivel + 1;
  } else {
    criarNo(item, &(arvore->dir));
    arvore->dir->pai = arvore;
    arvore->dir->nivel = arvore->dir->pai->nivel + 1;
  }
  return true;
}


// Desaloca da memoria toda a arvore
void disposeArvore(HEAP *arvore) {
  if (!vazia(arvore)) {
    disposeArvore(arvore->esq);
    disposeArvore(arvore->dir);
    free(arvore);
  }
}


/*
| Objetivos: Percorre a arvore, visitando primeiro a subarvore esquerda,
|            depois a raiz e por ultimo a subarvore direita.
*/
void inOrdem(HEAP *arvore, void (*visite)(HEAP*) ) {
  if (!vazia(arvore)) {         
    inOrdem(arvore->esq, visite);
    visite(arvore);
    inOrdem(arvore->dir, visite);
  }
}


// Visita um NO da arvore, imprimindo o valor da chave entre parenteses
void imprimir(HEAP *arvore) {
  printf("\nChave do NO: (%d) Nivel do NO: (%d)", arvore->item.chave, arvore->nivel);
}


/* Percorre a arvore ate encontrar a posicao de insercao, que sera a posicao
   que fará com que a arvore heap continue sendo completa ou quase completa, 
   apos isso, local recebe a posição de insercao */
void encontraPos(HEAP *arvore, HEAP **local) {
  if (!vazia(arvore)) {
    if((arvoreImcompleta(arvore)) && (*local == NULL)) { // O local de insercao e o filho esquerdo do NO
      *local = arvore; 
      return;
    } else if ((arvoreImcompleta(arvore)) && (!arvoreQuaseCompleta(*local)) && (arvore->nivel < (*local)->nivel)) {
      *local = arvore; 
      return;
    } else if (arvoreQuaseCompleta(arvore)) { // O local de insercao e o filho direito do NO
      *local = arvore; 
      return;
    } else if (arvoreCompleta(arvore)) { // Possui dois filhos então entao deve procurar em suas sub-arvores
      encontraPos(arvore->esq, local);
      encontraPos(arvore->dir, local);
    }
  }
  return;
}


/////////////////////////////////////////////////////

int main() {
  HEAP *arv = NULL;
  HEAP *local = NULL;
  ITEM item;

  item.chave = 10;
  criarNo(item, &arv);
  item.chave = 9;
  adicionarFilho(item, NoEsquerdo, arv);
  item.chave = 8;
  adicionarFilho(item, NoEsquerdo, arv->esq);
  item.chave = 7;
  adicionarFilho(item, NoDireito, arv->esq);
  item.chave = 6;
  adicionarFilho(item, NoDireito, arv);
  

  /*          10
          9       6
        8   7              
  */
  encontraPos(arv, &local);
  printf("(%d)", local->item.chave);
  // printf("//////////////////////\n");
  
  // inOrdem(arv, imprimir);

  disposeArvore(arv);
  return 0;
}
