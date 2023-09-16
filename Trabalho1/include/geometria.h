#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "lista.h"

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
}XPOLIGONO;

typedef struct POLIITER{
}XPOLIITER;

// Vertice

void getVCoords(XVERTICE* v, double* x, double* y);

void setVCoord(XVERTICE* v, double x, double y);

void getVColor(XVERTICE* v, double* r, double* g, double* b);

enStatus setVColor(XVERTICE* v, double r, double g, double b);

XVERTICE createVertice(double x, double y, double r, double g, double b);

// Poligono

XPOLIITER getVerticeIterator(XPOLIGONO* poli);

XVERTICE getNextVertice(XPOLIITER* iter);

unsigned int getID(XPOLIGONO* poli);

void addVertice(XPOLIGONO* poli, XVERTICE v);

void removeVertice(XVERTICE v, XPOLIGONO poli);

unsigned int genPoliID();

#endif // GEOMETRIA_H
