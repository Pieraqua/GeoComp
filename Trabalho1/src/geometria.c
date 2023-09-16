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

    return it;
}

//terminar
XVERTICE getNextVertice(XPOLIITER* iter){
    XVERTICE v;
    v.B = 0;
    v.G = 0;
    v.R = 0;
    v.x = 0;
    v.y = 0;

    return v;
}

unsigned int getID(XPOLIGONO* poli){
    return poli->id;
}

void addVertice(XPOLIGONO* poli, XVERTICE v){}

void removeVertice(XVERTICE v, XPOLIGONO poli){}

unsigned int genPoliID(){
    return poli_id_atual++;
}
