
/******************************************************************************
* Name  : findserver.c
* Author: Julia Nelson
* Date  : June 22, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 10 - Sockets
******************************************************************************/

/*
Fill in the TODO sections

Run the binary as the server.  It listens on a randomly chosen port.

Goal: Get the client to find the open port and receive the hidden message.

*/
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define IP_ADDR "127.0.0.1"
#define BUFLEN  128

int create_socket(struct timeval *tv) {
    int client_socket;

    // TODO: Create a client socket using TCP. If it fails, print the error
    // message: "Error: Failed to create socket. %s.\n", where %s is strerror of
    // errno and return -1.
    // similar to client.c inclass code .. if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    if((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: Failed to create socket. %s.\n", strerror(errno));
        return -1;  // maybe set retval to -1 and goto EXIT
    } 
    // TODO: Set the receive timeout option for the client socket. Use the
    // function's argument, struct timeval *tv, in the function call to
    // setsockopt. If setsockopt fails, print the error message:
    // "Error: Cannot set socket options. %s.\n", where %s is strerror of errno
    // and return -1.
    // Hint: Look up SO_RCVTIMEO.
    //
    // in class code chatserver.c  ..  if (setsockopt( server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) != 0) 
    // helpful example found at https://www.linuxquestions.org/questions/programming-9/setsockopt-so_rcvtimeo-468572/
    //      if below doesnt work try second attempt.. else if(setsockopt(client_sock, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)&tv, sizeof(struct timeval)) != 0) {
    if(setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (struct timeval *)tv, sizeof(struct timeval)) != 0) {
        fprintf(stderr, "Error: Cannot set socket options. %s.\n", strerror(errno));
        return -1; // maybe set retval to -1 and goto EXIT
    } 
    return client_socket;
}

int main() {
    int client_socket = -1, bytes_recvd, retval = EXIT_SUCCESS;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    char buf[BUFLEN];

    memset(&serv_addr, 0, addrlen);
    int ip_conversion = inet_pton(AF_INET, IP_ADDR, &serv_addr.sin_addr);
    if (ip_conversion == 0) {
        fprintf(stderr, "Error: Invalid IP address '%s'.\n", IP_ADDR);
        return -1;
    } else if (ip_conversion < 0) {
        fprintf(stderr, "Error: Failed to convert IP address. %s.\n",
                strerror(errno));
        return -1;
    }
    serv_addr.sin_family = AF_INET;

    struct timeval tv;
    // TODO: Set the tv to be 0 seconds, 250 microseconds.
    tv.tv_sec = 0;
    tv.tv_usec = 250; // can change to 1000 if not working on my computer

    // TODO:
    // Loop over ports 1024 up to and including 65535.
    for (int i = 1024; i <= 65535; i++) {
        //   Modify the sin_port field of serv_addr to the port you are trying.
        serv_addr.sin_port = htons(i); // man 3 htons == The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
   
        //   Create a client socket by calling create_socket() above. If it fails,
        //   set retval to EXIT_FAILURE and transfer execution to EXIT below.
        if((client_socket = create_socket(&tv)) < 0) {
            retval = EXIT_FAILURE;
            goto EXIT;
        }
        //   Attempt to connect to the server... like client.c but success not failure
        if (connect(client_socket, (struct sockaddr *)&serv_addr, addrlen) >= 0) {
            //   If it does connect,
            //     Attempt to receive up to BUFLEN - 1 characters.
            // like server.c line 76 but success not failure
            if ((bytes_recvd = recv(client_socket, buf, BUFLEN - 1, 0)) > 0) {
                //     If data is received,
                //       Null-terminate it.
                buf[bytes_recvd] = '\0';
                //       Print "Found server on port %d.\n"
                printf("Found server on port %d.\n", i);
                //             "Received message from server: %s\n"
                printf("Received message from server: %s\n", buf);
                //       Transfer execution to EXIT below.
                goto EXIT;
            }
        }
        //   Close the socket.
        close(client_socket);
    }
    printf("No server was found.\n");

EXIT:
    if (fcntl(client_socket, F_GETFD) >= 0) {
        close(client_socket);
    }
    return retval;
}







