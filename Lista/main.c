#include <stdio.h>
#include "lista.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int testesListaDupla()
{
    XLISTA_DUPLA lista;

    createListaDupla(&lista);

    assert(lista.anterior == &lista);
    assert(lista.proximo == &lista);
    assert(lista.item == NULL);

    srand((unsigned int)time(NULL));

    int item_1 = rand()%500 + 1;
    int item_2 = rand()%500 + 1;
    int item_3 = rand()%500 + 1;

    int it1 = item_1, it2 = item_2, it3 = item_3;

    addListaDupla(&lista, &item_1);
    assert(lista.item == &item_1);
    assert(lista.proximo->item == &item_1);
    assert(lista.anterior->item == &item_1);

    addListaDupla(&lista, &item_2);
    assert(lista.anterior->proximo->item == &item_1);
    assert(lista.proximo->anterior->item == &item_1);
    assert(lista.anterior->item == &item_2);
    assert(lista.proximo->item == &item_2);
    assert(lista.item == &item_1);

    addListaDupla(&lista, &item_3);
    assert(lista.anterior->proximo->item == &item_1);
    assert(lista.proximo->anterior->item == &item_1);
    assert(lista.anterior->item == &item_3);
    assert(lista.proximo->item == &item_2);
    assert(lista.item == &item_1);

    clearListaDupla(&lista);
    assert(lista.item == NULL);
    assert(lista.proximo == &lista);
    assert(lista.anterior == &lista);

    assert(item_1 == it1);
    assert(item_2 == it2);
    assert(item_3 == it3);

    return 0;
}


int testesListaSimples()
{
    XLISTA_SIMPLES lista;

    createListaSimples(&lista);

    assert(lista.proximo == NULL);
    assert(lista.item == NULL);

    srand((unsigned int)time(NULL));

    int item_1 = rand()%500 + 1;
    int item_2 = rand()%500 + 1;
    int item_3 = rand()%500 + 1;

    int it1 = item_1, it2 = item_2, it3 = item_3;

    addListaSimples(&lista, &item_1);
    assert(lista.item == &item_1);
    assert(lista.proximo == NULL);

    addListaSimples(&lista, &item_2);
    assert(lista.proximo->proximo == NULL);
    assert(lista.proximo->item == &item_2);
    assert(lista.item == &item_1);

    addListaSimples(&lista, &item_3);
    assert(lista.proximo->proximo->proximo == NULL);
    assert(lista.proximo->proximo->item == &item_3);
    assert(lista.proximo->item == &item_2);
    assert(lista.item == &item_1);

    clearListaSimples(&lista);
    assert(lista.item == NULL);
    assert(lista.proximo == NULL);

    assert(item_1 == it1);
    assert(item_2 == it2);
    assert(item_3 == it3);

    return 0;
}

/** Testes lista */
int main()
{
    int result = 0;
    printf("Hello World!\n");
    result += testesListaDupla();
    result += testesListaSimples();

    if(result)
        return -1;

    printf("All tests passed\n");
    return 0;
}
