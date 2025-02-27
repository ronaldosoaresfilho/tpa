# Nome do executável
TARGET = tpa

# Diretórios
SRC_DIR = ./app/src
BUILD_DIR = ./build
LIB_DIR = ./app/lib
APP_DIR = ./app
INCLUDE_DIR = $(APP_DIR) 

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -g -Wextra -I$(INCLUDE_DIR)

# Arquivos fonte
SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/*.c)

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

$(BUILD_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Gera o executável
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ -lm #-lreadline

# Limpa os arquivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
