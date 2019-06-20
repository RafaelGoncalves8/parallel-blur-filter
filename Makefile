# Compilation macros
CC=gcc
CFLAGS=-Wextra -lfreeimage

DIR=src
TESTS=test
TARGET=./main
MAIN=$(DIR)/main.c
BLUR=$(DIR)/blur.c
IMG_PROC=$(DIR)/imageprocessing.c

all:$(BLUR) $(IMG_PROC)
	$(CC) -o$(TARGET) $(MAIN) $(BLUR) $(IMG_PROC) $(CFLAGS)
