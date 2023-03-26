#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int TIPOCHAVE;

typedef enum {NoEsquerdo, NoDireito, NoPai, NoRaiz} DIRECAO;

typedef struct {
    TIPOCHAVE chave;
//    char valor[100];
} ITEM;

typedef struct estrutura
{
    ITEM item;
    struct estrutura *esq;
    struct estrutura *dir;
    struct estrutura *pai;
} HEAP;

// Inicializa a arvore binaria deixando-a pronta para ser utilizada.
void inicializar(HEAP *arvore)
{
    arvore = NULL;
}

// Retorna true se a arvore esta vazia (igual a NULL)

bool vazia(HEAP *arvore)
{
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
  (*arvore)->pai = NULL;
  (*arvore)->esq = NULL;
  (*arvore)->dir = NULL;
}

// Testa se o No indicado por Direcao a partir de arv existe
bool existeNo(DIRECAO direcao, HEAP *arvore)
{
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

// Visita um NO da arvore, imprimindo o valor da chave
// entre parenteses
void visite(HEAP *arvore) {
    printf("(%d)", arvore->item.chave);
}

/* HEAP* encontraPos(HEAP *arvore){
   if(!existeNo(NoEsquerdo,arvore)){
      return arvore->esq;
   }
   else{
      if(!existeNo(NoDireito,arvore)){
         return arvore->dir;
      }
      else {
         return encontraPos(arvore->esq) && encontraPos(arvore->dir);
      }
   }
} */
void swap(ITEM *a, ITEM *b) {
    ITEM temp = *a;
    *a = *b;
    *b = temp;
}

bool inserir(ITEM item, HEAP *arvore){
   if(vazia(arvore)){
      criarNo(item,arvore);
   }
   HEAP *pAtual = arvore;
   while(1){
      if(pAtual->esq == NULL){
         pAtual->esq = arvore;
         arvore->pai = pAtual;
         break;
      }
      else if(pAtual->dir == NULL){
         pAtual->dir = arvore;
         arvore->pai = pAtual;
         break;
      }
      else {
         if(pAtual->esq->esq == NULL || pAtual ->esq->dir == NULL){
            pAtual = pAtual->esq;
         } else pAtual = pAtual->dir;
      }
   }
   while(arvore->pai != NULL && arvore->item.chave > arvore->pai->item.chave) {
      swap(&arvore->item, &arvore->pai->item);
      arvore = arvore->pai;
   }
}

/* void heapify(HEAP *arvore){
   if(arvore->item.chave < arvore->esq->item.chave){
      HEAP *aux = arvore->esq;
      if(aux->esq == NULL && aux->dir == NULL){
         aux->pai = arvore->pai;
         arvore->pai = aux;
         arvore->esq == NULL;
         aux->dir = arvore->dir;
         arvore->dir = NULL;
         aux->dir->pai = aux;
      }
   }
} */

