#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//ESTRUTURA DO NÓ
struct No{
    char dado[50];
    int bal;
    struct No *pai,
              *esq,
              *dir;
};

//INSERÇÃO (protótipos)
void inserirAVL(struct No** ptr, char *x, struct No** pai, bool *h);
void Caso1(struct No** ptr, bool *h);
void Caso2(struct No** ptr, bool *h);
void Aloca(struct No** ptr, char *x);
void trocaNos(struct No** ptm, struct No** ptg);

//IMPRESSÃO (protótipos)
void imprime(struct No* pth, int nivel);
void imprimir_em_pre_ordem(struct No* ptr);
void imprimir_em_ordem(struct No* ptr);
void imprimir_em_pos_ordem(struct No* ptr);
void imprimirRotacao(int );

//BUSCA (protótipo)
struct No* buscaArvore(struct No** ptr, char*);

//UTILITÁRIOS (protótipos)
void PredSucc(struct No** raiz);
struct No* predecessor(struct No* ptg);
struct No* sucessor(struct No* ptg);
struct No* minimoNo(struct No* ptf);
struct No* maximoNo(struct No* ptf);

//REMOÇÃO (protótipos)
struct No* removeNo(struct No* , char* );
struct No* removerCaso1(struct No* raiz);
struct No* removerCaso2(struct No* raiz);
int altura(struct No* N);

//MENUS E INTERFACE (protótipos)
void cabecalho();
void menu();

void imprime(struct No* pth, int nivel)
{
    char *dado;
    int k;

    if(pth == NULL) return;

    imprime(pth->dir, nivel + 1);

    for(k = 0; k < nivel*6; k++)
        printf("  ");

    dado = pth->dado;
    printf("%s (%d)\n", dado, pth->bal);

    imprime(pth->esq, nivel + 1);
}

void imprimirRotacao(int r)
{
    switch(r){
        case 1:
            printf("\nRotacao simples a direita\n");
            break;
        case 2:
            printf("\nRotacao dupla a direita\n");
            break;
        case 3:
            printf("\nRotacao simples a esquerda\n");
            break;
        case 4:
            printf("\nRotacao dupla a esquerda\n");
            break;
    }
}

void cabecalho()
{
    printf("TRABALHO:\n");
    printf(" Arvore AVL\n\n");
    printf("DISCIPLINA:\n");
    printf(" Algoritmos III\n\n");
    printf("PROFESSOR:\n");
    printf(" Rodrigo Porfirio da Silva Sacchi\n\n");
    printf("ALUNOS DE SISTEMAS DE INFORMACAO:\n");
    printf(" Carlos Antonio Goncalves Martins\n");
    printf(" Rafael Spoladore Ferreira dos Reis\n");

    printf("\n\n");
    system("pause");
    system("cls");
    printf("Digite a primeira palavra: ");
}

void menu()
{
    struct No* raiz = NULL;
    bool h;
    char palavra[50];
    int x=1;

    cabecalho();
    scanf("\n%[^\n]s", palavra);
    printf("\n\n");

    raiz = (struct No*)malloc(sizeof(struct No));
    strcpy(raiz->dado, palavra);
    raiz->bal = 0;
    raiz->dir = NULL;
    raiz->esq = NULL;
    raiz->pai = NULL;
    imprime(raiz, 1);
    printf("\n\n\n\n");

    while(x){
        printf("Escolha o numero correspondente ao que deseja fazer: ");
        printf("\n1) Inserir");
        printf("\n2) Remover");
        printf("\n3) Buscar");
        printf("\n4) Imprimir em ordem");
        printf("\n5) Imprimir pre-ordem");
        printf("\n6) Imprimir pos-ordem");
        printf("\n7) PredSucc");
        printf("\n0) Sair\n");
        scanf("%d",&x);
        switch(x){
            case 1:
                printf("\nDigite a palavra que deseja inserir: ");
                scanf("\n%[^\n]s",palavra);
                inserirAVL(&raiz, palavra, &raiz->pai, &h);
                break;
            case 2:
                printf("\nDigite a palavra que deseja remover: ");
                scanf("\n%[^\n]s",palavra);
                raiz = removeNo(raiz, palavra);
                break;
            case 3:
                printf("\nDigite a palavra que deseja buscar: ");
                scanf("\n%[^\n]s",palavra);
                if( buscaArvore(&raiz, palavra) == NULL )
                    printf("\nElemento nao encontrado!");
                else
                    printf("\nElemento encontrado: %s (%d)",buscaArvore(&raiz, palavra)->dado,buscaArvore(&raiz, palavra)->bal);
                break;
            case 4:
                printf("\nEm ordem: | ");
                imprimir_em_ordem(raiz);
                break;
            case 5:
                printf("\nEm pre-ordem: | ");
                imprimir_em_pre_ordem(raiz);
                break;
            case 6:
                printf("\nEm pos-ordem: | ");
                imprimir_em_pos_ordem(raiz);
                break;
            case 7:
                PredSucc(&raiz);
                break;
            case 0:
                break;
            default :
                printf("\nOpcao invalida.");
                break;
        }
        printf("\n\n");
        imprime(raiz, 1);
        printf("\n\n\n\n");
    }

    system("pause");
}

void inserirAVL( struct No** ptr, char *x, struct No** pai, bool *h )
{
    if( *ptr == NULL ){
        Aloca( &(*ptr), x );
        (*ptr)->pai = *pai;
        *h = true;
    }
    else if( strcmp (x,(*ptr)->dado) == 0 ){
        printf("Elemento ja existe.\n");
    }
    else if( strcmp(x,( *ptr )->dado) < 0){
        inserirAVL( &(*ptr)->esq, x, &(*ptr), h );

        if( *h ){
            switch( (*ptr)->bal ){
                case 1:
                    (*ptr)->bal = 0;
                    *h = false;
                    break;
                case 0:
                    (*ptr)->bal=-1;
                    break;
                case -1:
                    Caso1( &(*ptr), h );
                    break;
            }
        }
    }
    else if( strcmp(x,( *ptr )->dado) > 0 ){
        inserirAVL( &(*ptr)->dir , x, &(*ptr), h );

        if( *h ){
            switch( (*ptr)->bal ){
                case -1:
                    (*ptr)->bal = 0;
                    *h = false;
                    break;
                case 0:
                    (*ptr)->bal = 1;
                    break;
                case 1:
                    Caso2( &(*ptr), h );
                    break;
            }
        }
    }
}

void Caso1(struct No** ptr, bool *h)
{
    struct No *ptu;

    ptu = (*ptr)->esq;

    if( ptu->bal == -1 ){
        imprimirRotacao(1);
        (*ptr)->esq = ptu->dir;

        if((*ptr)->esq != NULL) (*ptr)->esq->pai = *ptr;
        ptu->dir = *ptr;
        (*ptr)->bal = 0;
        ptu->pai = (*ptr)->pai;
        (*ptr)->pai = ptu;
        trocaNos(&(*ptr),&ptu);
    }
    else{
        imprimirRotacao(2);
        struct No *ptv;
        ptv = ptu->dir;
        ptu->dir = ptv->esq;

        if(ptu->dir != NULL)ptu->dir->pai = ptu;
        ptv->esq = ptu;
        (*ptr)->esq = ptv->dir;

        if((*ptr)->esq != NULL)(*ptr)->esq->pai = (*ptr);
        ptv->dir = *ptr;
        ptv->pai = (*ptr)->pai;
        (*ptr)->pai = ptv;
        ptu->pai = ptv;

        if( ptv->bal == -1 ){
            (*ptr)->bal = 1;
        }
        else{
            (*ptr)->bal = 0;
        }
        if( ptv->bal == 1 ){
            ptu->bal = -1;
        }
        else{
            ptu->bal = 0;
        }
        trocaNos(&(*ptr),&ptv);
    }
    (*ptr)->bal = 0;
    *h = false;
}

void Caso2( struct No** ptr, bool *h )
{
    struct No* ptu;

    ptu = (*ptr)->dir;

    if( ptu->bal == 1 ){
        (*ptr)->dir = ptu->esq;

        if((*ptr)->dir != NULL) (*ptr)->dir->pai = *ptr;
        ptu->esq = *ptr;
        ptu->pai = (*ptr)->pai;
        (*ptr)->pai = ptu;
        (*ptr)->bal = 0;
        trocaNos(&(*ptr),&ptu);
        imprimirRotacao(3);
    }
    else{
        struct No* ptv;

        ptv = ptu->esq;

        ptu->esq = ptv->dir;

        if(ptu->esq != NULL) ptu->esq->pai = ptu;
        ptv->dir = ptu;
        ptu->pai = ptv;
        (*ptr)->dir = ptv->esq;

        if((*ptr)->dir != NULL) (*ptr)->dir->pai = (*ptr);
        ptv->esq = *ptr;
        ptv->pai = (*ptr)->pai;
        (*ptr)->pai = ptv;

        if( ptv->bal == 1){
            (*ptr)->bal = 1;
        }
        else{
            (*ptr)->bal = 0;
        }
        if( ptv->bal == -1 ){
            ptu->bal = 1;
        }
        else{
            ptu->bal = 0;
        }
        trocaNos(&(*ptr),&ptv);
        imprimirRotacao(4);
    }
    (*ptr)->bal = 0;
    *h = false;
}

void Aloca( struct No **ptr, char *x )
{
    *ptr = ( struct No* ) malloc ( sizeof ( struct No ));

    strcpy( (*ptr)->dado, x );

    (*ptr)->bal = 0;
    (*ptr)->dir = NULL;
    (*ptr)->esq = NULL;
}

void trocaNos(struct No** ptm, struct No** ptg)
{
    struct No *aux = NULL;

    aux = (*ptm);
    (*ptm) = (*ptg);
    (*ptg) = aux;
}

void imprimir_em_pre_ordem( struct No* ptr)
{
    if( ptr != NULL ){
        printf("%s | ", ptr->dado);
        imprimir_em_pre_ordem(ptr->esq);
        imprimir_em_pre_ordem(ptr->dir);
    }
}

void imprimir_em_ordem( struct No* ptr)
{
    if( ptr != NULL ){
        imprimir_em_ordem(ptr->esq);
        printf("%s | ", ptr->dado);
        imprimir_em_ordem(ptr->dir);
    }
}

void imprimir_em_pos_ordem(struct No* ptr)
{
    if(ptr != NULL){
        imprimir_em_pos_ordem(ptr->esq);
        imprimir_em_pos_ordem(ptr->dir);
        printf("%s | ", ptr->dado);
    }
}

struct No* buscaArvore(struct No** ptr, char *x)
{
    if( *ptr == NULL || strcmp(x, (*ptr)->dado) == 0 ){
        return *ptr;
    }
    else if( strcmp(x,( *ptr )->dado) < 0 ){
        return buscaArvore( &(*ptr)->esq, x );
    }
    else{
        return buscaArvore( &(*ptr)->dir, x );
    }
}

void PredSucc(struct No** raiz)
{
    char palavra[50];
    printf("\nDigite a palavra: ");
    scanf("\n%[^\n]s",palavra);
    if( buscaArvore( &(*raiz), palavra ) == NULL)
        printf("\nElemento digitado nao existe.\n\n");
    else{
        if( predecessor( buscaArvore( &(*raiz), palavra ) ) == NULL )
            printf("\nNao tem predecessor.\n\n");
        else
            printf("\nPredecessor: %s (%d)\n\n",predecessor( buscaArvore( &(*raiz), palavra ) )->dado,predecessor( buscaArvore( &(*raiz), palavra ) )->bal);
        if( sucessor( buscaArvore( &(*raiz), palavra ) ) == NULL )
            printf("Nao tem sucessor.\n\n");
        else
            printf("Sucessor: %s (%d)\n\n",sucessor( buscaArvore( &(*raiz), palavra ) )->dado,sucessor( buscaArvore( &(*raiz), palavra ) )->bal);
    }
}

struct No* predecessor(struct No* ptg)
{
    struct No* aux;

    if(ptg->esq != NULL)
        return maximoNo(ptg->esq);

    aux = ptg->pai;

    while(aux != NULL && ptg == aux->esq){
        ptg = aux;
        aux = aux->pai;
    }

    return aux;
}

struct No* sucessor(struct No* ptg)
{
    struct No* aux;

    if(ptg->dir != NULL)
        return minimoNo(ptg->dir);

    aux = ptg->pai;

    while(aux != NULL && ptg == aux->dir){
        ptg = aux;
        aux = aux->pai;
    }

    return aux;
}

struct No* minimoNo(struct No* ptf)
{
    while(ptf->esq != NULL)
        ptf = ptf->esq;

    return ptf;
}

struct No* maximoNo(struct No* ptf)
{
    while(ptf->dir != NULL)
        ptf = ptf->dir;

    return ptf;
}

struct No* removeNo(struct No* raiz, char *key)
{
    if( raiz == NULL)
        return raiz;

    if(strcmp(key, raiz->dado) < 0)
        raiz->esq = removeNo(raiz->esq, key);

    else if(strcmp(key, raiz->dado) > 0)
        raiz->dir = removeNo(raiz->dir, key);

    else{
        if((raiz->esq == NULL) || (raiz->dir == NULL)){
                struct No* temp;
            if((raiz->esq == NULL) && (raiz->dir == NULL)){
                raiz = NULL;
            }
            else if((raiz->esq != NULL) && (raiz->dir == NULL)){
                temp = raiz->esq;
                temp->pai = raiz->pai;
                raiz = temp;
            }
            else if((raiz->esq == NULL) && (raiz->dir != NULL)){
                temp = raiz->dir;
                temp->pai = raiz->pai;
                raiz = temp;
            }
            temp = NULL;
        }

        else{
            struct No* temp = minimoNo(raiz->dir);
            strcpy(raiz->dado, temp->dado);
            raiz->dir = removeNo(raiz->dir, temp->dado);
        }
    }

    if(raiz == NULL)
        return raiz;

    raiz->bal = altura(raiz->dir) - altura(raiz->esq);

    if(raiz->bal < -1 && raiz->esq->bal < 0){
        imprimirRotacao(1);
        return removerCaso1(raiz);//simples a direita
    }
    if(raiz->bal < -1 && raiz->esq->bal >= 0){
        imprimirRotacao(2);
        raiz->esq = removerCaso2(raiz->esq);//simples a esquerda
        return removerCaso1(raiz);//simples a direita
    }

    if(raiz->bal > 1 && raiz->dir->bal > 0){
        imprimirRotacao(3);
        return removerCaso2(raiz);//simples a esquerda
    }
    if(raiz->bal > 1 && raiz->dir->bal <= 0)
    {
        imprimirRotacao(4);
        raiz->dir = removerCaso1(raiz->dir);//simples a direita
        return removerCaso2(raiz);//simples a esquerda
    }
    return raiz;
}

struct No* removerCaso1(struct No* raiz)//remoção rotação a direita
{
    struct No *x = raiz->esq;
    struct No *T2 = x->dir;

    x->dir = raiz;
    x->pai = raiz->pai;
    raiz->pai = x;
    raiz->esq = T2;
    if(T2 != NULL)
        T2->pai = raiz;

    raiz->bal = altura(raiz->dir) - altura(raiz->esq);
    x->bal = altura(x->dir) - altura(x->esq);
    return x;
}

struct No* removerCaso2(struct No* raiz)//remoção rotação a esquerda
{
    struct No *x = raiz->dir;
    struct No *T2 = x->esq;

    x->esq = raiz;
    x->pai = raiz->pai;
    raiz->pai = x;
    raiz->dir = T2;
    if(T2 != NULL)
        T2->pai = raiz;


    raiz->bal = altura(raiz->dir) - altura(raiz->esq);
    x->bal = altura(x->dir) - altura(x->esq);

    return x;
}

int altura(struct No* N)
{
   if (N==NULL)
       return 0;
   else
   {
       int altura1 = altura(N->esq);
       int altura2 = altura(N->dir);

       if (altura1 > altura2)
           return(altura1+1);
       else return(altura2+1);
   }
}

int main()
{
    menu();

    return 0;
}
