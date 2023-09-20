#include "include/geometria_dcel.h"
#include <assert.h>

void assert_vertices_equal(XVERTICE v1, XVERTICE v2)
{
	assert(v1.x == v2.x);
	assert(v1.y == v2.y);
	assert(v1.R == v2.R);
	assert(v1.G == v2.G);
	assert(v1.B == v2.B);

}

int testes()
{
	
	XLISTA_DUPLA vertices;
	XVERTICE v1, v2, v3, v4;
	XPOLIGONO poli;

	v1 = createVertice(1, 1, 0, 0, 0);
	v2 = createVertice(-1, 1, 0, 0, 1);
	v3 = createVertice(-1, -1, 0, 1, 0);
	v4 = createVertice(1, -1, 1, 0, 0);

	createListaDupla(&vertices);

	addListaDupla(&vertices, &v1);
	addListaDupla(&vertices, &v2);
	addListaDupla(&vertices, &v3);
	addListaDupla(&vertices, &v4);

	createPoligonoFromVertices(&poli, &vertices);

	XDCEL_TOPOLOGY topologia;

	createTopologyFromPolygon(&topologia, &poli);

	XDCEL_HALF_EDGE* edge = ((XDCEL_FACE*)topologia.faces.item)->halfedge;

	assert_vertices_equal(edge->origin->v, v4);
	assert_vertices_equal(edge->next->origin->v, v1);
	assert_vertices_equal(edge->next->next->origin->v, v2);
	assert_vertices_equal(edge->next->next->next->origin->v, v3);
	assert_vertices_equal(edge->next->next->next->next->origin->v, v4);

	assert_vertices_equal(edge->twin->origin->v, v1);
	assert_vertices_equal(edge->twin->next->origin->v, v4);
	assert_vertices_equal(edge->twin->next->next->origin->v, v3);
	assert_vertices_equal(edge->twin->next->next->next->origin->v, v2);
	assert_vertices_equal(edge->twin->next->next->next->next->origin->v, v1);

	assert_vertices_equal(edge->origin->v, v4);
	assert_vertices_equal(edge->previous->origin->v, v3);
	assert_vertices_equal(edge->previous->previous->origin->v, v2);
	assert_vertices_equal(edge->previous->previous->previous->origin->v, v1);
	assert_vertices_equal(edge->previous->previous->previous->previous->origin->v, v4);

	assert_vertices_equal(edge->twin->origin->v, v1);
	assert_vertices_equal(edge->twin->previous->origin->v, v2);
	assert_vertices_equal(edge->twin->previous->previous->origin->v, v3);
	assert_vertices_equal(edge->twin->previous->previous->previous->origin->v, v4);
	assert_vertices_equal(edge->twin->previous->previous->previous->previous->origin->v, v1);

	assert(edge->incidentFace == topologia.faces.item);
	assert(edge->twin->incidentFace == NULL);
	assert(edge->next->incidentFace == topologia.faces.item);
	assert(edge->next->twin->incidentFace == NULL);
	assert(edge->next->next->incidentFace == topologia.faces.item);
	assert(edge->next->next->twin->incidentFace == NULL);
	assert(edge->next->next->next->incidentFace == topologia.faces.item);
	assert(edge->next->next->next->twin->incidentFace == NULL);

	XDCEL_HALF_EDGE* newedge = createEdge(&topologia, edge->origin, edge->previous, edge->previous->previous);

	assert(newedge->origin == edge->origin);
	assert(newedge->twin->origin == edge->twin->previous->origin);
	assert(newedge == edge->previous);
	assert(newedge == edge->next->next);

	assert(edge->twin->next->twin->next == newedge->twin);
	assert(edge->twin->next->next->twin->previous == newedge->twin);

	clearListaDupla(&vertices);
	clearTopology(&topologia);

	return 1;
}

int main()
{
	if (testes())
		printf("All tests passing\n");
	return 0;
}