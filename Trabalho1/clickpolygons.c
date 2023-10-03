#include "clickpolygons.h"
#include <stdio.h>
#include "graphics.h"
#include "rendererDCEL.h"
#include <windows.h>
#include "shaders.h"

XESTADOPOLYGONS estadoPolygons;

int VAO_highlight, VBO_hightlight;

float vertice_highlight_data[10];

XSHADER shader_highlight;

void initClickPoligons()
{
    estadoPolygons.poligono.id = genPoliID();
    estadoPolygons.poligono.num_vertices = 0;

    createListaDupla(&(estadoPolygons.poligono.vertices));


    glGenBuffers(1, &VBO_hightlight);
    glGenVertexArrays(1, &VAO_highlight);

    glBindVertexArray(VAO_highlight);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_hightlight);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    initShader("../resource/shaders/vertexShader2D.vs", "../resource/shaders/fragmentShader2D.fs", &shader_highlight);
    useShader(shader_highlight);
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

    XVERTICE ponto;
    static int ponto_atual = 0;
    static XDCEL_VERTEX* v1 = NULL, *v2 = NULL;
    static XLISTA_SIMPLES* face_selecionada = NULL;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &(x), &(y));
        glfwGetWindowSize(window, &(width), &(height));

        if (ponto_atual == 0) {
            // Conversao para (-1, 1)
            ponto.x = ((x) / width - 0.5) * 2;
            ponto.y = -((y) / height - 0.5) * 2;
            ponto.B = 0.0f;
            ponto.R = 1.0f;
            ponto.G = 0.0f;

            // Encontra a face onde está o ponto
            face_selecionada = &(estadoPolygons.top.faces);
            while (face_selecionada != NULL)
            {
                if (DCEL_isInFace(ponto, *((XDCEL_FACE*)face_selecionada->item)))
                {
                    v1 = DCEL_getClosestVerticeInFace(ponto, *((XDCEL_FACE*)face_selecionada->item));
                    ponto_atual++;
                    fprintf(stdout, "Ponto1: %f %f\n", v1->v.x, v1->v.y);
                    break;
                }
                else face_selecionada = face_selecionada->proximo;
            }

            
        }
        else if (ponto_atual == 1)
        {
            // Conversao para (-1, 1)
            ponto.x = ((x) / width - 0.5) * 2;
            ponto.y = -((y) / height - 0.5) * 2;
            ponto.B = 0.0f;
            ponto.R = 1.0f;
            ponto.G = 0.0f;
            if (DCEL_isInFace(ponto, *((XDCEL_FACE*)face_selecionada->item)))
            {
                v2 = DCEL_getClosestVerticeInFace(ponto, *((XDCEL_FACE*)face_selecionada->item));

                fprintf(stdout, "Ponto2: %f %f\n", v2->v.x, v2->v.y);

                if (v1 != v2)
                {
                    fprintf(stdout, "Criando aresta\n");
                    CP_createEdge(v1, v2, face_selecionada->item);
                }
            }

            ponto_atual = 0;
            face_selecionada = NULL; v1 = NULL; v2 = NULL;
         }

        if (estadoPolygons.poligono.num_vertices >= 3)
        {
            DCEL_RENDERER_update();
        }

    }

}

void mouse_button_clickVertice(GLFWwindow* window, int button, int action, int mods)
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
                    
                    piscaVertice(DCEL_getClosestVerticeInFace(GEO_createPonto(x, y), *(XDCEL_FACE*)atual->item) , window);
                    break;
                }
                atual = atual->proximo;
            }

        }

    }

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

void CP_highlight_draw()
{
    glBindVertexArray(VAO_highlight);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_hightlight);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 10, vertice_highlight_data, GL_STATIC_DRAW);

    glBindVertexArray(VAO_highlight);
    useShader(shader_highlight);
    glDrawArrays(GL_LINES, 0, 2);
}

void piscaVertice(XDCEL_VERTEX* vertice, GLFWwindow* window)
{
    XDCEL_HALF_EDGE* atual = vertice->halfedge, * primeiro = atual;


    do
    {
        vertice_highlight_data[0] = atual->origin->v.x;
        vertice_highlight_data[1] = atual->origin->v.y;
        vertice_highlight_data[2] = 1.0f;
        vertice_highlight_data[3] = 0.0f;
        vertice_highlight_data[4] = 0.0f;
        vertice_highlight_data[5] = atual->next->origin->v.x;
        vertice_highlight_data[6] = atual->next->origin->v.y;
        vertice_highlight_data[7] = 1.0f;
        vertice_highlight_data[8] = 0.0f;
        vertice_highlight_data[9] = 0.0f;

        clearScreen();

        DCEL_RENDERER_draw();
        CP_highlight_draw();

        renderGraphics(window);
        Sleep(500);
        atual = atual->previous->twin;

    } while (atual != primeiro);

}

void piscaArestas(XDCEL_FACE* face, GLFWwindow* window)
{
    XDCEL_HALF_EDGE* atual = face->halfedge, * primeiro = atual;


    do
    {
        vertice_highlight_data[0] = atual->origin->v.x;
        vertice_highlight_data[1] = atual->origin->v.y;
        vertice_highlight_data[2] = 1.0f;
        vertice_highlight_data[3] = 0.0f;
        vertice_highlight_data[4] = 0.0f;
        vertice_highlight_data[5] = atual->next->origin->v.x;
        vertice_highlight_data[6] = atual->next->origin->v.y;
        vertice_highlight_data[7] = 1.0f;
        vertice_highlight_data[8] = 0.0f;
        vertice_highlight_data[9] = 0.0f;

        clearScreen();

        DCEL_RENDERER_draw();
        CP_highlight_draw();

        renderGraphics(window);
        Sleep(500);
        atual = atual->next;

    } while (atual != primeiro);

}

void  CP_createVertice(XVERTICE ponto)
{
    XLISTA_SIMPLES_IT it = getIteratorLS(&(estadoPolygons.top.faces));
    XDCEL_FACE* face_atual = getItemItLS(&it);
    XDCEL_HALF_EDGE* aresta_atual, *primeira, *prox_aresta_final = NULL, *prev_aresta_final = NULL;

    double menor_dist = LONG_MAX, dist = LONG_MAX;
    XVERTICE ponto_atual, *origem_nova;

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

            if (prev_aresta_final != NULL && prox_aresta_final != NULL) {
                *origem_nova = ponto_atual;
                origem_nova->R = (rand() % 256) / 256.0f;
                origem_nova->B = !prev_aresta_final->origin->v.B;
                origem_nova->G = 0.0f;//(rand() % 256) / 256.0f;
                printf("Candidato escolhido: x%f y%f\n", ponto_atual.x, ponto_atual.y);
                // Adiciona o vertice no meio da aresta
                createEdge(&(estadoPolygons.top), origem_nova, prev_aresta_final, prox_aresta_final);
            }
            break;
        }

        face_atual = getItemItLS(&it); 

    }
}

void CP_createEdge(XDCEL_VERTEX* ponto1, XDCEL_VERTEX* ponto2, XDCEL_FACE* face)
{
    XDCEL_HALF_EDGE* prev = ponto1->halfedge, * next = ponto2->halfedge;

    XDCEL_HALF_EDGE* primeira = ponto1->halfedge;

    do {
        if (prev->incidentFace == face)
        {
            break;
        }
        prev = prev->previous->twin;
    } while (primeira != prev);

    prev = prev->previous;

    primeira = ponto2->halfedge;

    do {
        if (next->incidentFace == face)
        {
            break;
        }
        next = next->previous->twin;
    } while (primeira != next);


    createEdge(&(estadoPolygons.top), NULL, prev, next);
    DCEL_RENDERER_update();
}