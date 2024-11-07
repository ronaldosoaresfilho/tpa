# Dependências: jansson, gtk4

# Nome do executável
TARGET = tpa

# Diretórios
SRC_DIR = ./app/src
BUILD_DIR = ./build
DATA_DIR = ./app/data
INCLUDE_DIR = $(SRC_DIR)

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -g -Wextra -I$(INCLUDE_DIR) $(shell pkg-config --cflags gtk4)  

# Arquivos fonte
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

# Regra padrão
all: $(BUILD_DIR) $(TARGET)

# Cria o diretório build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compila os arquivos .c em objetos .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Gera o executável
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(shell pkg-config --libs gtk4) -ljansson -lm

# Limpa os arquivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
