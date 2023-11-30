#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "rendererDCEL.h"
#include "geometria_dcel.h"
#include "ui.h"
#include "clickpolygons.h"
#include "circle.h"

#define TRUE 1
#define FALSE 0

GLFWwindow* window = NULL;

typedef enum {
    STATE_DRAWING_POLI_1,
    STATE_DRAWING_POLI_2,
    STATE_CIRCLES
} enCurrentState;

struct AppStatus {
    enCurrentState state;
    int poli;
};

struct AppStatus appStatus;

void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void dummyFunc(GLFWwindow* window, int button, int action, int mods) {}


void statePoligono1() { 
    if(appStatus.poli < 49) appStatus.poli++;
    printf("Poligono %d\n", appStatus.poli); appStatus.state = STATE_DRAWING_POLI_1;  
    }

void statePoligono2() {
    if(appStatus.poli > 0) appStatus.poli--;
    printf("Poligono %d\n", appStatus.poli); appStatus.state = STATE_DRAWING_POLI_2; 
}

void stateCircles() {
    printf("Circulos\n"); appStatus.state = STATE_CIRCLES;
}


int main()
{
    initGraphics(&window);
    DCEL_RENDERER_init();

    printf("Hello World!\n");

    initClickPoligons();
    initCircle();
    initUI();
    
    UI_addIcon("../resource/icons/P1.png", statePoligono1);
    UI_addIcon("../resource/icons/P2.png", statePoligono2);
    UI_addIcon("../resource/icons/U.png", CP_uniaoPolis);
    UI_addIcon("../resource/icons/Intersect.png", CP_intersectPolis);
    UI_addIcon("../resource/icons/Negate.png", dummyFunc);
    //UI_addIcon("../resource/icons/ClickCheckmarkicon.png", CP_yesClicked);
    UI_addIcon("../resource/icons/ClickNoicon.png", CP_noClicked);
    UI_addIcon("../resource/icons/TriangleCircleIcon.png", stateCircles);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, processInput);

    appStatus.state = STATE_DRAWING_POLI_1;
    appStatus.poli = 0;

    glEnable(GL_PROGRAM_POINT_SIZE);

    //render loop
    while(!glfwWindowShouldClose(window))
    {
        clearScreen();
        if(appStatus.state == stateCircles)
            DCEL_RENDERER_drawPoints();
        else
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

    if (appStatus.state == STATE_DRAWING_POLI_1)
        mouse_button_clickPolygon(window, button, action, mods, appStatus.poli);
    else if (appStatus.state == STATE_DRAWING_POLI_2)
        mouse_button_clickPolygon(window, button, action, mods, appStatus.poli);
    else if (appStatus.state == STATE_CIRCLES)
        mouse_button_clickCircle(window, button, action, mods, 0);
}

