#include "lista.h"
#include <stdlib.h>
#include <stdio.h>



void clearListaDupla(XLISTA_DUPLA* lista)
{
    if(lista == NULL)
    {
        printf("ERRO::LISTA::LIMPALISTADUPLA - lista nula\n");
        return;
    }

    while(lista->item != NULL)
    {
        removeListaDupla(lista, lista->item);
    }
}

void addListaDupla(XLISTA_DUPLA* lista, void* item){
    if(item == NULL || lista == NULL)
    {
        printf("ERRO::LISTA::ADDLISTADUPLA - item nulo\n");
        return;
    }

    if(lista->anterior == lista && lista->item == NULL)
    {
        lista->item = item;
        return;
    }

    XLISTA_DUPLA* novalista = (XLISTA_DUPLA*)malloc(sizeof(XLISTA_DUPLA));

    if(novalista == NULL)
    {
        printf("ERRO::LISTA::ADDLISTADUPLA - erro de alocacao de memoria\n");
        return;
    }

    novalista->item = item;
    lista->anterior->proximo = novalista;
    novalista->anterior = lista->anterior;
    lista->anterior = novalista;
    novalista->proximo = lista;

}

enStatus removeListaDupla(XLISTA_DUPLA* lista, void* item){
    if(item == NULL || lista == NULL)
    {
        printf("ERRO::LISTA::ADDLISTADUPLA - item nulo\n");
        return FAIL;
    }

    XLISTA_DUPLA* primeira = lista;

    while(item != lista->item)
    {
        lista = lista->proximo;
        if(lista == primeira)
        {
            printf("ADDLISTADUPLA - item nao encontrado\n");
            return FAIL;
        }
    }

    if(primeira == lista && primeira->proximo != primeira){
        lista = lista->proximo;
        primeira->item = lista->item;
        lista->anterior->proximo = lista->proximo;
        lista->proximo->anterior = lista->anterior;
    }
    else{
        lista->anterior->proximo = lista->proximo;
        lista->proximo->anterior = lista->anterior;
    }

    if(primeira != lista)
        free(lista);
    else lista->item = NULL;

    return SUCCESS;
}

XLISTA_DUPLA_IT getIteratorLD(XLISTA_DUPLA* lista){
    XLISTA_DUPLA_IT it;
    it.atual = lista;

    return it;
}

void* getItemItLD(XLISTA_DUPLA_IT* it){
    void* item= it->atual->item;
    it->atual = it->atual->proximo;

    return item;
}

void* getItemItLD_rev(XLISTA_DUPLA_IT* it) {
    void* item = it->atual->item;
    it->atual = it->atual->anterior;

    return item;
}

void createListaDupla(XLISTA_DUPLA* novaLista)
{
    if (novaLista == NULL)
    {
        printf("lista nula");
        return;
    }
    novaLista->anterior = novaLista;
    novaLista->item = NULL;
    novaLista->proximo = novaLista;
    return;
}

void addListaSimples(XLISTA_SIMPLES* lista, void* item){
    if(lista->item == NULL)
    {
        lista->item = item;
        return;
    }

    while(lista->proximo != NULL)
        lista = lista->proximo;

    XLISTA_SIMPLES* newitem = (XLISTA_SIMPLES*)malloc(sizeof(XLISTA_SIMPLES));

    if(newitem == NULL)
    {
        printf("ERRO::LISTA::ADICIONALISTASIMPLES - erro de alocacao de memoria\n");
        return;
    }

    lista->proximo = newitem;
    newitem->proximo = NULL;
    newitem->item = item;
}

enStatus removeListaSimples(XLISTA_SIMPLES* lista, void* item){
    XLISTA_SIMPLES* anterior = lista;
    while(lista->item != item)
    {
        if(lista->proximo == NULL) return FAIL;
        anterior = lista;
        lista = lista->proximo;
    }

    if(lista->item == NULL)
    {
        printf("Item nao encontrado\n");
        return FAIL;
    }

    anterior->proximo = NULL;
    free(lista);
    return SUCCESS;
}

XLISTA_SIMPLES_IT getIteratorLS(XLISTA_SIMPLES* lista){
    XLISTA_SIMPLES_IT it;
    it.atual = lista;

    return it;
}

void* getItemItLS(XLISTA_SIMPLES_IT* it){
    void* item = NULL;
    if(it != NULL && it->atual != NULL ){
        item = it->atual->item;
        it->atual = it->atual->proximo;
    }
    
    return item;
}


void createListaSimples(XLISTA_SIMPLES* novaLista)
{
    novaLista->item = NULL;
    novaLista->proximo = NULL;
}

void clearListaSimples(XLISTA_SIMPLES* lista)
{
    XLISTA_SIMPLES* iterador = lista;

    lista->item = NULL;
    while(lista->proximo != NULL){
        while(iterador->proximo->proximo != NULL)
        {
            iterador = iterador->proximo;

        }
        free(iterador->proximo);
        iterador->proximo = NULL;
        iterador = lista;
    }

}
