CC=g++
CFLAGS=-std=c++11 -Wall

# Define the directories.
COMMON_DIR=./common
SERVER_DIR=./server/src
CLIENT_DIR=./client/src

# Define the output binaries.
SERVER_BIN=./out/server
CLIENT_BIN=./out/client

# Define the source files.
COMMON_SRC=$(shell find $(COMMON_DIR) -name '*.cpp')
SERVER_SRC=$(shell find $(SERVER_DIR) -name '*.cpp') $(COMMON_SRC)
CLIENT_SRC=$(shell find $(CLIENT_DIR) -name '*.cpp') $(COMMON_SRC)

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN)

clean:
	rm -rf out

.PHONY: all clean
