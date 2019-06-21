# Compilation macros
CC=gcc
CFLAGS=-Wextra -lfreeimage

SRC=src
OUT=out
TESTS=test
TARGET=./main
MAIN=$(SRC)/main.c
BLUR=$(SRC)/blur.c
IMG_PROC=$(SRC)/imageprocessing.c

all:$(BLUR) $(IMG_PROC) $(MAIN)
	$(CC) -o$(TARGET) $(MAIN) $(BLUR) $(IMG_PROC) $(CFLAGS)
	
clean:
	$(RM) $(TARGET)
	$(RM) $(OUT)/*

