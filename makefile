# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Target executable
TARGET = fetch-info

# Source files
SRCS = fetch-info.c

# Default rule: Automatically rebuild if source changes
all: $(TARGET)

# Compile fetch-info only if fetch-info.c has changed
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Run the program without arguments
run: $(TARGET)
	./$(TARGET)

# Run the program with a PID argument
run-pid: $(TARGET)
	./$(TARGET) $(PID)

# Force rebuild if needed
rebuild:
	$(MAKE) clean
	$(MAKE) all

# Clean up the compiled files
clean:
	rm -f $(TARGET)