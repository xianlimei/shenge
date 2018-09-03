#pragma once
#ifndef __SHORTNAMELIST
#define __SHORTNAMELIST

#include<stdio.h>
#include<string.h>
#include"smb2tools.h"
#include"smb2Name.h"
#include"unicode2utf8.h"


#define ShortName FileName

#define LIST_NUM 256
#define FNV1_PRIME 0x20180526
#define FNV1_INIT  0x19990424

#define NameList  struct NameNode**

struct NameNode{
    struct NameNode* next,*prev;
    struct FileName filename;
    struct ShortName shortname;
};

//extern struct NameNode* gNameList[LIST_NUM];

NameList getNewNameList();

unsigned getHash(char* name);

//ÓÃÓÚžøœÚµã·ÖÅä¿ÕŒä£¬·ñÔò·µ»ØNULL
struct NameNode* allocNameNode();

void fprintNameList(NameList list);
void fprintNameNode(struct NameNode* temp,FILE* ShortNameList);

void addNameNodeToList(NameList list,struct NameNode* item);
void delNameNodeToList(NameList list,struct NameNode* item);

void freeNameList(NameList temp);

struct NameNode* searchNNByShortName(NameList list,char* shortname);

void printNameNode(struct NameNode* head);
void printNameList(NameList list);

//输入find包的头指针，如果里面含有对应文件名缩写信息，则添加到表里面，返回0,否则返回-1
//FileInformationClass也需要等于3或者25
int setNameNode(NameList list,char* head);

char* getUTF8Name(char* head);
char* getUTF8ShortName(char* head);
int checkNameList(NameList list);

unsigned getShortNameListNum(NameList list);

void replaceShortName(char* string,NameList list,char** resltString);



#endif