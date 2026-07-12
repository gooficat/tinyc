CC := gcc
LD := ld
CC_ARGS := -g -Wall -Wextra -Wpedantic -O0
LD_ARGS := -lc

TARGET := tinykern.bin

SRC_DIR := src
OBJ_DIR := obj


SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LD_ARGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CC_ARGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)


.PHONY: all clean
