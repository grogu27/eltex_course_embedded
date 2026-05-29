CC:=gcc
CFLAGS:=-Wall -Werror
TARGET_CLIENT:=client
TARGET_SERVER1:=proc_server
TARGET_SERVER2:=thread_server

SRC_CLIENT:=$(wildcard *client.c)
OBJ_CLIENT:=$(SRC_CLIENT:.c=.o)
SRC_SERVER1:=$(wildcard *multiprocess*server.c)
SRC_SERVER2:=$(wildcard *multithread*server.c)

OBJ_SERVER1:=$(SRC_SERVER1:.c=.o)
OBJ_SERVER2:=$(SRC_SERVER2:.c=.o)


all: $(TARGET_CLIENT) $(TARGET_SERVER1) $(TARGET_SERVER2)

$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CC) $< -o $@

$(TARGET_SERVER1): $(OBJ_SERVER1)
	$(CC) $< -o $@

$(TARGET_SERVER2): $(OBJ_SERVER2)
	$(CC) $< -o $@

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_SERVER1) $(OBJ_SERVER2) $(OBJ_CLIENT) $(TARGET_CLIENT) $(TARGET_SERVER1) $(TARGET_SERVER2)  

.PHONY: print_vars clean all

	