
#include"smb2tools.h"

//struct test { int a : 1; int b : 31; };

int main() {

	
	FILE* test; //�ļ�ָ��
	INT32 size;  //�ļ�����
	char* buffer;  //��ȡ��������
	INT32 result;  //���ض�ȡ����
	char* temp;
	int index = 0,i,j=1;
	INT32 smbSize;
	INT32 netBIOSSize,smb2SizeSum=0;
	commandNum_s gNetBIOSInfo;

	int isResponse = 1;

	test = fopen("smb2_otherPro", "rb");

	//test = fopen("iftest_response", "rb");	//done
	//test = fopen("read1_response", "rb");		//done
	//test = fopen("iftest_request", "rb");		//done
	//test = fopen("read1_request", "rb");		//done
	//test = fopen("read1_1_request", "rb");     //    //done
	//test = fopen("iftest2//iftest2_stream6_response", "rb");	//done
	//test=fopen("iftest2//iftest2_stream6_request","rb");		//done
	//test = fopen("iftest3//iftest3_request", "rb"); 
	//test=fopen("iftest3_1_request","rb");
	//test = fopen("iftest3//iftest3_response", "rb");   //done
	//test=fopen("mytest1//mytest1_stream0_request","rb");  //done
	//test=fopen("mytest1//mytest1_stream1_request","rb");  //done
	//test=fopen("mytest1//mytest1_stream0_response","rb");  //done
	//test=fopen("mytest1//mytest1_stream1_response","rb");  //done
	//test=fopen("mytest2//mytest2_request_stream1","rb"); //1-4  //done
	//test=fopen("mytest2//mytest2_response_stream2","rb");  //1-4   //done
	//test=fopen("mytest3//mytest3_request","rb");	//done
	//test=fopen("mytest3//mytest3_response","rb");	//done
	//test=fopen("mytest4//mytest4_1_request","rb");	//done
	//test=fopen("mytest4//mytest4_2_request","rb");	//done
	//test=fopen("mytest4//mytest4_2_request_1","rb");	//done
	//test=fopen("mytest4//mytest4_1_response","rb");	   //done
	//test=fopen("mytest4//mytest4_2_response","rb");		//done
	//test=fopen("mytest4//mytest4_2_response_1","rb");		//done


	if (test == NULL) { printf("�ļ���ȡʧ��"); return -1; }

	//��ȡ�ļ���С
	fseek(test, 0, SEEK_END);
	size = ftell(test);
	rewind(test); // ����rewind()���ļ�ָ���Ƶ���stream(��)ָ���Ŀ�ʼ��, ͬʱ���������صĴ����EOF���

	buffer = (char*)malloc(sizeof(char)*size);
	if (buffer == NULL) { printf("�ڴ����ʧ��"); fclose(test); return -1; }

	result = fread(buffer, 1, size, test);
	if (result != size) { printf("reading error"); fclose(test); return -1; }
	fclose(test);


	temp = buffer;


	while(1){
		printf("��%d����\n",j++);
		getNetBIOSInfo(temp,isResponse,&gNetBIOSInfo);
		smb2SizeSum=0;
		netBIOSSize=getNetBIOSSize(temp);
		printf("netbios_size_%d\n",netBIOSSize);
		for(i=0;i<gNetBIOSInfo.smb2Num;i++){
			smb2SizeSum+=gNetBIOSInfo.smb2Size[i];
			if(smb2SizeSum>netBIOSSize+7){printf("size_large");goto OUT;}
			printSMB2Type(gNetBIOSInfo.opcode[i]);
			printf("%d\n",gNetBIOSInfo.smb2Size[i]);
		}
		if(smb2SizeSum<netBIOSSize){printf("size_small");break;}
		printf("\n\n");
		temp+=netBIOSSize+4;
		if(temp>=buffer + (size - 1)){printf("getallpackage");break;}
	}
OUT:


	
	free(buffer);
	getchar();
	return 0;
}



/*	
	if (isResponse) {
		while (1) {
			index++;

			printf("��%d\n", index);
			netBIOSSize = getNetBIOSSize(temp);
			printf("%lu\n", netBIOSSize);
			printSMB2Type(((SMB2Header*)(temp + 4))->opcode);
			smbSize = getSMB2ResponseSize(temp + 4);
			if (!smbSize) { printf("wrong\n"); getchar(); }
			printf("%d\n\n", smbSize);
			temp += smbSize + 4;
			if (smbSize && (netBIOSSize > smbSize) && checkSMB2Signature(temp + align8Bytes(smbSize) - smbSize)) {
				temp += align8Bytes(smbSize) - smbSize;
				while (checkSMB2Signature(temp)) {
					printf("%d\n\n", getSMB2ResponseSize(temp));
					temp += align8Bytes(getSMB2ResponseSize(temp));
				}
			}
			if (temp >= buffer + (size - 1)) { printf("get all successful"); break; }
		}
	}
	else {
		while (1) {
			index++;
			smb2SizeSum=0;
			
			printf("��%d\n", index);
			netBIOSSize = getNetBIOSSize(temp);
			printf("%lu\n", netBIOSSize);
			printSMB2Type(((SMB2Header*)(temp + 4))->opcode);
			smbSize = getSMB2RequestSize(temp + 4);
			if (!smbSize) { printf("wrong\n"); getchar(); }
			printf("%d\n\n", smbSize);
			temp += smbSize + 4;
			smb2SizeSum+=smbSize;
			if (smbSize && (netBIOSSize > smbSize)) {
				if(getBug(temp-smbSize)==BUG_GETINFO_SIZE1){
					temp+=8;
					smb2SizeSum+=8;
				}
				if(checkSMB2Signature(temp + align8Bytes(smbSize) - smbSize)){
					temp += align8Bytes(smbSize) - smbSize;
					smb2SizeSum+=align8Bytes(smbSize)-smbSize;
					while (checkSMB2Signature(temp)) {
				
						printf("%d\n\n", getSMB2RequestSize(temp));
					if((smb2SizeSum+getSMB2RequestSize(temp))==netBIOSSize)temp+=getSMB2RequestSize(temp);
						else {temp += align8Bytes(getSMB2RequestSize(temp));smb2SizeSum+=align8Bytes(getSMB2RequestSize(temp));}
				}
				}}
			if (temp >= buffer + (size - 1)) { printf("get all successful"); break; }
		}
	}
*/	
