#include"smb2tools.h"

INT32 getSMB2ResponseSize(char *head) {
	if (!checkSMB2Signature(head))return 0;
	switch (((SMB2Header*)head)->opcode) {
	case SMB2_NEGOTIATEPROTOCOL:
		return getSMB2NegProSize(head,1);
		break;
	case SMB2_SESSIONSETUP:
		return getSMB2SesSetupSize(head, 1);
		break;
	case SMB2_TREECONNECT:
		return getSMB2TreeConSize(head, 1);
		break;
	case SMB2_LOCTL:
		return getSMB2LoctlSize(head, 1);
		break;
	case SMB2_CREATE:
		return getSMB2CreateSize(head, 1);
		break;
	case SMB2_GETINFO:
		return getSMB2GetInfoSize(head, 1);
		break;
	case SMB2_CLOSE:
		return getSMB2CloseSize(head, 1);
		break;
	case SMB2_WRITE:
		return getSMB2WriteSize(head, 1);
		break;
	case SMB2_SETINFO:
		return getSMB2SetInfoSize(head, 1);
		break;
	case SMB2_FIND:
		return getSMB2FindSize(head, 1);
		break;
	case SMB2_READ:
		return getSMB2ReadSize(head, 1);
		break;
	case SMB2_NOTIFY:
		return getSMB2NotifySize(head,1);
		break;
	case SMB2_SESSIONLOGOFF:
		return getSMB2SesLogOffSize(head,1);
		break;
	case SMB2_TREEDISCONNECT:
		return getSMB2TreeDisconSize(head,1);
		break;
	case SMB2_BREAK:
		return getSMB2BreakSize(head,1);
		break;
	case SMB2_LOCK:
		return getSMB2LockSize(head,1);
		break;
	case SMB2_FLUSH:
		return getSMB2FlushSize(head,1);
	case SMB2_KEEPALIVE:
		return getSMB2KeepAliveSize(head,1);
		break;

	default:
		return 0;
	}
}

INT32 getSMB2RequestSize(char* head) {
	if (!checkSMB2Signature(head))return 0;
	switch (((SMB2Header*)head)->opcode) {
	case SMB2_NEGOTIATEPROTOCOL:
		return getSMB2NegProSize(head, 0);
		break;
	case SMB2_SESSIONSETUP:
		return getSMB2SesSetupSize(head, 0);
		break;
	case SMB2_TREECONNECT:
		return getSMB2TreeConSize(head, 0);
		break;
	case SMB2_LOCTL:
		return getSMB2LoctlSize(head, 0);
		break;
	case SMB2_CREATE:
		return getSMB2CreateSize(head, 0);
		break;
	case SMB2_GETINFO:
		return getSMB2GetInfoSize(head, 0);
		break;
	case SMB2_CLOSE:
		return getSMB2CloseSize(head, 0);
		break;
	case SMB2_WRITE:
		return getSMB2WriteSize(head, 0);
		break;
	case SMB2_SETINFO:
		return getSMB2SetInfoSize(head, 0);
		break;
	case SMB2_FIND:
		return getSMB2FindSize(head, 0);
		break;
	case SMB2_READ:
		return getSMB2ReadSize(head, 0);
		break;
	case SMB2_TREEDISCONNECT:
		return getSMB2TreeDisconSize(head,0);
		break;
	case SMB2_NOTIFY:
		return getSMB2NotifySize(head,0);
		break;
	case SMB2_CANCEL:
		return getSMB2CancelSize(head,0);
		break;
	case SMB2_SESSIONLOGOFF:
		return getSMB2SesLogOffSize(head,0);
		break;
	case SMB2_BREAK:
		return getSMB2BreakSize(head,0);
		break;
	case SMB2_LOCK:
		return getSMB2LockSize(head,0);
		break;
	case SMB2_FLUSH:
		return getSMB2FlushSize(head,0);
	case SMB2_KEEPALIVE:
		return getSMB2KeepAliveSize(head,0);
		break;

	default:
		return 0;
	}
}

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

void getNetBIOSInfo(char* head,int r,struct NetBIOSInfo* info){
	INT32 nbSize=getNetBIOSSize(head);
	INT32 smbSize;

	//������
	INT32 sum=0;


	char* temp=head;
	int i=0;
	temp+=4;
	info->smb2Num=0;
	if(r==0){
		smbSize=getSMB2ResponseSize(temp);
		if(smbSize+66>=nbSize){
			info->smb2Num=1;
			info->smb2Size[0]=nbSize;
			info->opcode[0]=((SMB2Header*)(temp))->opcode;
		}else{
			while(checkSMB2Signature(temp)&&temp<=head+nbSize+4){
				smbSize=align8Bytes(getSMB2ResponseSize(temp));
				info->opcode[i]=((SMB2Header*)(temp))->opcode;
				info->smb2Num++;
				info->smb2Size[i]=smbSize;
				temp += smbSize;
				sum+=smbSize;
				i++;
			}
			if(sum>nbSize+7||sum<nbSize)printf("smb2 header not aligned");
		}
	}else{
		smbSize=getSMB2RequestSize(temp);
		if(smbSize+66>=nbSize){
			info->smb2Num=1;
			info->smb2Size[0]=nbSize;
			info->opcode[0]=((SMB2Header*)(temp))->opcode;
		}else{
			while(checkSMB2Signature(temp)&&temp<head+nbSize+4){
				if(nbSize==217&&getBug(temp)==BUG_GETINFO_SIZE1)
					smbSize=getSMB2RequestSize(temp)+8;
				else smbSize=align8Bytes(getSMB2RequestSize(temp));
				info->opcode[i]=((SMB2Header*)(temp))->opcode;
				info->smb2Num++;
				info->smb2Size[i]=smbSize;
				temp+=smbSize;
				sum+=smbSize;
				i++;
			}
			if(sum>nbSize+7||sum<nbSize)printf("smb2 header not aligned");
		}
	}
}









/***����Ϊ��ȡ����smb2�������С�ĺ���***/
INT32 getSMB2NegProSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp),i;
	INT32 sizeTemp = 0;
	if (r == 1) {
		//return (INT32)(*(INT16*)(temp + 56+headSize))+(INT32)(*(INT16*)(temp+headSize+58));
		if (*(INT16*)(temp + headSize + 6)) {
			sizeTemp = *(INT32*)(temp + headSize + 60);
			for (i = *(INT16*)(temp + headSize + 6); i > 0; i--) {
				if (i > 1)sizeTemp += align8Bytes((INT32)(8 + *(INT16*)(temp + sizeTemp + 2)));
				else sizeTemp += (INT32)(8 + *(INT16*)(temp + sizeTemp + 2));
			}return sizeTemp;
		}
		else {
			return (INT32)(*(INT16*)(temp + 56+headSize))+(INT32)(*(INT16*)(temp+headSize+58));
		}
	}
	else {
		if (*(INT32*)(temp + headSize + 92)) {
			sizeTemp = *(INT32*)(temp+ 92);
			for (i = *(INT16*)(temp + headSize + 96); i > 0; i--) {
				if(i>1)sizeTemp += align8Bytes((INT32)(8 + *(INT16*)(temp + sizeTemp + 2)));
				else sizeTemp += (INT32)(8 + *(INT16*)(temp + sizeTemp + 2));
			}
			return sizeTemp;
		}
		else {
			sizeTemp += (INT32)(*(INT16*)(temp + headSize));
			sizeTemp += headSize;
			sizeTemp += (*(INT16*)(temp + headSize + 2)) << 1;
			return sizeTemp;
		}
			
	}
}
INT32 getSMB2SesSetupSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {
		if(*(INT16*)(temp+headSize+4))return (INT32)(*(INT16*)(temp+4+headSize))+ (INT32)(*(INT16*)(temp + headSize + 6));
		else return (INT32)(headSize + 9);
	}
	else {		/**********δ����*********/
		return (INT32)(*(INT16*)(temp + 12 + headSize)) + (INT32)(*(INT16*)(temp + headSize + 14));
	}
}
INT32 getSMB2TreeConSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {
		return (INT32)(16 + headSize);
	}
	else {			/**********δ����*********/
		return (INT32)(*(INT16*)(temp + 4 + headSize)) + align8Bytes((INT32)(*(INT16*)(temp + headSize + 6))+4)-(INT32)4;
	}
}
INT32 getSMB2LoctlSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {
		if ((*(INT16*)(temp + headSize)) == 0x09) {
			return (INT32)(9 + headSize);
		}
		else {      /*���indata ��outdata���У������ֵ����ȶ*/
			return (INT32)(   /**********δ����*********/
				*(INT32*)(temp + headSize + 24) + *(INT32*)(temp + 28 + headSize) + *(INT32*)(temp + 36 + headSize)
				);
		}
	}
	else {/**********δ����*********/
		return (*(INT32*)(temp + headSize + 24) + *(INT32*)(temp + headSize + 28) + *(INT32*)(temp + headSize + 40));
	}
}

INT32 getSMB2CreateSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		if ((*(INT16*)(temp + headSize)) == 0x09) {
			return (INT32)(9 + headSize);
		}
		else {/**********δ����*********/
			if(*(INT32*)(temp + headSize + 80))return (INT32)(*(INT32*)(temp + headSize + 80) + *(INT32*)(temp + headSize + 84));
			else return (INT32)(88+headSize);
		}
	}
	else {/**********δ����*********/
		if (*(INT32*)(temp + headSize + 48)) {
			return (INT32)(*(INT32*)(temp + 48 + headSize) + *(INT32*)(temp + 52 + headSize));
		}
		else if (*(INT16*)(temp + 46 + headSize) &&*(INT16*)(temp + 44 + headSize)) {
			//return (INT32)(*(INT16*)(temp + 44 + headSize) + align8Bytes((INT32)*(INT16*)(temp + 46 + headSize)));
			return (INT32)(*(INT16*)(temp + 44 + headSize) +((INT32)*(INT16*)(temp + 46 + headSize)));
		}
		else return (INT32)(headSize + 64);
	}
}

INT32 getSMB2GetInfoSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		if (((SMB2Header*)temp)->nt_status == SMB2_NT_STATUS_SUCCESS || ((SMB2Header*)temp)->nt_status == SMB2_NT_STATUS_BUFFER_OVERFLOW) {
			return (INT32)(
				(INT32)(*(INT16*)(temp+headSize+2))+*(INT32*)(temp+headSize+4)
				);
		}
		else if (((SMB2Header*)temp)->nt_status == SMB2_NT_STATUS_BUFFER_TOO_SMALL) {
			return (INT32)(headSize + 12);/**********δ����*********/
		}
		else {/**********δ����*********/
			return (INT32)(headSize + 9);
		}
	}
	else {
		if (((*(char*)(temp + headSize + 2) == (char)0x01 && (*(char*)(temp + headSize + 3) == 0x12 )||(*(char*)(temp+headSize+3)==0x06)) ||(*(char*)(temp + headSize + 2) == (char)0x02 && (*(char*)(temp + headSize + 3) == 0x05 || *(char*)(temp + headSize + 3) == 0x04))) && *(INT16*)(temp + headSize + 38) == 0)
		//if( *(INT16*)(temp + headSize + 36) != 0xff)
		{				//δ֪ԭ��
			return (INT32)(headSize + 41);
		}
		return (INT32)(headSize + 40);
	}
}
INT32 getSMB2CloseSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		if (((SMB2Header*)temp)->nt_status == SMB2_NT_STATUS_SUCCESS) {
			return (INT32)(headSize + 60);
		}
		else return (INT32)(headSize + 9);
	}
	else {/**********δ����*********/
		return (INT32)(headSize + 24);
	}
}
INT32 getSMB2WriteSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {   /*��������******************************************/
		if(*(char*)(temp+headSize+16))return (INT32)(headSize + 17);
		else return (INT32)(headSize + 16);
	}
	else {/**********δ����*********/
		return (INT32)((INT32)(*(INT16*)(temp + headSize + 2)) + *(INT32*)(temp + headSize + 4));
	}
}
INT32 getSMB2SetInfoSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		return (INT32)(headSize + 2);
	}
	else {/**********δ����*********/
		return (INT32)((INT32)(*(INT16*)(temp + headSize + 8))+*(INT32*)(temp + headSize + 4));
	}
}
INT32 getSMB2FindSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		if(((SMB2Header*)temp)->nt_status==SMB2_NT_STATUS_SUCCESS)
			return (INT32)((INT32)(*(INT16*)(temp + headSize + 2)) + *(INT32*)(temp + headSize + 4));
		else return (INT32)(headSize + 9);
	}
	else {/**********δ����*********/
		return (INT32)((INT32)(*(INT16*)(temp + headSize + 24)) + (INT32)(*(INT16*)(temp + headSize + 26)));
	}
}
INT32 getSMB2ReadSize(char* temp, int r) {
	INT16 headSize = getSMB2HeadSize(temp);
	if (r == 1) {/**********δ����*********/
		if (((SMB2Header*)(temp))->nt_status == SMB2_NT_STATUS_END_OF_FILE) {
			return (INT32)(headSize + 9);
		}
		return (INT32)((INT32)(*(char*)(temp + headSize + 2)) + *(INT32*)(temp + headSize + 4));
	}
	else {/**********δ����*********/
		return (INT32)(headSize + 49);
	}
}
INT32 getSMB2TreeDisconSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	return (INT32)(headSize+4);
}
INT32 getSMB2NotifySize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	if(r==1){
		if(((SMB2Header*)(temp))->nt_status==SMB2_NT_STATUS_PENDING||\
			((SMB2Header*)(temp))->nt_status==SMB2_NT_STATUS_CANCELLED){
			return (INT32)(headSize+9);
		}else if(((SMB2Header*)(temp))->nt_status==SMB2_NT_STATUS_NOTIFY_ENUM_DIR){
			return (INT32)(headSize+8);
		}else if(((SMB2Header*)(temp))->nt_status==SMB2_NT_STATUS_SUCCESS&&(*(INT16*)(temp+headSize+2))) 
		{return (INT32)(*(INT16*)(temp+headSize+2)+*(INT32*)(temp+headSize+4));}
		else return (INT32)(headSize+8);
	}else{
		return (INT32)(headSize+32);
	}
}
INT32 getSMB2CancelSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	return (INT32)(headSize+4);
}
INT32 getSMB2SesLogOffSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	return (INT32)(headSize+4);
}
INT32 getSMB2BreakSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	return (INT32)(headSize+24);
}
INT32 getSMB2KeepAliveSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	return (INT32)(headSize+4);
}
INT32 getSMB2FlushSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	if(r==1)return (INT32)(headSize+4);
	else return (INT32)(headSize+24);
}
INT32 getSMB2LockSize(char* temp,int r){
	INT16 headSize=getSMB2HeadSize(temp);
	if(r==1)return (INT32)(headSize+4);
	else return (INT32)(headSize+52);
}








int getBug(char *head) {
	 char* tmp1;
	if (((SMB2Header*)(head))->opcode == SMB2_GETINFO&&((SMB2Header*)(head))->r==0) {
		tmp1 = head + getSMB2HeadSize(head);
		if (*(char*)(tmp1 + 2) == (char)0x02 && *(char*)(tmp1 + 3) == (char)0x01) {
			return BUG_GETINFO_SIZE1;
		}
	}
	return BUG_UNKNOWN;
}