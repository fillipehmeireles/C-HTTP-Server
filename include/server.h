#ifndef server__h
#define server__h

#define MAX_CONS 10
#define BUF_SIZE 1024

typedef const char* ContentType;
static const ContentType ContentTypeHTML = "Content-Type: text/html";
typedef const char* Method;
typedef const char* HTTP_Response_Status ;
static const Method GET = "GET";

static const HTTP_Response_Status HTTP_Status_OK_R = "200 OK";
static const HTTP_Response_Status HTTP_Status_NotFound_R = "200 OK";
static const char* HTTP_VERSION = "HTTP/1.1";

typedef void Callback;
typedef struct Controller {
    char *route;
    Method method;
    void(*Callback)(int*);
} Controller;


struct AppController {
    Controller **controllers;
    int controllers_c;
};

struct AppController* create_app_controller();
typedef struct HTTP_request {
    char* method;
    char* path;
} HTTP_request;

typedef struct HTTP_response{
    char* http_version;
    HTTP_Response_Status http_status;
    char* content_type;
    char* connection_state;
    char* body;
} HTTP_response;

void add_controller(struct AppController *app_controller,Method method, char *route, Callback *callback);
void connection_handler (int*);
void run(struct AppController*, int);
#endif
