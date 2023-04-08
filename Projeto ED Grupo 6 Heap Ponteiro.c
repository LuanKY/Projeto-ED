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


/*
| Objetivo: Percorrer a arvore, da subarvore esquerda para a subarvore,
|           direita, apos achar o ultimo NO da arvore que possua o 
|           nivel maximo da arvore, apos isso retorna ele
*/
HEAP* ultimoNo(HEAP* arvore, HEAP* ultimo) {
  if (arvore == NULL) { // Caso a arvore seja vazia, retorna o ultimo NO visitado
    return ultimo;
  }

  HEAP* ultimo_esq = ultimoNo(arvore->esq, arvore);
  HEAP* ultimo_dir = ultimoNo(arvore->dir, arvore);

  // Faz a comparacao entre os niveis dos ultimos NOs visitados na subarvore esquerda e na subarvore direita
  int nivel_ultimo_esq = ultimo_esq != NULL ? ultimo_esq->nivel : -1;
  int nivel_ultimo_dir = ultimo_dir != NULL ? ultimo_dir->nivel : -1;
  
  if (arvore->nivel >= nivel_ultimo_esq && arvore->nivel >= nivel_ultimo_dir) { // Verifica se o nivel do NO atual e maior ou igual aos niveis dos ultimos NOs visitados, e se for, significa que este e o ultimo NO da arvore que possui o nivel mais alto
    return arvore;
  } else if (nivel_ultimo_dir >= nivel_ultimo_esq) { // Verifica se o nivel do ultimo NO visitado na subarvore direita e maior ou igual ao nivel do ultimo NO visitado na subarvore esquerda, significa que ele e o ultimo NO da arvore
    return ultimo_dir;
  } else { // Significa que o ultimo NO visitado na subarvore esquerda e o ultimo NO da arvore
    return ultimo_esq;
  }
}


/*
| Objetivo: Percorrer a arvore pelo lado esquerdo e apos achar o
|           NO mais inferior a esquerda, retorna ele
*/
HEAP* primeiroNo(HEAP* no) {
  if (no == NULL) { // Verifica se chegou no final da arvore
    return NULL;
  }
  while (no->esq != NULL) { // Percorre a arvore ate encontrar o NO mais inferior a esquerda
    no = no->esq;
  }
  return no;
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
| Objetivo: Captura o ultimo NO da arvore, e caso ele possua pai,
|           faz com que o seu pai aponte pra NULL pelo lado certo, 
|           apos isso re-insere esse no na arvore
*/
void ajuste(HEAP **arvore) {
  HEAP *ultimo = ultimoNo(*arvore, NULL);

  if (ultimo->pai != NULL) {
    if (ultimo->pai->esq == ultimo) // Verifica se o avo do NO aponta para ele pelo lado esquerdo, e caso aponte, faz com que esse avo aponte para seu pai pelo lado esquerdo
      ultimo->pai->esq = NULL;
    if (ultimo->pai->dir == ultimo) // Verifica se o avo do NO aponta para ele pelo lado direito, e caso aponte, faz com que esse avo aponte para seu pai pelo lado direito
      ultimo->pai->dir = NULL;
    
    inserirNo(ultimo->item, arvore);
    free(ultimo);
  }
}


/*
| Objetivo: Procurar um No que contenha uma chave igual a passada. Caso
|           encontre, move ele para que se torne uma folha da arvore e
|           depois limpa da memoria, apos isso reorganiza a arvore,
|           para que continue matendo as propriedades de uma heap
*/
bool removerNo(TIPOCHAVE chave, HEAP **arvore, bool recursao) {
  HEAP *no = NULL;

  if (encontrarChave(chave, &no, *arvore)) {
    // Cria uma copia do NO
    HEAP *noTemp = (HEAP*) malloc(sizeof(HEAP));
    noTemp->pai = no->pai;
    noTemp->esq = no->esq;
    noTemp->dir = no->dir;
    noTemp->nivel = no->nivel;
    noTemp->item = no->item;

    // Salva o ultimo NO da arvore, aquele que substituira o NO removido
    HEAP *ultimo = NULL;
    ultimo = ultimoNo(*arvore, ultimo);

    if ((ultimo == no) && (recursao == false)) { // Se for o NO mais a direita da arvore e nao for uma recursão, elimina o NO da arvore
      if (no->pai != NULL) {
        
        if(no->pai->esq == no) {
          no->pai->esq = NULL; // Elimina o NO da arvore
        } else if(no->pai->dir == no) {
          no->pai->dir = NULL;
        }
      }

      free(no);
      free(noTemp);
      deslocar(NoRaiz, arvore); // Garante que a arvore sempre seja utilizada a partir de sua raiz
      return true;
    }

    if ((no->esq == NULL) && (no->dir == NULL)) { // Se nao tiver nenhum filho, altera o pai pra NULL, limpa da memoria e realoca o novo filho dele
      if (ultimo->nivel >= no->nivel) { // Garante que o ultimo pode ser trocado com o NO sem perder suas propriedades
        if(no->pai->esq == no) {
          no->pai->esq = NULL; // Elimina o NO da arvore

          if(ultimo->pai->esq == ultimo) {
            ultimo->pai->esq = NULL;
          } else if(ultimo->pai->dir == ultimo) {
            ultimo->pai->dir = NULL;
          }

          no->pai->esq = ultimo;
        } else if(no->pai->dir == no) {
          no->pai->dir = NULL; // Elimina o NO da arvore
          ultimo = ultimoNo(*arvore, ultimo);
          
          if (no->pai->esq != ultimo) { // Se o ultimo nao for o filho esquerdo do pai do NO
            if(ultimo->pai->esq == ultimo) {
              ultimo->pai->esq = NULL;
            } else if(ultimo->pai->dir == ultimo) {
              ultimo->pai->dir = NULL;
            }
            no->pai->dir = ultimo;
          }
      }
      
      ultimo->nivel = no->nivel;
      ultimo->pai = no->pai;
      free(no);
      heapify(ultimo);
      }
      return true;
    }

    if ((no->esq != NULL) && (no->dir == NULL)) { // Se tiver um filho e esse filho for o da esquerda
      // Alterando o NO para oculpar a posicao do seu filho esquerdo
      no->nivel = no->esq->nivel;
      no->pai = no->esq;
      no->esq = NULL;
      no->dir = NULL;

      // Alterando o filho esquerdo do NO (que agora e seu pai) para oculpar a posicao anterior do NO
      no->pai->nivel = noTemp->nivel;
      no->pai->pai = noTemp->pai;
      no->pai->esq = no;
      no->pai->dir = NULL;

      if (no->pai->pai != NULL) {
        if (no->pai->pai->esq == no) // Verifica se o avo do NO aponta para ele pelo lado esquerdo, e caso aponte, faz com que esse avo aponte para seu pai pelo lado esquerdo
          no->pai->pai->esq = no->pai;
        if (no->pai->pai->dir == no) // Verifica se o avo do NO aponta para ele pelo lado direito, e caso aponte, faz com que esse avo aponte para seu pai pelo lado direito
          no->pai->pai->dir = no->pai;
      }
      
      free(noTemp);
      
      deslocar(NoRaiz, arvore); // Garante que a arvore sempre seja utilizada a partir de sua raiz
      if (primeiroNo(*arvore) == ultimoNo(*arvore, NULL))
        removerNo(chave, arvore, false);
      else
        removerNo(chave, arvore, true);
    }

    if ((no->esq != NULL) && (no->dir != NULL)) { // Se tiver os dois filho, troca com o maior entre os filhos, e chama a funcao novamente
      if (no->dir->item.chave >= no->esq->item.chave) {
        // Alterando o NO para oculpar a posicao do seu filho esquerdo
        no->nivel = no->esq->nivel;
        no->pai = no->dir;
        no->esq = no->dir->esq;
        no->dir = no->dir->dir;

        // Alterando o filho esquerdo do NO (que agora e seu pai) para oculpar a posicao anterior do NO
        no->pai->nivel = noTemp->nivel;
        no->pai->pai = noTemp->pai;
        no->pai->esq = noTemp->esq;
        no->pai->dir = no;

        if (no->pai->pai != NULL) { // Verifica se o NO e raiz, caso nao seja faz com que esse avo aponte para seu pai pelo lado direito
          no->pai->pai->dir = no->pai;
        }

        // Alterando seu antigo filho esquerdo para apontar para o atual pai
        no->pai->esq->pai = no->pai;
      } else {
        // Alterando o NO para oculpar a posicao do seu filho esquerdo
        no->nivel = no->esq->nivel;
        no->pai = no->esq;
        no->dir = no->esq->dir;
        no->esq = no->esq->esq;

        // Alterando o filho esquerdo do NO (que agora e seu pai) para oculpar a posicao anterior do NO
        no->pai->nivel = noTemp->nivel;
        no->pai->pai = noTemp->pai;
        no->pai->esq = no;
        no->pai->dir = noTemp->dir;

        if (no->pai->pai != NULL) { // Verifica se o NO e raiz, caso nao seja faz com que esse avo aponte para seu pai pelo lado esquerdo
          no->pai->pai->esq = no->pai;
        }

        // Alterando seu antigo filho esquerdo para apontar para o atual pai
        no->pai->dir->pai = no->pai; 
      }

      if (no->esq != NULL) { // Verifica se NO tem filho esquerdo e caso tenha, faz com que esse filho aponte para ele como pai
        no->esq->pai = no;
      } else if (no->dir != NULL) { // Verifica se NO tem filho direito e caso tenha, faz com que esse filho aponte para ele como pai
        no->dir->pai = no;
      }

      free(noTemp);

      deslocar(NoRaiz, arvore); // Garante que a arvore sempre seja utilizada a partir de sua raiz
      if (primeiroNo(*arvore) == ultimoNo(*arvore, NULL))
        removerNo(chave, arvore, false);
      else
        removerNo(chave, arvore, true);
    }
  } else {
    return true;
  }
  return true;
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





/////////////////////////////////////////////////////

int main() {
  HEAP *arv = NULL;
  ITEM item, item2;

  // Criar arvore inserindo os elementos de forma a manter sempre sendo uma arvore heap
  printf("\Inserindo 1\n");
  item.chave = 1;
  inserirNo(item, &arv);
  printf("\Inserindo 9\n");
  item.chave = 9;
  inserirNo(item, &arv);
  printf("\Inserindo 18\n");
  item.chave = 18;
  inserirNo(item, &arv);
  printf("\Inserindo 7\n");
  item.chave = 7;
  inserirNo(item, &arv);
  printf("\Inserindo 6\n");
  item.chave = 6;
  inserirNo(item, &arv);
  printf("\Inserindo 5\n");
  item.chave = 5;
  inserirNo(item, &arv);
  printf("\Inserindo 2\n");
  item.chave = 2;
  inserirNo(item, &arv);

  printf("\nImprimindo Arvore...\n");
  inOrdem(arv, imprimir);

  /*          18
          7       9
        1  6    5   2        
  */


 // Buscar item na arvore, usando a cahve como parametro de busca
  TIPOCHAVE ch = 2;
  if (obterItem(ch, &item2, arv))
    printf("\n\nConseguimos obter o dado do No com chave = %d\n", item2.chave);
  else
    printf("\n\nErro: nao foi obtido o dado do No com chave = %d\n", ch);

  // Altera uma chave da arvore, e reorganiza ela a partir daquele determinado NO
  printf("\nAlterando chave 2 para 25\n");
  item2.chave = 25;
  alterarItem(item, item2, &arv);
  

  /*          25
          7       18
        1  6    5   9        
  */
  printf("\nImprimindo Arvore...\n");
  inOrdem(arv, imprimir);

  printf("\n\nRemovendo 25\n");
  removerNo(25, &arv, false);
  

  /*          18
          7       9
        1  6    5           
  */

  printf("\nImprimindo Arvore...\n");
  inOrdem(arv, imprimir);

  printf("\n\nRemovendo 1\n");
  removerNo(1, &arv, false);

  /*          18
          7       9
        5  6               
  */
  
  //Mostrar arvore completa
  printf("\nImprimindo Arvore...\n");
  inOrdem(arv, imprimir);

  printf("\n\nRemovendo 9\n");
  removerNo(9, &arv, false);

  /*          18
          7       6
        5                 
  */
  
  //Mostrar arvore completa
  printf("\nImprimindo Arvore...\n");
  inOrdem(arv, imprimir);

  //limpar arvore da memoria
  disposeArvore(arv);
  return 0;
}
