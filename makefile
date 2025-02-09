# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Target executables
TARGET_FETCH = fetch-info
TARGET_PIPE = pipe4

# Source files
SRCS_FETCH = fetch-info.c
SRCS_PIPE = pipe4.c

# Default rule: Build both executables
all: $(TARGET_FETCH) $(TARGET_PIPE)

# Compile fetch-info only if fetch-info.c has changed
$(TARGET_FETCH): $(SRCS_FETCH)
	$(CC) $(CFLAGS) $(SRCS_FETCH) -o $(TARGET_FETCH)

# Compile pipe4 only if pipe4.c has changed
$(TARGET_PIPE): $(SRCS_PIPE)
	$(CC) $(CFLAGS) $(SRCS_PIPE) -o $(TARGET_PIPE)

# Run fetch-info without arguments
run-fetch: $(TARGET_FETCH)
	./$(TARGET_FETCH)

# Run fetch-info with a PID argument
run-fetch-pid: $(TARGET_FETCH)
	./$(TARGET_FETCH) $(PID)

# Run pipe4 without arguments
run-pipe: $(TARGET_PIPE)
	./$(TARGET_PIPE)

# Run pipe4 with arguments (if needed)
run-pipe-args: $(TARGET_PIPE)
	./$(TARGET_PIPE) $(ARGS)

# Force rebuild of both programs
rebuild:
	$(MAKE) clean
	$(MAKE) all

# Clean up the compiled files
clean:
	rm -f $(TARGET_FETCH) $(TARGET_PIPE)
