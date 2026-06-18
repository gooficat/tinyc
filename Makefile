CC := tcc
CC_FLAGS := -g -O0

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

TARGET := tinyc

EXEC_SUFFIX := .exe

all: $(TARGET)

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

$(TARGET): $(OBJS)
	$(CC) $(CC_FLAGS) $(OBJ_DIR)/*.o -o $(BIN_DIR)/$(TARGET)$(EXEC_SUFFIX)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CC_FLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/*$(EXEC_SUFFIX)
