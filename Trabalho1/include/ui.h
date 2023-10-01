#ifndef UI_H
#define UI_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int initUI();
void drawUI();
int UI_addIcon(const char* icon_path, void (*ui_function));
void UI_click(GLFWwindow* window, int button, int action, int mods);



#endif
