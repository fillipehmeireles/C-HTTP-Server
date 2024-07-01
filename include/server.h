#ifndef server__h
#define server__h

#define MAX_CONS 100
#define BUF_SIZE 1024

static const char* HTTP_404_MESSAGE = "<head> <title>404 Not Found</title> </head><body><h1>Not Found</h1><p>The requested URL %s was not found on this server.</p></body></html>";
static const char* HTTP_405_MESSAGE = "<!DOCTYPE html><html><head><title>405 Method Not Allowed</title></head><body><h1>Method Not Allowed</h1><p>The requested method is not allowed for the URL.</p></body></html>";

typedef const char* HTTP_STATUS;
static const HTTP_STATUS HTTP_STATUS_OK ="200 OK";
static const HTTP_STATUS HTTP_STATUS_NOT_FOUND = "404 Not Found";
static const HTTP_STATUS HTTP_STATUS_MethodNotAllowed = "405 Method Not Allowed";

typedef const char* HTTP_HEADERS;
static const HTTP_HEADERS HTTP_VERSION = "HTTP/1.1";
static const HTTP_HEADERS HTTP_H_ContentTypeHTML = "Content-Type: text/html";
static const HTTP_HEADERS HTTP_H_ContentTypeJSON = "Content-Type: text/json";
static const HTTP_HEADERS HTTP_H_ConnectionClose = "Connection: close";

typedef const char* Method;
static const Method GET = "GET";

typedef struct HTTPResponseOptions{
    int *client_socket;
    char *body;
    HTTP_STATUS status_code; 
} HTTPResponseOptions;

typedef void(Callback)(int*);
typedef struct Controller {
    char *route;
    Method method;
    Callback *callback;
} Controller;

struct AppController {
    Controller **controllers;
    int controllers_c;
};

struct AppController* create_app_controller(void);

typedef struct RequestHandlerOptions{
    int* client_socket;
    struct AppController* app_controller;    
} RequestHandlerOptions;

typedef struct HTTP_request {
    char* method;
    char* path;
} HTTP_request;

void add_controller(struct AppController *app_controller,Method method, char *route, Callback *callback);
void connection_handler (int*);
void run(struct AppController*, int);

void server_response(HTTPResponseOptions http_response_options);
void server_response_json(HTTPResponseOptions http_response_options);
#endif
