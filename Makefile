CC=g++
CFLAGS=-std=c++11 -Wall
GTEST_FLAGS=-lgtest -lgtest_main -pthread

COMMON_DIR=./common
SERVER_DIR=./server/src
CLIENT_DIR=./client/src
TEST_DIR=./tests

SERVER_BIN=./out/server
CLIENT_BIN=./out/client
COMMON_TEST_BIN=./out/test_common
SERVER_TEST_BIN=./out/test_server
CLIENT_TEST_BIN=./out/test_client

COMMON_SRC=$(shell find $(COMMON_DIR) -name '*.cpp')
SERVER_SRC=$(shell find $(SERVER_DIR) -name '*.cpp') $(COMMON_SRC)
CLIENT_SRC=$(shell find $(CLIENT_DIR) -name '*.cpp') $(COMMON_SRC)

COMMON_TEST_SRC=$(shell find $(TEST_DIR)/common -name '*.cpp') $(COMMON_SRC)
SERVER_TEST_SRC=$(shell find $(TEST_DIR)/server -name '*.cpp') $(filter-out ./server/src/main.cpp, $(SERVER_SRC))
CLIENT_TEST_SRC=$(shell find $(TEST_DIR)/client -name '*.cpp') $(filter-out ./client/src/main.cpp, $(CLIENT_SRC))

all: $(SERVER_BIN) $(CLIENT_BIN) common_test server_test client_test

$(SERVER_BIN): $(SERVER_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(SERVER_SRC) -o $(SERVER_BIN)

$(CLIENT_BIN): $(CLIENT_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o $(CLIENT_BIN)

common_test: $(COMMON_TEST_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(COMMON_TEST_SRC) -o $(COMMON_TEST_BIN) $(GTEST_FLAGS)

server_test: $(SERVER_TEST_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(SERVER_TEST_SRC) -o $(SERVER_TEST_BIN) $(GTEST_FLAGS)

client_test: $(CLIENT_TEST_SRC)
	mkdir -p out
	$(CC) $(CFLAGS) $(CLIENT_TEST_SRC) -o $(CLIENT_TEST_BIN) $(GTEST_FLAGS)

clean:
	rm -rf out

.PHONY: all common_test server_test client_test clean
