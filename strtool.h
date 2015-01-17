#ifndef STRTOOL_H
#define STRTOOL_H

#include <string.h>

/* string to integer, reverse */
int StringToInt(int len,char *buf);

/* integer to string , reverse */
int IntToString(int n,char *buf);

/* judge if the buf = "end" */
int isEOF(char *buf,int len);


#endif
