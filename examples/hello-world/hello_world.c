#include "server.h"
#include <stdio.h>
#include <stdlib.h>

void hello(int *client_socket){
    HTTPResponseOptions http_response_options = {.client_socket=client_socket,.body="<h1>Hello World, C!</h1>",.status_code=HTTP_STATUS_OK};
    server_response(http_response_options);
}

int main(int argc, char** argv) {
    if(argc < 2)
    {
        printf("Please provide a port. \nUsage: hello_world_server <PORT>");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    struct AppController *app_controller = create_app_controller();
    add_controller(app_controller,GET,"/hello",&hello);
    run(app_controller, port);
    return EXIT_SUCCESS;
}

