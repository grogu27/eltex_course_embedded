CC := gcc
CFLAGS := -Wall -Wextra -Werror  
LDFLAGS:= -pthread

TARGET:= main
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(OBJ)

print_vars:
	@echo $(OBJ)
	@echo $(SRC)

.PHONY: all run clean print_vars
