/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : server.c                                           */
/*  PRINCIPAL AUTHOR     : LiuYang                                            */
/*  SUBSYSTEM NAME       : C&S transmit                                       */
/*  MODULE NAME          : server                                             */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2015/01/05                                         */
/*  DESCRIPTION          : SERVER program for course project                  */
/******************************************************************************/
 
 /*
 *  Revision log:
 *
 * Created by Liuyang, 2015/01/05 recieve file
 * v1.1 by Liuyang, 2015/01/06 use rollqueue to be buffer
 * v2.0 by liuyang, 2015/01/11 pthread
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "transmit.h"
#include "strtool.h"
#include "sock.h"

#define numOfListen 10 
#define MAXSIZE 4096 
#define OUT_FILE_NAME "out"

int main()
{
    struct sockaddr_in client;
    int listenfd;
    int connectfd;
    int addrlen;
    char buf[MAXSIZE];
    int len;
    int file_size;
    int pthread_num;
	
    /* prepare socket */
    listenfd = server_socket_Init();
    listen(listenfd,numOfListen);
    /* accept client */
    addrlen = sizeof(client);
    printf("-------Waiting for client--------\n");
    if(-1 == (connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen)))
    {
        printf("Accept failed: BEFORE get file size & pthread number.\n");
        exit(1);
    }
	
    /* get file size */
    while((len = recv(connectfd,buf,MAXSIZE,0)) <=0);
    file_size = StringToInt(len,buf);
    send(connectfd,"ok",2,0);
	
    /* get pthread number */
    while((len = recv(connectfd,buf,MAXSIZE,0)) <=0);
    pthread_num = StringToInt(len,buf);
    send(connectfd,"ok",2,0);

    close(connectfd);

    /* start to recieve file */
    do_Recv_File(listenfd,file_size,pthread_num,OUT_FILE_NAME);

    close(listenfd);
    printf("Finsh!\n"); 
	
    return 0;
}




