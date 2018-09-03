#pragma once
#ifndef __SMB2NAME
#define __SMB2NAME

#include<stdio.h>
#include<string.h>
#include"smb2tools.h"

struct FileName{
    char* name;
};

struct Fid{
    INT64 pers;
    INT64 vola;
};


//ÓÃÓÚŽ¢ŽæÎÄŒþÃûºÍFIDÐÅÏ¢µÄœÚµã
struct FNode{
    struct FNode* next,*prev;
    struct FileName filename;
    struct Fid fid;
};

struct FNode_tobe{
    struct NFode_tobe* next,*prev;
    struct FNode* fnode;
    INT64 messageId;
};


//ÓÃÓÚ±ÈœÏÁœžöfidÊÇ·ñÏàÍ¬ÏàÍ¬·µ»Ø1£¬·ñÔò·µ»Ø0
int fidCmp(struct Fid* tmp1,struct Fid* tmp2);

//ÓÃÓÚ±ÈœÏÁœžöÎÄŒþÃûÊÇ·ñÏàÍ¬£¬ÏàÍ¬·µ»Ø1£¬·ñÔò·µ»Ø0
int fnameCmp(struct FileName* tmp1,struct FileName* tmp2);

//ÓÃÓÚžøFNode·ÖÅä¿ÕŒäÈç¹ûÎÞ·š·ÖÅä¿ÕŒäÔò·µ»ØNULL
struct FNode* allocFNode();

//ÓÃÓÚžøË«ÁŽ±íÌíŒÓœÚµã,ÌíŒÓÎ»ÖÃÊÇÁŽ±íµÄ×îÇ°¶Ë
void addFNodeToArray(struct FNode* head,struct FNode* item);

//ÓÃÓÚÉŸ³ýÁŽ±íÖÐµÄÔªËØ
void delFNodeFromArray(struct FNode* head,struct Fid* fid);

//ÓÃÓÚ²éÕÒ¶ÔÓŠfidÊÇ·ñÔÚÁŽ±íÖÐ,·µ»Ø¶ÔÓŠœÚµãÖžÕë£¬·ñÔò·µ»ØNULL
struct FNode* searchFNodeByFid(struct FNode* head,struct Fid* fid);
struct FNode* searchFNodeByName(struct FNode* head,struct FileName* filename);

//ÓÃÓÚŽòÓ¡µ¥žöfid
void printFNode(struct FNode* tmp);
//ÓÃÓÚŽòÓ¡ËùÓÐŽò¿ªµÄfid
void printFNodeArray(struct FNode* head);

//ÓÃÓÚcreate_requestÃüÁîµ÷ÓÃ
//void requestFid(char* head);

//ÓÃÓÚcreate_requestÃüÁîµ÷ÓÃ
//void responseFid(char* head);


#endif