CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2

# Target executable
TARGET = fetch-info

# Source files
SRCS = fetch-info.c

# Compile with 'make fetch-info' to produce the executable
fetch-info: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Run the program without arguments
run: fetch-info
	./$(TARGET)

# Run the program with a PID argument
run-pid: fetch-info
	./$(TARGET) $(PID)

# Clean up the compiled files
clean:
	rm -f $(TARGET)