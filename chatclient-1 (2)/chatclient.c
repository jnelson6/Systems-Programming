/*******************************************************************************
 * Name        : chatclient.c
 * Author      : Michael Zylka, Roma Razdan
 * Date        : 5/7/2021
 * Description : Create a chat client that is able to communicate with a server
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "util.h"

#define MAX_MSG_LEN  1024
#define MAX_NAME_LEN 20


int client_socket = -1;
char inbuf[BUFLEN + 1];
char outbuf[MAX_MSG_LEN + 1];
char username[MAX_NAME_LEN + 1];

/*
 *handles parsing the ip and port strings and returns errors if they are invalid
 *sets the port and server_addr
 */
void handle_ip_and_port(char* ip_addr, char* port_num, int *port, struct sockaddr_in *server_addr) {
    if (inet_pton(AF_INET, ip_addr, &server_addr->sin_addr) == 0) {
        fprintf(stderr, "Error: Invalid IP address '%s'.\n", ip_addr);
        exit(EXIT_FAILURE);
    }
    server_addr->sin_family = AF_INET;

    if (parse_int(port_num, port, "port number.") == false) {
        exit(EXIT_FAILURE);
    }
    else if (*port < 1024 || *port > 65535) {
        fprintf(stderr, "Error: Port must be in range [1024, 65535].\n");
        exit(EXIT_FAILURE);
    }
    server_addr->sin_port = htons(*port);
}

int handle_stdin() {
    int message = get_string(outbuf, MAX_MSG_LEN + 1);
    if (message == TOO_LONG) {
        printf("Sorry, limit your message to %d characters.\n", MAX_MSG_LEN);
    }
    else {
        if (send(client_socket, outbuf, strlen(outbuf), 0) <0) {
            fprintf(stderr, "Error: Failed to send message. %s\n", strerror(errno));
        }
        if(strcmp(outbuf, "bye") == 0) {
            printf("Goodbye.\n");
            return 1;
        }
    }
    return 0;
}

int handle_client_socket() {
    int bytes_recvd;
    if ((bytes_recvd = recv(client_socket, inbuf, BUFLEN - 1, 0)) < 0) {
        fprintf(stderr, "Warning: Failed to receive incoming message. %s.\n", strerror(errno));
    }

    else if(bytes_recvd == 0) {
        fprintf(stderr, "\nConnection to server has been lost.\n");
        return 2;
    }
    inbuf[bytes_recvd] = '\0';
    if (strcmp(inbuf, "bye") == 0) {
        printf("\nServer initiated shutdown.\n");
        return 1;
    }
    else {
        printf("\n%s\n", inbuf);
    }
    return 0;

}


int main(int argc, char* argv[]) {

    /*
     *Initalizes variables to be used for the ip address, port, username, and messages
     */
    int bytes_recvd;
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in); 
    memset(&server_addr, 0, addrlen);
    int port;
    int retval;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }


    handle_ip_and_port(argv[1], argv[2], &port, &server_addr);

    printf("Enter your username: ");
    scanf("%s", username);
    if (strlen(username) > MAX_NAME_LEN) {
        fprintf(stderr, "Sorry, limit your username to %d characters.\n", MAX_NAME_LEN);
        exit(EXIT_FAILURE);
    }
    printf("Hello, %s. Let's try to connect to the server.\n", username);


    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0))< 0) {
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
    }

    if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) == -1) {
        fprintf(stderr, "Error: Failed to connect to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    if((bytes_recvd = recv(client_socket, inbuf, MAX_MSG_LEN - 1, 0)) < 0) {
        fprintf(stderr, "Error: Failed to receive message from server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    if(bytes_recvd == 0) {
        fprintf(stderr, "All connections are busy. Try again later.\n");
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    inbuf[bytes_recvd] = '\0';
    
    printf("\n%s\n\n", inbuf);

    if(send(client_socket, username, strlen(username), 0) <0) {
        fprintf(stderr, "Error: Failed to send username to server. %s.\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto EXIT;
    }

    printf("[%s]: ", username);
    fflush(stdout);

    fd_set IO;

    while(true) {


        FD_ZERO(&IO);
        FD_SET(client_socket, &IO);
        FD_SET(STDIN_FILENO, &IO);

        if (select(client_socket + 1, &IO, NULL, NULL, NULL) < 0) {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            retval = EXIT_FAILURE;
            goto EXIT;
        }

        if (FD_ISSET(client_socket, &IO)) {
            int temp = handle_client_socket();
            if(temp == 1) {
                retval = EXIT_SUCCESS;
                goto EXIT;
            }
            else if (temp == 2) {
                retval = EXIT_FAILURE;
                goto EXIT;
            }
        }
        if (FD_ISSET(STDIN_FILENO, &IO)) {
            if(handle_stdin() == 1) {
                retval = EXIT_SUCCESS;
                goto EXIT;
            }
        }

        printf("[%s]: ", username);
        fflush(stdout);

    }

EXIT:
    if(fcntl(client_socket, F_GETFD) >= 0) {
        close(client_socket);
    }

    exit(retval);
}
