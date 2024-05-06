all: cli-serTCP

cli-serTCP: cliente servidor

clienteTCP: cliente.c
	gcc -o bin/cliente cliente.c -lm

servidorTCP: servidor.c
	gcc -o bin/servidor servidor.c -lm

clear:
	rm cliente servidor