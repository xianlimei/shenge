#include "proxyserver.h"
#include "fname_decode.h"


int print_read_list_node(read_command_node * head) {
    FILE * fp_d;
    head = head->next;
    if(head == NULL) {
        fp_d = fopen("read_list.txt","a+");
        fprintf(fp_d,"%s","no node\n\n\n");
        fclose(fp_d);
    }
    fp_d = fopen("read_list.txt","a+");
    for(read_command_node * node = head; node != NULL ; node = node->next) {
        fprintf(fp_d,"%s %lx %s","read list MessageID is 0x",node->read_mid,"\n");
        fprintf(fp_d,"%s %lx %s","read list fid_low is 0x",node->fid_low,"\n");
        fprintf(fp_d,"%s %lx %s","read list fid_high is 0x",node->fid_high,"\n");
        fprintf(fp_d,"%s %lx %s","read list file_offset is",node->read_offset,"\n\n");
    }
    fprintf(fp_d,"%s","\n\n\n");
    fclose(fp_d);
    return 0;
}

int print_creat_list_node(creat_command_node * head) {
    FILE * fp_d;
    head = head->next;
    if(head == NULL) {
        fp_d = fopen("creat_list.txt","a+");
        fprintf(fp_d,"%s","no node\n\n\n");
        fclose(fp_d);
    }
    fp_d = fopen("creat_list.txt","a+");
    for(creat_command_node * node = head; node != NULL ; node = node->next) {
        fprintf(fp_d,"%s %lx %s","creat list MessageID is 0x",node->creat_mid,"\n");
        fprintf(fp_d,"%s %lx %s","creat list fid_low is 0x",node->fid_low,"\n");
        fprintf(fp_d,"%s %lx %s","creat list fid_high is 0x",node->fid_high,"\n");
        //fprintf(fp_d,"%s %lx %s","list file_offset is",node->,"\n\n");
    }
    fprintf(fp_d,"%s","\n\n\n");
    fclose(fp_d);
    return 0;
}

creat_command_node * creat_creat_list_node(creat_command_node * head , smb2_header_p smb2hdr) {
    FILE *fp_d;
    fp_d = fopen("creat_list.txt","a+");
    fprintf(fp_d,"%s","add node\n");
    fclose(fp_d);
    creat_command_node *last = head;
    while(last->next != NULL) {
        last = last->next;
    }
    last->next = (creat_command_node *)malloc(sizeof(creat_command_node));
    last = last->next;
    if(last == NULL) {
        printf("error:creat_read_list_node malloc failed!\n");
        return NULL;
    }
    printf("creat list node malloc success\n");
    memset(last,0,sizeof(creat_command_node));
    last->creat_mid = smb2hdr->MessageID;
    last->filename_length = *(uint16_t *)((uint8_t *)smb2hdr + 110);
    last->file_name = NULL;
    last->next = NULL;
    last->fp = NULL;
    return last;
}

read_command_node  * creat_read_list_node(read_command_node * head ,smb2_header_p smb2hdr) {
    printf("creat read list node\n");
    read_command_node *last = head;
    while(last->next != NULL) {
        last = last->next;
    }

    printf("1111\n");
    last->next = (read_command_node *)malloc(sizeof(read_command_node));
    last = last->next;
    printf("2222\n");
    if(last == NULL) {
        printf("error:creat_read_list_node malloc failed!\n");
        return NULL;
    }
    printf("read list node malloc success\n");
    memset(last,0,sizeof(read_command_node));
    last->read_mid = smb2hdr->MessageID;
    last->read_offset = *(uint64_t *)((uint8_t *)smb2hdr + 72);
    last->fid_low = *(uint64_t *)((uint8_t *)smb2hdr + 80);
    last->fid_high = *(uint64_t *)((uint8_t *)smb2hdr + 88);
    last->next = NULL;
    return last;
}

int del_creat_list_node_by_mid(creat_command_node * head, uint64_t mid) {
    FILE *fp_d;
    fp_d = fopen("creat_list.txt","a+");
    fprintf(fp_d,"%s","del node by mid\n");
    fclose(fp_d);
    creat_command_node * temp;
    for(creat_command_node * node = head; node->next != NULL ; node = node->next) {
        if(node->next->creat_mid == mid) {
            temp = node->next;
            node->next = node->next->next;
            if(temp->file_name != NULL) {
                free(temp->file_name);
            }
            free(temp);
            return 0;
        }
    }
    printf("not found the read list node\n");
    return 1;
}

FILE * find_creat_list_node_by_fid(creat_command_node * head, uint64_t fid_low, uint64_t fid_high) {

    FILE *fp_d;
    // fp_d = fopen("creat_list.txt","a+");
    // fprintf(fp_d,"%s","find node by fid\n");
    // fclose(fp_d);
    for(creat_command_node * node = head; node->next != NULL ; node = node->next) {
        if(node->next->fid_low == fid_low && node->next->fid_high == fid_high) {
            printf("creat mid is %lu\n",node->next->creat_mid);
            printf("fid low is %lx\n",node->next->fid_low);
            printf("fid high is %lx\n",node->next->fid_high);
            return node->next->fp;
        }
    }
    
    printf("not found the read list node\n");
    return NULL;
}

int del_creat_list_node_by_fid(creat_command_node * head, smb2_header_p smb2hdr) {
    uint64_t fid_low = *(uint64_t *)((uint8_t *)smb2hdr + 72);
    uint64_t fid_high = *(uint64_t *)((uint8_t *)smb2hdr + 80);
    FILE *fp_d;
    fp_d = fopen("creat_list.txt","a+");
    fprintf(fp_d,"%s","del node by fid\n");
    
    creat_command_node * temp;
    for(creat_command_node * node = head; node->next != NULL ; node = node->next) {
        if(node->next->fid_low == fid_low && node->next->fid_high == fid_high) {
            temp = node->next;
            if(node->next->fp != NULL) {
                fprintf(fp_d,"%s","close file fp\n");
                fclose(node->next->fp);
            }
            node->next = node->next->next;
            if(temp->file_name != NULL) {
                free(temp->file_name);
            }
            free(temp);
            fclose(fp_d);
            return 0;
        }
    }
    fclose(fp_d);
    printf("not found the read list node\n");
    return 1;
}


int add_fid_creat_list_node(creat_command_node * head, smb2_header_p smb2hdr,NameList name_list) {
    FILE *fp_d;
    uint64_t mid = smb2hdr->MessageID;

    fp_d = fopen("creat_list.txt","a+");
    fprintf(fp_d,"%s","add message\n");
    fclose(fp_d);
    creat_command_node * temp;
    for(creat_command_node * node = head; node->next != NULL ; node = node->next) {
        if(node->next->creat_mid == mid) {
            node->next->fid_low = *(uint64_t *)((uint8_t *)smb2hdr + 128);
            node->next->fid_high = *(uint64_t *)((uint8_t *)smb2hdr + 136);

            char* newFileName;
            int long_name_length;
            if(node->next->file_name != NULL) {
                replaceShortName(node->next->file_name,name_list,&newFileName);
            }else {
                newFileName=NULL;
            }
//            if(strcmp(newFileName,node->next->file_name)&&newFileName){
//                FILE* shit;
//                shit=fopen("FILE_NAME_REPLACEMENT","a+");
//                fprintf(shit,"origin Name:%s\nnew name:%s\n\n",node->next->file_name,newFileName);
//                fclose(shit);
//            }

            long_name_length = strlen(newFileName)-1;
            for(;long_name_length>=0;long_name_length--){
                if(newFileName[long_name_length]=='\\')
                    newFileName[long_name_length]='/';
            }
            node->next->fp = openFileByString(newFileName);
            free(newFileName);

            return 0;
        }
    }
    printf("not found the read list node\n");
    return 1;

}


uint64_t pop_read_list_node(read_command_node * head, uint64_t mid , smb_session_p s) {
    FILE *fp_d;
    fp_d = fopen("read_list.txt","a+");
    fprintf(fp_d,"%s","del node\n");
    fclose(fp_d);
    read_command_node * temp;
    uint8_t flag = 0;
    for(read_command_node * node = head; node->next != NULL ; node = node->next) {
        if(node->next->read_mid == mid) {
            temp = node->next;
            s->file_content_offset = node->next->read_offset;
            s->current_fid_low = node->next->fid_low;
            s->current_fid_high = node->next->fid_high;
            node->next = node->next->next;
            free(temp);
            return 0;
        }
    }

    printf("not found the read list node\n");
    return 1;

}