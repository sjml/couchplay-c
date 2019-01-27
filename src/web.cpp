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

#include "display.h"

#define MAX_WS_CLIENTS (5)

struct t_ws_client {
    struct mg_connection *conn;
    int state;
} static ws_clients[MAX_WS_CLIENTS];

int WebSocketConnectHandler(const struct mg_connection *conn, void *cbdata) {
    struct mg_context *ctx = mg_get_context(conn);
    int reject = 1;
    int i;
    
    mg_lock_context(ctx);
    for (i = 0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].conn == NULL) {
            ws_clients[i].conn = (struct mg_connection *)conn;
            ws_clients[i].state = 1;
            mg_set_user_connection_data(ws_clients[i].conn, (void *)(ws_clients + i));
            reject = 0;
            break;
        }
    }
    mg_unlock_context(ctx);
    
    fprintf(stdout, "Websocket client %s\n", (reject? "rejected" : "accepted"));
    return reject;
}

void WebSocketReadyHandler(struct mg_connection *conn, void *cbdata) {
    const char* text = "READY";
    struct t_ws_client* client = (t_ws_client*)mg_get_user_connection_data(conn);
    
    mg_websocket_write(conn, MG_WEBSOCKET_OPCODE_TEXT, text, strlen(text));
    fprintf(stdout, "Greeting message sent to client.\n");
    assert(client->conn == conn);
    assert(client->state == 1);
    
    client->state = 2;
}

int WebSocketDataHandler(struct mg_connection* conn, int bits, char* data, size_t len, void* cbdata) {
    struct t_ws_client* client = (t_ws_client*)mg_get_user_connection_data(conn);
    assert(client->conn == conn);
    assert(client->state >= 1);
    
    if ((((unsigned char)bits) & 0x0F) == MG_WEBSOCKET_OPCODE_TEXT) {
        std::string dataString(data);
        GLfloat c[3] = {0.0f, 0.0f, 0.0f};
        if (dataString.substr(0, 3) == "up ") {
            c[0] = 0.2f;
            c[1] = 0.2f;
            c[2] = 0.2f;
        }
        else if (dataString.substr(0, 5) == "down ") {
            if (dataString[5] == 'r') {
                c[0] = 1.0f;
                c[1] = 0.2f;
                c[2] = 0.2f;
            }
            else if (dataString[5] == 'b') {
                c[0] = 0.2f;
                c[1] = 0.2f;
                c[2] = 1.0f;
            }
        }
        SetBG(c);
    }
//
//    fprintf(stdout, "Websocket got %lu bytes of ", (unsigned long)len);
//    switch (((unsigned char)bits) & 0x0F) {
//        case MG_WEBSOCKET_OPCODE_CONTINUATION:
//            fprintf(stdout, "continuation");
//            break;
//        case MG_WEBSOCKET_OPCODE_TEXT:
//            fprintf(stdout, "text");
//            break;
//        case MG_WEBSOCKET_OPCODE_BINARY:
//            fprintf(stdout, "binary");
//            break;
//        case MG_WEBSOCKET_OPCODE_CONNECTION_CLOSE:
//            fprintf(stdout, "close");
//            break;
//        case MG_WEBSOCKET_OPCODE_PING:
//            fprintf(stdout, "ping");
//            break;
//        case MG_WEBSOCKET_OPCODE_PONG:
//            fprintf(stdout, "pong");
//            break;
//        default:
//            fprintf(stdout, "unknown(%1xh)", ((unsigned char)bits) & 0x0F);
//            break;
//    }
//    fprintf(stdout, "\ndata:");
//    fwrite(data, len, 1, stdout);
//    fprintf(stdout, "\n\n");
    
    return 1;
}

void WebSocektCloseHandler(const struct mg_connection* conn, void* cbdata) {
    struct mg_context* ctx = mg_get_context(conn);
    struct t_ws_client* client = (t_ws_client*)mg_get_user_connection_data(conn);
    assert(client->conn == conn);
    assert(client->state >= 1);
    
    mg_lock_context(ctx);
    client->state = 0;
    client->conn = NULL;
    mg_unlock_context(ctx);
    
    fprintf(stdout, "Client dropped\n\n");
}

void InformWebSockets(struct mg_context *ctx) {
    static unsigned long cnt = 0;
    char text[32];
    size_t textlen;
    int i;
    
    sprintf(text, "%lu", ++cnt);
    textlen = strlen(text);
    
    mg_lock_context(ctx);
    for (i=0; i < MAX_WS_CLIENTS; i++) {
        if (ws_clients[i].state == 2) {
            mg_websocket_write(ws_clients[i].conn, MG_WEBSOCKET_OPCODE_TEXT, text, textlen);
        }
    }
    mg_unlock_context(ctx);
}


///////////////////////////////////////
///////////////////////////////////////
///////////////////////////////////////

struct mg_context *ctx;

std::string StartWebServer() {
    mg_init_library(0);
    const char *options[] = {
        "document_root", "/Users/shane/Projects/couchplay/web",
        "listening_ports", "8080",
        "websocket_timeout_ms", "3600000",
        "static_file_max_age", "0",
        NULL };
    ctx = mg_start(NULL, 0, options);
    
    mg_set_websocket_handler(ctx, "/websocket", WebSocketConnectHandler, WebSocketReadyHandler, WebSocketDataHandler, WebSocektCloseHandler, 0);
    
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
