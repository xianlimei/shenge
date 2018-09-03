#include"short_name_list.h"

//struct NameNode* gNameList[LIST_NUM]={NULL};

NameList getNewNameList(){
    NameList result=(NameList)malloc(LIST_NUM*sizeof(struct NameNode*));
    memset(result,0,LIST_NUM*sizeof(struct NameNode*));
    return result;
}

unsigned int getHash(char* name){
   // return name[0]%LIST_NUM;
    
    unsigned int value,i,length;
    length=strlen(name);
    for (value = FNV1_INIT, i=0; i < length; i++) {
            value *= (unsigned int)FNV1_PRIME;
            value ^= (unsigned int)(name[i]);
        }
    return (value%LIST_NUM);   
}



struct NameNode* allocNameNode(){
    struct NameNode* temp=(struct NameNode*)malloc(sizeof(struct NameNode));
    if(temp==NULL)return NULL;
    else{
        temp->next=NULL;
        temp->prev=NULL;
        temp->filename.name=NULL;
        temp->shortname.name=NULL;
    }return temp;
}

void delNameNodeToList(NameList list,struct NameNode* item){
    struct NameNode** temp=&list[getHash(item->shortname.name)];
    if(!*temp||!item)return;
    if(item->prev=NULL){
        (*temp)=item->next;
        if(!item->next){
            item->next->prev=NULL;
        }
    }else
        if(item->next==NULL){
        item->prev->next=NULL;
    }else{
        item->prev->next=item->next;
        item->next->prev=item->prev;
    }
    free(item);
}

void addNameNodeToList(NameList list,struct NameNode* item){
    struct NameNode** temp=&list[getHash(item->shortname.name)];
    if(*temp==NULL){
        *temp=item;
        item->next=NULL;
        item->prev=NULL;
        return;
    }
    item->next=*temp;
    (*temp)->prev=item;
    item->prev=NULL;
    *temp=item;
}

void freeNameList(NameList temp){
    int i;
    struct NameNode* head;
    for(i=0;i<LIST_NUM;i++){
        if(temp[i]==NULL){
            continue;
        };
        head=temp[i];
        while(head!=NULL){
            if(head->next==NULL){
                free(head->filename.name);
                free(head->shortname.name);
                free(head);
                break;
            }
            head=head->next;
            free(head->prev->filename.name);
            free(head->prev->shortname.name);
            free(head->prev);
        }
    }
    free(temp);
}

struct NameNode* searchNNByShortName(NameList list,char* shortname){
    int i=0;
    struct NameNode* head;
    struct NameNode** temp=&list[getHash(shortname)];
    head=*temp;
    if(head==NULL)return NULL;
        while(head!=NULL){
            if(!strcmp(shortname,head->shortname.name)){
                return head;
            }
            head=head->next;}
    return NULL;
}

void printNameNode(struct NameNode* temp){
    printf("filename:%s\n",temp->filename.name);
    printf("shortname:%s\n\n",temp->shortname.name);
}

void fprintNameNode(struct NameNode* temp,FILE* ShortNameList){
    fprintf(ShortNameList,"filename:%s\n",temp->filename.name);
    fprintf(ShortNameList,"shortname:%s\n\n",temp->shortname.name);
}

void fprintNameList(NameList list){
    FILE* ShortNameList;
    int i=0;
    struct NameNode* head;
    ShortNameList=fopen("listOfShortName.txt","w+");
    fprintf(ShortNameList,"the totol of the name is %u\n",getShortNameListNum(list));
    for(;i<LIST_NUM;i++){
        if(list[i]==NULL)continue;
        head=list[i];
        while(head!=NULL){
            fprintNameNode(head,ShortNameList);
            head=head->next;
        }
    }
    fclose(ShortNameList);
}

//如果list不为空，返回1.否则返回0
int checkNameList(NameList list){
    int i;
    for(i=0;i<LIST_NUM;i++){
        if(list[i])return 1;
    }
    return 0;
}

void printNameList(NameList list){
    int i=0;
    struct NameNode* head;
    for(;i<LIST_NUM;i++){
        if(list[i]==NULL)continue;
        head=list[i];
        while(head!=NULL){
            printNameNode(head);
            head=head->next;
        }
    }
}

char* getUTF8Name(char* head){
    char* result;
    int len=getUtf8LenFromUnicode(head+0x68,(int)(*(INT32*)(head+0x3c)));
    result=(char*)malloc(len+1);
    //if(result==NULL)exit(0);
    unicode2utf8(head+0x68,(size_t)(*(INT32*)(head+0x3c)),result,len+1);
    result[len]='\0';
    return result;
}

char* getUTF8ShortName(char* head){
    char* result;
    int len=getUtf8LenFromUnicode(head+0x46,(int)(*(char*)(head+0x44)));
    result=(char*)malloc(len+1);
    //if(result==NULL)exit(0);
    unicode2utf8(head+0x46,(size_t)(*(char*)(head+0x44)),result,len+1);
    result[len]='\0';
    return result;
}

int setNameNode(NameList list,char* head){
    struct NameNode* temp;
    char* shortname;


    head+=*((INT16*)(head+0x42));

    if(!*(INT32*)(head-4))return -1;

    while(*(INT32*)head!=0){   //fa0
        if(*(head+68)!='\0'){
            shortname=getUTF8ShortName(head);
            if(!searchNNByShortName(list,shortname)){
                temp=allocNameNode();
                temp->filename.name=getUTF8Name(head);
                temp->shortname.name=shortname;
                addNameNodeToList(list,temp);
            }else free(shortname);
        }
        head+=*((INT32*)head);
    }
    if(*(head+68)!='\0'){
        shortname=getUTF8ShortName(head);
        if(!searchNNByShortName(list,shortname)){
            temp=allocNameNode();
            temp->filename.name=getUTF8Name(head);
            temp->shortname.name=shortname;
            addNameNodeToList(list,temp);
        }else free(shortname);
    }
    return 0;
}

unsigned getShortNameListNum(NameList list){
    unsigned sum=0;
    int i;
    struct NameNode* head;
    for(i=0;i<LIST_NUM;i++){
        if(list[i]!=NULL){
            head=list[i];
            while(head!=NULL){
                head=head->next;
                sum++;
            }
        }
    }
    return sum;
}

void replaceShortName(char* string,NameList list,char** resltString){
    int i=0;
    int j,k,a,tmp;
    //char *result=*resltString;
    struct NameNode* node;
    int size=strlen(string);
    int newSize;
    char temp[13]={0};
    newSize=size;
    while(string[i]=='\\'){i++;}

    while(i<size){
        j=i;
        while(string[j]!='\\'&&string[j]!='\0'){j++;};
        if(string[i+6]=='~'&&i>=j-12&&i<=j-8){
            memset(temp,0,13);
            for(k=0;k<j-i;k++){
                temp[k]=string[i+k];
            }
            if(node=searchNNByShortName(list,temp)){
                newSize+=(strlen(node->filename.name)-strlen(temp));
            }
        }
        i=j+1;
    }

    *resltString=(char*)malloc(newSize);

    i=0;
    while(string[i]=='\\'){
        //(*resltString)[i]=string[i];
        i++;
    }
    a=0;
    while(i<size){
        j=i;
        while(string[j]!='\\'&&string[j]!='\0'){j++;}
        if(string[i+6]=='~'&&i>=j-12&&i<=j-8){
            memset(temp,0,13);
            for(k=0;k<j-i;k++){
                temp[k]=string[i+k];
            }
            if(node=searchNNByShortName(list,temp)){
                tmp=strlen(node->filename.name);
                for(k=0;k<tmp;k++,a++){
                    (*resltString)[a]=node->filename.name[k];
                }
                (*resltString)[a]=string[j];
                a++;
            }else {
                for(;i<=j;i++){
                    (*resltString)[a]=string[i];
                    a++;
                }
            }
        }else {
            for(;i<=j;i++){
                (*resltString)[a]=string[i];
                a++;
            }
        }
        i=j+1;
    }
    (*resltString)[a]='\0';
    //return result;
}
