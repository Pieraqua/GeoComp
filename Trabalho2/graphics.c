#include "graphics.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

int success;
char infoLog[512];

char wireframe = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void glfw_onError(int error, const char* description)
{
    // print message in Windows popup dialog box
    printf(description);
    printf("\nGLFW error\n");
}

int initGraphics(GLFWwindow** window){
    glfwInit();
    glfwSetErrorCallback(glfw_onError);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    *window = glfwCreateWindow(1280,720,"opeingeele", NULL, NULL);
    if(*window == NULL)
    {
        printf("glfw morreu");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(*window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad morreu");
        return -1;
    }

    glViewport(0,0,1280,720);
    glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);


    return 0;
}

void clearScreen()
{
    // render commands
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void renderGraphics(GLFWwindow* window)
{
    // check and call events, swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void deinitGraphics()
{
    glfwTerminate();
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
}