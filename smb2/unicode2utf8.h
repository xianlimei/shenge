

#ifndef __UNICODE2UTF8_H
#define __UNICODE2UTF8_H


#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>

size_t getUtf8LenFromUnicode(char* inputUnicode, int inlen);

//把unicode转换成utf-8，如果失败返回-1,成功返回0
size_t unicode2utf8(char* input,size_t inlen,char* output,size_t outlen);

#endif
