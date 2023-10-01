#include <stdlib.h>
#include <stdio.h>
#include "geometria.h"

// Vertice

void getVCoords(XVERTICE* v, double*x, double*y){
    if(v != NULL && x != NULL && y != NULL){
        *x = v->x;
        *y = v->y;
    }
    else
    {
        printf("ERRO::GEOMETRIA::GETVCOORDS - vertice nulo");
    }
}

void setVCoord(XVERTICE* v, double x, double y){
    if(v == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOORDS - vertice nulo");
        return;
    }
    v->x = x;
    v->y = y;
}

void getVColor(XVERTICE* v, double* r, double* g, double* b){
    if(v == NULL || r == NULL || g == NULL || b == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOLOR - vertice nulo");
        return;
    }
    *r = v->R;
    *g = v->G;
    *b = v->B;
}

enStatus setVColor(XVERTICE* v, double r, double g, double b){
    if(v == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOLOR - vertice nulo");
        return FAIL;
    }

    v->R = r;
    v->G = g;
    v->B = b;
    return SUCCESS;
}

XVERTICE createVertice(double x, double y, double r, double g, double b){
    XVERTICE v;
    v.x = x;
    v.y = y;
    v.R = r;
    v.G = g;
    v.B = b;
    return v;
}

// Poligono
unsigned int poli_id_atual = 0;

XPOLIITER getVerticeIterator(XPOLIGONO* poli){
    XPOLIITER it;
    it.poli = poli;
    it.atual = &(poli->vertices);
    return it;
}

XVERTICE* getNextVertice(XPOLIITER* iter){
    XVERTICE* v = NULL;

    if (iter->atual == NULL || iter == NULL)
    {
        return NULL;
    }
    
    v = iter->atual->item;
    iter->atual = iter->atual->proximo;

    return v;
}

unsigned int getID(XPOLIGONO* poli){
    return poli->id;
}

void addVertice(XPOLIGONO* poli, XVERTICE v){

    XVERTICE* v_novo = (XVERTICE*)malloc(sizeof(XVERTICE));

    if (v_novo == NULL)
    {
        printf("ERRO::GEOMETRIA::ADDVERTICE - falha ao alocar memoria\n");
        return;
    }

    *v_novo = v;

    addListaDupla(&(poli->vertices), v_novo);
    poli->num_vertices++;
}

void removeVertice( XPOLIGONO* poli, XVERTICE* v){
    removeListaDupla(&(poli->vertices), v);
    poli->num_vertices--;
}

unsigned int genPoliID(){
    return poli_id_atual++;
}


void createPoligonoFromVertices(XPOLIGONO* poli, XLISTA_DUPLA* vertices)
{
    XLISTA_DUPLA_IT it = getIteratorLD(vertices);

    XVERTICE* primeiro = (XVERTICE*)getItemItLD(&it);

    poli->num_vertices = 0;
    poli->id = genPoliID();
    createListaDupla(&(poli->vertices));

    if (primeiro == NULL)
    {
        printf("WARN::GEOMETRIA::INICIAPOLIGONOUSANDOVERTICES - lista vazia\n");
        return;
    }

    addVertice(poli, *primeiro);
    XVERTICE* v = (XVERTICE*)getItemItLD(&it);

    while (v != NULL && v != primeiro) {
        addVertice(poli, *v);
        v = (XVERTICE*)getItemItLD(&it);
    } 
}

void criaPoligono(XPOLIGONO* poli)
{
    poli->num_vertices = 0;
    poli->id = genPoliID();
    createListaDupla(&(poli->vertices));
}

void limpaPoligono(XPOLIGONO* poli)
{
    XVERTICE* v_1 = NULL;

    while (poli->vertices.item != NULL)
    {
         v_1 = poli->vertices.item;
        removeVertice(poli, v_1);
        free(v_1);
    }
}