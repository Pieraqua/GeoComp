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

void piscaArestas(XDCEL_FACE* face, GLFWwindow* window)
{
    XDCEL_HALF_EDGE* atual = face->halfedge, *primeiro = atual;

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

    }
    while (atual != primeiro);

}

void CP_noClicked()
{
    limpaPoligono(&(estadoPolygons.poligono));
    DCEL_RENDERER_clear();

    printf("Poligono reiniciado\n");
}

