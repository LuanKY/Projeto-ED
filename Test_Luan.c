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


/*
| Objetivos: Retorna true se a chave for encontrada. Neste caso, p
|            aponta para o No. Se a chave nao for encontrada, retorna false
|            e p aponta para o No que seria o seu pai (caso existisse).
*/
bool encontrarChave(TIPOCHAVE chave, HEAP **p, HEAP *arvore) {
  HEAP *pAnt = NULL;
  bool achou = false;
  *p = arvore;

  // Laco que fara o deslocamento de p ate que tenha chegado ao local onde
  // deveria estar o No ou tenha o encontrado
  while (!vazia(*p) && !achou) {
    pAnt = *p;
    if (chave == (*p)->item.chave)
      achou = true;
    else {
      if (chave < (*p)->item.chave)
        deslocar(NoEsquerdo, p);
      else
        deslocar(NoDireito, p);
    }
  }

  // Testa se nao achou a chave na arvore, pois nesse caso p devera estar
  //  apontando para o No que seria seu pai, ou seja, pAnt
  if (!achou)
    *p = pAnt;

  return achou;
}


/* Percorre a arvore ate encontrar a posicao de insercao, que sera a posicao
   que fará com que a arvore heap continue sendo completa ou quase completa, 
   apos isso, local recebe a posição de insercao */
void encontraPos(HEAP *arvore, HEAP **local) {
  if (!vazia(arvore)) {
    if((arvoreImcompleta(arvore)) && (*local == NULL)) { // O local de insercao e o filho esquerdo do NO mais a esquerda
      *local = arvore; 
      return;
    } else if ((arvoreImcompleta(arvore)) && (!arvoreQuaseCompleta(*local)) && (arvore->nivel < (*local)->nivel)) { // O local de insercao e o filho esquerdo do NO com nivel mais baixo
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


/* Insere o item passado no local de insercao correto, que e obtido atraves da funcao encontraPos */
void inserir(ITEM item, HEAP **arvore) {
  HEAP *local = NULL;
  encontraPos(*arvore, &local);
  if (vazia(*arvore)) {
    criarNo(item, arvore);
  } else if (arvoreImcompleta(local)) {
    adicionarFilho(item, NoEsquerdo, local);
  } else if (arvoreQuaseCompleta(local)) {
    adicionarFilho(item, NoDireito, local);
  }

  // Chama a funcao de reorganizar a arvore a partir da que foi inserida até chegar no NO Raiz
}

/*
  Objetivos: Procura um No que contenha uma chave igual a passada. Caso
             encontre, copia item sobre o item do No e retorna true. Se
             nao encontrar, retorna false
*/
void alterar(ITEM itemAnt, ITEM itemAtual, HEAP *arvore) {
  HEAP *p;
  if (encontrarChave(itemAnt.chave, &p, arvore)) {
    printf("//////////////////////\n");
    p->item = itemAtual;
  }
}



/////////////////////////////////////////////////////

int main() {
  HEAP *arv = NULL;
  ITEM item, item2;

  item.chave = 10;
  inserir(item, &arv);
  item.chave = 9;
  inserir(item, &arv);
  item.chave = 8;
  inserir(item, &arv);
  item.chave = 7;
  inserir(item, &arv);
  item.chave = 6;
  inserir(item, &arv);
  item.chave = 5;
  inserir(item, &arv);


  item2.chave = 25;
  alterar(item, item2, arv);

  

  /*          10
          9       8
        7  6    5          
  */

  // inserir(item, &arv);
  // encontraPos(arv, &local);
  // printf("(%d)", local->item.chave);
  // printf("//////////////////////\n");
  
  inOrdem(arv, imprimir);

  disposeArvore(arv);
  return 0;
}
