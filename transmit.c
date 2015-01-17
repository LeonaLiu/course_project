/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : transmit.c                                         */
/*  PRINCIPAL AUTHOR     : LiuYang                                            */
/*  SUBSYSTEM NAME       : C&S transmit                                       */
/*  MODULE NAME          : sock                                               */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2015/01/16                                         */
/*  DESCRIPTION          : transmit func body for C&S course project          */
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
#include <time.h>

#include "dbtime.h"
#include "transmit.h"
#include "strtool.h"
#include "sock.h"

#define MAX_THREAD 20
#define MAXSIZE 4096 
#define DO_PRINTF 0 

typedef struct ThreadArg   
{
    char *file_name;
    int fd;
    int start_pos;   
    int limit;   
    int no;   
}ThreadArg;   /* for pthread */
 
static void download_part(ThreadArg *targ);
static void upload_part(ThreadArg *targ);

/* prepare to recv file */
void do_Recv_File(int listenfd,int file_size,int pthread_num,char *file_name)
{
    struct sockaddr_in client;
    int addrlen;
    struct ThreadArg targs[MAX_THREAD];
    int block;
    int i;
    FILE *out;
    pthread_t tids[MAX_THREAD];
    time_t t_start, t_end;

    addrlen = sizeof(client);
    /* recieve client request */
    for(i=0;i<pthread_num;i++)
    {	
	if(-1 == (targs[i].fd = accept(listenfd,(struct sockaddr*)&client,&addrlen)))
        {
            printf("Accept failed: pthread%d\n",i);
            exit(1);
        }
    }
    printf("*****loading...\n");
    /*dbtime_start(); */
    t_start = time(NULL);

    /* create file */
    if(NULL == (out = fopen(file_name,"wb")))
    {
    	printf("Error: File CAN NOT open\n");
    	exit(1);
    } 
    fclose(out);
	
    /* file size of each pthread */ 
    block = file_size / pthread_num;
	
    for(i=0;i<pthread_num;i++)
    {   
        /* prepare information for pthread */
        targs[i].file_name = file_name;
        targs[i].start_pos = block * i; 
        targs[i].limit = block;  
	if(i == pthread_num-1)   /* the last pthread */
	{
            targs[i].limit += (file_size % pthread_num);
        }
        targs[i].no = i;

        /* recv pthread */
        pthread_create(&tids[i], NULL, (void *)download_part, (void *)&targs[i]);   
    }

    /* pthread close */
    for(i=0;i<pthread_num;i++)
    {   
        pthread_join(tids[i],NULL);
    }
    /*  dbtime_endAndShow(); */
    t_end = time(NULL);
    printf("The total is %0.fs\n",difftime(t_end,t_start));
}

/* recv file */
static void download_part(ThreadArg *targ)
{
    FILE *out;
    int len;
    char buf[MAXSIZE];
	
   /* if(1 == targ->no)
    {
        dbtime_start();
    }*/
    /* open output file */
    out = fopen(targ->file_name,"rb+");
    if(-1 == fseek(out,targ->start_pos,0))   /* find location of writting */
    {
        printf("pthread%d Error: fseek\n",targ->no);
	exit(1);
    }
    send(targ->fd,"next",4,0);
    while(1)
    {
        while((len = recv(targ->fd,buf,MAXSIZE,0))<=0);
        if(1 == isEOF(buf,len))  /* if end of this part */
        {
            break;
	}
	fwrite(buf,len,1,out);
        if(DO_PRINTF != 0) 
	{
            printf("S:pthread%d:write a buf: %dB\n",targ->no,len);
	}
	
	/* request for next */
	send(targ->fd,"next",4,0);
    }
    fclose(out);
    close(targ->fd);
   /* if(1 == targ->no)
    {
        dbtime_endAndShow();
    }*/
    pthread_exit(NULL);
}

/* prepare to send file */
void do_Send_File(int file_size,int pthread_num,char *file_name)
{
    struct ThreadArg targs[MAX_THREAD];
    pthread_t tids[MAX_THREAD];
    int block;
    int i;
 
    /* size of each pthread should write */ 
    block = file_size / pthread_num;
	
    for(i=0;i<pthread_num;i++)
    {   
        /* prepare information for pthread */
	targs[i].file_name = file_name;
        targs[i].start_pos = block * i; 
        targs[i].limit = block;
        if(i == pthread_num-1)   /* the last pthread */
        {
            targs[i].limit += (file_size % pthread_num);
        }
        targs[i].no = i;
        targs[i].fd = client_socket_Init();
        pthread_create(&tids[i], NULL, (void *)upload_part, (void *)&targs[i]);   
    }       

    /* close pthread */
    for(i=0;i<pthread_num;i++)
    {   
        pthread_join(tids[i],NULL);   
    }
}

/* upload file */
static void upload_part(ThreadArg *targ)
{
    FILE *in;
    int len;
    char buf[MAXSIZE];
    char reply[MAXSIZE];

    /* open output file */
    in = fopen(targ->file_name,"rb+");
    if(-1 == fseek(in,targ->start_pos,0))   /* find location of writting */
    {
        printf("pthread%d Error: fseek\n",targ->no);
        exit(1);
    }

    /* wait for server request start */
    while(recv(targ->fd,reply,MAXSIZE,0) != 4);
	
    while((len = fread(buf,1,MAXSIZE,in))>0)   /* read 512B to buf */
    {
        /* test whether this part has no data */
        if(targ->limit <=0)
        {
            break;
        }
        
       /* send one part of file */
       if(-1 == send(targ->fd,buf,len,0))
       {
           printf("Send file faild.\n");
           close(targ->fd);
           exit(1);
        }
        /* calculate remaind */
        targ->limit -= len;
        if(DO_PRINTF != 0) 
	{
            printf("C:pthread%d:send a buf: %dB\n",targ->no,len);
        }

        /* wait for server request next */
        while(recv(targ->fd,reply,MAXSIZE,0) != 4);
    }

    send(targ->fd,"end",3,0); /* tell server that "End of this part" */

    fclose(in);
    close(targ->fd);
    pthread_exit(NULL);
}


