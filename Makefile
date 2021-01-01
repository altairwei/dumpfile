CC = gcc

dumpfile:
	${CC} src/dumpfile.c -o dumpfile

clean:
	rm dumpfile