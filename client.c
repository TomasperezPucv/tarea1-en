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
    USO:  ./client <puerto> <mensaje>
    donde <puerto> es un numero entre 1025 y 65535 
    (en el arcv estan guardados los parametros
    donde en este caso 
    argv[1] es el puerto y argv[2] es el mensajito)     
*/
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso es: %s <puerto> <mensaje>\n", argv[0]);
        exit(1);
    }
    int puerto = atoi(argv[1]);
    char *mensaje = argv[2];
    int clientFd;
    struct sockaddr_in broker;

    printf("creando el socket\n");
    if((clientFd = socket(AF_INET, SOCK_STREAM,0))<0){
        error("no se puedo crear el socket :I\n");
    }

    broker.sin_family = AF_INET;
    broker.sin_port = htons(puerto);
    if (inet_pton(AF_INET, "127.0.0.1", &broker.sin_addr) <= 0) {
        error("Dirección no es valida :(\n");
    }

    printf("conectando al servidor (broker) :O");
    if(connect(clientFd,(struct sockaddr*)&broker, sizeof(broker))<0){
        error("conexion fallida D:");
    }

    printf("enviando mensaje al servidor :O\n");
    if (send(clientFd, mensaje, strlen(mensaje),0) < 0) {
        error("Error en enviar el mensaje  ;-;\n");
    }

    




    return 0;
}