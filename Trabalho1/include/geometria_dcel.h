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


#endif // GEOMETRIA_DCEL_H
