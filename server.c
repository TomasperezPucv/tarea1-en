#include <stdio.h> // perror, printf
#include <sys/socket.h>
#include <arpa/inet.h>// sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <unistd.h> // write, read, close
#include <string.h> // strlen, memset
#include <stdlib.h> // exit, atoi
#define MAX_MSG_SIZE 1024
//wrapper para errores
void error(char *msg){
    perror(msg);
    perror("\n");
    exit(1);
}
/*
    USO:  ./server <puerto>
    donde <puerto> es un numero entre 1025 y 65535      
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso es: %s <puerto>\n", argv[0]);
        exit(1);
    }
    int puerto = atoi(argv[1]);
    int serverFd;
    struct sockaddr_in6 server_addr, broker_addr;
    char buffer[MAX_MSG_SIZE];
    socklen_t broker_len = sizeof(broker_addr);

    //creamos el socket UDP en IPv6
    if ((serverFd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        error("Error creando socket UDP/IPv6  :p\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_addr = in6addr_any;  // Acepta conexiones de cualquier dirección
    server_addr.sin6_port = htons(puerto);

    // Enlazar el socket al puerto
    if (bind(serverFd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error en bind :c\n");
    }

    printf("Servidor escuchando en puerto %d (UDP/IPv6)\n", puerto);
    // recibir mensaje
    memset(buffer, 0, MAX_MSG_SIZE);
    if (recvfrom(serverFd, buffer, MAX_MSG_SIZE, 0, (struct sockaddr *)&broker_addr, &broker_len) < 0) {
        error("Error recibiendo mensaje, pucha :c\n");
    }

    // Mostrar dirección y puerto del broker
    char broker_ip[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET6, &broker_addr.sin6_addr, broker_ip, INET6_ADDRSTRLEN);
    printf("Mensaje recibido desde broker: %s:%d\n", broker_ip, ntohs(broker_addr.sin6_port));

    // Imprimir el mensaje recibido
    printf("Mensaje recibido: %s\n", buffer);

    close(serverFd);


    return 0;
}
