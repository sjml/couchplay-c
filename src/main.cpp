#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#pragma clang diagnostic pop
#include <civetweb.h>
#include <qrcodegen.h>

// Returns hostname for the local computer
void checkHostName(int hostname)
{
    if (hostname == -1)
    {
        perror("gethostname");
        exit(1);
    }
}

// Returns host information corresponding to host name
void checkHostEntry(struct hostent * hostentry)
{
    if (hostentry == NULL)
    {
        perror("gethostbyname");
        exit(1);
    }
}

// Converts space-delimited IPv4 addresses
// to dotted-decimal format
void checkIPbuffer(char *IPbuffer)
{
    if (NULL == IPbuffer)
    {
        perror("inet_ntoa");
        exit(1);
    }
}

int main(int argc, const char * argv[]) {
    
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    
    // To retrieve hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    checkHostName(hostname);
    
    // To retrieve host information
    host_entry = gethostbyname(hostbuffer);
    checkHostEntry(host_entry);
    
    // To convert an Internet network
    // address into ASCII string
    IPbuffer = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0]));
    
    printf("Hostname: %s\n", hostbuffer);
    printf("Host IP: %s", IPbuffer);
    
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    
    GLFWwindow* window = glfwCreateWindow(1024, 1024, "CouchPlay", NULL, NULL);
    if (!window) {
        exit(EXIT_FAILURE);
    }
    
    glfwMakeContextCurrent(window);
    
    
    struct mg_context *ctx;
    mg_init_library(0);
    const char *options[] = { "document_root", "/Users/shane/Projects/couchplay/web", "listening_ports", "8080", NULL };
    ctx = mg_start(NULL, 0, options);

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
    
    uint8_t qrCode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    std::string text = "http://";
    text += IPbuffer;
    text += ":8080";
    enum qrcodegen_Ecc errCor = qrcodegen_Ecc_LOW;
    bool ok = qrcodegen_encodeText(text.c_str(), tempBuffer, qrCode, errCor, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
    if (!ok) {
        // TODO: error
    }
    
    int qrl = qrcodegen_getSize(qrCode);
    size_t dataSize = sizeof(GLubyte) * qrl * qrl * 1;
    GLubyte* imageData = (GLubyte*)malloc(dataSize);
    for (int y = 0; y < qrl; y++) {
        for (int x = 0; x < qrl; x++) {
            if (qrcodegen_getModule(qrCode, x, y)) {
                imageData[(y*qrl)+x] = 0;
            }
            else {
                imageData[(y*qrl)+x] = 255;
            }
        }
    }
    
    glEnable(GL_TEXTURE_2D);
    
    GLuint texRef;
    glGenTextures(1, &texRef);
    glBindTexture(GL_TEXTURE_2D, texRef);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, qrl, qrl, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
    free(imageData);
    
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
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
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    mg_stop(ctx);
    mg_exit_library();
    
    return EXIT_SUCCESS;
}
