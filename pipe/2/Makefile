CC:=gcc
CFLAGS:=-Wall
TARGET_SERVER:=server
TARGET_CLIENT:=client

SRC_SERVER:=$(wildcard *server*.c)
OBJ_SERVER:=$(SRC_SERVER:.c=.o)
SRC_CLIENT:=$(wildcard *client*.c)
OBJ_CLIENT:=$(SRC_CLIENT:.c=.o)
TMP_FILE:=$(wildcard *pipe*)

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(TARGET_SERVER): $(OBJ_SERVER)
	$(CC) $< -o $@

$(TARGET_CLIENT): $(OBJ_CLIENT)
	$(CC) $< -o $@

%.o: %.C
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ_CLIENT) $(OBJ_SERVER) $(TARGET_SERVER) $(TARGET_CLIENT) $(TMP_FILE)

.PHONY: all clean