CC:=gcc
CFLAGS:= -MMD -MP
TARGET_CLIENT:=client
TARGET_SERVER:=server
HEADERS:=$(wildcard *.h)

SRC_CLIENT:=$(wildcard *client.c)
SRC_CLIENT+=shm.c
OBJ_CLIENT:=$(SRC_CLIENT:.c=.o)
SRC_SERVER:=$(wildcard *server.c)
SRC_SERVER+=shm.c
OBJ_SERVER:=$(SRC_SERVER:.c=.o)
DEP_CLIENT := $(OBJ_CLIENT:.o=.d)
DEP_SERVER := $(OBJ_SERVER:.o=.d)

all: $(TARGET_CLIENT) $(TARGET_SERVER) 

$(TARGET_CLIENT): $(OBJ_CLIENT) 
	$(CC) $^ -o $@ 

$(TARGET_SERVER): $(OBJ_SERVER) 
	$(CC) $^ -o $@ 

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

print_vars:
	@echo SRC_CLIENT = $(SRC_CLIENT)
	@echo OBJ_CLIENT = $(OBJ_CLIENT)
	@echo SRC_SERVER = $(SRC_SERVER)
	@echo OBJ_SERVER = $(OBJ_SERVER)

clean:
	rm -rf $(OBJ_SERVER) $(OBJ_CLIENT) $(TARGET_CLIENT) $(TARGET_SERVER)  $(DEP_CLIENT) $(DEP_SERVER)

.PHONY: print_vars clean all

-include $(DEP_CLIENT) $(DEP_SERVER)