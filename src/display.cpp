#include "display.h"

#include <stdlib.h>

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

GLFWwindow* window;
std::vector<Actor*> actors;
GLfloat bg_color[3] = {0.2f, 0.2f, 0.2f};

void InitGL() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    window = glfwCreateWindow(800, 800, "CouchPlay", NULL, NULL);
    if (!window) {
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    glClearDepth(1.0f);
    glPolygonMode(GL_FRONT, GL_FILL);
    glShadeModel(GL_FLAT);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearStencil(0);
    SetBG(bg_color);
}

void TerminateGL() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void AddActor(Actor* a) {
    actors.push_back(a);
}

void SetBG(GLfloat c[3]) {
    bg_color[0] = c[0];
    bg_color[1] = c[1];
    bg_color[2] = c[2];
}

void GameLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(bg_color[0], bg_color[1], bg_color[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (int i=0; i < actors.size(); i++) {
            actors[i]->Render();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
