CC:=gcc
CFLAGS:=-Wall 
TARGET:=calc
LDFLAGS:= -lcalc
LIB_DIR:=./lib
OBJ_DIR:=./obj
BIN_DIR:=./bin
SRC_DIR:=./source
HDR_DIR:=./include
LIB:=libcalc.a
LIB_FLAG:=-lcalc
SRC:=$(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c)) # main.c должен отдельно компилироваться
HDR:=$(wildcard $(HDR_DIR)/*.h)
OBJ:=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
#OBJ:=$(SRC:.c=.o)

DEBUG:=0
#CFLAGS += $(if $(filter 1,$(DEBUG)),-g, -Werror)
ifeq ($(DEBUG),1)
CFLAGS += -g
endif

all: $(TARGET)

$(TARGET): $(LIB_DIR)/$(LIB) $(OBJ_DIR)/main.o | $(BIN_DIR)
	$(CC) $(OBJ_DIR)/main.o -o $(BIN_DIR)/$@ -L$(LIB_DIR) $(LIB_FLAG) 

$(LIB_DIR)/$(LIB): $(OBJ) | $(LIB_DIR)
	ar rcs $@ $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR) # main.o не следует запихивать в статическую библиотеку
	$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR)/main.o

clean:
	@echo "Cleaning..."
	@rm -rf ./obj ./bin ./lib

$(TARGET): $(HDR)

$(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR):
	@mkdir -p $@

$(LIB_DIR):
	@mkdir -p $@

run: $(TARGET)
	@./bin/calc

.PHONY: clean test all