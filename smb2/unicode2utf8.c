//
// Created by ted on 18-7-23.
//

#include"unicode2utf8.h"

size_t getUtf8LenFromUnicode(char* inputUnicode,int inlen){
    size_t i,result=0;
    for(i=0;i<inlen;i+=2){
        if(*(unsigned short int*)(inputUnicode+i)==0)return result;
        else if(*(unsigned short int*)(inputUnicode+i)<=0x7f){
            result+=1;
        }else if(*(unsigned short int*)(inputUnicode+i)<=0x7FF){
            result+=2;
        }else{
            result+=3;
        }

    }
    return result;
}


size_t unicode2utf8(char* input,size_t inlen,char* output,size_t outlen){
    iconv_t cd=iconv_open("UTF-8","UNICODE");
    size_t result;
    if(cd==(iconv_t)-1){
        printf("initial the iconv failed");
        return -1;
    }
    memset(output,0,outlen);
    if((result=iconv(cd,&input,&inlen,&output,&outlen))==(size_t)-1){
        iconv_close(cd);
        printf("conv failed");
        return -1;
    }
    iconv_close(cd);
    return result;
}
