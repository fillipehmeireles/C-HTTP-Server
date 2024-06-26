all:
	clang -Wall -I include/ include/cJSON.c src/server.c src/main.c -o bin/chttp-server
