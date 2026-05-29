CC:=gcc
CFLAGS:=-Wall -Werror
TARGET_CLIENT:=client
TARGET_SERVER:=server


SRC_CLIENT:=$(wildcard *client.c)
OBJ_CLIENT:=$(SRC_CLIENT:.c=.o)
SRC_SERVER:=$(wildcard *server.c)
OBJ_SERVER:=$(SRC_SERVER:.c=.o)


all: $(TARGET_CLIENT) $(TARGET_SERVER)

$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CC) $< -o $@

$(TARGET_SERVER): $(OBJ_SERVER)
	$(CC) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

print_vars:
	@echo SRC_CLIENT = $(SRC_CLIENT)
	@echo OBJ_CLIENT = $(OBJ_CLIENT)
	@echo SRC_SERVER = $(SRC_SERVER)
	@echo OBJ_SERVER = $(OBJ_SERVER)

clean:
	rm -rf $(OBJ_SERVER) $(OBJ_CLIENT) $(TARGET_CLIENT) $(TARGET_SERVER) 

.PHONY: print_vars clean all

	