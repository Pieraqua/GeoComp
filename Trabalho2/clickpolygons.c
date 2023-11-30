#include "clickpolygons.h"
#include <stdio.h>
#include "graphics.h"
#include "rendererDCEL.h"
#if  defined(_WIN32) || defined(_WIN64)
    #include <Windows.h>
#else
    #include <unistd.h>
#endif
#include <limits.h>
#include "shaders.h"

XESTADOPOLYGONS estadoPolygons;

int VAO_highlight, VBO_hightlight;

float vertice_highlight_data[10];

XSHADER shader_highlight;

void initClickPoligons()
{
    for (int i = 0; i < 50; i++)
    {
        criaPoligono(&(estadoPolygons.poligono[i]));
    }

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

void CP_addVertice(XVERTICE ponto, int poli)
{
    addVertice(&(estadoPolygons.poligono[poli]), ponto);
}


void mouse_button_clickPolygon(GLFWwindow* window, int button, int action, int mods, int poli)
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

        fprintf(stdout, "Ponto %d: %f %f, poli %d\n", estadoPolygons.poligono[poli].num_vertices,ponto.x, ponto.y, poli);

        CP_addVertice(ponto, poli);

        DCEL_RENDERER_clear();
        for(int i = 0; i < 50; i++)
        {
            if(estadoPolygons.poligono[i].num_vertices >= 3)
            {
                createTopologyFromPolygon(&(estadoPolygons.top[i]), &(estadoPolygons.poligono[i]));
                DCEL_RENDERER_add(&(estadoPolygons.top[i]));
            }
        }
    }

}

void CP_noClicked()
{
    for (int i = 0; i < 50; i++)
    {
        limpaPoligono(&(estadoPolygons.poligono[i]));
    }
    DCEL_RENDERER_clear();

    printf("Poligono reiniciado\n");
}


void mouse_button_clickEdge(GLFWwindow* window, int button, int action, int mods, int poli)
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

        if (estadoPolygons.poligono[poli].num_vertices > 3)
        {
            fprintf(stdout, "in poli: %d, dcel %d\n", DCEL_isInFace(createVertice(x, y, 0, 0, 0),
                *((XDCEL_FACE*)estadoPolygons.top[poli].faces.item)), GEO_dentroPoligono(&(estadoPolygons.poligono[poli]), createVertice(x, y, 0, 0, 0)));

            XLISTA_SIMPLES* atual = &(estadoPolygons.top[poli].faces);
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

            // Encontra a face onde est� o ponto
            face_selecionada = &(estadoPolygons.top[poli].faces);
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
                    CP_createEdge(v1, v2, face_selecionada->item, poli);
                }
            }

            ponto_atual = 0;
            face_selecionada = NULL; v1 = NULL; v2 = NULL;
         }

        if (estadoPolygons.poligono[poli].num_vertices >= 3)
        {
            DCEL_RENDERER_update();
        }


    }

}

void mouse_button_clickVertice(GLFWwindow* window, int button, int action, int mods, int poli)
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

        if (estadoPolygons.poligono[poli].num_vertices > 3)
        {
            fprintf(stdout, "in poli: %d, dcel %d\n", DCEL_isInFace(createVertice(x, y, 0, 0, 0),
                *((XDCEL_FACE*)estadoPolygons.top[poli].faces.item)), GEO_dentroPoligono(&(estadoPolygons.poligono[poli]), createVertice(x, y, 0, 0, 0)));

            XLISTA_SIMPLES* atual = &(estadoPolygons.top[poli].faces);
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

        fprintf(stdout, "Ponto %d: %f %f\n", estadoPolygons.poligono[poli].num_vertices, ponto.x, ponto.y);

        CP_createVertice(ponto, poli);

        if (estadoPolygons.poligono[poli].num_vertices >= 3)
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
        #if  defined(_WIN32) || defined(_WIN64)
        //Sleep(1000); // Sleep 1 segundo
        Sleep(500); // Sleep 0,5 segundo
      #else
        //sleep(1); // Sleep 1 segundo
        usleep(500*1000);  // Sleep 0,5 segundo (500 milisegundos)
      #endif
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
        #if  defined(_WIN32) || defined(_WIN64)
        //Sleep(1000); // Sleep 1 segundo
        Sleep(500); // Sleep 0,5 segundo
      #else
        //sleep(1); // Sleep 1 segundo
        usleep(500*1000);  // Sleep 0,5 segundo (500 milisegundos)
      #endif
        atual = atual->next;

    } while (atual != primeiro);

}

void  CP_createVertice(XVERTICE ponto, int poli)
{
    XLISTA_SIMPLES_IT it = getIteratorLS(&(estadoPolygons.top[poli].faces));
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
                printf("ERRO::CLICKPOLYGONS::CREATEVERTICE - erro de aloca��o de memoria\n");
                return;
            }

            if (prev_aresta_final != NULL && prox_aresta_final != NULL) {
                *origem_nova = ponto_atual;
                origem_nova->R = (rand() % 256) / 256.0f;
                origem_nova->B = !prev_aresta_final->origin->v.B;
                origem_nova->G = 0.0f;//(rand() % 256) / 256.0f;
                printf("Candidato escolhido: x%f y%f\n", ponto_atual.x, ponto_atual.y);
                // Adiciona o vertice no meio da aresta
                createEdge(&(estadoPolygons.top[poli]), origem_nova, prev_aresta_final, prox_aresta_final);
            }
            break;
        }

        face_atual = getItemItLS(&it); 

    }
}

void CP_createEdge(XDCEL_VERTEX* ponto1, XDCEL_VERTEX* ponto2, XDCEL_FACE* face, int poli)
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


    createEdge(&(estadoPolygons.top[poli]), NULL, prev, next);
    DCEL_RENDERER_update();
}

void CP_uniaoPolis(GLFWwindow* window, int button, int action, int mods)
{
    int* res_internos = (int*)malloc(sizeof(int)*estadoPolygons.poligono[0].num_vertices);
    if(!res_internos)
    {
        printf("Erro uniao polis - falta de memoria\n");
        return;
    }

    findInternalPoints(&(estadoPolygons.poligono[0]), &(estadoPolygons.poligono[1]), res_internos);

    XVERTICE* ponto;
    XLISTA_DUPLA_IT iterador = getIteratorLD(&(estadoPolygons.poligono[0].vertices));
    for(int i = 0; i < estadoPolygons.poligono[0].num_vertices; i++)
    {
        ponto = getItemItLD(&iterador);
        
        ponto->G = !res_internos[i];
        
    }
    free(res_internos);
    res_internos = (int*)malloc(sizeof(int)*estadoPolygons.poligono[1].num_vertices);
    if(!res_internos)
    {
        printf("Erro uniao polis - falta de memoria\n");
        return;
    }

    findInternalPoints(&(estadoPolygons.poligono[1]), &(estadoPolygons.poligono[0]), res_internos);

    iterador = getIteratorLD(&(estadoPolygons.poligono[1].vertices));
    for(int i = 0; i < estadoPolygons.poligono[1].num_vertices; i++)
    {
        ponto = getItemItLD(&iterador);
        ponto->G = !res_internos[i];
        
    }

    free(res_internos);



    DCEL_RENDERER_clear();
    if(estadoPolygons.poligono[0].num_vertices >= 3)
    {
        createTopologyFromPolygon(&(estadoPolygons.top[0]), &(estadoPolygons.poligono[0]));
        DCEL_RENDERER_add(&(estadoPolygons.top[0]));
    }
    if (estadoPolygons.poligono[1].num_vertices >= 3)
    {
        createTopologyFromPolygon(&(estadoPolygons.top[1]), &(estadoPolygons.poligono[1]));
        DCEL_RENDERER_add(&(estadoPolygons.top[1]));
    }

    XLISTA_SIMPLES_IT it;
    it = getIteratorLS(&(estadoPolygons.poligonos_cortados));
    XPOLIGONO* poli = getItemItLS(&it);
    while (poli != NULL)
    {
        if (poli->num_vertices >= 1)
        {

            createTopologyFromPolygon(&(estadoPolygons.top[1]), &(estadoPolygons.poligono[1]));
            DCEL_RENDERER_add(&(estadoPolygons.top[1]));
        }
    }


}

void CP_intersectPolis(GLFWwindow* window, int button, int action, int mods)
{
    XLISTA_SIMPLES pontos_intersect;
    createListaSimples(&pontos_intersect);

    XPOLIGONO *poli1_int, *poli2_int;

    poli1_int = &estadoPolygons.poligono[47];
    poli2_int = &estadoPolygons.poligono[48];

    GEO_pontosIntersect_WeilerAtherton(&(estadoPolygons.poligono[0]), &(estadoPolygons.poligono[1]), &pontos_intersect, poli1_int);
    GEO_pontosIntersect_WeilerAtherton(&(estadoPolygons.poligono[1]), &(estadoPolygons.poligono[0]), &pontos_intersect, poli2_int);

    XLISTA_SIMPLES_IT it = getIteratorLS(&pontos_intersect);
    XVERTICE* v;

    /*
    v = (XVERTICE*)getItemItLS(&it);
    while(v != NULL)
    {
        //addVertice(&(estadoPolygons.poligono[2]), *v);
        v = (XVERTICE*)getItemItLS(&it);
    }
    */
    clearListaSimples(&pontos_intersect);

    DCEL_RENDERER_clear();

    for (int i = 0; i < 50; i++)
    {
        if (estadoPolygons.poligono[i].num_vertices >= 1)
        {
            createTopologyFromPolygon(&(estadoPolygons.top[i]), &(estadoPolygons.poligono[i]));
            DCEL_RENDERER_add(&(estadoPolygons.top[i]));
        }
    }

}