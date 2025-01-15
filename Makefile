CC = gcc
CFLAGS = -Wall -Iinclude -fexec-charset=GBK 

SRC = src/main.c src/frequency.c src/huffman.c src/encode.c src/decode.c src/linked_list.c src/verify.c src/match.c src/word_count.c src/fuzzy_match.c src/generate.c src/read.c
OBJ = $(SRC:.c=.o)

TARGET = huffman

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lcurl -ljson-c

clean:
	rm -f $(OBJ) $(TARGET)
