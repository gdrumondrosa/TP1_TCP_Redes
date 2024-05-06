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
    char tipo[5], enderecoIP[15], msg[100];
    struct sockaddr_in endServidor;

    strcpy(tipo, argv[1]);
    strcpy(enderecoIP, argv[2]);
    porta      = atoi(argv[3]);

    /* Criacao do socket */
    csocket = socket(AF_INET, SOCK_STREAM, 0);

    /* Conexao (abertura ativa) */
    memset(&endServidor, 0, sizeof(endServidor));
    endServidor.sin_family      = AF_INET;
    endServidor.sin_addr.s_addr = inet_addr(enderecoIP);
    endServidor.sin_port        = htons(porta);
    
    err = connect(csocket, (struct sockaddr*) &endServidor, sizeof(endServidor));
    
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