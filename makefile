.PHONY: compile clean

compile: client server
	@echo "compile success"
client: client.o networking.o
	@gcc -o client client.o networking.o

client.o: client.c networking.h
	@gcc -c -Wall client.c
server: server.o networking.o blackjack.o
	@gcc -o server server.o networking.o blackjack.o
server.o: server.c networking.h blackjack.h
	@gcc -c -Wall server.c
networking.o: networking.c networking.h blackjack.h
	@gcc -Wall -c networking.c
blackjack.o: blackjack.c blackjack.h
	@gcc -Wall -c blackjack.c

clean:
	@rm -f *.o client server
