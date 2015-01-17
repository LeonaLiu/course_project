#ifndef TRANSMIT_H
#define TRANSMIT_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/* recv file */
void do_Recv_File(int listenfd,int file_size,int pthread_num,char *file_name);

/* send file */
void do_Send_File(int file_size,int pthread_num,char *file_name);

#endif
