//
// Created by ted on 18-7-23.
//

#include"smb2tools.h"
INT32 getNetBIOSSize(char *head) {

    return nb2hb((INT32)(((NetBIOSHeader*)head)->length),24);
}

INT32 nb2hb(INT32 temp, int bitNum) {
    INT32 result = 0, mask = 0xff;
    while ((bitNum -= 8) >= 0) {
        result <<= 8;
        result += temp&mask;
        temp >>= 8;
    }
    return result;
}

INT16 getSMB2HeadSize(char* head) {
    return (INT16)(((SMB2Header*)head)->header_length);
}

int checkSMB2Signature(char* head) {
    return !( ((SMB2Header*)head)->SMB2_signature!=(INT32)1112364030);
}

INT16 getSMB2Type(char* head) {
    return ((SMB2Header*)head)->opcode;
}

int getSMB2R(char* head) {
    return (int)(((SMB2Header*)head)->r);
}

INT32 align8Bytes(INT32 temp) {
    return (INT32)(temp + 7)&~(7);
}

void printSMB2Type(INT16 opcode) {
    switch (opcode) {
        case SMB2_NEGOTIATEPROTOCOL:
            printf("negotia\n");
            break;
        case SMB2_SESSIONSETUP:
            printf("session set up\n");
            break;
        case SMB2_TREECONNECT:
            printf("tree connect\n");
            break;
        case SMB2_LOCTL:
            printf("loctl\n");
            break;
        case SMB2_CREATE:
            printf("create\n");
            break;
        case SMB2_GETINFO:
            printf("getinfo\n");
            break;
        case SMB2_CLOSE:
            printf("close\n");
            break;
        case SMB2_WRITE:
            printf("write\n");
            break;
        case SMB2_SETINFO:
            printf("setinfo\n");
            break;
        case SMB2_FIND:
            printf("find\n");
            break;
        case SMB2_READ:
            printf("read\n");
            break;
        case SMB2_TREEDISCONNECT:
            printf("treedisconnect\n");
            break;
        case SMB2_NOTIFY:
            printf("notify\n");
            break;
        case SMB2_CANCEL:
            printf("cancel\n");
            break;
        case SMB2_SESSIONLOGOFF:
            printf("sessionlogoff\n");
            break;
        case SMB2_BREAK:
            printf("break\n");
            break;
        case SMB2_LOCK:
            printf("lock\n");
            break;
        case SMB2_FLUSH:
            printf("flush\n");
            break;
        case SMB2_KEEPALIVE:
            printf("keepalive\n");
            break;

        default:
            printf("UNKNOW\n");
    }
}


void getNetBIOSInfo(char* head,struct NetBIOSInfo* info){
    int i;
    info->size=getNetBIOSSize(head);
    info->smb2Num=0;
    head+=4;
    while(((struct SMB2Header*)head)->chain_offset!=0){
        info->smb2Size[info->smb2Num]=((struct SMB2Header*)head)->chain_offset;
        head+=((struct SMB2Header*)head)->chain_offset;
        info->smb2Num++;

    }
    info->smb2Size[info->smb2Num]=info->size;
    for(i=0;i<info->smb2Num;i++){
        info->smb2Size[info->smb2Num]-=info->smb2Size[i];
    }
    info->smb2Num++;
}