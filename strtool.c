/******************************************************************************/
/* Copyright (C) sa614381@mail.ustc.edu.cn 2014-2015                          */
/*                                                                            */
/*  FILE NAME            : strtool.c                                           */
/*  PRINCIPAL AUTHOR     : LiuYang                                            */
/*  SUBSYSTEM NAME       : C&S transmit                                       */
/*  MODULE NAME          : strtool                                            */
/*  LANUAGE              : C                                                  */
/*  TARGET ENVIROMENT    : ANY                                                */
/*  DATE OF FIRST RELEASE: 2015/01/16                                         */
/*  DESCRIPTION          : string tool for C&S course project                 */
/******************************************************************************/
 
/*
 *  Revision log:
 *
 * Created by Liuyang, 2015/01/16 string tool for C&S course project
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strtool.h"

/* string to integer, reverse */
int StringToInt(int len,char *buf)
{
    int i;
    int sum = 0;
    int temp;
	
    for(i=len-1;i>=0;i--)
    {
        temp = buf[i] - '0';
        sum = sum * 10 + temp;
    }
    return sum;
}

/* integer to string , reverse */
int IntToString(int n,char *buf)
{
    int i;
    
    if(n<=0)
    {
        printf("Wrong IntToString : n must not be negative\n");
        exit(1);
    }
    for(i=0;n>0;i++,n=n/10)
    {
        buf[i] = n % 10 + '0';
    }
    buf[i] = '\0';
    return i;
}

/* judge if the buf = "end" */
int isEOF(char *buf,int len)
{
    if(3 == len)
    {
    	if('e' == buf[0] && 'n' == buf[1] && 'd' == buf[2])
        {
            return 1;
        }
    }
    return 0;
}

