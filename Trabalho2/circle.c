#include "circle.h"
#include "geometria.h"
#include "graphics.h"
#include <stdio.h>

struct XSTATUS_CIRCLE {
	XVERTICE a;
	XVERTICE b;
	XVERTICE c;
	XVERTICE d;

	int pontoAtual;
};

struct XSTATUS_CIRCLE statusCircle;

void initCircle()
{
	statusCircle.a = createVertice(0, 0, 0, 0, 0);
	statusCircle.b = createVertice(0, 0, 0, 0, 0);
	statusCircle.c = createVertice(0, 0, 0, 0, 0);
	statusCircle.d = createVertice(0, 0, 0, 0, 0);

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


    }
}

// Fala se o ponto d está no círculo formado por a,b,c
int inCircle(XVERTICE a, XVERTICE b, XVERTICE c, XVERTICE d)
{

}