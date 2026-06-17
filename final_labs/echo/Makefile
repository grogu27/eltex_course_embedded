CC := gcc
CFLAGS := -Wall -Werror -I include/
LDFLAGS :=

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
INCLUDE_DIR := include

TARGET_CLIENT := $(BIN_DIR)/client
TARGET_SERVER := $(BIN_DIR)/server

SRC := $(wildcard $(SRC_DIR)/*.c)

SRC_CLIENT := $(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/server_main.c, $(SRC))
SRC_SERVER := $(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/client_main.c, $(SRC))

OBJ_CLIENT := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_CLIENT))
OBJ_SERVER := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_SERVER))

DIRS := $(OBJ_DIR) $(BIN_DIR)

all: $(DIRS) $(TARGET_CLIENT) $(TARGET_SERVER)

$(DIRS):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET_CLIENT): $(OBJ_CLIENT) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TARGET_SERVER): $(OBJ_SERVER) | $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

print_vars:
	@echo "SRC = $(SRC)"
	@echo "SRC_CLIENT = $(SRC_CLIENT)"
	@echo "OBJ_CLIENT = $(OBJ_CLIENT)"
	@echo "SRC_SERVER = $(SRC_SERVER)"
	@echo "OBJ_SERVER = $(OBJ_SERVER)"
	@echo "DIRS = $(DIRS)"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean print_vars