#include <stdio.h> // perror, printf
#include <sys/socket.h>
#include <arpa/inet.h>// sockaddr_in, AF_INET, SOCK_STREAM,
                       // INADDR_ANY, socket etc...
#include <unistd.h> // write, read, close
#include <string.h> // strlen, memset
#include <stdlib.h> // exit, atoi
#define MAX_MSG_SIZE 1024

// wrapper para manejo de errores
void error(char *msg) {
    perror(msg);
    perror("\n");
       exit(1);
}
/*
    USO:  ./broker <puerto escucha> <puerto servidor>
    donde <puerto escucha|servidor> es un numero entre 1025 y 65535      
 */
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso es: %s <puerto escucha> <puerto servidor>\n", argv[0]);
        exit(1);
    }
    int puerto_esc = atoi(argv[1]);
    int puerto_serv = atoi(argv[2]);
    
    //aca va la configuracion del lado TCP/IPv4 
    // osea client -> broker
    int brokerFd, clienteFd;
    struct sockaddr_in broker_addr, client_addr;
    char buffer[MAX_MSG_SIZE];
    socklen_t client_len = sizeof(client_addr);

    //crear socket TCP para escuchar
    if((brokerFd = socket(AF_INET, SOCK_STREAM,0))<0){
        error("Error creando el socket TCP :(\n");
    }
    memset(&broker_addr,0,sizeof(broker_addr));
    broker_addr.sin_family = AF_INET;
    broker_addr.sin_addr.s_addr= INADDR_ANY;
    broker_addr.sin_port = htons(puerto_esc);


    //vinculamos el socket TCP
    if(bind(brokerFd,(struct sockaddr*)&broker_addr, sizeof(broker_addr))<0){
        error("Error al bind D:\n");
    }

    // Escuchar conexiones
    if (listen(brokerFd, 5) < 0) {
        error("Error en listen :/\n");
    }

    printf("Broker escuchando en puerto %d (TCP)\n", puerto_esc);

    // Aceptar conexion del cliente
    if((clienteFd = accept(brokerFd,(struct sockaddr*)&client_addr,&client_len))<0){
        error("Error en el accept ;-;\n");
    }

    // Mostrar dirección y puerto del cliente
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
    printf("Conectado al cliente: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Recibir mensaje del cliente
    memset(buffer, 0, MAX_MSG_SIZE);
    if (recv(clienteFd, buffer, MAX_MSG_SIZE, 0) < 0) {
        error("Error recibiendo mensaje del cliente D;<\n");
    }
    printf("Mensaje recibido del cliente: %s\n", buffer);

    close(clienteFd);
    close(brokerFd);

    //aca va la configuracion del lado UDP/IPv6 
    // osea broker -> server
    int udpFd;
    struct sockaddr_in6 server_addr;

    //creamos el socket UDP para enviar al servidor
    if ((udpFd = socket(AF_INET6, SOCK_DGRAM, 0)) < 0) {
        error("Error creando socket UDP :c\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin6_family = AF_INET6;
    server_addr.sin6_port = htons(puerto_serv);

    if (inet_pton(AF_INET6, "::1", &server_addr.sin6_addr) <= 0) {
        error("Dirección IPv6 no válida???\n");
    }

    // Enviar mensaje al servidor
    if (sendto(udpFd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        error("Error enviando mensaje al servidor, nonooononoonon CASIII Asajkfbgahjskgiujk >:c\n");
    }

    printf("Mensaje enviado al servidor en puerto %d (UDP/IPv6)\n", puerto_serv);

    close(udpFd);


    return 0;
}