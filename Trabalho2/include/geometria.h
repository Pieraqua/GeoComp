#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "lista.h"

#define GEOMETRIA_VER 100

#define TRUE 1
#define FALSE 0

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

typedef struct Vector2
{
    double x;
    double y;
    XVERTICE origem;
}XVECTOR2;


double GEO_distanceFromPointToLineSegment(XVERTICE ponto, XVERTICE lineStart, XVERTICE lineEnd);
XVECTOR2 GEO_normalizeVector(XVECTOR2 vec);
double GEO_dotProduct(XVECTOR2 vec1, XVECTOR2 vec2);
XVECTOR2 GEO_toVec(XVERTICE ponto);
XVERTICE GEO_createPonto(double x, double y);
XVECTOR2 GEO_vectorDiff(XVECTOR2 vec1, XVECTOR2 vec2);
double GEO_magnitude(XVECTOR2 vec);
double GEO_dist(XVERTICE inicio, XVERTICE fim);
XVERTICE GEO_nearestPointOnLine(XVECTOR2 line, XVERTICE ponto);
int GEO_dentroPoligono(XPOLIGONO* poligono, XVERTICE ponto);
XVECTOR2 GEO_vecFromPoints(XVERTICE origem, XVERTICE fim);
XVECTOR2 GEO_projectToVector(XVECTOR2 vec, XVECTOR2 target);

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

double AREA(XVERTICE p1, XVERTICE p2, XVERTICE p3);

int LEFT(XVERTICE p1, XVERTICE p2, XVERTICE P3);
int LEFTON(XVERTICE p1, XVERTICE p2, XVERTICE p3);
double AREA_POLI(XPOLIGONO* poligono);
int ORIENTACAO_POLI(XPOLIGONO* pontos);
int CONVEXO(XLISTA_DUPLA* pontos);
void CONVEXO_POLI(XPOLIGONO* poli, int* resultados);
int INTERSECTA(XVERTICE pontos1_1, XVERTICE pontos1_2, XVERTICE pontos2_1, XVERTICE pontos2_2);
int ORELHA(XLISTA_DUPLA* pontos);
void ORELHA_POLI(XPOLIGONO* poli, int* resultados);

// Recebe um vetor de p1.nPontos slots onde será colocado 1 para os pontos internos e 0 para os pontos externos
void findInternalPoints(XPOLIGONO* p1, XPOLIGONO* p2, int* results_p1);

#endif // GEOMETRIA_H