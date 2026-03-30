CFLAGS = -Wall -Wextra -O0 -fPIC -Iinclude
LIBNAME = libmesh.a
CC = gcc

# --- Primeira Biblioteca Externa ---
LIB_ANTERIOR_PATH = /c/Users/joaop/OneDrive/Desktop/Drive_Sync/Iniciacao_Cientifica/Codigos/GitHub/codigos/tcam/linalg
LIB_ANTERIOR_NAME = linalg

# --- Segunda Biblioteca Externa (Substitua pelos dados reais) ---
LIB_NOVA_PATH = /c/Users/joaop/OneDrive/Desktop/Drive_Sync/Iniciacao_Cientifica/Codigos/GitHub/codigos/tcam/mmio
LIB_NOVA_NAME = mmio

# **Adiciona o caminho dos headers de todas as bibliotecas na compilação**
CFLAGS += -I$(LIB_ANTERIOR_PATH) -I$(LIB_NOVA_PATH)

# Adiciona o caminho de todas as bibliotecas externas em LDFLAGS
LDFLAGS = -L$(LIB_ANTERIOR_PATH) -L$(LIB_NOVA_PATH)

# Adiciona a inclusão de todas as bibliotecas externas em LIBS
LIBS = -l$(LIB_ANTERIOR_NAME) -l$(LIB_NOVA_NAME)

SRC = mesh.c
OBJ = $(SRC:.c=.o)

# Regra padrão
all: $(LIBNAME)

# Cria a biblioteca estática (.a)
$(LIBNAME): $(OBJ)
	ar rcs $@ $^

# Compila cada .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa arquivos gerados
clean:
	rm -f $(OBJ) $(LIBNAME)