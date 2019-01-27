#pragma once

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

class Actor {
public:
    Actor();
    void Update(float dt);
    void Render();
    void SetTexture(GLuint tex);
    void SetColor(GLfloat color[3]);
    
private:
    GLuint texture;
    GLfloat color[3];
};
