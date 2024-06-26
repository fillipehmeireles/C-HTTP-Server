#include "cJSON.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

void hello(int *client_socket){
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";

    write(*client_socket, response, strlen(response));

    close(*client_socket);
}
int main(int argc, char** argv) {
    int port = atoi(argv[1]);
    struct AppController *app_controller = create_app_controller();
    add_controller(app_controller,GET,"/hello",&hello);
    run(app_controller, port);
    return EXIT_SUCCESS;
}

