#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "lista.h"

#define GEOMETRIA_VER 100

typedef struct VERTICE
{
    double x;
    double y;
    double R;
    double G;
    double B;
}XVERTICE;

typedef struct POLIGONO{
    unsigned int id;
    XLISTA_DUPLA vertices;
    unsigned int num_vertices;
}XPOLIGONO;

typedef struct POLIITER{
    XPOLIGONO* poli;
    XLISTA_DUPLA* atual;
}XPOLIITER;

// Vertice

void getVCoords(XVERTICE* v, double* x, double* y);

void setVCoord(XVERTICE* v, double x, double y);

void getVColor(XVERTICE* v, double* r, double* g, double* b);

enStatus setVColor(XVERTICE* v, double r, double g, double b);

XVERTICE createVertice(double x, double y, double r, double g, double b);

// Poligono

/** Copia uma lista de @p vertices e cria um poligono a partir dela */
void createPoligonoFromVertices(XPOLIGONO* poli, XLISTA_DUPLA *vertices);

void criaPoligono(XPOLIGONO* poli);

XPOLIITER getVerticeIterator(XPOLIGONO* poli);

XVERTICE* getNextVertice(XPOLIITER* iter);

unsigned int getID(XPOLIGONO* poli);

void addVertice(XPOLIGONO* poli, XVERTICE v);

void removeVertice(XPOLIGONO* poli, XVERTICE* v);

unsigned int genPoliID();

void limpaPoligono(XPOLIGONO* poli);

#endif // GEOMETRIA_H
