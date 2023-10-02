#include "geometria_dcel.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

XDCEL_HALF_EDGE* createEdge(XDCEL_TOPOLOGY* plano, XVERTICE* origin, XDCEL_HALF_EDGE* previous, XDCEL_HALF_EDGE* next)
{
	XDCEL_HALF_EDGE* novoHE = (XDCEL_HALF_EDGE*)malloc(sizeof(XDCEL_HALF_EDGE));

	if (novoHE == NULL)
	{
		printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - novoHE\n");
		return NULL;
	}

	novoHE->twin = (XDCEL_HALF_EDGE*)malloc(sizeof(XDCEL_HALF_EDGE));

	if (novoHE->twin == NULL)
	{
		free(novoHE);
		printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - twin\n");
		return NULL;
	}

	novoHE->twin->twin = novoHE;

	XDCEL_VERTEX* vertice;
	XDCEL_FACE *face;
	XDCEL_HALF_EDGE* atual;

	// Caso 1 -> aresta inicial, next == null, prev == null
	if (next == NULL && previous == NULL)
	{
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (!vertice)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
			return NULL;
		}

		vertice->halfedge = novoHE;
		vertice->v = *origin;

		novoHE->origin = vertice;
		novoHE->incidentFace = NULL;
		novoHE->next = NULL;
		novoHE->previous = NULL;

		novoHE->twin->origin = NULL;
		novoHE->twin->incidentFace = NULL;
		novoHE->twin->next = NULL;
		novoHE->twin->previous = NULL;
	}
	// Caso 2 -> next == null
	else if (next == NULL)
	{
		// 2.1 - Continuando a linha
		if (previous->next == NULL) {
			vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
			if (!vertice)
			{
				free(novoHE->twin);
				free(novoHE);
				printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
				return NULL;
			}

			vertice->halfedge = novoHE;
			vertice->v = *origin;

			novoHE->origin = vertice;
			novoHE->incidentFace = previous->incidentFace;
			novoHE->next = NULL;
			novoHE->previous = previous;

			previous->next = novoHE;

			novoHE->twin->origin = NULL;
			novoHE->twin->incidentFace = NULL;
			novoHE->twin->next = previous->twin;
			novoHE->twin->previous = NULL;

			previous->twin->previous = novoHE->twin;
			previous->twin->origin = novoHE->origin;
		}
		// 2.2 - Divergindo de uma linha existente
		else
		{
			novoHE->origin = previous->next->origin;
			novoHE->incidentFace = previous->incidentFace;
			novoHE->next = NULL;
			novoHE->previous = previous;

			novoHE->twin->origin = NULL;
			novoHE->twin->incidentFace = previous->next->incidentFace;
			novoHE->twin->next = previous->next;
			novoHE->twin->previous = NULL;

			previous->next = novoHE;
			novoHE->twin->next->previous = novoHE->twin;
		}
	}
	// caso 7 -> prev == null
	else if (previous == NULL)
	{
		// 7.1 - Continuando a linha
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (!vertice)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
			return NULL;
		}
		vertice->halfedge = novoHE;
		vertice->v = *origin;

		if(next->previous == NULL)
		{
			novoHE->origin = vertice;
			novoHE->incidentFace = next->incidentFace;
			novoHE->next = next;
			novoHE->previous = NULL;

			next->previous = novoHE;

			novoHE->twin->origin = next->origin;
			novoHE->twin->incidentFace = next->twin->incidentFace;
			novoHE->twin->previous = next->twin;
			novoHE->twin->next = NULL;

			next->twin->next = novoHE->twin;
		}
		// 7.2 - Divergindo de uma linha existente
		else
		{
			novoHE->origin = vertice;
			novoHE->incidentFace = next->incidentFace;
			novoHE->next = next;
			novoHE->previous = NULL;

			novoHE->twin->origin = next->origin;
			novoHE->twin->incidentFace = next->previous->incidentFace;
			novoHE->twin->next = NULL;
			novoHE->twin->previous = next->previous;

			next->previous = novoHE;
			novoHE->twin->previous->next = novoHE->twin;
		}
	}
	// Caso 3 -> fechando poligono, 
	// next != null, prev != null, next->prev == null, prev->next == null
	else if (next != NULL && previous != NULL && next->previous == NULL && previous->next == NULL)
	{
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (!vertice)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
			return NULL;
		}
		vertice->halfedge = novoHE;
		vertice->v = *origin;

		face = (XDCEL_FACE*)malloc(sizeof(XDCEL_FACE));
		if (!face)
		{
			free(vertice);
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - face\n");
			return NULL;
		}
		novoHE->origin = vertice;
		novoHE->incidentFace = face;
		novoHE->next = next;
		novoHE->previous = previous;

		next->previous = novoHE;
		previous->next = novoHE;

		novoHE->twin->origin = next->origin;
		novoHE->twin->incidentFace = next->twin->incidentFace;
		novoHE->twin->next = previous->twin;
		novoHE->twin->previous = next->twin;

		next->twin->next = novoHE->twin;
		previous->twin->previous = novoHE->twin;
		previous->twin->origin = vertice;

		// Atualiza as faces com a nova face criada
		atual = novoHE->next;
		while (atual != novoHE)
		{
			atual->incidentFace = face;
			atual = atual->next;
		}

		face->halfedge = novoHE;
		addListaSimples(&(plano->faces), face);

	}
	// Caso 4 -> fechando poligono externo
	// next != null, prev != null, next->prev != null, prev->next == null
	else if (next != NULL && previous != NULL && next->previous != NULL && previous->next == NULL)
	{
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (!vertice)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
			return NULL;
		}
		vertice->halfedge = novoHE;
		vertice->v = *origin;

		face = (XDCEL_FACE*)malloc(sizeof(XDCEL_FACE));
		if (!face)
		{
			free(vertice);
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - face\n");
			return NULL;
		}

		face->halfedge = novoHE;

		novoHE->origin = vertice;
		novoHE->incidentFace = face;
		novoHE->next = next;
		novoHE->previous = previous;

		novoHE->twin->origin = next->origin;
		novoHE->twin->incidentFace = next->previous->incidentFace;
		novoHE->twin->next = previous->twin;
		novoHE->twin->previous = next->previous;

		previous->next = novoHE;
		previous->twin->previous = novoHE->twin;
		previous->twin->origin = vertice;
		
		next->previous->next = novoHE->twin;
		next->previous = novoHE;

		// Atualiza as faces com a nova face criada
		atual = novoHE->next;
		while (atual != novoHE)
		{
			atual->incidentFace = face;
			atual = atual->next;
		}
		face->halfedge = novoHE;
		addListaSimples(&(plano->faces), face);
	}
	// caso 5 -> fechando poligono externo por baixo
	// next != null, prev != null, next->prev == null, prev->next != null
	else if (next != NULL && previous != NULL && next->previous == NULL && previous->next != NULL)
	{
		face = (XDCEL_FACE*)malloc(sizeof(XDCEL_FACE));
		if (!face)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - face\n");
			return NULL;
		}

		face->halfedge = novoHE;

		novoHE->origin = previous->next->origin;
		novoHE->incidentFace = face;
		novoHE->next = next;
		novoHE->previous = previous;

		novoHE->twin->origin = next->origin;
		novoHE->twin->incidentFace = next->twin->incidentFace;
		novoHE->twin->next = previous->next;
		novoHE->twin->previous = next->twin;

		previous->next = novoHE;

		next->previous = novoHE;
		next->twin->next = novoHE->twin;
		novoHE->twin->next->previous = novoHE->twin;

		// Atualiza as faces com a nova face criada
		atual = novoHE->next;
		while (atual != novoHE)
		{
			atual->incidentFace = face;
			atual = atual->next;
		}
		face->halfedge = novoHE;
		addListaSimples(&(plano->faces), face);

	}
	// caso 9 -> adicionando uma aresta no meio de duas
	else if (origin != NULL && next != NULL && previous != NULL && next->previous == previous && previous->next == next)
	{
		vertice = (XDCEL_VERTEX*)malloc(sizeof(XDCEL_VERTEX));
		if (!vertice)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - vertice\n");
			return NULL;
		}
		vertice->halfedge = novoHE;
		vertice->v = *origin;

		novoHE->origin = vertice;
		novoHE->incidentFace = next->incidentFace;
		novoHE->next = next;
		novoHE->previous = previous;

		novoHE->twin->origin = next->origin;
		novoHE->twin->incidentFace = next->twin->incidentFace;
		novoHE->twin->next = previous->twin;
		novoHE->twin->previous = next->twin;

		next->previous = novoHE;
		previous->next = novoHE;

		next->twin->next = novoHE->twin;
		previous->twin->previous = novoHE->twin;
	}
	// caso 6 -> fechando poligono interno
	// next != null, prev != null, next->prev != null, prev->next != null 
	else if (next != NULL && previous != NULL && next->previous != NULL && previous->next != NULL)
	{
		face = (XDCEL_FACE*)malloc(sizeof(XDCEL_FACE));
		if (!face)
		{
			free(novoHE->twin);
			free(novoHE);
			printf("ERROR::GEOMETRIADCEL::CREATEHALFEDGE - erro ao alocar mem�ria - face\n");
			return NULL;
		}

		face->halfedge = novoHE;
		// consertando a face que vamos dividir em 2
		if(next->incidentFace != NULL)
			next->incidentFace->halfedge = novoHE->twin;

		novoHE->origin = previous->next->origin;
		novoHE->incidentFace = face;
		novoHE->next = next;
		novoHE->previous = previous;

		novoHE->twin->origin = next->origin;
		novoHE->twin->incidentFace = next->previous->incidentFace;
		novoHE->twin->next = previous->next;
		novoHE->twin->previous = next->previous;

		previous->next = novoHE;
		novoHE->twin->next->previous = novoHE->twin;

		next->previous = novoHE;
		novoHE->twin->previous->next = novoHE->twin;

		// Atualiza as faces com a nova face criada
		atual = novoHE->next;
		while (atual != novoHE)
		{
			atual->incidentFace = face;
			atual = atual->next;
		}

		face->halfedge = novoHE;
		addListaSimples(&(plano->faces), face);
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
		return;
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
 * @return a face que foi destru�da no processo.
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


void createPolygonFromFace(XPOLIGONO* poli, XDCEL_FACE* face)
{
	poli->num_vertices = 0;
	clearListaDupla(&(poli->vertices));
	
	XVERTICE v;

	XDCEL_HALF_EDGE* edge = face->halfedge, *primeira = edge;

	do{
		v = edge->origin->v;
		addVertice(poli, v);
		edge = edge->next;
	} while (edge != primeira);
}


void createPolygonsFromTopology(XLISTA_SIMPLES* listaPoligonos, XDCEL_TOPOLOGY* plano)
{
	clearListaSimples(listaPoligonos);
	printf("Nao implementado - createpolyfromtopology");
}

int DCEL_isInFace(XVERTICE ponto, XDCEL_FACE face)
{
	XVERTICE primeiro = *((XVERTICE*)face.halfedge->origin);
	XVERTICE proximo = *((XVERTICE*)face.halfedge->next->origin);

	XDCEL_HALF_EDGE* atual = face.halfedge;
	int res = 0;
	XVERTICE infx = createVertice(LONG_MAX, ponto.y, 0, 0, 0);

	// verifica se intersecta com todas as arestas do poligono
	do
	{
		res += INTERSECTA(ponto, infx, primeiro, proximo);
		atual = atual->next;
		primeiro = (atual->origin->v);
		proximo = (atual->next->origin->v);
	} while (atual != face.halfedge);
	// impar = dentro
	if (res % 2)
	{
		return TRUE;
	}
	// par = fora
	else return FALSE;
}