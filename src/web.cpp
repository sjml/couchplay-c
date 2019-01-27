#include "web.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <civetweb.h>

struct mg_context *ctx;

std::string StartWebServer() {
    mg_init_library(0);
    const char *options[] = { "document_root", "/Users/shane/Projects/couchplay/web", "listening_ports", "8080", NULL };
    ctx = mg_start(NULL, 0, options);
    
    char hostbuffer[256];
    gethostname(hostbuffer, sizeof(hostbuffer));
    struct hostent *host_entry = gethostbyname(hostbuffer);
    char *IPbuffer = inet_ntoa(*((struct in_addr*)
                           host_entry->h_addr_list[0]));
    
    std::string output = "http://";
    output += IPbuffer;
    output += ":8080";
    
    std::cout << "Hosting web client on " << output << std::endl;
    
    return output;
}

void StopWebServer() {
    mg_stop(ctx);
    mg_exit_library();
}
