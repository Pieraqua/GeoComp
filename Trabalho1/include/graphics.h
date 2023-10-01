#ifndef __GRAPHICS_H
#define __GRAPHICS_H
#include "glad/glad.h"
#include <GLFW/glfw3.h>

int initGraphics(GLFWwindow** window);
void clearScreen();
void renderGraphics(GLFWwindow* window);
void deinitGraphics();

#endif