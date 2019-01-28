#pragma once

#if __APPLE__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdocumentation"
#endif
#include <GLFW/glfw3.h>
#if __APPLE__
	#pragma clang diagnostic pop
#endif

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
