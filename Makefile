CC= gcc
CFLAGS= -Wall -g -I.
TARGET= main
CFILES= main.c registros.c funcionalidades.c

all :
	$(CC) $(CFLAGS) $(CFILES) -o $(TARGET)

run :
	./$(TARGET)

val :
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET) < test.in

hex :
	hexdump -Cv tec.bin