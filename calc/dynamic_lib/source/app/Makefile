.PHONY: all clean

CC ?= gcc
CFLAGS ?= -Wall -Werror

SRC := $(wildcard *.c)
OBJ := ../../obj/app/main.o

BIN_DIR := ../../bin
OBJ_DIR := ../../obj/app
LIB_DIR := ../../lib

ifeq ($(DEBUG), 1)
CFLAGS+=-g
endif

TARGET := $(BIN_DIR)/calc

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) -o $@ -L$(LIB_DIR) -lcalc

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(TARGET)