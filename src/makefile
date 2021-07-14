CC = gcc

K88: main.o processor.o readwrite.o utils.o
	$(CC) main.o processor.o readwrite.o utils.o -o K88

main.o: main.c
	$(CC) -c main.c

processor.o: processor.c
	$(CC) -c processor.c

readwrite.o: readwrite.c
	$(CC) -c readwrite.c

utils.o: utils.c
	$(CC) -c utils.c

clean:
	rm *.o K88