#include"smb2Name.h"

int fidCmp(struct Fid* tmp1,struct Fid* tmp2){
    if(tmp1==NULL||tmp2==NULL)return 0;
    return ((tmp1->pers==tmp2->pers)&&(tmp1->vola==tmp2->vola));
}

int fnameCmp(struct FileName* tmp1,struct FileName* tmp2){
    return !strcmp(tmp1->name,tmp2->name);
}


struct FNode* allocFNode(){
    struct FNode* temp=(struct FNode*)malloc(sizeof(struct FNode));
    if(temp==NULL)return NULL;
    else {
        temp->filename.name=NULL;
        temp->next=NULL;
        temp->prev=NULL;
    }return temp;
}

void addFNodeToArray(struct FNode* head,struct FNode* item){
    if(head==NULL){
        head=item;
        item->next=NULL;
        item->prev=NULL;
    }
    item->next=head;
    head->prev=item;
    item->prev=NULL;
    head=item;
}

void delFNodeFromArray(struct FNode* head,struct Fid* fid){
    struct FNode* tmp;
    if(head==NULL)return;
    tmp=head;
    if(fidCmp(&head->fid,fid)){
        head->next->prev=NULL;
        head=head->next;
        free(tmp);
    }else{
        while(tmp!=NULL){
            if(fidCmp(&tmp->fid,fid)){
                tmp->prev->next=tmp->next;
                if(tmp->next)tmp->next->prev=tmp->prev;
                free(tmp);
                return;
            }
            tmp=tmp->next;
        }
    }
}

struct FNode* searchFNodeByFid(struct FNode* head,struct Fid* fid){
    while(head!=NULL){
        if(fidCmp(&head->fid,fid)){
            return head;
        }
        head=head->next;
    }
    return NULL;
}

struct FNode* searchFNodeByName(struct FNode* head,struct FileName* filename){
    while(head!=NULL){
        if(fnameCmp(&head->filename,filename)){
            return head;
        }
        head=head->next;
    }
    return NULL;
}



void printFNode(struct FNode* tmp){
    unsigned int i;
    printf("filename:\n");
    printf("%s\n",tmp->filename.name);
    printf("\nfid:\n");
    for(i=0;i<16;i++){
        printf("%x",*(1+(char*)(&(tmp->fid))));
    }
    printf("\n\n");

}

void printFNodeArray(struct FNode* head){
    int i=0;
    while(head!=NULL){
        printf("第%d个node",i++);
        printFNode(head);
        head=head->next;
    }
}


