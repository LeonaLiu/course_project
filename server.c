/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : server.c                                           */
/*  PRINCIPAL AUTHOR     : MengNing                                           */
/*  SUBSYSTEM NAME       : C&S socket                                         */
/*  MODULE NAME          : server                                             */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2014/12/02                                         */
/*  DESCRIPTION          : SERVER program for C&S socket test homework        */
/******************************************************************************/
 
 /*
 *  Revision log:
 *
 * Created by Liuyang, 2015/01/05 recieve file
 * v1.1 by Liuyang, 2015/01/06 use rollqueue to be buffer
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
#include <pthread.h>

#include "dbtime.h"
 /*#include "rollqueue.h" */

#define RQLEN 8192 
#define BUFSIZE 4096 /* 4k */

#define PORT 6381
#define IPADDRESS "127.0.0.1"
#define numOfListen 5

typedef struct rqueue
{
    char *text[RQLEN];
    int head;
    int tail;
    int num;
    int len;
}rqueueT;

static int socket_Init(const char* ip, int port);
static int AtEndOfFile(char *arr);
static void *HandleRecv(int fd);
static void *HandleWrite(void);

void DeQueue(rqueueT *queue,char *out);
void EnQueue(rqueueT *queue,char *in);
int IsEmpty(rqueueT *queue);
int IsFull(rqueueT *queue);
void InitQueue(rqueueT *queue);
void FreeQueue(rqueueT *queue);

FILE *output;
rqueueT queue;

int main()
{
    struct sockaddr_in client;
    int listenfd;
    int connectfd;
    int addrlen;
    char reply[20] = "SERVER_RECV_SUCCESS!";
    pthread_t tid_recv;
    pthread_t tid_write;

    /* open a output file to write */
    output = fopen("OUT","w");

    /* init roll queue */
    InitQueue(&queue);

    /* prepare socket */
    listenfd = socket_Init(IPADDRESS,PORT);
    listen(listenfd,numOfListen);
    /*  wait for accept */
    addrlen = sizeof(client);
    printf("-------Waiting for client--------\n");
    if((connectfd = accept(listenfd,(struct sockaddr*)&client,&addrlen)) == -1)
    {
        printf("Accept failed.\n");
        exit(1);
    }

    /*  calculate time */
    dbtime_startTest("All");    /*  tips: add -lrt to makefile */
    /* recv from client */
    if(pthread_create(&tid_recv,NULL,(void*)HandleRecv,(void*)connectfd) != 0)
    {
        printf("pthread_create Error: Recv\n");
        exit(1);
    }
    /* output to file */
    if(pthread_create(&tid_write,NULL,(void*)HandleWrite,NULL) != 0)
    {
        printf("pthread_create Error: Write\n");
        exit(1);
    }
    pthread_join(tid_recv,NULL);
    pthread_join(tid_write,NULL);
       // printf("%d:Clinet %s send %dB to you\n",i,inet_ntoa(client.sin_addr),htons(client.sin_port),msglen);
      //  i++;

    dbtime_endAndShow();
    printf("Finsh!\n");
    send(connectfd,reply,sizeof(reply),0);
    FreeQueue(&queue);
    fclose(output);
    close(connectfd);
    close(listenfd);
    return 0;
}

static void *HandleRecv(int fd)
{
    char buf[BUFSIZE];
    int i =0;

    printf("recving......\n");
//    dbtime_startTest("Recv");
    while(recv(fd,buf,BUFSIZE,0) > 0)
    {
        while(IsFull(&queue))  /* wait if queue if full */
        {
           printf("wait %d\n",i++);
           sleep(1);
        }
        EnQueue(&queue,buf);
        if(AtEndOfFile(buf) == 1)
        {
            break; 
        }
        send(fd,"next",4,0);
    }
    printf("End of recving\n");
    return NULL;
}

static void *HandleWrite(void)
{
    char buf[BUFSIZE];

    printf("writing......\n");
//    dbtime_startTest("Write");
    while(1)
    {
        while(IsEmpty(&queue));
        DeQueue(&queue,buf);
        if(AtEndOfFile(buf) == 1)
        {
            break; 
        }
        fwrite(buf,BUFSIZE,1,output);
    }
    printf("End of writing\n");
    return NULL;
}

/* judge if it is a END message */
static int AtEndOfFile(char *arr)
{
    if('e' == arr[0] && 'n' == arr[1] && 'd' == arr[2])
    {
        return 1;
    }
    return 0;
}

static int socket_Init(const char* ip, int port)
{
    int listenfd;
    struct sockaddr_in serveraddr;
    
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == listenfd)
    {
        printf("socket failure.Please run again.\n");
        exit(1);
    }  
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&serveraddr.sin_addr);
    serveraddr.sin_port = htons(port);
    if(-1 == bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))
    {
        printf("bind failure.Please run again.\n");
        exit(1);
    }
    return listenfd;
}

void InitQueue(rqueueT *queue)
{
    int i;

    queue->head = 0;
    queue->tail= 0;
    queue->num= 0;
    queue->len = RQLEN;
    for(i=0;i<queue->len;i++)
    {
        queue->text[i] = NULL;
    }
}

void FreeQueue(rqueueT *queue)
{
    int i;
    
    for(i=0;i<queue->len;i++)
    {
        if(queue->text[i] != NULL)
        {
            free(queue->text[i]);
        }
    }
}

int IsFull(rqueueT *queue)
{
    if(queue->num == queue->len)
    {
        return 1;
    }
    return 0;
}

int IsEmpty(rqueueT *queue)
{
    if(0 == queue->num)
    {
        return 1;
    }
    return 0;
}

void EnQueue(rqueueT *queue,char *in)
{
    char *ptr;
    if(1 == IsFull(queue))
    {
        printf("Error rollqueue: En\n");
        exit(1);
    }
    ptr = (char *)malloc(sizeof(char) * BUFSIZE);
    memcpy(ptr,in,BUFSIZE);
    queue->text[queue->tail] = ptr;
    queue->tail = (queue->tail + 1) % queue->len;
    queue->num++;
   // printf("En Queue\n");
}

void DeQueue(rqueueT *queue,char *out)
{
    char *ptr;

    if(1 == IsEmpty(queue))
    {
        printf("Error rollqueue: De\n");
        exit(1);
    }
    ptr = queue->text[queue->head]; 
    memcpy(out,ptr,BUFSIZE);
    free(ptr);
    queue->text[queue->head] = NULL;
    queue->head = (queue->head + 1) % queue->len;
    queue->num--;
   // printf("De Queue\n");
}





