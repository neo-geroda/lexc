# Compiler to use
CC = gcc

# Compiler flags:
# -Iinclude tells the compiler to look inside the 'include' folder for .h files
# -g adds debugging info (for gdb)
# -Wall turns on warnings
CFLAGS = -Iinclude -g -Wall

# The final executable name
TARGET = compiler

# Source files
SRCS = src/main.c src/lexer.c src/parser.c src/semantic.c src/symbol_table.c src/ast.c

# Object files (auto-generated from SRCS)
OBJS = $(SRCS:.c=.o)

# Main build target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f src/*.o $(TARGET)