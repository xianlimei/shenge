#pragma once
#ifndef __SMB2TOOLS
#define __SMB2TOOLS

#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>

#define INT16 uint16_t
#define INT32 uint32_t
#define INT64 uint64_t


#define SMB2_NEGOTIATEPROTOCOL 0x00
#define SMB2_SESSIONSETUP 0x01
#define SMB2_SESSIONLOGOFF 0x02
#define SMB2_TREECONNECT 0x03
#define SMB2_TREEDISCONNECT 0x04
#define SMB2_CREATE 0x05
#define SMB2_CLOSE 0x06
#define SMB2_FLUSH 0x07
#define SMB2_READ 0x08
#define SMB2_WRITE 0x09
#define SMB2_LOCK 0x0a
#define SMB2_LOCTL 0x0b
#define SMB2_CANCEL 0x0c
#define SMB2_KEEPALIVE 0x0d
#define SMB2_FIND 0x0e
#define SMB2_NOTIFY 0x0f
#define SMB2_GETINFO 0x10
#define SMB2_SETINFO 0x11
#define SMB2_BREAK 0x12


#define SMB2_NT_STATUS_SUCCESS 0x00000000
#define SMB2_NT_STATUS_BUFFER_OVERFLOW 0x80000005
#define SMB2_NT_STATUS_BUFFER_TOO_SMALL 0xc0000023
#define SMB2_NT_STATUS_NO_MORE_FILES 0x80000006
#define SMB2_NT_STATUS_END_OF_FILE 0xc0000011
#define SMB2_NT_STATUS_PENDING 0x00000103
#define SMB2_NT_STATUS_CANCELLED 0xC0000120
#define SMB2_NT_STATUS_NOTIFY_ENUM_DIR 0x0000010C



typedef struct NetBIOSHeader {
	INT32 message_type : 8;
	INT32 length : 24;
} NetBIOSHeader;

typedef struct NetBIOSInfo{
	int opcode[10];
	INT32 smb2Size[10];
	int smb2Num;
} commandNum_s;
typedef commandNum_s *commandNum_p;

typedef struct SMB2Header {
	INT32 SMB2_signature;
	INT16 header_length;
	INT16 credits_charge;
	INT32 nt_status;
	INT16 opcode;
	INT16 credits_granted;
	INT32 r : 1;
	INT32 p : 1;
	INT32 c : 1;
	INT32 s : 1;
	INT32 priority : 3;
	INT32 flags_padding1 : 21;
	INT32 dfs_operation : 1;
	INT32 reply_operation : 1;
	INT32 flags_padding : 2;
	INT32 chain_offset;
	INT64 command_sequence_number;
	INT32 pid;
	INT32 tid;
	INT64 userID;
	char signature_address;
    
}SMB2Header;


//获得SMB2回应包数据流中一个包的大小   指针指向SMB2头
INT32 getSMB2ResponseSize(char* head);

//获得SMB2请求包数据流中一个包的大小   指针指向SMB2头
INT32 getSMB2RequestSize(char* head);

//获取一个NetBIOS包的总大小，里面可能包含多个SMB2包
INT32 getNetBIOSSize(char* head);

//网络字节转到主机字节，其中bitNum标志位数，必须是8的倍数，最大32位
INT32 nb2hb(INT32 temp, int bitNum);

//获取SMB2头大小，错误返回0
INT16 getSMB2HeadSize(char* head);

//根据smb2头签名判断是不是smb2头....不是返回0，是则返回1
int checkSMB2Signature(char* head);

//获取SMB2包类型
INT16 getSMB2Type(char* head);

//获取SMB2包的r标志位，判断是否是响应
int getSMB2R(char* head);

//使输入的数字8字节对齐（一个NetBIOS中包含的多个smb命令当中，命令8字节对齐
INT32 align8Bytes(INT32 temp);

//调试显示协议类型
void printSMB2Type(INT16 opcode);

//输出netbios里面的smb2大小信息
void getNetBIOSInfo(char* head,int r,struct NetBIOSInfo* info);





/**以下为获取不同命令的长度（包括命令头）的函数群，其中temp指向头，r判断是否是响应***/
INT32 getSMB2NegProSize(char* temp,int r);
INT32 getSMB2SesSetupSize(char* temp, int r);
INT32 getSMB2TreeConSize(char* temp, int r);
INT32 getSMB2LoctlSize(char* temp, int r);
INT32 getSMB2CreateSize(char* temp, int r);
INT32 getSMB2GetInfoSize(char* temp, int r);
INT32 getSMB2CloseSize(char* temp, int r);
INT32 getSMB2WriteSize(char* temp, int r);
INT32 getSMB2SetInfoSize(char* temp, int r);
INT32 getSMB2FindSize(char* temp, int r);
INT32 getSMB2ReadSize(char* temp, int r);
INT32 getSMB2TreeDisconSize(char* temp,int r);
INT32 getSMB2NotifySize(char* temp,int r);
INT32 getSMB2CancelSize(char* temp,int r);
INT32 getSMB2SesLogOffSize(char* temp,int r);
INT32 getSMB2BreakSize(char* temp,int r);
INT32 getSMB2KeepAliveSize(char* temp,int r);
INT32 getSMB2LockSize(char* temp,int r);
INT32 getSMB2FlushSize(char* temp,int r);
/**************************************************************************************************************/

//用来获取未知bug类型
#define BUG_UNKNOWN 0
#define BUG_GETINFO_SIZE1 1
int getBug(char *head);








#endif