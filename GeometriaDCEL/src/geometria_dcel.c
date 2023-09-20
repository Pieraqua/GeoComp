#include "geometria_dcel.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

XDCEL_HALF_EDGE* createEdge(XDCEL_TOPOLOGY* plano, XVERTICE* origin, XDCEL_HALF_EDGE* previous, XDCEL_HALF_EDGE* next)
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
	XDCEL_VERTEX* vertice;
	if (previous == NULL || previous->next == NULL) {
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (vertice == NULL)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar memória - vertice\n");
			return NULL;
		}

		vertice->v = *origin;
	}
	else vertice = previous->next->origin;

	vertice->halfedge = novoHE;

	novoHE->origin = vertice;
	novoHE->next = next;
	novoHE->previous = previous;
	novoHE->twin->twin = novoHE;

	// Criação de nova face
	if (next != NULL)
	{
		XDCEL_FACE* novaFace = (XDCEL_FACE*)malloc(sizeof(XDCEL_FACE));

		if (novaFace == NULL)
		{
			printf("ERRO::GEOMETRIADCEL::CRIAHE - erro ao alocar memoria\n");
			// todo: desfazer todas as operações até aqui
			return;
		}

		novaFace->halfedge = novoHE;
		novoHE->incidentFace = novaFace;
		novoHE->twin->origin = next->origin;
		

		// fechando um poligono
		if (next->previous == NULL)
		{
			previous->next = novoHE;
			previous->twin->previous = novoHE->twin;
			previous->twin->origin = novoHE->origin;
			novoHE->previous = previous;
			novoHE->twin->next = previous->twin;
			novoHE->twin->incidentFace = previous->twin->incidentFace;
			novoHE->twin->previous = next->twin;


			next->previous = novoHE;
			next->twin->next = novoHE->twin;

			// Propaga a face nova
			XDCEL_HALF_EDGE* atual = novoHE->next;
			while (atual != NULL && atual != novoHE)
			{
				atual->incidentFace = novaFace;
				atual = atual->next;
			}
		}
		// Criando nova face em poligono existente
		else
		{
			novoHE->twin->next = previous->next;
			novoHE->twin->next->previous = novoHE->twin;
			previous->next = novoHE;
			novoHE->previous = previous;

			next->previous->next = novoHE->twin;
			novoHE->twin->previous = next->previous;
			novoHE->next = next;
			next->previous = novoHE;

			novoHE->twin->incidentFace = novoHE->twin->next->incidentFace;

			// Propaga a face nova
			XDCEL_HALF_EDGE* atual = novoHE->next;
			while (atual != novoHE)
			{
				atual->incidentFace = novaFace;
				atual = atual->next;
			}
		}

		XDCEL_HALF_EDGE* iterador = novoHE->next;
		while (iterador != NULL && iterador != novoHE)
		{
			iterador->incidentFace = novaFace;
			iterador = iterador->next;
		}

		addListaSimples(&(plano->faces), novaFace);
	}
	else
	{
		novoHE->twin->origin = NULL;
		novoHE->twin->previous = NULL;
		novoHE->next = NULL;
	}

	if(previous != NULL)
	{
		// caso base
		if (next == NULL)
		{
			novoHE->previous = previous;
			novoHE->twin->next = previous->twin;

			previous->next = novoHE;
			previous->twin->previous = novoHE->twin;
		}
		novoHE->twin->incidentFace = previous->twin->incidentFace;
		previous->twin->origin = novoHE->origin;
		novoHE->incidentFace = previous->incidentFace;
	}
	else
	{
		novoHE->previous = NULL;
		novoHE->twin->next = NULL;
		novoHE->twin->incidentFace = NULL;
		novoHE->incidentFace = NULL;
	}


	return novoHE;
}

void createTopologyFromPolygon(XDCEL_TOPOLOGY* topologia, XPOLIGONO* poli)
{
	clearTopology(topologia);
	createListaSimples(&(topologia->faces));
	XPOLIITER it;
	it = getVerticeIterator(poli);
	XVERTICE* primeiro = getNextVertice(&it);
	XVERTICE* atual = getNextVertice(&it);
	if (primeiro == NULL || atual == NULL)
	{
		printf("WARN::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - poligono vazio/aberto\n");
		return topologia;
	}


	XDCEL_HALF_EDGE* primeira = createEdge(topologia, primeiro, NULL, NULL);
	if (primeira == NULL)
	{
		printf("ERROR::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - falha em criar halfedge inicial\n");
		clearTopology(topologia);
		return;
	}


	XDCEL_HALF_EDGE* edge_atual = primeira;
	while (atual != NULL && ((XVERTICE*)it.atual->item) != primeiro)
	{
		edge_atual = createEdge(topologia, atual, edge_atual, NULL);
		if (edge_atual == NULL)
		{
			clearTopology(topologia);
			printf("ERROR::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - falha em criar halfedge\n");
			return;
		}
		atual = getNextVertice(&it);
	}

	if (atual != NULL)
	{
		edge_atual = createEdge(topologia, atual, edge_atual, primeira);
		atual = getNextVertice(&it);
	}
	else
	{
		clearTopology(topologia);
		printf("WARN::GEOMETRIADCEL::CREATETOPOLOGYFROMPOLYGON - poligono aberto\n");
		return;
	}


	return;
}

/** @brief Remove uma aresta, liberando a memoria consumida por ela.
 * @return a face que foi destruída no processo.
*/
void removeEdge(XDCEL_TOPOLOGY* plano, XDCEL_HALF_EDGE* edge)
{
	// Ajustar quinas formadas
	edge->previous->next = edge->twin->next;
	edge->next->previous = edge->twin->previous;

	edge->twin->previous->next = edge->next;
	edge->twin->next->previous = edge->previous;

	// Ajustar faces

	XDCEL_HALF_EDGE* primeira = edge->next;
	edge->next->incidentFace = edge->twin->incidentFace;
	XDCEL_HALF_EDGE* atual = edge->next->next;
	while (atual != primeira)
	{
		atual->incidentFace = edge->twin->incidentFace;
	}

	XDCEL_FACE* destroyedFace = NULL;
	if (edge->incidentFace != NULL)
	{
		edge->incidentFace->halfedge = NULL;
		destroyedFace = edge->incidentFace;
		removeListaSimples(&(plano->faces), destroyedFace);
	}

	// Ajustar vertices
	if (edge->origin->halfedge == edge)
	{
		edge->origin->halfedge = getNextHEfromVertex(edge);
	}
	if(edge->twin->origin->halfedge == edge->twin)
		edge->twin->origin->halfedge = getNextHEfromVertex(edge->twin);


	free(edge->twin);
	free(edge);


	return;
}

void removeVerticeDeTopologia(XDCEL_TOPOLOGY* plano, XDCEL_VERTEX* vertice)
{

}

void clearTopology(XDCEL_TOPOLOGY* plano)
{

}

XDCEL_HALF_EDGE* getNextHEfromVertex(XDCEL_HALF_EDGE* atual)
{
	return atual->previous->twin;
}
