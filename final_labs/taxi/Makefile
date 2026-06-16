CC := gcc
CFLAGS := -Wall -Wextra -I include/ -Werror

TARGET := main
BINDIR := bin/
OBJDIR := obj/
SRCDIR := src/

SRCS := $(wildcard $(SRCDIR)*.c)
OBJ := $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))
BINS := $(patsubst $(SRCDIR)%.c,bin/%,$(SRCS))

all: $(BINDIR)$(TARGET)

$(BINDIR)$(TARGET): $(OBJ) | bin
	$(CC) $(OBJ) -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c | obj
	$(CC) -c $(CFLAGS)  $< -o $@ 

bin:
	mkdir -p bin

obj:
	mkdir -p obj

run: 
	./$(BINDIR)$(TARGET)

clean:
	rm -rf bin obj

print_vars:
	@echo $(SRCS)
	@echo $(OBJ)
	

.PHONY: clean run bin all obj print_vars
	

