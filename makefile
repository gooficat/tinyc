CC := gcc
LD := gcc
CC_ARGS := -g -Wall -Wextra -Wpedantic -O0
LD_ARGS :=

TARGET := tinyc

SRC_DIR := src
OBJ_DIR := .obj


SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LD_ARGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CC_ARGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(SRC_DIR)/* $(OBJ_DIR)/*

.PHONY: all clean
