#include <stdlib.h>
#include <stdio.h>
#include "geometria.h"
#include <math.h>

// Vertice

void getVCoords(XVERTICE* v, double*x, double*y){
    if(v != NULL && x != NULL && y != NULL){
        *x = v->x;
        *y = v->y;
    }
    else
    {
        printf("ERRO::GEOMETRIA::GETVCOORDS - vertice nulo");
    }
}

void setVCoord(XVERTICE* v, double x, double y){
    if(v == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOORDS - vertice nulo");
        return;
    }
    v->x = x;
    v->y = y;
}

void getVColor(XVERTICE* v, double* r, double* g, double* b){
    if(v == NULL || r == NULL || g == NULL || b == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOLOR - vertice nulo");
        return;
    }
    *r = v->R;
    *g = v->G;
    *b = v->B;
}

enStatus setVColor(XVERTICE* v, double r, double g, double b){
    if(v == NULL)
    {
        printf("ERRO::GEOMETRIA::SETVCOLOR - vertice nulo");
        return FAIL;
    }

    v->R = r;
    v->G = g;
    v->B = b;
    return SUCCESS;
}

XVERTICE createVertice(double x, double y, double r, double g, double b){
    XVERTICE v;
    v.x = x;
    v.y = y;
    v.R = r;
    v.G = g;
    v.B = b;
    return v;
}

// Poligono
unsigned int poli_id_atual = 0;

XPOLIITER getVerticeIterator(XPOLIGONO* poli){
    XPOLIITER it;
    it.poli = poli;
    it.atual = &(poli->vertices);
    return it;
}

XVERTICE* getNextVertice(XPOLIITER* iter){
    XVERTICE* v = NULL;

    if (iter->atual == NULL || iter == NULL)
    {
        return NULL;
    }
    
    v = iter->atual->item;
    iter->atual = iter->atual->proximo;

    return v;
}

unsigned int getID(XPOLIGONO* poli){
    return poli->id;
}

void addVertice(XPOLIGONO* poli, XVERTICE v){

    XVERTICE* v_novo = (XVERTICE*)malloc(sizeof(XVERTICE));

    if (v_novo == NULL)
    {
        printf("ERRO::GEOMETRIA::ADDVERTICE - falha ao alocar memoria\n");
        return;
    }

    *v_novo = v;

    addListaDupla(&(poli->vertices), v_novo);
    poli->num_vertices++;
}

void removeVertice( XPOLIGONO* poli, XVERTICE* v){
    removeListaDupla(&(poli->vertices), v);
    poli->num_vertices--;
}

unsigned int genPoliID(){
    return poli_id_atual++;
}


void createPoligonoFromVertices(XPOLIGONO* poli, XLISTA_DUPLA* vertices)
{
    XLISTA_DUPLA_IT it = getIteratorLD(vertices);

    XVERTICE* primeiro = (XVERTICE*)getItemItLD(&it);

    poli->num_vertices = 0;
    poli->id = genPoliID();
    createListaDupla(&(poli->vertices));

    if (primeiro == NULL)
    {
        printf("WARN::GEOMETRIA::INICIAPOLIGONOUSANDOVERTICES - lista vazia\n");
        return;
    }

    addVertice(poli, *primeiro);
    XVERTICE* v = (XVERTICE*)getItemItLD(&it);

    while (v != NULL && v != primeiro) {
        addVertice(poli, *v);
        v = (XVERTICE*)getItemItLD(&it);
    } 
}

void criaPoligono(XPOLIGONO* poli)
{
    poli->num_vertices = 0;
    poli->id = genPoliID();
    createListaDupla(&(poli->vertices));
}

void limpaPoligono(XPOLIGONO* poli)
{
    XVERTICE* v_1 = NULL;

    while (poli->vertices.item != NULL)
    {
         v_1 = poli->vertices.item;
        removeVertice(poli, v_1);
        free(v_1);
    }
}



double AREA(XVERTICE p1, XVERTICE p2, XVERTICE p3)
{
    return ((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y)) / 2;
}

int LEFT(XVERTICE p1, XVERTICE p2, XVERTICE p3)
{
    return AREA(p1, p2, p3) > 0;
}

int LEFTON(XVERTICE p1, XVERTICE p2, XVERTICE p3)
{
    return AREA(p1, p2, p3) >= 0;
}

// Recebe um poligono @p poligono
// @return a area do poligono
double AREA_POLI(XPOLIGONO* poligono)
{
    double _area = 0;
    if (poligono->num_vertices < 3)
        return 0;

    XLISTA_DUPLA* iterador = &(poligono->vertices);

    for (unsigned int i = 0; i < poligono->num_vertices; i++)
    {
        _area += ((XVERTICE*)iterador->item)->x * ((XVERTICE*)iterador->proximo->item)->y - ((XVERTICE*)iterador->proximo->item)->x * ((XVERTICE*)iterador->item)->y;
    }

    return _area;
}

// Recebe um vetor de @p pontos ordenados com @p len pontos
// @return 1 para anti-horario e 0 para horario
int ORIENTACAO_POLI(XPOLIGONO* pontos)
{
    return AREA_POLI(pontos) > 0;
}

// Recebe um vertice em uma sequencia de vertices
// @return 1 se @p n eh convexo e 0 se concavo
int CONVEXO(XLISTA_DUPLA* pontos)
{
    if (pontos == NULL || pontos->anterior == NULL || pontos->proximo == NULL)
        return 0;

    return AREA(*(XVERTICE*)pontos->anterior->item, *(XVERTICE*)pontos->item, *(XVERTICE*)pontos->proximo->item) > 0;
}

// Recebe um vetor de @p pontos ordenados com @p len pontos e um vetor @p resultados onde sera guardado
// 1 quando o vertice resultados[n] for convexo e 0 caso contrario, -1 caso erro
void CONVEXO_POLI(XPOLIGONO* poli, int* resultados)
{
    XLISTA_DUPLA* it;

    if (poli->num_vertices < 3)
    {
        resultados[0] = -1;
        return;
    }
    it = &(poli->vertices);
    for (unsigned int i = 0; i < poli->num_vertices; i++)
    {
        resultados[i] = CONVEXO(it);
        it = it->proximo;
    }
}

// Recebe dois vetores de 2 pontos que representam uma aresta.
// Retorna 1 se eles intersectam e 0 se nao.
// INTERSECCAO PROPRIA
int INTERSECTA(XVERTICE pontos1_1, XVERTICE pontos1_2, XVERTICE pontos2_1, XVERTICE pontos2_2)
{
    //printf("%d, %d, %d, %d, %f, %f\n", LEFT(pontos1_1, pontos1_2, pontos2_1),
    //    LEFT(pontos1_1, pontos1_2, pontos2_2), LEFT(pontos2_1, pontos2_2, pontos1_1),
    //    LEFT(pontos2_1, pontos2_2, pontos1_1), AREA(pontos1_1, pontos1_2, pontos2_1),
    //    AREA(pontos1_1, pontos1_2, pontos2_2));

    return (
        LEFT(pontos1_1, pontos1_2, pontos2_1) ^
        LEFT(pontos1_1, pontos1_2, pontos2_2)
        )
        &&
        (
            LEFT(pontos2_1, pontos2_2, pontos1_1) ^
            LEFT(pontos2_1, pontos2_2, pontos1_2)
            )
        &&
        // Nao colinear
        (
            (AREA(pontos1_1, pontos1_2, pontos2_1) != 0) &&
            (AREA(pontos1_1, pontos1_2, pontos2_2) != 0)
            );
}

int DENTRO(XVERTICE tri1, XVERTICE tri2, XVERTICE tri3, XVERTICE alvo)
{

    return LEFT(tri1, tri2, alvo) && LEFT(tri2, tri3, alvo) && LEFT(tri3, tri1, alvo);
}

int ORELHA(XLISTA_DUPLA* pontos)
{
    XVERTICE a, b, a_minus, a_plus;

    a = *((XVERTICE*)pontos->anterior->item);
    b = *((XVERTICE*)pontos->proximo->item);
    a_minus = *((XVERTICE*)pontos->anterior->anterior->item);
    a_plus = *(XVERTICE*)pontos->item;

    if (!CONVEXO(pontos)) return 0;

    // verificar se intersecta com qqr vertice
    XLISTA_DUPLA* it = pontos->proximo, * anterior = pontos->anterior;
    int i = 0;
    if (it == pontos) return 0;

    do
    {
        if (DENTRO(a, a_plus, b, *(XVERTICE*)it->item)) {
            return 0;
        }
        it = it->proximo;
        i++;
    } while (it != anterior);

    return 1;
}

void ORELHA_POLI(XPOLIGONO* poli, int* resultados)
{
    XLISTA_DUPLA_IT it = getIteratorLD(&(poli->vertices));
    XVERTICE* item;
    for (unsigned int i = 0; i < poli->num_vertices; i++)
    {
        item = getItemItLD(&it);

        resultados[i] = ORELHA(item);
    }
}
int GEO_dentroPoligono(XPOLIGONO* poligono, XVERTICE ponto)
{
    XVERTICE primeiro = *((XVERTICE*)poligono->vertices.item);
    XVERTICE proximo = *((XVERTICE*)poligono->vertices.proximo->item);

    XLISTA_DUPLA* atual = &(poligono->vertices);
    int res = 0;
    XVERTICE infx = createVertice(1.0f, ponto.y, 0, 0, 0);

    // verifica se intersecta com todas as arestas do poligono
    for (int i = 0; i < poligono->num_vertices; i++)
    {
        res += INTERSECTA(ponto, infx, primeiro, proximo);
        atual = atual->proximo;
        primeiro = *((XVERTICE*)atual->item);
        proximo = *((XVERTICE*)atual->proximo->item);
    }
    // impar = dentro
    if (res % 2)
    {
        return TRUE;
    }
    // par = fora
    else return FALSE;
}

XVECTOR2 GEO_normalizeVector(XVECTOR2 vec)
{
    double size = sqrt(vec.x * vec.x + vec.y * vec.y);

    XVECTOR2 normalized;
    normalized.x = vec.x / size;
    normalized.y = vec.y / size;

    return normalized;
}

XVECTOR2 GEO_vectorDiff(XVECTOR2 vec1, XVECTOR2 vec2)
{
    XVECTOR2 res;
    res.x = vec1.x - vec2.x;
    res.y = vec1.y - vec2.y;
    return res;
}

XVERTICE GEO_createPonto(double x, double y)
{
    XVERTICE ponto;
    ponto.x = x;
    ponto.y = y;

    return ponto;
}

XVECTOR2 GEO_toVec(XVERTICE ponto)
{
    XVECTOR2 vec;
    vec.x = ponto.x;
    vec.y = ponto.y;

    vec.origem = GEO_createPonto(0, 0);

    return vec;
}

double GEO_dotProduct(XVECTOR2 vec1, XVECTOR2 vec2)
{
    return vec1.x * vec2.x + vec1.y * vec2.y;
}

XVECTOR2 GEO_vecFromPoints(XVERTICE origem, XVERTICE fim)
{
    XVECTOR2 res;
    res.x = fim.x - origem.x;
    res.y = fim.y - origem.y;
    res.origem = origem;

    return res;
}

double GEO_dist(XVERTICE inicio, XVERTICE fim)
{
    return sqrt((fim.x - inicio.x) * (fim.x - inicio.x) + (fim.y - inicio.y) * (fim.y - inicio.y));
}

double GEO_magnitude(XVECTOR2 vec)
{
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

XVERTICE GEO_nearestPointOnLine(XVECTOR2 line, XVERTICE ponto)
{
    XVECTOR2 lineDir = GEO_normalizeVector(line);
    XVECTOR2 v = GEO_vectorDiff(GEO_toVec(ponto), GEO_toVec(line.origem));
    double d = GEO_dotProduct(v, lineDir);

    XVERTICE res;

    if (d < 0)
        d = 0;

    if (d > GEO_magnitude(line))
    {
        d = GEO_magnitude(line);
    }

    res.x = lineDir.x * d + line.origem.x;
    res.y = lineDir.y * d + line.origem.y;

    return res;
}