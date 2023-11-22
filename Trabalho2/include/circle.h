#ifndef CIRCLE_H
#define CIRCLE_H
#include "geometria.h"
#include "graphics.h"

void initCircle();
void mouse_button_clickCircle(GLFWwindow* window, int button, int action, int mods, int poli);
int inCircle(XVERTICE a, XVERTICE b, XVERTICE c, XVERTICE d);

void findCircle(XVERTICE a, XVERTICE b, XVERTICE c, XVERTICE* center, double* radius);
void createCircle(XPOLIGONO* poli, XVERTICE center, double radius, int nPoints, int incircle);

#endif
