
CC = clang
CFLAGS = -Wall -Wextra -pedantic -std=c11
INCLUDES = -I include/ include/cJSON.c 
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
SRCDIR_EXAMPLE = examples/hello-world
SRC_EXAMPLE = $(wildcard $(SRCDIR_EXAMPLE)/*.c)
TARGET_EXAMPLE = hello_world_server

example:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRC) $(SRC_EXAMPLE) -o bin/$(TARGET_EXAMPLE)
