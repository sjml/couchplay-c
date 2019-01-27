#include <stdlib.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

int main(int argc, const char * argv[]) {
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    GLFWwindow* window = glfwCreateWindow(1024, 768, "OpenGL Window", NULL, NULL);
    if (!window) {
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
