

CC:=gcc
CFLAGS:=-Wall -Werror -MMD -MP
DEBUG:=0
BIN_DIR:=./bin
OBJ_DIR:=./obj
SRC_DIR:=./source
HDR_DIR:=./include
TARGET:=$(BIN_DIR)/tel_dir

SRC:=$(wildcard $(SRC_DIR)/*.c)
OBJ:=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP := $(OBJ:.o=.d)

ifeq ($(DEBUG), 1)
CFLAGS+=-g
endif

.PHONY: print_vars clean all run

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@ -I$(HDR_DIR) 

$(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)

print_vars:
	@echo "Исходники: $(SRC)"
	@echo "Объекты: $(OBJ)"
	@echo "Ключи компиляции: $(CFLAGS)"

run: all
	$(TARGET)

-include $(DEP)
