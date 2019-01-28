#include "qr.h"

#if __APPLE__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif
#include <GLFW/glfw3.h>
#if __APPLE__
#pragma clang diagnostic pop
#endif

#include <qrcodegen.h>

unsigned int bindQRTexture(std::string textToEncode) {
    uint8_t qrCode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
    
    enum qrcodegen_Ecc errCor = qrcodegen_Ecc_LOW;
    bool ok = qrcodegen_encodeText(textToEncode.c_str(), tempBuffer, qrCode, errCor, qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
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
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, qrl, qrl, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
    free(imageData);
    
    return texRef;
}
