/*
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
Similar to the accept funct
return 0 if success
-1 if failure

int read and write
this funct contains newsockfd, buffer and buffer_size
the buffer is the string wich we have been to pass
buffer_size is the string size so the messagge
can be grater of the string size!
*/

/*
filename server_ipaddress portno
argv[0] filename
argv[1] server_ipaddress
argv[2] portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    /*
    The first argument int domanai is refer to the type of protocol in our case AF_INET is ipv4
    INT TYPE -> SOCK_STREAM that we are using TCP protocoll
    On the other hands we can have a SOCK_DCRAM for UDP
    int protocol -> 0 is the default for the TCP protocol
    */
    if (sockfd < 0) {
        error("ERROR opening socket");
    }
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero(
        (char *) &serv_addr,
        sizeof(serv_addr)
    );
    serv_addr.sin_family = AF_INET;
    bcopy(
        (char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length
    );
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
        error("ERROR connectiong");
    }
    while(1) {
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("ERROR, writing to socket");
        }
        bzero(buffer, 256);
        n = read(sockfd, buffer, 255);
        if (n < 0) {
            error("ERROR, reading to socket");
        }
        printf("Server : %s\n", buffer);
        int l = strncmp("Bye", buffer, 3);
        if (l == 0) {
            break;
        }
    }
    close()
}