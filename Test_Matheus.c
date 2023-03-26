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

//Inicia aqui
HEAP* encontraPos(HEAP *arvore){
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
}
bool inserir(ITEM item, HEAP *arvore){
   HEAP *pPai;
   bool inseriu = true;
   if(vazia(arvore))
      criarNo(item,arvore);
   if(encontrarChave(item.chave, &pPai, arvore))
         inseriu = false;
   else {
      
   }
}

void heapify(HEAP *arvore){
   if(arvore->item.chave < arvore->esq->item.chave){
      
   }
}
