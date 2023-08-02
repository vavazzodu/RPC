CC = gcc
CFLAGS = -Wall -g -Wextra -Werror

target: exe

exe: main.o person.o libRPC.a
	$(CC) $(CFLAGS) ./object/main.o ./object/person.o -o ./executable/exe -L ./library/ -lRPC

main.o: ../main.c
	$(CC) $(CFLAGS) -c ../main.c -o ./object/main.o

person.o: ../Source/person.c
	$(CC) $(CFLAG) -c ../Source/person.c -o ./object/person.o

lib: ./library/libRPC.a

libRPC.a: Serialize.o
	ar rs ./library/libRPC.a ./object/Serialize.o

Serialize.o: ../Source/Serialize.c
	$(CC) $(CFLAGS) -c ../Source/Serialize.c -o ./object/Serialize.o

client.o: ../multiply.c
	$(CC) $(CFLAGS) -c ../multiply.c -o ./object/client.o

server.o: ../server.c
	$(CC) $(CFLAGS) -c ../server.c -o ./object/server.o

client: client.o libRPC.a
	$(CC) $(CFLAGS) ./object/client.o -o ./executable/client -L ./library/ -lRPC

server: server.o libRPC.a
	$(CC) $(CFLAGS) ./object/server.o -o ./executable/server -L ./library/ -lRPC
clean:
	rm ./object/*.o
	rm ./library/lib*
	rm ./executable/*
