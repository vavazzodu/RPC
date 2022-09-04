CC = gcc
CFLAGS = -Wall -g -Wextra -Werror

target: exe

exe: main.o lrpc.a
	$(CC) $(CFLAGS) main.o -o exe -L ./Library/ -lrpc
main.o: main.c
	$(CC) $(CFLAGS) main.c -o main.o
lrpc.a: Serialized.o 
	ar rs ./Library/librpc.a Serialized.o
Serialized.o: Serialized.c
	$(CC) $(CFLAGS) Serialized.c -o Serialized.o
clean:
	rm Serialized.o
	rm ./Library/librpc.a
	rm main.o
	rm exe
