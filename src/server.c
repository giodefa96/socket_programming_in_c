/*
int bind(int sockfd, const struct sockaddr *addr, sockelen_t_addrelen)

struct sockaddr {
sa_family_t_sa_family; -> 
char sa_data[14]; -> max 14 characters
}  
int bind return 0 if the execution is ok
or
-1 -> id it is a failure


int listen(int sockfd, int backlog);
listen for connection into the socket.
backlog -> number of connection a system can 
handle at single time, maybe 4 it's ok
max 4th connection at the time so we reject the 5th

newsockdf = accept(
                sockfd,
                (struct sockaddr *) &addr,
                &addrlen
                );
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "PortNumber not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("Error opening Socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); // This func clear any data
    portno = atoi(argv[1]); // Convert the string into a integer

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); // Host to network sort

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
        error("Binding Failed.");
    }
    listen(sockfd, 5); // the 5 is the maximum number of client that can be connected to the client at the time
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) {
        error("Error on Accept");
    }

    while(1) {
        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);
        if (n < 0) {
            error("Error on reading");
        }
        printf("Client : %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin); // the funct read byte from stream 
        
        n = write(newsockfd, buffer, strlen(buffer));
        if (n < 0) {
            error("Error on Writing");
        }
        int i = strcmp("Bye", buffer);
        if (i == 0) {
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}