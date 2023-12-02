#include <stdlib.h>
#include <stdio.h>
#include "geometria.h"
#include <math.h>
#include "lista.h"
#include "include/geometria_dcel.h"
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
    poli->num_vertices = 0; // algum bug :(
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

        resultados[i] = ORELHA(it.atual);
        item = getItemItLD(&it);
    }
}

// Retorna TRUE se o ponto estiver dentro do poligono e FALSE caso contrario
int GEO_dentroPoligono(XPOLIGONO* poligono, XVERTICE ponto)
{
    XVERTICE primeiro = *((XVERTICE*)poligono->vertices.item);
    XVERTICE proximo = *((XVERTICE*)poligono->vertices.proximo->item);

    XLISTA_DUPLA* atual = &(poligono->vertices);
    int res = 0;
    XVERTICE infx = createVertice(1.0f, ponto.y, 0, 0, 0);

    // verifica se intersecta com todas as arestas do poligono
    for (unsigned int i = 0; i < poligono->num_vertices; i++)
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
    ponto.R = 1;
    ponto.G = 1;
    ponto.B = 1;

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
    XVECTOR2 proj = GEO_projectToVector(GEO_vecFromPoints(line.origem, ponto), line);

    XVERTICE res;

    res.x = proj.x + line.origem.x;
    res.y = proj.y + line.origem.y;

    res.R = ponto.R;
    res.G = ponto.G;
    res.B = ponto.B;

    return res;
}

XVECTOR2 GEO_projectToVector(XVECTOR2 vec, XVECTOR2 target)
{
    if (vec.origem.x != target.origem.x || vec.origem.y != target.origem.y)
    {
        printf("WARN::GEO::PROJECTTOVECTOR - origens diferentes nos vetores\n");
    }

    XVECTOR2 proj;
    // d(t) = a + len * (b - a)

    double t = GEO_dotProduct(vec, target) / GEO_dotProduct(target, target);

    if (t < 0)
    {
        t = 0;
    }
    else if (t > 1)
    {
        t = 1;
    }

    proj.x = target.x * t;
    proj.y = target.y * t;

    proj.origem = vec.origem;

    return proj;
}

double GEO_distanceFromPointToLineSegment(XVERTICE ponto, XVERTICE lineStart, XVERTICE lineEnd)
{
    XVECTOR2 lineSeg = GEO_vecFromPoints(lineStart, lineEnd);

    XVECTOR2 pontoSeg = GEO_vecFromPoints(GEO_createPonto(0,0), ponto);
    XVECTOR2 pontoProjSeg = GEO_vecFromPoints(GEO_createPonto(0,0), GEO_nearestPointOnLine(lineSeg, ponto));

    double dist = GEO_magnitude(GEO_vectorDiff(pontoSeg, pontoProjSeg));

    return dist;
}

// Recebe um vetor de p1.nPontos slots onde será colocado 1 para os pontos internos e 0 para os pontos externos
void findInternalPoints(XPOLIGONO* p1, XPOLIGONO* p2, int* results_p1)
{
    XVERTICE ponto;
    XLISTA_DUPLA_IT iterador = getIteratorLD(&(p1->vertices));
    for(int i = 0; i < p1->num_vertices; i++)
    {
        ponto = *((XVERTICE*)getItemItLD(&iterador));
        results_p1[i] = GEO_dentroPoligono(p2, ponto);
    }
}

// Descobre o ponto onde intersecta (a1,a2),(v1,v2)
XVERTICE PONTO_INTERSECT(XVERTICE v1_1, XVERTICE v1_2, XVERTICE v2_1, XVERTICE v2_2)
{
    double a1, a2, b1, b2, c1, c2, xres, yres, det;
    if(INTERSECTA(v1_1,v1_2,v2_1,v2_2))
    {
        // retas paralelas ou coincidentes
        if(((v1_1.x-v1_2.x)*(v2_1.y-v2_2.y)-(v1_1.y-v1_2.y)*(v2_1.x-v2_2.x)) == 0)
        {
            return createVertice(0,0,0,0,0);
        }

        xres = ((v1_1.x*v1_2.y - v1_1.y*v1_2.x)*(v2_1.x-v2_2.x) - (v1_1.x-v1_2.x)*(v2_1.x*v2_2.y - v2_1.y*v2_2.x))/
                ((v1_1.x-v1_2.x)*(v2_1.y-v2_2.y)-(v1_1.y-v1_2.y)*(v2_1.x-v2_2.x));
        yres = ((v1_1.x*v1_2.y - v1_1.y*v1_2.x)*(v2_1.y-v2_2.y) - (v1_1.y-v1_2.y)*(v2_1.x*v2_2.y - v2_1.y*v2_2.x))/
                ((v1_1.x-v1_2.x)*(v2_1.y-v2_2.y)-(v1_1.y-v1_2.y)*(v2_1.x-v2_2.x));

        return createVertice(xres, yres, 1, 1, 1);
    } 
    return createVertice(0,0,0,0,0);
}

void GEO_intersecta(XPOLIGONO* poli, XVERTICE v1, XVERTICE v2, XVERTICE* results)
{
    XLISTA_DUPLA_IT it = getIteratorLD(&(poli->vertices));
    XVERTICE a1, a2;
    for(int i = 0; i < poli->num_vertices; i++)
    {
        a1 = *(XVERTICE*)getItemItLD(&it);
        a2 = *(XVERTICE*)getItemItLD(&it);
        
        results[i] = PONTO_INTERSECT(a1,a2,v1,v2);
        
    }
}

void GEO_pontosIntersect(XPOLIGONO* poli1, XPOLIGONO* poli2, XLISTA_SIMPLES* pontos_intersect)
{
    XLISTA_DUPLA_IT it1, it2;
    XVERTICE a1_1, a1_2, a2_1, a2_2, *ponto;

    char entrada = 0, first = 1;

    it1 = getIteratorLD(&(poli1->vertices));
    it2 = getIteratorLD(&(poli2->vertices));

    a1_1 = *(XVERTICE*)getItemItLD(&it1);
    for(int i = 0; i < poli1->num_vertices; i++)
    {
        a1_2 = a1_1;
        a1_1 = *(XVERTICE*)getItemItLD(&it1);

        if(first)
        {
            if(GEO_dentroPoligono(poli2, a1_1)) entrada = 1;
            first = 0;
        }
        
        a2_1 = *(XVERTICE*)getItemItLD(&it2);
        for(int z = 0; z < poli2->num_vertices; z++)
        {
            a2_2 = a2_1;
            a2_1 = *(XVERTICE*)getItemItLD(&it2);
            if(INTERSECTA(a1_1,a1_2,a2_1,a2_2))
            {
                ponto = (XVERTICE*)malloc(sizeof(XVERTICE));
                if (ponto == NULL)
                {
                    printf("erro de memoria\n"); return;
                }
                *ponto = PONTO_INTERSECT(a1_1,a1_2,a2_1,a2_2);

                ponto->G = entrada;
                entrada = !entrada;

                addListaSimples(pontos_intersect, ponto);
            }
        }
    }
}

void GEO_addVertexAfter(XLISTA_DUPLA* p1, XVERTICE* v)
{
    XLISTA_DUPLA* novaLista = malloc(sizeof(XLISTA_DUPLA));
    if (novaLista == NULL)
    {
        printf("erro de memoria\n");
        return;
    }
    XLISTA_DUPLA* p2 = p1->proximo;

    p1->proximo = novaLista;
    p2->anterior = novaLista;
    novaLista->proximo = p2;
    novaLista->anterior = p1;
    novaLista->item = v;
    
}
// Calcula os pontos de intersecção e devolve poli1_int e poli2_int como os poligonos que tem esses pontos de intersecção
void GEO_pontosIntersect_WeilerAtherton(XPOLIGONO* poli1, XPOLIGONO* poli2)
{
    XLISTA_DUPLA_IT it1, it2;
    XVERTICE a1_1, a1_2, a2_1, a2_2, * ponto1, *ponto2;

    char entrada = 0, first = 1;

    it1 = getIteratorLD(&(poli1->vertices));
    it2 = getIteratorLD(&(poli2->vertices));

    a1_2 = *(XVERTICE*)getItemItLD(&it1);
    for (int i = 0; i < poli1->num_vertices; i++)
    {
        a1_1 = a1_2;
        a1_2 = *(XVERTICE*)getItemItLD(&it1);

        a2_2 = *(XVERTICE*)getItemItLD(&it2);
        for (int z = 0; z < poli2->num_vertices; z++)
        {
            a2_1 = a2_2;
            a2_2 = *(XVERTICE*)getItemItLD(&it2);
            //addVertice(poli2_int, a2_1);
            if (INTERSECTA(a1_1, a1_2, a2_1, a2_2))
            {
                ponto1 = (XVERTICE*)malloc(sizeof(XVERTICE));
                if (ponto1 == NULL) { printf("erro de memoria\n"); return; };
                *ponto1 = PONTO_INTERSECT(a1_1, a1_2, a2_1, a2_2);

                // Pontos de intersecção tem R = 0, B = 1 e G = ~entrada
                ponto1->R = 0;
                ponto1->B = 1;
                ponto1->G = LEFT(a1_1, a1_2, a2_2);
                GEO_addVertexAfter(it1.atual->anterior->anterior, ponto1); 
                
                ponto2 = (XVERTICE*)malloc(sizeof(XVERTICE));
                if (ponto2 == NULL) { printf("erro de memoria\n"); return; };
                *ponto2 = *ponto1;

                ponto2->G = !LEFT(a1_1, a1_2, a2_2);
                GEO_addVertexAfter(it2.atual->anterior->anterior, ponto2);
                poli1->num_vertices++;
                poli2->num_vertices++;
                ponto2->aux = it1.atual->anterior->anterior;
                ponto1->aux = it2.atual->anterior->anterior;
                
                it1.atual = it1.atual->anterior;
                it2.atual = it2.atual->anterior;
            }
        }
    }
}

// Recebe dois poligonos poli1 e poli2 e retorna o resultado da intersecção entre eles em uma lista simples
void getIntersectPolygons(XPOLIGONO* poli1, XPOLIGONO* poli2, XLISTA_SIMPLES* res)
{
    XPOLIGONO poli1_int, poli2_int;
    criaPoligono(&poli1_int);
    criaPoligono(&poli2_int);

    GEO_pontosIntersect_WeilerAtherton(poli1, poli2, &poli1_int);
    GEO_pontosIntersect_WeilerAtherton(poli2, poli1, &poli2_int);

    XLISTA_DUPLA_IT it_poli1, it_poli2;

    it_poli1 = getIteratorLD(&(poli1->vertices));
    it_poli2 = getIteratorLD(&(poli2->vertices));

    XVERTICE* v1 = getItemItLD(&it_poli1), *primeiro = v1, *v2 = getItemItLD(&it_poli2);
    // Pega o primeiro ponto de intersecção de entrada
    while(!(v1->R == 0 && v1->B == 1 && v1->G == 0))
    {
        v1 = getItemItLD(&it_poli1);
        if(v1 == primeiro) break;
    }

    // Se encontrou um ponto de entrada, inicia o clipping do poligono
    if(v1->R == 0 && v1->B == 1 && v1->G == 0)
    {
        it_poli2 = getIteratorLD(v1->aux);
        v2 = getItemItLD(&it_poli2);

        if(v2->x == v1->x && v2->y == v1->y)
        {
            // Aqui começa o clipping do poligono
            XPOLIGONO* atual = (XPOLIGONO*)malloc(sizeof(XPOLIGONO));
            if(atual == NULL)
            {
                printf("Erro de memoria\n"); return;
            }
            criaPoligono(atual);
            addVertice(atual,*v1);

            primeiro = v1;
            v1->R = 1;
            XVERTICE* vatual, *vaux;
            XLISTA_DUPLA_IT it_atual = getIteratorLD(v2->aux);

            vatual = getItemItLD(&it_atual);
            vatual = getItemItLD(&it_atual);
            while(vatual->x != primeiro->x || vatual->y != primeiro->y)
            {
                addVertice(atual, *vatual);
                // Encontrou ponto de saída
                if(vatual->R == 0 && vatual->B == 1 && vatual->G == 1)
                {
                    vatual->R = 1;
                    vatual->B = 0;
                    
                    // Encontra o ponto de saida no outro poligono
                    it_atual = getIteratorLD(vatual->aux);
                    vaux = getItemItLD(&it_atual);
                    if(vaux->x != vatual->x && vaux->y != vatual->y)
                    {
                        printf("erro\n");
                    }
                    vatual->R = 1;
                    vatual->B = 0;
                }
                // Encontrou um ponto que já foi visitado
                else if(vatual->R == 1 && vatual->B == 0 && vatual->G == 1)
                {
                    break;
                }
                vatual = getItemItLD(&it_atual);
            }

            addListaSimples(res, atual);
        }
    }
    limpaPoligono(&poli1_int);
    limpaPoligono(&poli2_int);
    
}