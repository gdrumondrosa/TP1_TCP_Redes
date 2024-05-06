CC = gcc
CFLAGS = -lm
BIN_DIR = bin
CLIENT_TARGET = $(BIN_DIR)/client
SERVER_TARGET = $(BIN_DIR)/server

.PHONY: all clean

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): cliente.c | $(BIN_DIR)
	$(CC) $(CFLAGS) cliente.c -o $(CLIENT_TARGET)

$(SERVER_TARGET): servidor.c | $(BIN_DIR)
	$(CC) $(CFLAGS) servidor.c -o $(SERVER_TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

