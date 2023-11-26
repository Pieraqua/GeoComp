#ifndef CLICKPOLYGONS_H
#define CLICKPOLYGONS_H

#include "geometria.h"
#include "geometria_dcel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef struct
{
    XPOLIGONO poligono[50];
    XDCEL_TOPOLOGY top[50];
    XLISTA_SIMPLES poligonos_cortados;
    XLISTA_SIMPLES topologias_cortadas;
    int curPoli;
}XESTADOPOLYGONS;

#define DEFAULT_COLOR 1.0f, 1.0f, 1.0f

void CP_addVertice(XVERTICE ponto, int poli);
void mouse_button_clickPolygon(GLFWwindow* window, int button, int action, int mods, int poli);
void initClickPoligons();
void CP_noClicked();
void CP_statsPressed();
void mouse_button_clickEdge(GLFWwindow* window, int button, int action, int mods, int poli);
void mouse_button_clickVertice(GLFWwindow* window, int button, int action, int mods, int poli);
void piscaArestas(XDCEL_FACE* face, GLFWwindow* window);
void piscaVertice(XDCEL_VERTEX* vertice, GLFWwindow* window);

void  CP_createVertice(XVERTICE ponto, int poli);
void CP_createEdge(XDCEL_VERTEX* ponto1, XDCEL_VERTEX* ponto2, XDCEL_FACE* face, int poli);
void CP_uniaoPolis(GLFWwindow* window, int button, int action, int mods);
void CP_intersectPolis(GLFWwindow* window, int button, int action, int mods);
#endif