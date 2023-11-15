CC=g++
CFLAGS=-std=c++11 -Wall

# Define the directories.
SERVER_DIR=./server/src
CLIENT_DIR=./client/src

# Define the output binaries.
SERVER_BIN=./out/server
CLIENT_BIN=./out/client

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN):
	mkdir -p out
	$(CC) $(CFLAGS) $(SERVER_DIR)/main.cpp -o $(SERVER_BIN)

$(CLIENT_BIN):
	mkdir -p out
	$(CC) $(CFLAGS) $(CLIENT_DIR)/main.cpp -o $(CLIENT_BIN)

clean:
	rm -rf out

.PHONY: all clean
