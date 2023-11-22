#include "circle.h"
#include "geometria.h"
#include "graphics.h"
#include <stdio.h>
#include <math.h>
#include "clickpolygons.h"
#include "rendererDCEL.h"

struct XSTATUS_CIRCLE {
	XVERTICE a;
	XVERTICE b;
	XVERTICE c;
	XVERTICE d;

    XESTADOPOLYGONS poliCircle[4];

	int pontoAtual;
};

struct XSTATUS_CIRCLE statusCircle;


void initCircle()
{
	statusCircle.a = createVertice(0, 0, 0, 0, 0);
	statusCircle.b = createVertice(0, 0, 0, 0, 0);
	statusCircle.c = createVertice(0, 0, 0, 0, 0);
	statusCircle.d = createVertice(0, 0, 0, 0, 0);

    statusCircle.poliCircle[0].poligono.id = genPoliID();
    statusCircle.poliCircle[0].poligono.num_vertices = 0;

    createListaDupla(&(statusCircle.poliCircle[0].poligono.vertices));

    statusCircle.poliCircle[1].poligono.id = genPoliID();
    statusCircle.poliCircle[1].poligono.num_vertices = 0;

    createListaDupla(&(statusCircle.poliCircle[1].poligono.vertices));

    statusCircle.poliCircle[2].poligono.id = genPoliID();
    statusCircle.poliCircle[2].poligono.num_vertices = 0;

    createListaDupla(&(statusCircle.poliCircle[2].poligono.vertices));

    statusCircle.poliCircle[3].poligono.id = genPoliID();
    statusCircle.poliCircle[3].poligono.num_vertices = 0;

    createListaDupla(&(statusCircle.poliCircle[3].poligono.vertices));

    addVertice(&(statusCircle.poliCircle[0].poligono), statusCircle.a);
    addVertice(&(statusCircle.poliCircle[1].poligono), statusCircle.b);
    addVertice(&(statusCircle.poliCircle[2].poligono), statusCircle.c);
    addVertice(&(statusCircle.poliCircle[3].poligono), statusCircle.d);

	statusCircle.pontoAtual = 0;
}

void mouse_button_clickCircle(GLFWwindow* window, int button, int action, int mods, int poli)
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

        printf("Ponto %d: %f %f\n", statusCircle.pontoAtual, ponto.x, ponto.y);

        DCEL_RENDERER_clear();
        switch (statusCircle.pontoAtual) {
        case 0:
            statusCircle.a.x = ponto.x;
            statusCircle.a.y = ponto.y;
            statusCircle.pontoAtual++;
            (*(XVERTICE*)statusCircle.poliCircle[0].poligono.vertices.item) = ponto;
            break;
        case 1:
            statusCircle.b.x = ponto.x;
            statusCircle.b.y = ponto.y;
            statusCircle.pontoAtual++;
            (*(XVERTICE*)statusCircle.poliCircle[1].poligono.vertices.item) = ponto;
            break;
        case 2:
            statusCircle.c.x = ponto.x;
            statusCircle.c.y = ponto.y;
            statusCircle.pontoAtual++;
            (*(XVERTICE*)statusCircle.poliCircle[2].poligono.vertices.item) = ponto;

            findCircle(statusCircle.a, statusCircle.b, statusCircle.c);
            
            break;
        case 3:
            statusCircle.d.x = ponto.x;
            statusCircle.d.y = ponto.y;
            statusCircle.pontoAtual = 0;
            (*(XVERTICE*)statusCircle.poliCircle[3].poligono.vertices.item) = ponto;

            printf("In Circle: %d\n", inCircle(statusCircle.a, statusCircle.b, statusCircle.c, statusCircle.d));

            break;
        default:
            statusCircle.pontoAtual = 0;
            break;
        }

        createTopologyFromPolygon(&(statusCircle.poliCircle[0].top), &(statusCircle.poliCircle[0].poligono));
        DCEL_RENDERER_add(&(statusCircle.poliCircle[0].top));
        createTopologyFromPolygon(&(statusCircle.poliCircle[1].top), &(statusCircle.poliCircle[1].poligono));
        DCEL_RENDERER_add(&(statusCircle.poliCircle[1].top));
        createTopologyFromPolygon(&(statusCircle.poliCircle[2].top), &(statusCircle.poliCircle[2].poligono));
        DCEL_RENDERER_add(&(statusCircle.poliCircle[2].top));
        createTopologyFromPolygon(&(statusCircle.poliCircle[3].top), &(statusCircle.poliCircle[3].poligono));
        DCEL_RENDERER_add(&(statusCircle.poliCircle[3].top));

    }
}

void findCircle(XVERTICE a, XVERTICE b, XVERTICE c)
{
    double x1 = a.x;
    double x2 = b.x;
    double x3 = c.x;

    double y1 = a.y;
    double y2 = b.y;
    double y3 = c.y;

    double x12 = x1 - x2;
    double x13 = x1 - x3;

    double y12 = y1 - y2;
    double y13 = y1 - y3;

    double y31 = y3 - y1;
    double y21 = y2 - y1;

    double x31 = x3 - x1;
    double x21 = x2 - x1;

    // x1^2 - x3^2
    double sx13 = pow(x1, 2) - pow(x3, 2);

    // y1^2 - y3^2
    double sy13 = pow(y1, 2) - pow(y3, 2);

    double sx21 = pow(x2, 2) - pow(x1, 2);
    double sy21 = pow(y2, 2) - pow(y1, 2);

    double f = ((sx13) * (x12)
        +(sy13) * (x12)
        +(sx21) * (x13)
        +(sy21) * (x13))
        / (2 * ((y31) * (x12)-(y21) * (x13)));
    double g = ((sx13) * (y12)
        +(sy13) * (y12)
        +(sx21) * (y13)
        +(sy21) * (y13))
        / (2 * ((x31) * (y12)-(x21) * (y13)));

    double _c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    double h = -g;
    double k = -f;
    double sqr_of_r = h * h + k * k - _c;

    // r is the radius
    float r = sqrt(sqr_of_r);

    printf("Centre = (%f,%f)\n", h, k);
    printf("Radius = %f\n", r);
}

// Fala se o ponto d est� no c�rculo formado por a,b,c
int inCircle(XVERTICE a, XVERTICE b, XVERTICE c, XVERTICE d)
{
}