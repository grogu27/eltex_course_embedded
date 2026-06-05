CC := gcc
CFLAGS := -Wall -Wextra -Werror -I include/
LDFLAGS := -lncurses

TARGET2 := my_mc
TARGET1 := part1
BINDIR := bin/
SRCDIR := src/
OBJDIR := obj/

SRCS := $(wildcard $(SRCDIR)*.c)
SRCS := $(filter-out $(SRCDIR)part1.c, $(SRCS))
OBJ := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRCS))

all: $(BINDIR)$(TARGET1) $(BINDIR)$(TARGET2)

$(BINDIR)$(TARGET1): $(SRCDIR)part1.c | $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@

$(BINDIR)$(TARGET2): $(OBJ) | $(BINDIR)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) 

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

run-part1: $(BINDIR)$(TARGET1)
	./$<

run-my_mc: $(BINDIR)$(TARGET2)
	./$<

run: run-my_mc

clean:
	rm -rf $(BINDIR) $(OBJDIR)

printf_vars:
	@echo "OBJ: $(OBJ)"
	@echo "SRCS: $(SRCS)"

.PHONY: all clean run run-part1 run-my_mc printf_vars