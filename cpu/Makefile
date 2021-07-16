CC = gcc

K88: cpu.o processor.o readwrite.o utils.o
	$(CC) cpu.o processor.o readwrite.o utils.o -o K88

cpu.o: cpu.c
	$(CC) -c cpu.c

processor.o: processor.c
	$(CC) -c processor.c

readwrite.o: readwrite.c
	$(CC) -c readwrite.c

utils.o: utils.c
	$(CC) -c utils.c

clean:
	rm *.o K88