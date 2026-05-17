CC:=gcc
CFLAGS:=-Wall
TARGET:=prog

SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $< -o $@

%.o: %.C
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

.PHONY: all clean