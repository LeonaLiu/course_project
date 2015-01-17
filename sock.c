/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : sock.c                                             */
/*  PRINCIPAL AUTHOR     : LiuYang                                            */
/*  SUBSYSTEM NAME       : C&S transmit                                       */
/*  MODULE NAME          : sock                                               */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2015/01/16                                         */
/*  DESCRIPTION          : sock tool for C&S course project                   */
/******************************************************************************/
 
/*
 *  Revision log:
 *
 * Created by Liuyang, 2015/01/16 socket tool for C&S course project
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

#include "sock.h"

#define PORT 6381
#define IPADDRESS "127.0.0.1"

/* create a socket connect : server */
int server_socket_Init(void)
{
    int listenfd;
    struct sockaddr_in serveraddr;
    int opt = SO_REUSEADDR;
    
    if(-1 == (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("socket failure.Please run again.\n");
        exit(1);
    } 
	
    /* make port useful immeditately after release */
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
	
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(PORT);
	
    if(-1 == bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))
    {
        printf("bind failure.Please run again.\n");
        exit(1);
    }

    return listenfd;
}


/* create a socket connect : client */
int client_socket_Init(void)
{
    struct sockaddr_in server;
    int sockfd;

    if(-1 == (sockfd = socket(AF_INET, SOCK_STREAM, 0)))
    {
        printf("Create socket failed.\n");
        exit(1);
    }

    /* set addr of server */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if(-1 == (inet_aton(IPADDRESS,&server.sin_addr)))
    {
        printf("set server address failed.\n");
        close(sockfd);
        exit(1);
    }

    if(-1 == (connect(sockfd,(struct sockaddr *)&server,sizeof(server))))
    {
        printf("Connect failed.\n");
        close(sockfd);
        exit(1);
    }

    return sockfd;
}
