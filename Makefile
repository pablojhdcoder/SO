# Variables
CC = gcc                                  # Compilador
CFLAGS = -g -Wall -Wextra                 # Flags para el compilador
SRC = main.c Functions.c Commands.c FileList.c HistoryList.c CommandList.c MemoryBlockList.c ProcessesList.c DirectoryList.c # Archivos fuente
OBJ = $(SRC:.c=.o)                        # Archivos objeto
TARGET = p3                               # Nombre del ejecutable

# Regla por defecto: compilar todo
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para crear los archivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencias entre los archivos fuente y sus cabeceras
main.o: main.c Functions.h Commands.h HistoryList.h FileList.h CommandList.h MemoryBlockList.h
Functions.o: Functions.c Functions.h
HistoryList.o: HistoryList.c HistoryList.h
FileList.o: FileList.c FileList.h
CommandList.o: CommandList.c CommandList.h
Commands.o: Commands.c Commands.h
MemoryBlockList.o: MemoryBlockList.c MemoryBlockList.h
ProcessesList.o: ProcessesList.c ProcessesList.h
DirectoryList.o: DirectoryList.c DirectoryList.h



# Regla para mostrar ayuda
help:
	@echo "Makefile para compilar el programa."
	@echo "Comandos disponibles:"
	@echo "  make           Compila el programa."
	@echo "  make clean     Elimina los archivos objeto y el ejecutable."
	@echo "  make help      Muestra esta ayuda."

# Regla para limpiar archivos generados
clean:
	rm -f $(OBJ) $(TARGET)

# Definir reglas "phony" (que no corresponden a archivos)
.PHONY: all clean help
