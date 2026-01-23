# libmslog/Makefile
CC = gcc
CFLAGS = -Wall -O2 -I./inc -pthread
SRC_DIR = ./src
INC_DIR = ./inc
LIB_DIR = ./lib

SRC_FILES = $(SRC_DIR)/mslog.c \
            $(SRC_DIR)/mslog_mem_pool.c \
            $(SRC_DIR)/mslog_thread.c \
            $(SRC_DIR)/mslog_utils.c

OBJ_FILES = $(SRC_FILES:.c=.o)

LIB_NAME = $(LIB_DIR)/libmslog.a


all: $(LIB_NAME)


$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


$(LIB_NAME): $(OBJ_FILES)
	mkdir -p $(LIB_DIR)
	ar rcs $@ $(OBJ_FILES)


clean:
	rm -f $(OBJ_FILES)
	rm -rf $(LIB_DIR)
