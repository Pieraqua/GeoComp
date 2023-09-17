#include "geometria_dcel.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

static XDCEL_HALF_EDGE* _createHalfEdge(XVERTICE* origin, XDCEL_FACE* incidentFace, XDCEL_HALF_EDGE* previous)
{
	XDCEL_HALF_EDGE* novoHE = (XDCEL_HALF_EDGE*)malloc(sizeof(XDCEL_HALF_EDGE));

	if (novoHE == NULL)
	{
		printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar memória - novoHE\n");
		return NULL;
	}

	novoHE->twin = (XDCEL_HALF_EDGE*)malloc(sizeof(XDCEL_HALF_EDGE));

	if (novoHE->twin == NULL)
	{
		free(novoHE);
		printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar memória - twin\n");
		return NULL;
	}

	XDCEL_VERTEX* vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));

	if (vertice == NULL)
	{
		free(novoHE->twin);
		free(novoHE);
		printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar memória - vertice\n");
		return NULL;
	}

	vertice->halfedge = novoHE;
	vertice->v = *origin;

	novoHE->origin = vertice;
	novoHE->incidentFace = incidentFace;
	novoHE->twin->incidentFace = NULL;

	if (previous != NULL)
	{
		novoHE->previous = previous;
		novoHE->twin->next = previous->twin;
		novoHE->twin->incidentFace = previous->twin->incidentFace;

		previous->next = novoHE;
		previous->twin->previous = novoHE->twin;
		previous->twin->origin = novoHE->origin;
	}
	else
	{
		novoHE->previous = NULL;
		novoHE->twin->next = NULL;
		novoHE->twin->incidentFace = NULL;
	}

	novoHE->next = NULL;
	novoHE->twin->previous = NULL;

	return novoHE;
}

XDCEL_FACE createTopologyFromPolygon(XPOLIGONO* poli)
{
	XDCEL_FACE face;

	face.halfedge = NULL;

	XPOLIITER it;
	it = getVerticeIterator(poli);
	XVERTICE* primeiro = getNextVertice(&it);
	XVERTICE* atual = getNextVertice(&it);


	if (primeiro == NULL || atual == NULL )
	{
		printf("WARN::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - poligono vazio/aberto\n");
		return face;
	}

	face.halfedge = _createHalfEdge(primeiro, &face, NULL);

	if (face.halfedge == NULL)
	{
		printf("ERROR::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - falha em criar halfedge inicial\n");
		return face;
	}

	XDCEL_HALF_EDGE* edge_atual = face.halfedge;

	while (atual != NULL && atual != primeiro)
	{
		edge_atual = _createHalfEdge(atual, &face, edge_atual);
		if (edge_atual == NULL)
		{
			printf("ERROR::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - falha em criar halfedge\n");
			return face;
		}
		atual = getNextVertice(&it);
	}

	if (atual == NULL)
	{
		printf("WARN::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - poligono aberto\n");
		return face;
	}

	face.halfedge->previous = edge_atual;
	face.halfedge->twin->next = edge_atual;

	return face;
}

void clearTopology(XDCEL_FACE* face)
{

}
