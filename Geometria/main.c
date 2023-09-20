#include "geometria.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>

double randMax(double max)
{
	return (double)rand() / (RAND_MAX / max);

}

int testesVertice()
{
	double x1 = randMax(2) - 1, y1 = randMax(2) - 1, r1 = randMax(1), g1 = randMax(1), b1 = randMax(1);

	XVERTICE v1 = createVertice(x1, y1, r1, g1, b1);

	assert(x1 == v1.x);
	assert(y1 == v1.y);
	assert(r1 == v1.R);
	assert(g1 == v1.G);
	assert(b1 == v1.B);

	double testx1, testy1, testr1, testg1, testb1;

	getVCoords(&v1, &testx1, &testy1);

	assert(testx1 == x1);
	assert(testy1 == y1);

	getVColor(&v1, &testr1, &testg1, &testb1);

	assert(testr1 == r1);
	assert(testg1 == g1);
	assert(testb1 == b1);

	assert(x1 == v1.x);
	assert(y1 == v1.y);
	assert(r1 == v1.R);
	assert(g1 == v1.G);
	assert(b1 == v1.B);

	return 0;

}

int testesPoligono()
{
	XPOLIGONO poli;

	criaPoligono(&poli);

	assert(poli.vertices.item == NULL);
	assert(poli.vertices.proximo == &(poli.vertices));
	assert(poli.vertices.anterior == &(poli.vertices));
	assert(poli.num_vertices == 0);

	XVERTICE v1, v2, v3;
	double x1 = randMax(1) - 1, y1 = randMax(1) - 1, r1 = randMax(1), g1 = randMax(1), b1 = randMax(1);
	double x2 = randMax(1), y2 = randMax(1) - 1, r2 = randMax(1), g2 = randMax(1), b2 = randMax(1);
	double x3 = randMax(1), y3 = randMax(1), r3 = randMax(1), g3 = randMax(1), b3 = randMax(1);
	
	v1 = createVertice(x1, y1, r1, g1, b1);
	addVertice(&poli, v1);

	assert(poli.num_vertices == 1);

	XPOLIITER it = getVerticeIterator(&poli);
	XVERTICE* v_test = getNextVertice(&it);

	assert(v_test->x == v1.x);
	assert(v_test->y == v1.y);
	assert(v_test->R == v1.R);
	assert(v_test->G == v1.G);
	assert(v_test->B == v1.B);

	v2 = createVertice(x2, y2, r2, g2, b2);
	addVertice(&poli, v2);
	assert(poli.num_vertices == 2);

	it = getVerticeIterator(&poli);
	v_test = getNextVertice(&it);

	assert(v_test->x == v1.x);
	assert(v_test->y == v1.y);
	assert(v_test->R == v1.R);
	assert(v_test->G == v1.G);
	assert(v_test->B == v1.B);

	v_test = getNextVertice(&it);

	assert(v_test->x == v2.x);
	assert(v_test->y == v2.y);
	assert(v_test->R == v2.R);
	assert(v_test->G == v2.G);
	assert(v_test->B == v2.B);

	v3 = createVertice(x3, y3, r3, g3, b3);
	addVertice(&poli, v3);
	assert(poli.num_vertices == 3);

	it = getVerticeIterator(&poli);
	v_test = getNextVertice(&it);

	v_test = getNextVertice(&it);

	assert(v_test->x == v2.x);
	assert(v_test->y == v2.y);
	assert(v_test->R == v2.R);
	assert(v_test->G == v2.G);
	assert(v_test->B == v2.B);

	v_test = getNextVertice(&it);

	assert(v_test->x == v3.x);
	assert(v_test->y == v3.y);
	assert(v_test->R == v3.R);
	assert(v_test->G == v3.G);
	assert(v_test->B == v3.B);

	limpaPoligono(&poli);

	assert(poli.vertices.item == NULL);
	assert(poli.vertices.proximo == &poli.vertices);
	assert(poli.vertices.anterior == &poli.vertices);

	XLISTA_DUPLA vertices;

	createListaDupla(&vertices);
	assert(poli.num_vertices == 0);

	addListaDupla(&vertices, &v1);
	addListaDupla(&vertices, &v2);
	addListaDupla(&vertices, &v3);

	createPoligonoFromVertices(&poli, &vertices);
	assert(poli.num_vertices == 3);

	it = getVerticeIterator(&poli);

	v_test = getNextVertice(&it);

	assert(v_test->x == v1.x);
	assert(v_test->y == v1.y);
	assert(v_test->R == v1.R);
	assert(v_test->G == v1.G);
	assert(v_test->B == v1.B);

	v2 = createVertice(x2, y2, r2, g2, b2);
	addVertice(&poli, v2);

	it = getVerticeIterator(&poli);
	v_test = getNextVertice(&it);

	assert(v_test->x == v1.x);
	assert(v_test->y == v1.y);
	assert(v_test->R == v1.R);
	assert(v_test->G == v1.G);
	assert(v_test->B == v1.B);

	v_test = getNextVertice(&it);

	assert(v_test->x == v2.x);
	assert(v_test->y == v2.y);
	assert(v_test->R == v2.R);
	assert(v_test->G == v2.G);
	assert(v_test->B == v2.B);

	v3 = createVertice(x3, y3, r3, g3, b3);
	addVertice(&poli, v3);

	it = getVerticeIterator(&poli);
	v_test = getNextVertice(&it);

	v_test = getNextVertice(&it);

	assert(v_test->x == v2.x);
	assert(v_test->y == v2.y);
	assert(v_test->R == v2.R);
	assert(v_test->G == v2.G);
	assert(v_test->B == v2.B);

	v_test = getNextVertice(&it);

	assert(v_test->x == v3.x);
	assert(v_test->y == v3.y);
	assert(v_test->R == v3.R);
	assert(v_test->G == v3.G);
	assert(v_test->B == v3.B);

	limpaPoligono(&poli);

	assert(poli.vertices.item == NULL);
	assert(poli.vertices.proximo == &poli.vertices);
	assert(poli.vertices.anterior == &poli.vertices);

	return 0;
}

int testesGeometria()
{
	int result = 0;

	srand((unsigned int)time(NULL));
	

	result += testesVertice();

	result += testesPoligono();

	return !result;
}

int main()
{
	printf("Lista ver: %d, Geometri ver: %d\n", LISTA_VER, GEOMETRIA_VER);
	if(testesGeometria())
		printf("All tests passing\n");

	

	return 0;
}