#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include "include/cJSON.h"

void hello(int *client_socket, RequestURLResources request_resources)
{
    HTTPResponseOptions http_response_options = {.client_socket = client_socket, .body = "<h1>Hello World, C!</h1>", .status_code = HTTP_STATUS_OK};
    server_response(http_response_options);
}

void hello2post(int *client_socket, RequestURLResources request_resources)
{
    cJSON *json = cJSON_CreateObject(); 
    cJSON_AddStringToObject(json, "name", "John Doe"); 
    cJSON_AddNumberToObject(json, "age", 30); 
    cJSON_AddStringToObject(json, "email", "john.doe@example.com"); 

    char *json_str = cJSON_Print(json);  
    HTTPResponseOptions http_response_options = {.client_socket = client_socket, .body = json_str, .status_code = HTTP_STATUS_OK};
    server_response_json(http_response_options);
}

void hello_w_resource(int *client_socket, RequestURLResources request_resources)
{   
    char* resource = ParamResource(request_resources,"id");
    char body[BUF_SIZE];
    char *b = "<h1> the resource is %s </h1>";
    sprintf(body, b, resource);

    HTTPResponseOptions http_response_options = {.client_socket = client_socket, .body = body , .status_code = HTTP_STATUS_OK};
    server_response(http_response_options);
}

void hello_w_2resource(int *client_socket, RequestURLResources request_resources)
{   
    char* id = ParamResource(request_resources,"id");
    char* uuid = ParamResource(request_resources,"uuid");
    char body[BUF_SIZE];
    char *b = "<h1> the resource id is %s and uuid is %s  </h1>";
    sprintf(body, b, id, uuid);

    HTTPResponseOptions http_response_options = {.client_socket = client_socket, .body = body , .status_code = HTTP_STATUS_OK};
    server_response(http_response_options);
}


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please provide a port. \nUsage: hello_world_server <PORT>");
        exit(EXIT_FAILURE);
    }
    int port = atoi(argv[1]);
    struct AppController *app_controller = create_app_controller();
    add_controller(app_controller, GET, "/hello", &hello);
    add_controller(app_controller, POST, "/hello", &hello2post);
    add_controller(app_controller, GET, "/hello/<:id>", &hello_w_resource);
    add_controller(app_controller, GET, "/hello/<:id>/world/<:uuid>", &hello_w_2resource);
    run(app_controller, port);
    return EXIT_SUCCESS;
}
