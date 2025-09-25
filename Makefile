# Nome do executável
PROG = output/brkga.exe

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -Iinclude

# Arquivos objeto
OBJS = main.o brkga_functions.o aux_functions.o debug.o

# Regra principal
$(PROG): $(OBJS)
	@mkdir -p output
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

# Compilação dos objetos
main.o: src/main.c include/brkga.h
	$(CC) $(CFLAGS) -c src/main.c -o main.o

brkga_functions.o: src/brkga_functions.c include/brkga.h
	$(CC) $(CFLAGS) -c src/brkga_functions.c -o brkga_functions.o

aux_functions.o: src/aux_functions.c include/brkga.h
	$(CC) $(CFLAGS) -c src/aux_functions.c -o aux_functions.o

debug.o: src/debug.c include/brkga.h
	$(CC) $(CFLAGS) -c src/debug.c -o debug.o

# Limpeza
clean:
	rm -f *.o $(PROG)
