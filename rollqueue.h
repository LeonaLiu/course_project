#include <string.h>

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

void DeQueue(rqueueT *queue,char *out);
void EnQueue(rqueueT *queue,char *in);
int IsEmpty(rqueueT *queue);
int IsFull(rqueueT *queue);
void InitQueue(rqueueT *queue);
void FreeQueue(rqueueT *queue);


