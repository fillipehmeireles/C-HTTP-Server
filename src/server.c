#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>

void request_handler(int *client_socket, struct AppController *app_controller);
void split_route(char* route, PathResources *path_resources)
{
    char p[strlen(route) + 1];
    strcpy(p, route);
    char* path = p;
    char *token = strtok(path, "/");
    path_resources->path_counter = 0;
    while(token != NULL){
        path_resources->resources = (char **)realloc(path_resources->resources, (path_resources->path_counter + 2) * strlen(token));

        if(!path_resources->resources)
        {
            perror("[!] Error on reallocating to path resources");
            exit(EXIT_FAILURE);
        }
        path_resources->resources[path_resources->path_counter] = token;
        path_resources->path_counter++;

        token = strtok(NULL,"/");
    }
}

struct AppController *create_app_controller(void)
{
    struct AppController *app_controller = (struct AppController *)malloc(sizeof(struct AppController));
    if (!app_controller)
    {
        perror("[!] Error on creating app controller");
        exit(EXIT_FAILURE);
    }
    app_controller->controllers_c = 0;
    return app_controller;
}

Controller *create_controller(Method method, char *route, Callback *callback)
{
    Controller *controller = (Controller *)malloc(sizeof(Controller));
    if (!controller)
    {
        perror("[!] Error on creating controller");
        exit(EXIT_FAILURE);
    }
    controller->method = method;
    PathResources* path_resources = (PathResources*)malloc(sizeof(PathResources));
    if (!path_resources)
    {
        perror("[!] Error on creating path_resources");
        exit(EXIT_FAILURE);
    }
    char p[strlen(route) + 1];
    strcpy(p, route);

    char *token = strtok(p, "/");
    path_resources->path_counter = 0;
    while(token != NULL){
        path_resources->resources = (char **)realloc(path_resources->resources, (sizeof(char*)));

        if(!path_resources->resources)
        {
            perror("[!] Error on reallocating to path resources");
            exit(EXIT_FAILURE);
        }
        path_resources->resources[path_resources->path_counter] = strdup(token);
        path_resources->path_counter++;

        token = strtok(NULL,"/");
    }

    controller->path_resources = path_resources;

    controller->callback = callback;

    return controller;
}

void add_controller(struct AppController *app_controller, Method method, char *route, Callback *callback)
{
    app_controller->controllers = (Controller **)realloc(app_controller->controllers, (app_controller->controllers_c + 1) * sizeof(Controller *));
    if (!app_controller->controllers)
    {
        fprintf(stderr, "[!] Error on adding controller [%s %s] to the app controller", method, route);
        exit(EXIT_FAILURE);
    }
    Controller *controller = create_controller(method, route, callback);
    app_controller->controllers[app_controller->controllers_c] = controller;
    app_controller->controllers_c++;
}

void *request_handler_t(void *options)
{
    RequestHandlerOptions *request_handler_opts = (RequestHandlerOptions *)options;
    struct AppController *app_controller = request_handler_opts->app_controller;
    int *client_socket = request_handler_opts->client_socket;
    request_handler(client_socket, app_controller);
    return NULL;
}

void request_handler(int *client_socket, struct AppController *app_controller)
{
    if (!app_controller->controllers)
    {
        printf("[<!>] You still didn't declare a route.");
        return;
    }

    char *buffer = (char *)malloc(BUF_SIZE);
    int bytes_read = read(*client_socket, buffer, BUF_SIZE - 1);

    if (bytes_read < 0)
    {
        perror("[!] Error reading from socket");
        close(*client_socket);
        return;
    }

    buffer[bytes_read] = '\0';
    char *token;
    int i = 0;
    char *headers[2];
    while ((token = strsep(&buffer, " ")))
    {
        headers[i] = token;
        i++;
        if (i == 2)
        {
            break;
        }
    }
    token = NULL;
    char *method = headers[0];
    char *p = headers[1];   
    char* path = p;
    PathResources* path_resources = (PathResources*)malloc(sizeof(PathResources));
    if (!path_resources)
    {
        perror("[!] Error on creating path_resources");
        exit(EXIT_FAILURE);
    }

    token = strtok(path, "/");
    path_resources->path_counter = 0;
    while(token != NULL){
        path_resources->resources = (char **)realloc(path_resources->resources, (path_resources->path_counter + 2) * strlen(token));

        if(!path_resources->resources)
        {
            perror("[!] Error on reallocating to path resources");
            exit(EXIT_FAILURE);
        }
        path_resources->resources[path_resources->path_counter] = token;
        path_resources->path_counter++;

        token = strtok(NULL,"/");
    }

    if (method == NULL || path == NULL)
    {
        close(*client_socket);
        return;
    }
    bool path_found, method_found = false;
    RequestURLResources request_resources = (RequestURLResources){.url_resources_counter=0};
    for (int i = 0; i < app_controller->controllers_c; i++)
    {

        if(app_controller->controllers[i]->path_resources->path_counter != path_resources->path_counter)
        {
            continue;
        }

        for(int j = 0; j < path_resources->path_counter; j++)
        { 
            char* resource_param = strstr(app_controller->controllers[i]->path_resources->resources[j],"<:");
            if(resource_param)
            {
                request_resources.url_resources = (URLResource**)realloc(request_resources.url_resources, ((request_resources.url_resources_counter + 1) * sizeof(URLResource*)));
                URLResource *url_resource = (URLResource*)malloc(sizeof(URLResource));
                char* resource = strtok(resource_param,"<:>");
                url_resource->key = resource;
                url_resource->value = path_resources->resources[j];
                request_resources.url_resources[request_resources.url_resources_counter] = url_resource;
                request_resources.url_resources_counter++;
                if(j == (path_resources->path_counter -1))
                {
                    path_found = true;
                }

                continue;
            }

            if(strcmp(app_controller->controllers[i]->path_resources->resources[j], path_resources->resources[j]) != 0)
            {
                break;
            }

            if(j == (path_resources->path_counter -1))
            {
                path_found = true;
            }
        }

        if(path_found)
        {
            if (strcmp((char *)app_controller->controllers[i]->method, method) == 0)
            {
                app_controller->controllers[i]->callback(client_socket, request_resources);
                method_found = true;
            }
        }
    }
    if (client_socket)
    {
        if (!path_found)
        {
            char body[BUF_SIZE];
            char *b = strdup(HTTP_404_MESSAGE);
            sprintf(body, b, path);
            server_response((HTTPResponseOptions){.client_socket = client_socket, .body = body, .status_code = HTTP_STATUS_NOT_FOUND});
        }
        else if (path_found && !method_found)
        {
            server_response((HTTPResponseOptions){.client_socket = client_socket, .body = strdup(HTTP_405_MESSAGE), .status_code = HTTP_STATUS_MethodNotAllowed});
        }
    }
}

void shutdown_server(int signal)
{
    printf("%d", signal);
    printf("[*] Shutting Down");
    exit(EXIT_SUCCESS);
}

void run(struct AppController *app_controller, int port)
{
    if (signal(SIGINT, shutdown_server) == SIG_ERR)
    {
        perror("Signal setting error: ");
        exit(EXIT_FAILURE);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("[!] Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[!] Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, MAX_CONS) < 0)
    {
        perror("[!] Error listening on socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("[*] Server listening on port %d\n", port);

    while (true)
    {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0)
        {
            perror("[!] Error accepting connection");
            continue;
        }

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, request_handler_t, &(RequestHandlerOptions){.client_socket = &client_socket, .app_controller = app_controller}) != 0)
        {
            perror("Error on creating thread for client");
        }
        pthread_detach(client_thread);
    }

    close(server_socket);
    close(client_socket);
}

void response_parser(char *response_cursor, HTTP_HEADERS content_type, HTTP_STATUS status_code, char *body)
{
    sprintf(response_cursor, "%s %s\r\n%s\r\n%s\r\n\r\n%s", HTTP_VERSION, status_code, content_type, HTTP_H_ConnectionClose, body);
}

void make_response(HTTPResponseOptions http_response_options, char* response)
{
    write(*http_response_options.client_socket, response, strlen(response));
    close(*http_response_options.client_socket);
}
void server_response(HTTPResponseOptions http_response_options)
{
    char response[BUF_SIZE];

    response_parser(response, HTTP_H_ContentTypeHTML, http_response_options.status_code, http_response_options.body);


    make_response(http_response_options, response);
}

void server_response_json(HTTPResponseOptions http_response_options)
{
    char response[BUF_SIZE];

    response_parser(response, HTTP_H_ContentTypeJSON, http_response_options.status_code, http_response_options.body);

    make_response(http_response_options, response);
}

char* ParamResource(RequestURLResources request_resources, char *key){
    char* value = NULL;
    for(int i = 0; i < request_resources.url_resources_counter; i++)
    {

        if(strcmp(request_resources.url_resources[i]->key,key) == 0)
        {
            value = strdup(request_resources.url_resources[i]->value);
            break;
        }
    }
    return value;
}
