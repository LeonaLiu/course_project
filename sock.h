#ifndef SOCK_H
#define SOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* create a socket connect : server */
int server_socket_Init(void);

/* create a socket connect : client */
int client_socket_Init(void);


#endif
