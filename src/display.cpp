#include "display.h"

#include <stdlib.h>

#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

GLFWwindow* window;
std::vector<Actor*> actors;

void InitGL() {
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    window = glfwCreateWindow(1024, 1024, "CouchPlay", NULL, NULL);
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
    glClearColor(.2f, 0.2f, 0.2f, 1.0f);
}

void TerminateGL() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void AddActor(Actor* a) {
    actors.push_back(a);
}

void GameLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (int i=0; i < actors.size(); i++) {
            actors[i]->Render();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
