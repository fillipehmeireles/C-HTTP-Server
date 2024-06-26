#include "server.h"
#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

void parse_response(char* response, HTTP_Response_Status status, ContentType content_type, char* body){

    const char *raw_response =
        "%s %s\r\n"
        "Content-Type: %s\r\n"
        "Connection: close\r\n"
        "\r\n %s" ;

    sprintf(response, raw_response,HTTP_VERSION,  status, content_type, body);

}

struct AppController *create_app_controller(){
    struct AppController *app_controller = (struct AppController*)malloc(sizeof(struct AppController)); 
    if(!app_controller){
        perror("[!] Error on creating app controller");
        exit(EXIT_FAILURE);
    }
    app_controller->controllers_c = 0;
    return app_controller;
}

Controller* create_controller(Method method, char *route, Callback *callback){
    Controller *controller = (Controller*)malloc(sizeof(Controller));
    if(!controller){
        perror("[!] Error on creating controller");
        exit(EXIT_FAILURE);
    }
    controller->method = method;
    controller->route = route;
    controller->Callback = callback;

    return controller;
}

void add_controller(struct AppController *app_controller,Method method, char *route, Callback *callback){
    app_controller->controllers = (Controller**)realloc(app_controller->controllers, (app_controller->controllers_c + 1) * sizeof(Controller*)); 
    if(!app_controller->controllers){
        fprintf(stderr,"[!] Error on adding controller [%s %s] to the app controller",method, route);
        exit(EXIT_FAILURE);
    }

    Controller *controller = create_controller(method, route, callback); 
    app_controller->controllers[app_controller->controllers_c] = controller;
    app_controller->controllers_c++;
}

void request_handler(int* client_socket, struct AppController *app_controller) { 
    if(!app_controller->controllers){
        printf("[<!>] You still didn't declare a route.");
        return;
    }

    char *buffer = (char*)malloc(BUF_SIZE);
    int bytes_read = read(*client_socket, buffer, BUF_SIZE - 1);

    if (bytes_read < 0) {
        perror("[!] Error reading from socket");
        close(*client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    char *token;
    int i = 0;
    char *headers[2];
    while((token = strsep(&buffer, " "))){
        headers[i] = token;
        i++;
        if(i == 2){
            break;
        }
    }

    char *method = headers[0];

    char* path = headers[1];
    if (method == NULL || path == NULL) { 
        printf("nomethod or path");
        close(*client_socket);
        return;
    }
    for(int i = 0; i < app_controller->controllers_c; i++)
    {
        if(strcmp(app_controller->controllers[i]->route,path) == 0 && strcmp((char*)app_controller->controllers[i]->method, method) == 0){
            app_controller->controllers[i]->Callback(client_socket);
            break;
        }
    }
    if(client_socket)
    {
        char response[BUF_SIZE];
        const char *raw_response =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n" 
            "<head>"
            "<title>404 Not Found</title>"
            "</head>"
            "<body>"
            "<h1>Not Found</h1>"
            "<p>The requested URL /%s was not found on this server.</p>"
            "</body>"
            "</html>";

        sprintf(response, raw_response, path);

        write(*client_socket, response, strlen(response));
        close(*client_socket);
    }
}

void run(struct AppController *app_controller, int port){
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("[!] Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[!] Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CONS) < 0) {
        perror("[!] Error listening on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("[*] Server listening on port %d\n", port);

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("[!] Error accepting connection");
            continue;
        }

        request_handler(&client_socket, app_controller);
    }

    close(server_socket);
}
