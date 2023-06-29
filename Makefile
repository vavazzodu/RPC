CC = gcc
CFLAGS = -Wall -g -Wextra -Werror

target: exe

exe: main.o person.o libRPC.a
	$(CC) $(CFLAGS) ./Library/main.o ./Library/person.o -o exe -L ./Library/ -lRPC
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o ./Library/main.o
person.o: ./Source/person.c
	$(CC) $(CFLAG) -c ./Source/person.c -o ./Library/person.o
lib: libRPC.a
libRPC.a: Serialize.o 
	ar rs ./Library/libRPC.a ./Library/Serialize.o
Serialize.o: ./Source/Serialize.c
	$(CC) $(CFLAGS) -c ./Source/Serialize.c -o ./Library/Serialize.o

clean:
	rm ./Library/*.o
	rm ./Library/lib*
	rm exe
