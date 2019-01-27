#include <stdlib.h>
#include <string.h>

#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include "display.h"
#include "web.h"
#include "qr.h"
#include "actor.h"

int main(int argc, const char * argv[]) {
    
    std::string webAddr = StartWebServer();
    
    InitGL();
    
    Actor* qr = new Actor();
    qr->SetTexture(bindQRTexture(webAddr));
    AddActor(qr);
    
    GameLoop();
    
    TerminateGL();
    StopWebServer();
    
    return EXIT_SUCCESS;
}
