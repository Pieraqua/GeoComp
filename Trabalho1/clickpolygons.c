#include "clickpolygons.h"
#include <stdio.h>
#include "graphics.h"
#include "rendererDCEL.h"

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

