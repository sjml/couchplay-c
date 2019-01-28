#include "web.h"

#include <iostream>
#include <sstream>
#include <vector>
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


struct mg_context *ctx;

std::string StartWebServer() {
    mg_init_library(0);
    const char *options[] = {
        "document_root", "./web",
        "listening_ports", "0",
        "websocket_timeout_ms", "3600000",
        "static_file_max_age", "0",
        NULL };
    ctx = mg_start(NULL, 0, options);
    if (ctx == NULL) {
        exit(EXIT_FAILURE);
    }

    mg_set_websocket_handler(ctx, "/websocket", WebSocketConnectHandler, WebSocketReadyHandler, WebSocketDataHandler, WebSocektCloseHandler, 0);

    std::vector<struct mg_server_ports> server_ports(50);
    mg_get_server_ports(ctx, (int)server_ports.size(), &server_ports[0]);
    int portNumber = server_ports[0].port;

    char hostbuffer[256];
    gethostname(hostbuffer, sizeof(hostbuffer));

    struct addrinfo hints;
    struct addrinfo* infoptr;
    hints.ai_family = AF_INET;

    getaddrinfo(hostbuffer, NULL, &hints, &infoptr);

    struct addrinfo* p;
    bool found = false;
    for (p = infoptr; p != NULL; p = p->ai_next) {
        getnameinfo(p->ai_addr, p->ai_addrlen, hostbuffer, sizeof(hostbuffer), NULL, 0, NI_NUMERICHOST);
        int sfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sfd == -1) {
            continue;
        }
        if (!bind(sfd, p->ai_addr, sizeof(struct sockaddr*))) {
            continue;
        }
        close(sfd);
        found = true;
        break;
    }

    if (found) {
        std::ostringstream output;
        output << "http://" << hostbuffer << ":" << portNumber;
        return output.str();
    }
    else {
        exit(EXIT_FAILURE);
    }
}

void StopWebServer() {
    mg_stop(ctx);
    mg_exit_library();
}
