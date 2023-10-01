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

int main()
{
    initGraphics(&window);
    DCEL_RENDERER_init();

    printf("Hello World!\n");

    initClickPoligons();
    initUI();
    
    UI_addIcon("../resource/icons/ClickPolygonicon.bmp");
    UI_addIcon("../resource/icons/ClickCheckmarkicon.png");
    UI_addIcon("../resource/icons/ClickNoicon.png");

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
    double x, y;
    int width, height;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        glfwGetCursorPos(window, &(x), &(y));
        glfwGetWindowSize(window, &(width), &(height));

        // Conversao para (-1, 1)
        x = ((x) / width - 0.5) * 2;
        y = -((y) / height - 0.5) * 2;

        fprintf(stdout, "CLICK UI: %f %f\n", x,y);

        if (x > 0.9f)
        {
            if (y <= 1.0f && y > 0.8f)
            {
            }
            else if (y <= 0.8f && y > 0.6f)
            {
            }
            else if (y <= 0.6f && y > 0.4f)
            {
                CP_noClicked();
            }
            else if (y <= 0.4f && y > 0.2f)
            {
            }
            else if (y <= 0.2f && y > 0.0f)
            {
            }
        }
    }

    mouse_button_clickPolygon(window, button, action, mods);
    
}