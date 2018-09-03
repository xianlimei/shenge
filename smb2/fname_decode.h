#ifndef __FNAME_DECODE
#define __FNAME_DECODE

#include<stdio.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
FILE* openFileByString(char* temp);
int  CreateDir(const char *sPathName);
#endif