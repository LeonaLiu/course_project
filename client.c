/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : client.c                                           */
/*  PRINCIPAL AUTHOR     : MengNing                                           */
/*  SUBSYSTEM NAME       : C&S socket                                         */
/*  MODULE NAME          : client                                             */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2015/01/05                                         */
/*  DESCRIPTION          : CLIENT program for course project                  */
/******************************************************************************/

/*
 *  Revision log:
 *
 * Created by Liuyang, 2015/01/05  send a 1G file
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 6381 
#define MAXLINE 100
#define NUM 4 
#define MAXSIZE 1024

int main()
{
    struct sockaddr_in server;
    int sockfd;
    char reply[MAXLINE];
    int relen;
    char serverip[50] = "127.0.0.1";
    char buf[MAXSIZE*NUM];
    FILE *file;
    int len;
    int i=0;

    /* input file */
    file = fopen("test","r");
    //create a TCP socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Create socket failed.\n");
        exit(1);
    }
    /* set addr of server */
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    if(inet_aton(serverip,&server.sin_addr) == -1)
    {
        printf("set server address failed.\n");
        close(sockfd);
        exit(1);
    }
    if(connect(sockfd,(struct sockaddr *)&server,sizeof(server)) == -1)
    {
        printf("Connect failed.\n");
        close(sockfd);
        exit(1);
    }
    /* send file  */
    while((len = fread(buf,MAXSIZE,NUM,file))>0)   /* read 1024B*NUM to buf */
    {
    //    printf("len = %d,i = %d\n",len,i++);
        if(send(sockfd,buf,len,0) == -1)
        {
            printf("Send mesg faild.\n");
            close(sockfd);
            exit(1);
        }
        while(recv(sockfd,reply,MAXLINE,0) != 4);
    }
    send(sockfd,"end",3,0);
    fclose(file);
    printf("Send file SUCCESS!\n");
    /* waiting for reply */
    if((relen = recv(sockfd,reply,MAXLINE,0)) == -1)
    {
        printf("Recv failed\n");
    }
    else
    {
        reply[relen] = '\0';
        printf("Sever reply: \"%s\".\n",reply);
    }
    close(sockfd);
    return 0;
}



