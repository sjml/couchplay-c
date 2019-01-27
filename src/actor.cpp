#include "actor.h"


GLfloat squareVertices[] = {
    2.5f, 7.5f,
    2.5f, 2.5f,
    7.5f, 7.5f,
    7.5f, 2.5f,
};

GLfloat UV[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

Actor::Actor() {
    this->texture = -1;
    this->color[0] = 1.0f;
    this->color[1] = 1.0f;
    this->color[2] = 1.0f;
}

void Actor::SetTexture(GLuint tex) {
    this->texture = tex;
}

void Actor::SetColor(GLfloat color[3]) {
    this->color[0] = color[0];
    this->color[1] = color[1];
    this->color[2] = color[2];
}

void Actor::Update(float dt) {
    
}

void Actor::Render() {
    glColor3fv(this->color);
    
    glBindTexture(GL_TEXTURE_2D, this->texture);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 10.0, 0.0, 10.0f, -1.0f, 1.0f);
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, squareVertices);
    glTexCoordPointer(2, GL_FLOAT, 0, UV);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glPopMatrix();
}
