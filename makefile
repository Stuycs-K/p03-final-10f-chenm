.PHONY: compile clean

compile: client server
	@echo "compile success"
client.c: client.o networking.o
	@gcc -o client client.o networking.o
client.o: client.c networking.h
	@gcc -c -Wall client.c
networking: networking.o networking.h
	@gcc -o networking networking.o
server: server.o networking.o
	@gcc -o server server.o networking.o
server.o: server.c networking.h
	@gcc -c -Wall server.c
clean:
	@rm -f *.o client server
