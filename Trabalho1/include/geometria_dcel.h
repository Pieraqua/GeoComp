#ifndef GEOMETRIA_DCEL_H
#define GEOMETRIA_DCEL_H
#include "geometria.h"

// DCEL - Doubly connected edge list
struct DCEL_VERTEX;
struct DCEL_FACE;

typedef struct DCEL_HALF_EDGE{
    struct DCEL_VERTEX* origin;
    struct DCEL_HALF_EDGE* twin;
    struct DCEL_FACE* incidentFace;
    struct DCEL_HALF_EDGE* next;
    struct DCEL_HALF_EDGE* previous;
}XDCEL_HALF_EDGE;

typedef struct DCEL_FACE{
    XDCEL_HALF_EDGE* halfedge;
}XDCEL_FACE;

typedef struct DCEL_VERTEX{
    XVERTICE v;
    XDCEL_HALF_EDGE* halfedge;
}XDCEL_VERTEX;

typedef struct {
    XLISTA_SIMPLES faces;
}XDCEL_TOPOLOGY;

void createPolygonsFromTopology(XLISTA_SIMPLES* listaPoligonos, XDCEL_TOPOLOGY* plano);
void createPolygonFromFace(XPOLIGONO* poli, XDCEL_FACE* face);
void createTopologyFromPolygon(XDCEL_TOPOLOGY* plano, XPOLIGONO* poli);
void clearTopology(XDCEL_TOPOLOGY* plano);
void removeVerticeDeTopologia(XDCEL_TOPOLOGY* plano, XDCEL_VERTEX* vertice);
void removeEdge(XDCEL_TOPOLOGY* plano, XDCEL_HALF_EDGE* edge);
XDCEL_HALF_EDGE* getNextHEfromVertex(XDCEL_HALF_EDGE* atual);
XDCEL_HALF_EDGE* createEdge(XDCEL_TOPOLOGY* plano, XVERTICE* origin, XDCEL_HALF_EDGE* previous, XDCEL_HALF_EDGE* next);

#endif // GEOMETRIA_DCEL_H
