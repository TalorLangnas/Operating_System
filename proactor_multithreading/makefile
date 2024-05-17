# Compiler
CC := gcc

# Compiler flags
# CFLAGS := -Wall -Wextra -g -pthread -fPIC
CFLAGS := -pthread -fPIC

# Source directories
SRC_DIR_B := B
SRC_DIR_C := C

# Object directories
OBJ_DIR_B := B
OBJ_DIR_C := C

# Source files
SRC_B := $(wildcard $(SRC_DIR_B)/*.c)
SRC_C := $(wildcard $(SRC_DIR_C)/*.c)

# Object files
OBJ_B := $(patsubst $(SRC_DIR_B)/%.c,$(OBJ_DIR_B)/%.o,$(SRC_B))

# Target files
TARGET_B := $(OBJ_DIR_B)/libproactor.so
TARGET_C := $(OBJ_DIR_C)/proactor_server

# Default target
all: $(TARGET_B) $(TARGET_C) activate_A

# Target: libproactor.so
$(TARGET_B): $(OBJ_B)
	$(CC) -shared -o $@ $^

# Pattern rule to build object files for B directory
$(OBJ_DIR_B)/%.o: $(SRC_DIR_B)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Target: proactor_server
$(TARGET_C): $(SRC_DIR_C)/proactor_server.c $(TARGET_B)
	$(CC) $(CFLAGS) -o $@ $< -I$(SRC_DIR_B) -L$(OBJ_DIR_B) -lproactor -pthread -Wl,-rpath=$(realpath $(OBJ_DIR_B))

# Target: activate_A
activate_A:
	$(MAKE) -C A

# Clean rule
clean:
	$(MAKE) -C A clean
	rm -f $(OBJ_DIR_B)/*.o $(TARGET_B) $(OBJ_DIR_C)/*.o $(TARGET_C)
