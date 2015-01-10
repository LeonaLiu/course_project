/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : server.c                                           */
/*  PRINCIPAL AUTHOR     : MengNing                                           */
/*  SUBSYSTEM NAME       : C&S socket                                         */
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
#include "dbtime.h"

#define PORT 6381
#define BACKLOG 10
#define MAXLINE 4096 

int main()
{
    struct sockaddr_in server;
    struct sockaddr_in client;
    int listenfd;
    int connectfd;
    int addrlen;
    char msg[MAXLINE];
    char reply[20] = "SERVER_RECV_SUCCESS!";
    int msglen;
    int opt = SO_REUSEADDR;
    FILE *output;
    int i;

    /* open a output file to write */
    output = fopen("OUT","w");
    /* create a TCP listen socket */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Create socket failed.\n");
        exit(0);
    }
    /* make port useful immeditately after release */
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
    /* set addr of server */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Bind failed.\n");
        close(listenfd);
        exit(0);
    }
    if(listen(listenfd,BACKLOG) == -1)
    {
        printf("Listen failed.\n");
        close(listenfd);
        exit(0);
    }
    addrlen = sizeof(client);
    printf("-------Waiting for client--------\n");
    if((connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen)) == -1)
    {
        printf("Accept failed.\n");
        exit(0);
    }
    /*  calculate time */
    dbtime_startTest("V2.0");   /*  tips: add -lrt to makefile */
    /* recieve file */
    //i = 0;
    while((msglen = recv(connectfd,msg,MAXLINE,0)) > 0)
    {
        if(3 == msglen)
        {
            if('e' == msg[0] && 'n' == msg[1] && 'd' == msg[2])
            {
                break; 
            }
        }
        fwrite(&msg,sizeof(msg),1,output);
        send(connectfd,"next",4,0);
       // printf("%d:Clinet %s send %dB to you\n",i,inet_ntoa(client.sin_addr),htons(client.sin_port),msglen);
      //  i++;
    }
    dbtime_endAndShow();
    send(connectfd,reply,sizeof(reply),0);
    fclose(output);
    close(connectfd);
    close(listenfd);
    return 0;
}



