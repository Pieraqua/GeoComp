#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "rendererDCEL.h"
#include "geometria_dcel.h"
#include "ui.h"
#include "clickpolygons.h"

#define TRUE 1
#define FALSE 0

GLFWwindow* window = NULL;

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void dummyFunc(GLFWwindow* window, int button, int action, int mods) {}

int main()
{
    initGraphics(&window);
    DCEL_RENDERER_init();

    printf("Hello World!\n");

    initClickPoligons();
    initUI();
    
    UI_addIcon("../resource/icons/ClickPolygonicon.bmp", dummyFunc);
    UI_addIcon("../resource/icons/ClickCheckmarkicon.png", dummyFunc);
    UI_addIcon("../resource/icons/ClickNoicon.png", CP_noClicked);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, processInput);

    //render loop
    while(!glfwWindowShouldClose(window))
    {
        clearScreen();

        DCEL_RENDERER_draw();
        drawUI();

        renderGraphics(window);
    }

    DCEL_RENDERER_clear();
    deinitGraphics();
    
    return 0;
}


void processInput(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        switch(key){
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, TRUE);
                break;
            case GLFW_KEY_F:
                break;
            case GLFW_KEY_P:
                //updateRendererDrawMode(MODE_DOTS);
                break;
            case GLFW_KEY_T:
                //updateRendererDrawMode(MODE_TRIANGLES);
                break;
            case GLFW_KEY_L:
                //updateRendererDrawMode(MODE_LINES);
                break;
            case GLFW_KEY_1:
                break;
            case GLFW_KEY_2:
                break;

        }
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    UI_click(window, button, action, mods);

    mouse_button_clickPolygon(window, button, action, mods);
    
}