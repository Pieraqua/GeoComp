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

typedef enum {
    STATE_DRAWING_POLI,
    STATE_VERTICE,
    STATE_EDGE
} enCurrentState;

struct AppStatus {
    enCurrentState state;
};

struct AppStatus appStatus;

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void dummyFunc(GLFWwindow* window, int button, int action, int mods) {}

void stateVertice() {
    printf("Vertice\n"); appStatus.state = STATE_VERTICE; }

void statePoligono() { 
    printf("Poligono\n"); appStatus.state = STATE_DRAWING_POLI; }

void stateEdge() {
    printf("Aresta\n"); appStatus.state = STATE_EDGE; }

int main()
{
    initGraphics(&window);
    DCEL_RENDERER_init();

    printf("Hello World!\n");

    initClickPoligons();
    initUI();
    
    UI_addIcon("../resource/icons/ClickPolygonicon.bmp", statePoligono);
    UI_addIcon("../resource/icons/ClickCheckmarkicon.png", dummyFunc);
    UI_addIcon("../resource/icons/ClickNoicon.png", CP_noClicked);
    UI_addIcon("../resource/icons/rotate_vertice.png", dummyFunc);
    UI_addIcon("../resource/icons/traverse_face.png", dummyFunc);
    UI_addIcon("../resource/icons/A.png", stateEdge);
    UI_addIcon("../resource/icons/V.png", stateVertice);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, processInput);

    appStatus.state = STATE_DRAWING_POLI;

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

    if(appStatus.state == STATE_DRAWING_POLI)
        mouse_button_clickPolygon(window, button, action, mods);
    if(appStatus.state == STATE_EDGE)
        mouse_button_clickEdge(window, button, action, mods);
    if (appStatus.state == STATE_VERTICE)
        mouse_button_clickVertice(window, button, action, mods);
}

