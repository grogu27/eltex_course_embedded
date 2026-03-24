.PHONY: all clean

CC ?= gcc
CFLAGS ?= -Wall -Werror -fPIC
DEBUG ?= 0

SRC := $(wildcard *.c)
OBJ_DIR := ../../obj/lib
LIB_DIR := ../../lib

OBJ := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC))
LIB := $(LIB_DIR)/libcalc.so

ifeq ($(DEBUG),1)
CFLAGS += -g
endif

all: $(LIB)

$(LIB): $(OBJ) | $(LIB_DIR)
	$(CC) -shared $^ -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR) $(LIB)