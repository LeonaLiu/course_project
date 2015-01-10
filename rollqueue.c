#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define RQLEN 8192 
#define BUFSIZE 4096 /* 4k */

typedef struct rqueue
{
    char *text[RQLEN];
    int head;
    int tail;
    int num;
    int len;
}rqueueT;

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


