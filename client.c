/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : client.c                                           */
/*  PRINCIPAL AUTHOR     : LiuYang                                            */
/*  SUBSYSTEM NAME       : C&S transmit                                       */
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
 * v2.0 by liuyang, 2015/01/13  pthread
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/stat.h> 
#include <unistd.h> 

#include "transmit.h"
#include "strtool.h"
#include "sock.h"

#define IN_FILE_NAME "test"  /* test file name */
#define MAXSIZE 64 
#define NUM_PTHREAD 4 

int main()
{
    int fd;
    char reply[MAXSIZE];
    char buf[MAXSIZE];
    int len;
    struct stat fileInfo;
    
    /* get file size */
    stat(IN_FILE_NAME,&fileInfo);
    
    /* create a TCP socket */
    fd = client_socket_Init();
    
    /* send file size */
    len = IntToString(fileInfo.st_size,buf);
    send(fd,(void*)buf,len,0);
    while(recv(fd,reply,MAXSIZE,0) != 2);
    
    /* send pthread number */
    len = IntToString(NUM_PTHREAD,buf);
    send(fd,(void*)buf,len,0);
    while(recv(fd,reply,MAXSIZE,0) != 2);
    close(fd);

    /* start to send file */
    do_Send_File(fileInfo.st_size,NUM_PTHREAD,IN_FILE_NAME);
	
    return 0;
}
