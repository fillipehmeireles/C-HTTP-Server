
CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Wreturn-type -Wunused-parameter 
INCLUDES_EXAMPLE = -I include/ examples/hello-world/include/cJSON.c -I examples/hello-world/include/
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
SRCDIR_EXAMPLE = examples/hello-world
SRC_EXAMPLE = $(wildcard $(SRCDIR_EXAMPLE)/*.c)
TARGET_EXAMPLE = hello_world_server

example:
	$(CC) $(CFLAGS) $(INCLUDES_EXAMPLE) $(SRC) $(SRC_EXAMPLE) -o bin/$(TARGET_EXAMPLE)
