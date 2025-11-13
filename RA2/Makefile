# nome do executável final
TARGET = P2

# Lista todos os arquivos-fonte (.c)
# Adicione ou remova arquivos .c conforme necessário 
SRC_FILES = source/P2.c \
            source/classifier.c \
            source/file_reader.c \
            source/p2_implementation.c \
			source/formatter.c \
			

OBJ_FILES = $(patsubst %.c, %.o, $(SRC_FILES))

# Flags do Compilador (GCC)
# -Wall: Habilita todos os warnings (boa prática)
# -Wextra: Habilita warnings extras
# -std=c99: Usa o padrão C99
# -g: Inclui informações para o GDB (debugging)
# -I: Especifica o diretório dos arquivos de cabeçalho (.h)
CFLAGS = -Wall -Wextra -std=c99 -g -Iinclude

# Regra principal (chamada ao digitar 'make')
all: $(TARGET)

# Regra para linkar os arquivos objeto e criar o executável final
$(TARGET): $(OBJ_FILES)
	@echo "🔗 Linking object files to create $(TARGET)..."
	$(CC) $(CFLAGS) $^ -o $@

# Regra genérica para compilar cada arquivo .c em um arquivo objeto .o
# flag -c de (COMPILE ONLY)
# flag -o de (OUTPUT)
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos gerados (chamada ao digitar 'make clean')
clean:
	@echo "🧹 Cleaning up generated files..."
	$(RM) $(OBJ_FILES) $(TARGET)
	$(RM) dados.bin

.PHONY: all clean