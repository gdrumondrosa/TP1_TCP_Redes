#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct {
double latitude;
double longitude;
} Coordinate;
// Criando uma coordenada
Coordinate coordCli = {-19.9735, -43.9671};


int main(int argc, char const *argv[]) {
    int csocket, porta, nbytes, err;
    char tipo[10], enderecoIP[15], msg[100];
    struct sockaddr_in endereco_ipv4;
    struct sockaddr_in6 endereco_ipv6;

    strcpy(tipo, argv[1]);
    strcpy(enderecoIP, argv[2]);
    porta      = atoi(argv[3]);

    if(!strcmp(tipo, "ipv4")) {
        /* Criacao do socket */
        csocket = socket(AF_INET, SOCK_STREAM, 0);

        /* Abertura ativa */
        memset(&endereco_ipv4, 0, sizeof(endereco_ipv4));
        endereco_ipv4.sin_family       = AF_INET;
        endereco_ipv4.sin_addr.s_addr  = inet_addr(enderecoIP);
        endereco_ipv4.sin_port         = htons(porta);

        err = connect(csocket, (struct sockaddr*) &endereco_ipv4, sizeof(endereco_ipv4));
    }
    else {
        /* Criacao do socket */
        csocket = socket(AF_INET6, SOCK_STREAM, 0);

        /* Abertura ativa */
        memset(&endereco_ipv6, 0, sizeof(endereco_ipv6));
        endereco_ipv6.sin6_family       = AF_INET6;
        endereco_ipv6.sin6_addr         = in6addr_any;
        endereco_ipv6.sin6_port         = htons(porta);

        err = connect(csocket, (struct sockaddr*) &endereco_ipv6, sizeof(endereco_ipv6));
    }
    
    /* Comunicacao */
    while(1) {
        printf("0 - Sair\n1 - Solicitar corrida\n");
        scanf(" %s", msg);
        nbytes = send(csocket, msg, strlen(msg)+1, 0);
        nbytes = recv(csocket, msg, 100, 0);
        if(!strcmp(msg, "1")) {
            sprintf(msg, "%f", coordCli.latitude);
            nbytes = send(csocket, msg, strlen(msg)+1, 0);
            nbytes = recv(csocket, msg, 100, 0);
            
            sprintf(msg, "%f", coordCli.longitude);
            nbytes = send(csocket, msg, strlen(msg)+1, 0);
            nbytes = recv(csocket, msg, 100, 0);

            nbytes = recv(csocket, msg, 100, 0);
            nbytes = send(csocket, msg, strlen(msg)+1, 0);
            if(!strcmp(msg, "Não foi encontrado um motorista.\n")) {
                printf("%s", msg);
            }
            else {
                while(strcmp(msg, "O motorista chegou.\n")) {
                    printf("%s", msg);
                    nbytes = recv(csocket, msg, 100, 0);
                    nbytes = send(csocket, msg, strlen(msg)+1, 0);
                }
                printf("%s", msg);
                break;
            }
        }
        else {
            break;
        }
    }
    
    /* Fechamento */
    close(csocket);

    return 0;
}