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


/*
| Objetivos: Cria um novo NO usando o apontador arvore passado contendo o item,
|            os apontadores para o pai e para os filhos contendo NULL e o nivel contendo 0;
*/
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
| Objetivo: Percorrer a arvore, visitando primeiro a subarvore esquerda,
|           depois a raiz e por ultimo a subarvore direita.
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
| Objetivo: Retornar true se a chave for encontrada. Neste caso, noDoItem
|           aponta para o No. Se a chave nao for encontrada, retorna false.
*/
bool encontrarChave(TIPOCHAVE chave, HEAP **noDoItem, HEAP *arvore) {
  if (!vazia(arvore)) {  
    if((arvore->item.chave == chave) && (*noDoItem == NULL)) {
      *noDoItem = arvore;
      return true;
    }     
    encontrarChave(chave, noDoItem, arvore->esq);
    encontrarChave(chave, noDoItem, arvore->dir);
  } else {
    return false;
  }

  if (*noDoItem != NULL)
    return true;
  else
    return false;
}


/*
| Objetivo: Partindo de um dado NO, verifica se a chave dele é menor
|           que a chave do seu pai, trocando assim suas respectivas
|           posicoes, ate chegar em uma chave menor ou igual que a sua, ou
|           ate quando chegar na raiz
*/
void heapify(HEAP *no) {
  // Cria uma copia do NO
  HEAP *noTemp = (HEAP*) malloc(sizeof(HEAP));
  noTemp->pai = no->pai;
  noTemp->esq = no->esq;
  noTemp->dir = no->dir;
  noTemp->nivel = no->nivel;
  noTemp->item = no->item;

  if (no->pai == NULL) {
    return;
  } else if (no->pai->item.chave < no->item.chave) { // Se o pai do NO possuir chave menor que a sua, eles trocam de posicao
    if (no == no->pai->esq) { // Verifica se o NO e filho esquerdo do seu pai
      // Alterando o NO para oculpar a posicao do seu pai
      no->nivel = no->pai->nivel;
      no->esq = no->pai;
      no->dir = no->pai->dir;
      no->pai = no->pai->pai;

      if (no->dir != NULL) // Verifica se o NO possui filho direito, e caso possua, faz com que esse filho o aponte como pai
        no->dir->pai = no;

      if (no->pai != NULL) { // Verifica se o NO e raiz, e caso nao seja, faz com que seu pai aponte para ele, com o lado correto
        if (no->pai->esq == no->esq)
          no->pai->esq = no;
      }
      // Alterando o pai do NO (que agora e filho esquerdo) para oculpar a posicao anterior do NO
      no->esq->nivel = noTemp->nivel;
      no->esq->pai = no;
      no->esq->esq = noTemp->esq;
      no->esq->dir = noTemp->dir;

      if (no->esq->esq != NULL) // Verifica se o antigo pai do NO possui filho esquerdo, e caso possua, faz com que esse filho o aponte como pai
        no->esq->esq->pai = no->esq;
      if (no->esq->dir != NULL) // Verifica se o antigo pai do NO possui filho direito, e caso possua, faz com que esse filho o aponte como pai
        no->esq->dir->pai = no->esq;

    } else if (no == no->pai->dir) { // Verifica se o NO e filho direito do seu pai
      // Alterando o NO para oculpar a posicao do seu pai
      no->nivel = no->pai->nivel;
      no->esq = no->pai->esq;
      no->dir = no->pai;
      no->pai = no->pai->pai;

      if (no->esq != NULL) // Verifica se o NO possui filho esquerdo, e caso possua, faz com que esse filho o aponte como pai
        no->esq->pai = no;

      if (no->pai != NULL) { // Verifica se o NO e raiz, e caso nao seja, faz com que seu pai aponte para ele, com o lado correto
        if (no->pai->dir == no->dir)
          no->pai->dir = no;
      }
      // Alterando o pai do NO (que agora e filho esquerdo) para oculpar a posicao anterior do NO
      no->dir->nivel = noTemp->nivel;
      no->dir->pai = no;
      no->dir->esq = noTemp->esq;
      no->dir->dir = noTemp->dir;

      if (no->dir->esq != NULL) // Verifica se o antigo pai do NO possui filho esquerdo, e caso possua, faz com que esse filho o aponte como pai
        no->dir->esq->pai = no->dir;
      if (no->dir->dir != NULL) // Verifica se o antigo pai do NO possui filho direito, e caso possua, faz com que esse filho o aponte como pai
        no->dir->dir->pai = no->dir;
    }
    free(noTemp);
    heapify(no);
  }
}


/*
| Objetivo: Percorrer a arvore ate encontrar a posicao de insercao, que sera a posicao
|           que fará com que a arvore heap continue sendo completa ou quase completa, 
|           apos isso, local recebe a posição de insercao 
*/
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


/*
| Objetivo: Inserir o item passado na posicao de insercao correta,
|           posicao essa, que e obtido atraves da funcao encontraPos.
*/
void inserirNo(ITEM item, HEAP **arvore) {
  HEAP *local = NULL;
  encontraPos(*arvore, &local); // Encontra a pos de insercao
  if (vazia(*arvore)) { // Se vazia, significa que e o primeiro NO da arvore
    criarNo(item, arvore);
  } else if (arvoreImcompleta(local)) { // Se incompleta, significa que sera inserido como NO esquerdo da arvore
    adicionarFilho(item, NoEsquerdo, local);
    heapify(local->esq); 
  } else if (arvoreQuaseCompleta(local)) { // Se quase completa, significa que sera inserido como NO direito da arvore
    adicionarFilho(item, NoDireito, local);
    heapify(local->dir);
  }
  deslocar(NoRaiz, arvore); // Garante que a arvore sempre seja utilizada a partir de sua raiz
}


/*
| Objetivo: Procurar um No que contenha uma chave igual a passada. Caso
|           encontre, copia o dado do No em item e retorna true. Se
|           nao encontrar, retorna false e item nao e modificado
*/
bool obterItem(TIPOCHAVE chave, ITEM *item, HEAP *arvore) {
  HEAP *noDoItem = NULL;
  if (encontrarChave(chave, &noDoItem, arvore)) {
    *item = noDoItem->item;
    return true;
  } else {
    return false;
  }
}


/*
| Objetivo: Procurar um No que contenha uma chave igual itemAnt. Caso
|           encontre, copia itemAtual sobre o item do No e retorna true. Se
|           nao encontrar, retorna false
*/
void alterarItem(ITEM itemAnt, ITEM itemAtual, HEAP **arvore) {
  HEAP *noDoItem = NULL;
  if (encontrarChave(itemAnt.chave, &noDoItem, *arvore)) {
    noDoItem->item = itemAtual;
    heapify(noDoItem);
    deslocar(NoRaiz, arvore); // Garante que a arvore sempre seja utilizada a partir de sua raiz
  }
}


/*
| Objetivo: Procurar um No que contenha uma chave igual a passada. Caso
|           encontre, move ele para que se torne uma folha da arvore e
|           depois limpa da memoria, apos isso reorganiza a arvore,
|           para que continue matendo as propriedades de uma heap
*/
bool removerNo() {
}


/////////////////////////////////////////////////////

int main() {
  HEAP *arv = NULL;
  ITEM item, item2;

  // Criar arvore inserindo os elementos de forma a manter sempre sendo uma arvore heap
  item.chave = 1;
  inserirNo(item, &arv);
  item.chave = 9;
  inserirNo(item, &arv);
  item.chave = 18;
  inserirNo(item, &arv);
  item.chave = 7;
  inserirNo(item, &arv);
  item.chave = 6;
  inserirNo(item, &arv);
  item.chave = 5;
  inserirNo(item, &arv);
  item.chave = 2;
  inserirNo(item, &arv);

  /*          18
          7       9
        1  6    5   2        
  */

 // Buscar item na arvore, usando a cahve como parametro de busca
  TIPOCHAVE ch = 2;
  if (obterItem(ch, &item2, arv))
    printf("Ok: obtido o dado do No com chave = %d\n", item2.chave);
  else
    printf("Erro: nao foi obtido o dado do No com chave = %d\n", ch);


  // Altera uma chave da arvore, e reorganiza ela a partir daquele determinado NO
  item2.chave = 25;
  alterarItem(item, item2, &arv);

  /*          25
          7       18
        1  6    5   9        
  */
  
  //Mostrar arvore completa
  inOrdem(arv, imprimir);

  //limpar arvore da memoria
  disposeArvore(arv);
  return 0;
}
