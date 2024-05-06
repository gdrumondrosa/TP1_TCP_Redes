#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

static double haversine(double lat1, double lon1,
                        double lat2, double lon2)
    {
        // distance between latitudes
        // and longitudes
        double dLat = (lat2 - lat1) *
                      M_PI / 180.0;
        double dLon = (lon2 - lon1) * 
                      M_PI / 180.0;
 
        // convert to radians
        lat1 = (lat1) * M_PI / 180.0;
        lat2 = (lat2) * M_PI / 180.0;
 
        // apply formulae
        double a = pow(sin(dLat / 2), 2) + 
                   pow(sin(dLon / 2), 2) * 
                   cos(lat1) * cos(lat2);
        double rad = 6371;
        double c = 2 * asin(sqrt(a));
        return 1000 * rad * c;
    }

typedef struct {
    double latitude;
    double longitude;
} Coordinate;
// Criando uma coordenada
Coordinate coordServ = {-19.9227,-43.9451};

int main(int argc, char const *argv[]) {
    char *eptr;
    int ssocket, csocket, porta, nbytes, err;
    double dist, latCli, longiCli;
    char tipo[5], dist_char[10], at[100], msg[100];
    struct sockaddr_in endereco, endCliente;
    int tamEndereco = sizeof(endCliente);

    /* Leitura da porta a ser atribuida ao servidor */
    strcpy(tipo, argv[1]);
    porta = atoi(argv[2]);

    /* Criacao do socket */
    ssocket = socket(AF_INET, SOCK_STREAM, 0);

    /* Abertura passiva */
    memset(&endereco, 0, sizeof(endereco));
    endereco.sin_family      = AF_INET;
    endereco.sin_addr.s_addr = INADDR_ANY;
    endereco.sin_port        = htons(porta);

    err = bind(ssocket, (struct sockaddr*) &endereco, sizeof(endereco));

    err = listen(ssocket, 1);

    printf("Aguardando solicitação.\n");

    while(1) {
        csocket = accept(ssocket, (struct sockaddr*) &endCliente, &tamEndereco);

        while(1) {
            nbytes = recv(csocket, msg, 2, 0);
            nbytes = send(csocket, msg, strlen(msg)+1, 0);

            if(!strcmp(msg, "1")) {
                nbytes = recv(csocket, msg, 100, 0);
                latCli = strtod(msg, &eptr);
                nbytes = send(csocket, msg, strlen(msg)+1, 0);

                nbytes = recv(csocket, msg, 100, 0);
                longiCli = strtod(msg, &eptr);
                nbytes = send(csocket, msg, strlen(msg)+1, 0);

                printf("Corrida disponível:\n0 - Recusar\n1 - Aceitar\n");
                scanf(" %s", msg);

                /* Comunicacao */
                if(!strcmp(msg, "1")) {
                    dist = haversine(latCli, longiCli, coordServ.latitude, coordServ.longitude);
                    while(dist > 0){
                        sprintf(dist_char, "%0.f", dist);
                        strcpy(at, "Motorista a ");
                        strcat(at, dist_char);
                        strcat(at, "m\n");
                        nbytes = send(csocket, at, strlen(at)+1, 0);
                        nbytes = recv(csocket, msg, 100, 0);

                        dist = dist - 400;
                        sleep(2);
                    }

                    printf("O motorista chegou.\n");
                    nbytes = send(csocket, "O motorista chegou.\n", strlen("O motorista chegou.\n")+1, 0);
                    nbytes = recv(csocket, msg, 100, 0);
                    printf("Aguardando solicitação.\n");
                    break;
                }
                else {
                    nbytes = send(csocket, "Não foi encontrado um motorista.\n", strlen("Não foi encontrado um motorista.\n")+1, 0);
                    nbytes = recv(csocket, msg, 100, 0);
                    printf("Aguardando solicitação.\n");
                }
            }
            else {
                break;
            }
        }
    }

    close(ssocket);

    return 0;
}