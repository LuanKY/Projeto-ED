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
  struct estrutura *esq;
  struct estrutura *dir;
  struct estrutura *pai;
} HEAP;


// Inicializa a arvore binaria deixando-a pronta para ser utilizada.
void inicializar(HEAP *arvore) {
  arvore = NULL;
}


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


/*
  Objetivo: Altera o valor do item armazenado no No da Arvore.
            Caso tenha sucesso, retorna true. Caso contrário, false.
*/
bool alterarItem(ITEM item, HEAP *arvore) {
  if (vazia(arvore))
    return false;

  arvore->item = item;
  return true;
}


// Cria um filho no NO apontado por Arvore na direcao informada
bool adicionarFilho(ITEM item, DIRECAO direcao, HEAP *arvore) {
  if (vazia(arvore) || (direcao == NoPai) || (direcao == NoRaiz) || existeNo(direcao, arvore))
    return false;  // Criacao ilegal de um filho

  if (direcao == NoEsquerdo) {
    criarNo(item, &(arvore->esq));
    arvore->esq->pai = arvore;
  } else {
    criarNo(item, &(arvore->dir));
    arvore->dir->pai = arvore;
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
| Objetivos: Desaloca da memoria arvore e seus descendentes, atualizando, se
|            necessario, o apontador do pai dessa arvore ou atribuindo o valor
|            NULL a arvore, quando for a raiz.
*/
void deltree(HEAP **arvore) {
  HEAP *pTemp = *arvore;

  /* 
    Testa se Arvore tem pai. Caso tenha, arvore se desloca para ele e pTemp
    continua apontando para o inicio da arvore a ser deletada, depois a
    arvore e apagada e o apontador do pai e atualizado com NULL. Caso Arvore
    nao tenha pai, a arvore e eliminada usando pTemp e arvore recebe NULL */
  if (existeNo(NoPai, *arvore)) {
    deslocar(NoPai, arvore);
    disposeArvore(pTemp);
    if ((*arvore)->esq == pTemp)
      (*arvore)->esq = NULL;
    else
      (*arvore)->dir = NULL;
  } else {
    disposeArvore(pTemp);
    arvore = NULL;
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
  printf("(%d)", arvore->item.chave);
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


/*
| Objetivos: Tenta inserir um No na Arvore Binaria de Busca apontada por arvore.
|            Se conseguir, retorna true.
*/
bool inserir(ITEM item, HEAP **arvore) {
  HEAP *pPai;
  bool inseriu = true;

  // Se arvore estiver vazia entao so e necessario criar o No, mas se nao
  // estiver, entao sera feita a procura (pela chave) na arvore. Se for achada
  // alguma ocorrencia da chave na arvore (chave duplicada), entao retornara
  // false. Caso contrario item sera adicionado em uma das subarvores de Arvore.
  if (vazia(*arvore))
    criarNo(item, arvore);
  else {
    if (encontrarChave(item.chave, &pPai, *arvore))
      inseriu = false;
    else {
      if (item.chave < pPai->item.chave)
        adicionarFilho(item, NoEsquerdo, pPai);
      else
        adicionarFilho(item, NoDireito, pPai);
    }
  }
  return inseriu;
}


/*
  Objetivos: Procura um No que contenha uma chave igual a passada. Caso
             encontre, copia o dado do No em item e retorna true. Se
             nao encontrar, retorna false e item nao e modificado
*/
bool obter(TIPOCHAVE chave, ITEM *item, HEAP *arvore) {
  HEAP *p;
  if (encontrarChave(chave, &p, arvore)) {
    *item = p->item;
    return true;
  } else {
    return false;
  }
}


//Possivel transformação em heapify
/*
  Objetivo: Determina o sucessor imediato do No apontado por Arvore. Quando
            encontrar, copia para arvore os dados contidos no Sucessor e
            muda o apontador sucessor para o seu No direito. No final,
            apaga o No onde se encontrava o Sucessor
*/
void substituir(HEAP **sucessor, HEAP **arvore) {
  HEAP *pApagar;
  // Se nao ha mais Nos a esquerda, entao sucessor ja eh o sucessor imediato
  if (vazia((*sucessor)->esq)) {
    (*arvore)->item = (*sucessor)->item;
    pApagar = *sucessor;

    // Se existir, corrige o apontador pai do seu filho direito
    // de sucessor, fazendo-o apontar para o pai do sucessor
    if (!vazia((*sucessor)->dir))
      (*sucessor)->dir->pai = (*sucessor)->pai;

    *sucessor = (*sucessor)->dir;
    free(pApagar);
  } else {
    substituir(&((*sucessor)->esq), arvore);
  }
}


/*
| Objetivos: Retira o No contendo chave da arvore apontada por arvore. Se
|            esse No for retirado retorna true, caso contrario, retorna false.
*/
bool remover(TIPOCHAVE chave, HEAP **arvore) {
  HEAP *pApagar;

  /* Se a arvore estiver vazia, o No nao sera retirado, pois nao existe. Mas
    se nao estiver vazia, sera feita a procura atraves de chamadas
    recursivas de remover, pegando a subarvore esquerda quando chave for
    menor que o valor da chave no No ou a subarvore direita quando for maior.
    Existem dois pontos de parada, o primeiro quando a subarvore onde esta
    sendo feita a busca esta vazia e o segundo quando a chave for encontrada.
    Nesse caso acontecera a remocao */
  if (vazia(*arvore)) 
    return false;

  if (chave < (*arvore)->item.chave)
    return remover(chave, &((*arvore)->esq));

  if (chave > (*arvore)->item.chave)
    return remover(chave, &((*arvore)->dir));

  /* Como nesse ponto item = arvore->item.chave, devemos verificar
    se o No possui duas, uma ou nenhuma subarvore. No primeiro caso
    deve ser procurado na subarvore direita o sucessor imediato do No
    e coloca-lo no lugar do No removido. Nos outros dois casos, so
    e necessario remover o No e ajustar os apontadores */

  if (vazia((*arvore)->esq)) {  // Arvore nao tem subarvores ou tem somente a direita
    pApagar = *arvore;

    /* Se existir, corrige o apontador pai do filho direito do No a 
        ser removido, fazendo-o apontar para o pai do No a ser removido */
    if (!vazia((*arvore)->dir))
      (*arvore)->dir->pai = (*arvore)->pai;

    (*arvore) = (*arvore)->dir;
    free(pApagar);
  } else {
    if (vazia((*arvore)->dir)) { // Arvore tem somente o filho esquerdo
      pApagar = (*arvore);

      /* Corrige o apontador pai do filho esquerdo do No a ser removido,
        fazendo-o apontar para o pai do No a ser removido */
      (*arvore)->esq->pai = (*arvore)->pai;
      *arvore = (*arvore)->esq;
      free(pApagar);
    } else {
      substituir(&((*arvore)->dir), arvore);
    }
  }
  return true;
}

/////////////////////////////////////////////////////
//Possivel transformar em Verificação se a arvore é ou não Heap
bool encadeamentoComNoPaiCorreto(HEAP *pai, HEAP *filhoEsq, HEAP *filhoDir) {
  bool ok = true;
  if (!vazia(pai)) {
    // Checa se o filho esquerdo esta ligado ao no atual corretamente via apontador pai
    // Faz o mesmo para as subarvores deste filho
    if (!vazia(filhoEsq))
      ok = (pai == filhoEsq->pai) && encadeamentoComNoPaiCorreto(filhoEsq, filhoEsq->esq, filhoEsq->dir);
    // Checa se o filho direito esta ligado ao no atual corretamente via apontador pai
    // Faz o mesmo para as subarvores deste filho
    if (!vazia(filhoDir) && ok)
      ok = (pai == filhoDir->pai) && encadeamentoComNoPaiCorreto(filhoDir, filhoDir->esq, filhoDir->dir);
  }
  return ok;
}





/////////////////////////////////////////////////////

int main() {
  HEAP *arv = NULL;
  inicializar(arv);
  ITEM item;

  item.chave = 7;
  inserir(item, &arv);
  item.chave = 6;
  inserir(item, &arv);
  item.chave = 5;
  inserir(item, &arv);
  item.chave = 11;
  inserir(item, &arv);
  item.chave = 9;
  inserir(item, &arv);
  item.chave = 8;
  inserir(item, &arv);
  item.chave = 10;
  inserir(item, &arv);
  item.chave = 12;
  inserir(item, &arv);
  item.chave = 15;
  inserir(item, &arv);

  /*          7
          6       11
        5      9     12
            8  10     15
  */

  printf("inOrdem: ");
  inOrdem(arv, imprimir);

  // Obtendo as caracteristicas da arvore
  int numNos, altura;
  float compMedio;
  caracteristicas(&numNos, &altura, &compMedio, arv);
  printf("\nNumero de Nos: %d - Altura: %d - Comprimento Medio: %.2f", numNos, altura, compMedio);

  printf("\nRemovendo 6 (No com filho esquerdo)\n");
  remover(6, &arv);
  printf("inOrdem: ");
  inOrdem(arv, imprimir);

  /*          7
          5       11
              9     12
            8  10     15
  */

  printf("\nRemovendo 5 (No Folha)\n");
  remover(5, &arv);
  printf("inOrdem: ");
  inOrdem(arv, imprimir);

  /*          7
                  11
              9     12
            8  10     15
  */

  printf("\nRemovendo 11 (No com 2 filhos)\n");
  remover(11, &arv);
  printf("inOrdem: ");
  inOrdem(arv, imprimir);
  printf("\n");

  /*
            7
                  12
              9     15
            8  10
  */


  TIPOCHAVE ch = 15;
  if (obter(ch, &item, arv))
    printf("Ok: obtido o dado do No com chave = %d\n", item.chave);
  else
    printf("Erro: nao foi obtido o dado do No com chave = %d\n", ch);

  disposeArvore(arv);
  return 0;
}