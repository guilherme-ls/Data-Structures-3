CC= gcc
CFLAGS= -Wall -g -I.
TARGET= main
CFILES= main.c registros.c funcionalidades.c funcoesAuxiliares.c funcoesFornecidas.c

all :
	$(CC) $(CFLAGS) $(CFILES) -o $(TARGET)

run :
	./$(TARGET)

valin :
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET) < TestCases/9.in > TestCases/9.out2

valout :
	valgrind --leak-check=full --show-leak-kinds=all -s ./$(TARGET) > test.out

# pode trocar os numeros para diferentes testes
test :
	for number in `seq 7 8` ; do \
	./$(TARGET) < TestCases/$$number.in > TestCases/$$number.out2; \
	diff -s TestCases/$$number.out TestCases/$$number.out2; \
	done

hex :
	hexdump -Cv tec.bin

dif :
	diff -s TestCases/5.out TestCases/5.out2