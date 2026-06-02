CC:=gcc
CFLAGS:=-Wall -Werror
TARGET_CLIENT:=client

SRC_CLIENT:=$(wildcard *client.c)
OBJ_CLIENT:=$(SRC_CLIENT:.c=.o)



all: $(TARGET_CLIENT) 

$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CC) $^ -o $@



%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@



clean:
	rm -rf  $(OBJ_CLIENT) $(TARGET_CLIENT) 
	
.PHONY: clean all

	