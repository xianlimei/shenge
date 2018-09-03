#include"fname_decode.h"


FILE* openFileByString(char* temp){
    int i,len=strlen(temp);
    char* dir;
    for(i=len-1;i>0;i--){
      if(temp[i]=='/'){
      	dir=malloc(i+3);
      	dir[0]='.';
      	dir[1]='/';
      	dir[i+2]='\0';
		strncpy(dir+2,temp,i);
		if(CreateDir(dir) == -1)
		    return  NULL;
	    break;
      }
    }
    
    return fopen(temp,"w+");
}


int   CreateDir(const   char   *sPathName)  
  {  
    char   DirName[256];  
    strcpy(DirName, sPathName);
    int   i,len = strlen(DirName);
    if(DirName[len-1]!='/')
        strcat(DirName, "/");
   
    len = strlen(DirName);
   
    for(i=1; i<len; i++)
    {
	    if(DirName[i]=='/')
	    {
	        DirName[i] = 0;
	        if(access(DirName,NULL)!=0)
	        {
                if(mkdir(DirName, 0755)==-1)
                {
		            perror("mkdir error");
		            return   -1;
                }
		    }
            DirName[i] = '/';
	    }  
    }
   
    return   0;
	} 
