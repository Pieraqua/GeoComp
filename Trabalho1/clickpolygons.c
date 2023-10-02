#include "clickpolygons.h"
#include <stdio.h>
#include "graphics.h"
#include "rendererDCEL.h"
#include <windows.h>

XESTADOPOLYGONS estadoPolygons;

void initClickPoligons()
{
    estadoPolygons.poligono.id = genPoliID();
    estadoPolygons.poligono.num_vertices = 0;

    createListaDupla(&(estadoPolygons.poligono.vertices));
}

void CP_addVertice(XVERTICE ponto)
{
    addVertice(&(estadoPolygons.poligono), ponto);
}


void mouse_button_clickPolygon(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    int width, height;
    XVERTICE ponto;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &(x), &(y));
        glfwGetWindowSize(window, &(width), &(height));

        // Conversao para (-1, 1)
        ponto.x = ((x) / width - 0.5) * 2;
        ponto.y = -((y) / height - 0.5) * 2;

        ponto.B = 1.0f;
        ponto.R = 1.0f;
        ponto.G = 1.0f;

        fprintf(stdout, "Ponto %d: %f %f\n", estadoPolygons.poligono.num_vertices,ponto.x, ponto.y);

        CP_addVertice(ponto);

        if(estadoPolygons.poligono.num_vertices >= 3)
        {
            DCEL_RENDERER_clear();
            createTopologyFromPolygon(&(estadoPolygons.top), &(estadoPolygons.poligono));
            DCEL_RENDERER_add(&(estadoPolygons.top));
        }

    }

}

void CP_noClicked()
{
    limpaPoligono(&(estadoPolygons.poligono));
    DCEL_RENDERER_clear();

    printf("Poligono reiniciado\n");
}


void mouse_button_clickEdge(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    int width, height;
    XVERTICE ponto;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &(x), &(y));
        glfwGetWindowSize(window, &(width), &(height));

        // Conversao para (-1, 1)
        x = ((x) / width - 0.5) * 2;
        y = -((y) / height - 0.5) * 2;

        if (estadoPolygons.poligono.num_vertices > 3)
        {
            fprintf(stdout, "in poli: %d, dcel %d\n", DCEL_isInFace(createVertice(x, y, 0, 0, 0),
                *((XDCEL_FACE*)estadoPolygons.top.faces.item)), GEO_dentroPoligono(&(estadoPolygons.poligono), createVertice(x, y, 0, 0, 0)));

            XLISTA_SIMPLES* atual = &(estadoPolygons.top.faces);
            while (atual != NULL)
            {
                if (DCEL_isInFace(createVertice(x, y, 0, 0, 0), *(XDCEL_FACE*)atual->item))
                {
                    piscaArestas((XDCEL_FACE*)atual->item, window);
                    break;
                }
                atual = atual->proximo;
            }

        }

    }

}

void mouse_button_clickVertice(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    int width, height;
    XVERTICE ponto;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &(x), &(y));
        glfwGetWindowSize(window, &(width), &(height));

        // Conversao para (-1, 1)
        ponto.x = ((x) / width - 0.5) * 2;
        ponto.y = -((y) / height - 0.5) * 2;

        ponto.B = 0.0f;
        ponto.R = 1.0f;
        ponto.G = 0.0f;

        fprintf(stdout, "Ponto %d: %f %f\n", estadoPolygons.poligono.num_vertices, ponto.x, ponto.y);

        CP_createVertice(ponto);

        if (estadoPolygons.poligono.num_vertices >= 3)
        {
            DCEL_RENDERER_update();
        }

    }

}



void piscaArestas(XDCEL_FACE* face, GLFWwindow* window)
{
    XDCEL_HALF_EDGE* atual = face->halfedge, * primeiro = atual;

    double green, blue;

    do
    {
        green = atual->origin->v.G;
        blue = atual->origin->v.B;
        //red2 = atual->next->origin->v.R;
        atual->origin->v.B = 0;
        atual->origin->v.G = 0;
        //atual->next->origin->v.R = 0;
        DCEL_RENDERER_update();
        clearScreen();

        DCEL_RENDERER_draw();

        renderGraphics(window);
        Sleep(500);
        atual->origin->v.G = green;
        atual->origin->v.B = blue;
        //atual->next->origin->v.R = red2;
        atual = atual->next;

    } while (atual != primeiro);

}

void  CP_createVertice(XVERTICE ponto)
{
    XLISTA_SIMPLES_IT it = getIteratorLS(&(estadoPolygons.top.faces));
    XDCEL_FACE* face_atual = getItemItLS(&it);
    XDCEL_HALF_EDGE* aresta_atual, *primeira, *prox_aresta_final = NULL, *prev_aresta_final = NULL;

    double menor_dist = LONG_MAX, dist = LONG_MAX;
    XVERTICE ponto_mais_prox, ponto_atual, *origem_nova;

    XVECTOR2 vetor_atual;

    ponto_atual = createVertice(0, 0, ponto.R, ponto.G, ponto.B);

    // Para cada face,
    while (face_atual != NULL) {
        // Para cada aresta,
        aresta_atual = face_atual->halfedge;
        primeira = face_atual->halfedge;

        if (DCEL_isInFace(ponto, *face_atual)) {
            do {
                dist = GEO_distanceFromPointToLineSegment(ponto, aresta_atual->origin->v, aresta_atual->next->origin->v);

                if (dist < menor_dist)
                {
                    ponto_atual = 
                        GEO_nearestPointOnLine(
                            GEO_vecFromPoints(aresta_atual->origin->v, 
                                aresta_atual->next->origin->v), 
                            ponto);
                    prev_aresta_final = aresta_atual;
                    prox_aresta_final = aresta_atual->next;
                    menor_dist = dist;
                }

                aresta_atual = aresta_atual->next;
            } while (aresta_atual != primeira);

            origem_nova = (XVERTICE*)malloc(sizeof(XVERTICE));

            if (origem_nova == NULL)
            {
                printf("ERRO::CLICKPOLYGONS::CREATEVERTICE - erro de alocação de memoria\n");
                return;
            }

            *origem_nova = ponto_atual;
            origem_nova->R = (rand()%256) / 256.0f;
            origem_nova->B = !prev_aresta_final->origin->v.B;
            origem_nova->G = (rand() % 256) / 256.0f;
            printf("Candidato escolhido: x%f y%f\n", ponto_atual.x, ponto_atual.y);
            // Adiciona o vertice no meio da aresta
            createEdge(&(estadoPolygons.top), origem_nova, prev_aresta_final, prox_aresta_final);

            break;
        }

        face_atual = getItemItLS(&it); 

    }
}


void addEdge(double x_click1, double y_click1, double x_click2, double y_click2 )
{

}