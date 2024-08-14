#Compiler and flag
CC = gcc
CFLAGS = -Wall -Wextra -fsanitize=address

# Source files
SRCS = spchk.c comparison.c sb.c

# Object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = spchk

# Default target
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)