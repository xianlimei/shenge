//
// Created by wqy on 18-2-5.
//

#include "proxyserver.h"
#include "unicode2utf8.h"

static pthread_mutex_t mutex_creat_list;
static pthread_mutex_t mutex_read_list;

#define FIND_MAX_LENGTH 70000


uint32_t file_tid;
read_command_node *read_list_head = NULL;
creat_command_node *creat_list_head = NULL;

void print_addr(char *info, uint32_t addr)
{
    uint8_t *addrp = (uint8_t *) &addr;
    printf("%s: %d.%d.%d.%d\n", info, *addrp, *(addrp + 1), *(addrp + 2), *(addrp + 3));
}

void proxy_server_init(proxy_server_p server, 
                    char *listen_ip, uint16_t listen_port, char *to_ip)
{
    server->listen_ip = inet_addr(listen_ip);
    server->listen_port = htons(listen_port);
    server->to_ip = inet_addr(to_ip);

    _create_server(server);
}

void _create_server(proxy_server_p server)
{
    int sock;
    struct sockaddr_in *addr;
    struct sockaddr_in *to_addr;

    addr = &server->listen_addr;
    to_addr = &server->to_addr;

    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = server->listen_ip;
    addr->sin_port = server->listen_port;

    to_addr->sin_family = AF_INET;
    to_addr->sin_addr.s_addr = server->to_ip;
    to_addr->sin_port = server->listen_port;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Failed to create socket");
        goto _CREATE_SERVER_FAIL;
    }

    if (bind(sock, (const struct sockaddr *) addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Failed to bind address");
        goto _CREATE_SERVER_FAIL;
    }

    if (listen(sock, 10) < 0)
    {
        perror("Failed to listen on port");
        goto _CREATE_SERVER_FAIL;
    }

    server->_sock = sock;
    return;

    _CREATE_SERVER_FAIL:
    if (sock > 0)
    {
        close(sock);
    }
    memset(server, 0, sizeof(proxy_server_t));
}

void proxy_server_start(proxy_server_p server)
{
    if (server->_sock <= 0)
    {
        printf("Invalid socket\n");
        return;
    }

    int sock;
    socklen_t addrsize;
    struct sockaddr_in addr;

    printf("\n\non \n\n");

    while (1)
    {
        pthread_t pthread;
        struct _on_accept_arg_s args;

        memset(&addr, 0, sizeof(struct sockaddr_in));
        addrsize = sizeof(struct sockaddr_in);

        // 这个 addrsize 必须初�?�化, 否则报错 Invalid argument
        // �?三个参数得值也不能�? NULL
        sock = accept(server->_sock, (struct sockaddr *) &addr, &addrsize);
        if (sock < 0)
        {
            perror("Failed to accept connection");
            goto START_ACCEPT_FAIL;
        }

        args.sock = sock;
        args.addr = addr;
        args.server = server;

        print_addr("accept addr", addr.sin_addr.s_addr);

        if (pthread_create(&pthread, NULL, _on_accept, &args) != 0)
        {
            printf("Failed to create thread, connection closed\n");
            goto START_ACCEPT_FAIL;
        }

        continue;

        START_ACCEPT_FAIL:
        if (sock)
        {
            close(sock);
        }
    }
}

static void *_on_accept(void *args)
{
    struct _on_accept_arg_s *s_args;
    struct sockaddr_in *to_addr;
    pthread_t pthread;
    uint32_t to_host;
    int sock;
    s_args = args;
    to_host = s_args->addr.sin_addr.s_addr;
    to_addr = &s_args->server->to_addr;

    printf("server %p\n", s_args->server);
    print_addr("to_addr", to_addr->sin_addr.s_addr);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("Failed to create socket");
        goto _ON_ACCEPT_FAIL;
    }

    if (connect(sock, (const struct sockaddr *) to_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("Failed to connect to remote host");
        goto _ON_ACCEPT_FAIL;
    }
    struct _handler_args recv_arg = {
            .srcsock = sock,
            .dstsock = s_args->sock
    };


    if (pthread_create(&pthread, NULL, _handler1, &recv_arg) != 0)
    {
        printf("Failed to create pthread, connection closed\n");
        goto _ON_ACCEPT_FAIL;
    }

    struct _handler_args send_arg = {
            .srcsock = s_args->sock,
            .dstsock = sock
    };

    
    _handler2(&send_arg);

    pthread_join(pthread, NULL);

    return NULL;

    _ON_ACCEPT_FAIL:
    if (sock)
    {
        close(sock);
    }
    if (s_args->sock)
    {
        close(s_args->sock);
    }
}


void filename_handle(smb_session_p s)
{
    if(s->smb1_or_smb2 == IPPROTO_SMB1 && s->request_or_response == SMB_REQUEST) {
        goto SMB1_PROCESSING;
    }else if(s->smb1_or_smb2 == IPPROTO_SMB2){
        goto SMB2_PROCESSING;
    }else{
        return;
    }


    SMB1_PROCESSING:
    
    if(s->current_command == SMB1_NT_CREATE_ANDX && s->filename_length > 0){
        printf("smb1 filename processing\n");
        if(s->if_already_malloc == 0){
            s->filename_utf_8 = (uint8_t *)malloc(s->smb_command_length -90 + 6); 
            s->if_already_malloc = 1;
        }
        printf("hhh1\n");
        if(s->remain_command_bytes > 0){
            printf("hhh2\n");
            for(int i = 0 ; i < s->filename_length / 2 ; i ++ ){
                s->filename_storage_offset += enc_unicode_to_utf8_one(s->filename_begin[i],(s->filename_utf_8 + s->filename_storage_offset),6);
            }
        }
        else{
            printf("hhh3\n");
            for(int i = 0 ; i < (s->filename_length - 2) / 2 ; i ++ ){
                s->filename_storage_offset += enc_unicode_to_utf8_one(s->filename_begin[i],(s->filename_utf_8 + s->filename_storage_offset),6);
            }
            if((s->fd = open("filename.txt",O_TRUNC | O_WRONLY | O_APPEND | O_CREAT,0644)) == -1){
                printf("error:open file failed\n");
            }
            write(s->fd,s->filename_utf_8,s->filename_storage_offset - 1);
            close(s->fd);
            printf("The name of file to be transferred is:%s\n",s->filename_utf_8);
        
            free(s->filename_utf_8);
            s->filename_storage_offset = 0;
            s->if_already_malloc = 0;
        }
        printf("The length of the file's name which will be dumped is %d bytes\n",s->filename_storage_offset);
    }
    return;

    SMB2_PROCESSING:
    printf("smb2 filename processing\n");
    if(s->current_command == SMB2_CREAT && s->filename_total_length > 0 && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid) {
        if(s->if_already_malloc == 0) {
            printf("record filename start\n");
            s->filename_offset = 0;
            s->filename_unicode = malloc(s->filename_total_length);
            s->last_creat_list_node->file_name = s->filename_unicode;
            s->if_already_malloc = 1;
            if(s->filename_unicode == NULL) {
                perror("malloc failed\n");
                s->if_already_malloc = 0;
            }
        }
        if(s->filename_length > 0 && s->filename_remain_length != 0) {
            printf("record filename\n");
            memcpy(s->filename_unicode + s->filename_offset, (uint8_t *)s->filename_begin, s->filename_length );
            s->filename_offset += s->filename_length;
            s->filename_remain_length -= s->filename_length;
        }
        if(s->if_already_malloc == 1 && s->filename_remain_length == 0) {
            int utf8_length;
            uint8_t *filename_utf8;
            
            printf("record finished\n");
            utf8_length = getUtf8LenFromUnicode((char *)s->last_creat_list_node->file_name,s->last_creat_list_node->filename_length);
            filename_utf8 = (uint8_t *)malloc(utf8_length + 1);
            unicode2utf8((char *)s->last_creat_list_node->file_name,s->last_creat_list_node->filename_length,(char *)filename_utf8,utf8_length);
            filename_utf8[utf8_length] = '\0';
            free(s->last_creat_list_node->file_name);
            s->last_creat_list_node->file_name = filename_utf8;
            s->last_creat_list_node->filename_length = utf8_length;
            
            s->fp = fopen("filename_list.txt","a+");
            fprintf(s->fp,"%s %lx %s","read list fid_low is 0x",s->last_creat_list_node->fid_low,"\n");
            fprintf(s->fp,"%s %lx %s","read list fid_high is 0x",s->last_creat_list_node->fid_high,"\n");
            fwrite(s->last_creat_list_node->file_name , s->last_creat_list_node->filename_length, 1 , s->fp);
            fprintf(s->fp,"%s","\n\n\n\n\n");
            fclose(s->fp);
            s->if_already_malloc = 0;
            s->filename_unicode = NULL;
        }
    }
    if(s->current_command==SMB2_FIND && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid){

    }


    // if(s->if_already_malloc == 1 && s->current_command ==  && s->filename_remain_length == 0 && s->request_or_response == SMB_REQUEST) {
    //     // s->fp = fopen("filename.txt","a+");
    //     // fwrite(s->filename_unicode , s->filename_total_length , 1 , s->fp);
    //     // fprintf(s->fp,"%s","\n");
    //     // fputc(0,s->fp);
    //     // fclose(s->fp);

    //     s->fp = fopen("filename_list.txt","a+");
    //     for(creat_command_node * node = creat_list_head->next; node != NULL ; node = node->next) {
    //         fprintf(s->fp,"%s %lx %s","read list MessageID is 0x",node->creat_mid,"\n");
    //         fprintf(s->fp,"%s %lx %s","read list fid_low is 0x",node->fid_low,"\n");
    //         fprintf(s->fp,"%s %lx %s","read list fid_high is 0x",node->fid_high,"\n");
    //         fwrite(node->file_name , node->filename_length , 1 , s->fp);
    //         fprintf(s->fp,"%s","\n\n");
    //     }
    //     fprintf(s->fp,"%s","\n\n\n\n\n");
    //     fclose(s->fp);



    //     printf("close filename fp\n");
    //     //free(s->filename_unicode);
    //     //s->filename_unicode = NULL;
    //     s->if_already_malloc = 0;
    // }


    printf("hey\n");
    return;

}

void filecontent_handle(smb_session_p s)
{
    FILE * fp_d;

    if(s->smb1_or_smb2 == IPPROTO_SMB1 && s->request_or_response ==SMB_RESPONSE) {
        goto SMB1_PROCESSING;
    }else if(s->smb1_or_smb2 == IPPROTO_SMB2 && s->request_or_response ==SMB_RESPONSE){
        goto SMB2_PROCESSING;
    }else{
        return;
    }


    SMB1_PROCESSING:
    printf("smb1 filecontent processing\n");
    if(s->current_command == SMB1_NT_CREATE_ANDX) {
        if((s->fd = open("filecontent.txt",O_TRUNC | O_WRONLY | O_APPEND | O_CREAT,0644)) == -1){
                printf("error:open file failed\n");
        }
        printf("Create file filecontent.txt\n");
    }
    if(s->current_command == SMB1_READ_ANDX) {
        if(s->file_content_length > 0){
            write(s->fd , s->file_content_begin , s->file_content_length);
            printf("succeed in writing %dbytes\n",s->file_content_length);
            s->file_content_length = 0;
        }
    }
    if(s->current_command == SMB1_CLOSE) {
        printf("closed fd\n");
        close(s->fd);
    }
    return;


    SMB2_PROCESSING:
    printf("smb2 filecontent processing\n");
    printf("current command is 0x%x\n",s->current_command);
    // if(s->current_command == SMB2_CREAT && s->smb2hdr->Status.Status == NT_STATUS_SUCCESS) {
        
    //     uint64_t allocation_size = *(uint64_t *)((char *)s->smb2hdr + CREAT_RESPONSE_ALLOCATION_SIZE_FIELD_OFFSET);
    //     uint64_t file_size = *(uint64_t *)((char *)s->smb2hdr + CREAT_RESPONSE_END_FILE_FIELD_OFFSET);
    //     printf("file size is %lu bytes\n",file_size);
    //     if(s->fp != NULL && file_size != 0 && s->current_tid == file_tid) {
    //         fp_d = fopen("logs.txt","a+");
    //         fprintf(fp_d,"%s","fp close in create command\n");
    //         fclose(fp_d);
    //         fclose(s->fp);
    //         s->fp = NULL;
    //     }
    //     if(file_size != 0) {
    //         fp_d = fopen("tree.txt","a+");
    //         fprintf(fp_d,"%s","creat response\n");
    //         fprintf(fp_d,"%s %u %s","tid is 0x",file_tid,"\n\n");
    //         fclose(fp_d);
    //         s->fp = fopen("filecontent.txt","w+");
    //         if(s->fp == NULL){
    //             printf("error:open file failed\n");
    //             return;
    //         }
    //         printf("Create file filecontent.txt\n");
    //     }
    // }

    // if(s->current_command == SMB2_CLOSE && s->smb2hdr->Status.Status == NT_STATUS_SUCCESS && s->current_tid == file_tid) {
    //     if(s->fp != NULL) {
    //         fclose(s->fp);
    //         s->fp = NULL;

    //         fp_d = fopen("logs.txt","a+");
    //         fprintf(fp_d,"%s","fp close in close command\n");
    //         fclose(fp_d);
    //     }
    // }
    if(s->current_command == SMB2_READ && s->current_tid == file_tid) {
        pthread_mutex_lock(&mutex_creat_list);
        s->fp = find_creat_list_node_by_fid(creat_list_head , s->current_fid_low , s->current_fid_high);
        pthread_mutex_unlock(&mutex_creat_list);
        printf("file content length is %u\n",s->file_content_length);
        printf("file content offset is %lu\n",s->file_content_offset);
        if(s->file_content_length > 0){
            printf("1111\n");
            if (fseek(s->fp, s->file_content_offset , SEEK_SET) == -1)
            {
                printf("error:lseek failed\n");
            }
            printf("2222\n");
            fp_d = fopen("write_offset.txt","a+");
            fprintf(fp_d,"%s %lu %s","write offset is ",s->file_content_offset," bytes\n");
            fprintf(fp_d,"%s %u %s","write length is ",s->file_content_length," bytes\n");
            fclose(fp_d);
            fwrite(s->file_content_begin , s->file_content_length , 1 ,s->fp);
            printf("succeed in writing %dbytes\n",s->file_content_length);
            s->file_content_offset += s->file_content_length;
            s->file_content_length = 0;
        }
    }

    return;

}







void record_logs(smb_session_p s)
{
    FILE * fp_d;
    int i;

    //judge whether it is a long command
    if(s->is_last_command == 0) {
        if(s->smb1_or_smb2 == IPPROTO_SMB1) {
            //record the position and length of file content if current command is READ_ANDX 
            if(s->current_command == SMB1_READ_ANDX && s->request_or_response == SMB_RESPONSE) {
                s->file_content_begin = ((uint8_t *)s->nethdr + 64);
                s->file_content_length = (s->remain_recv_bytes > s->smb_command_length + NETBIOS_HEADER_LENGTH) ? (s->smb_command_length - 60) : (s->remain_recv_bytes - 64);
                printf("remain recv bytes is %d bytess\n",s->remain_recv_bytes);
                printf("file content length is %d bytess\n",s->file_content_length);
                //record the position and length of file name
            }else if(s->current_command == SMB1_NT_CREATE_ANDX && s->request_or_response == SMB_REQUEST){
                s->filename_begin = (uint16_t * )(((uint8_t *)s->nethdr) + 88);
                s->filename_length = (s->remain_recv_bytes > s->smb_command_length + NETBIOS_HEADER_LENGTH) ? (s->smb_command_length - 84) : (s->remain_recv_bytes - 88);
                printf("filename_length is %d bytes\n",s->filename_length);
            }
        }else if(s->smb1_or_smb2 == IPPROTO_SMB2) {
            if(s->current_command == SMB2_TREE_CONNECT && s->request_or_response == SMB_RESPONSE && s->command_status == NT_STATUS_SUCCESS) {
                uint8_t share_type = *(uint8_t *)((uint8_t *)s->smb2hdr + 66);
                fp_d = fopen("tree.txt","a+");
                fwrite(s->smb2hdr,s->remain_recv_bytes,1,fp_d);
                fprintf(fp_d,"%s","\n");
                fprintf(fp_d,"%s %u %s","share type is 0x",share_type,"\n");
                
                if(share_type == PHYSICAL_DISK) {
                    file_tid = s->current_tid;
                    fprintf(fp_d,"%s %u %s","tid is 0x",s->current_tid,"\n\n");
                }
                fclose(fp_d);

                if(s->findCommand != NULL){
                    free(s->findCommand);
                    s->findCommand =NULL;
                }

                if(s->name_list != NULL){
                    freeNameList(s->name_list);
                    s->name_list = NULL;
                }

            }else if(s->current_command == SMB2_READ && s->request_or_response == SMB_RESPONSE && s->command_status == NT_STATUS_SUCCESS) {
                printf("smb2 read command: start read file\n");
                uint16_t read_command_data_offset = *(uint16_t *)((uint8_t *)s->smb2hdr + 66);
                uint32_t read_command_data_length = *(uint32_t *)((uint8_t *)s->smb2hdr + 68);
                fp_d = fopen("logs.txt","a+");
                fprintf(fp_d,"%s %lx %s","mid is 0x",s->smb2hdr->MessageID,"\n");
                fprintf(fp_d,"%s %u %s","read command data offset is",read_command_data_offset," bytes\n");
                fprintf(fp_d,"%s %u %s","read command data length is",read_command_data_length," bytes\n\n");
                fclose(fp_d);
                

                pthread_mutex_lock(&mutex_read_list);
                pop_read_list_node(read_list_head,s->smb2hdr->MessageID,s);
                print_read_list_node(read_list_head);
                pthread_mutex_unlock(&mutex_read_list);

                s->remain_read_length = read_command_data_length;
                s->file_content_begin = ((uint8_t *)s->smb2hdr + read_command_data_offset);
                s->file_content_length = (s->remain_recv_bytes - NETBIOS_HEADER_LENGTH - read_command_data_offset> s->remain_read_length) ? (s->remain_read_length) : (s->remain_recv_bytes - NETBIOS_HEADER_LENGTH - read_command_data_offset);
                s->remain_read_length -= s->file_content_length;

            }else if(s->current_command == SMB2_READ && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid) {
                printf("smb2 read command: record request logs\n");

                pthread_mutex_lock(&mutex_read_list);
                creat_read_list_node(read_list_head,s->smb2hdr);
                print_read_list_node(read_list_head);
                pthread_mutex_unlock(&mutex_read_list);

            }else if(s->current_command == SMB2_CREAT && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid) {
                uint16_t read_filename_offset = *(uint16_t *)((uint8_t *)s->smb2hdr + 108);
                uint16_t read_filename_length = *(uint16_t *)((uint8_t *)s->smb2hdr + 110);
                if(read_filename_length != 0) {
                    pthread_mutex_lock(&mutex_creat_list);
                    s->last_creat_list_node = creat_creat_list_node(creat_list_head,s->smb2hdr);
                    print_creat_list_node(creat_list_head);
                    pthread_mutex_unlock(&mutex_creat_list);
                    s->filename_total_length = read_filename_length;
                }else{
                    s->last_creat_list_node = NULL;
                    s->filename_total_length = 0;
                }

                s->filename_begin = (uint16_t *)((uint8_t *)s->smb2hdr + read_filename_offset);
                s->filename_length = (s->remain_recv_bytes > s->filename_total_length + read_filename_offset + NETBIOS_HEADER_LENGTH) ? (s->filename_total_length) : (s->remain_recv_bytes - 124);
                s->filename_remain_length = s->filename_total_length;
            }else if(s->current_command == SMB2_CREAT && s->request_or_response == SMB_RESPONSE && s->current_tid == file_tid) {
                pthread_mutex_lock(&mutex_creat_list);
                if(s->command_status != NT_STATUS_SUCCESS) {
                    del_creat_list_node_by_mid(creat_list_head,s->smb2hdr->MessageID);
                    print_creat_list_node(creat_list_head);
                }else{
                    file_attribute_p file_attribute = (file_attribute_p)((uint8_t *)s->smb2hdr + 120);
                    if(file_attribute->directory == 1) {
                        del_creat_list_node_by_mid(creat_list_head,s->smb2hdr->MessageID);
                        print_creat_list_node(creat_list_head);
                    }else{
                        add_fid_creat_list_node(creat_list_head,s->smb2hdr,s->name_list);
                        print_creat_list_node(creat_list_head);
                    }
                }
                pthread_mutex_unlock(&mutex_creat_list);
            }else if(s->current_command == SMB2_CLOSE && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid) {
                s->last_creat_list_node = NULL;
                pthread_mutex_lock(&mutex_creat_list);
                del_creat_list_node_by_fid(creat_list_head,s->smb2hdr);
                print_creat_list_node(creat_list_head);
                pthread_mutex_unlock(&mutex_creat_list);
            }
            else if(s->current_command == SMB2_TREE_DISCONNECT && s->request_or_response == SMB_RESPONSE && s->current_tid == file_tid){
                if(s->findCommand != NULL){
                    free(s->findCommand);
                    s->findCommand =NULL;
                }

                if(s->name_list != NULL){
                    freeNameList(s->name_list);
                    s->name_list = NULL;
                }
            }else if(s->current_command==SMB2_FIND && s->request_or_response == SMB_RESPONSE && s->current_tid == file_tid){
                uint16_t if_find_useful=*(uint16_t*)((char*)(s->smb2hdr)+0x42);
                if(if_find_useful!=0){
                    if(s->name_list==NULL){
                        s->name_list=getNewNameList();
                    }
                    fprintNameList(s->name_list);
                    if(s->smb_command_length <= s->remain_recv_bytes) {
                        setNameNode(s->name_list,s->smb2hdr);
                    }
                    else {
                        //s->if_already_malloc=2;
                        s->find_remain_length = s->smb_command_length;
                        s->find_length = s->smb_command_length;
                        if(s->findCommand==NULL)
                            s->findCommand = (uint8_t*)malloc(70000);
                        memset(s->findCommand,0,FIND_MAX_LENGTH);
                        if(s->if_have_netbiosHeader==1)
                        {
                            memcpy(s->findCommand,(char*)s->smb2hdr,s->remain_recv_bytes-4);
                            s->find_remain_length -= ( s->remain_recv_bytes-4);
                        }
                        else {
                            memcpy(s->findCommand,(char*)s->smb2hdr,s->remain_recv_bytes);
                            s->find_remain_length -= s->remain_recv_bytes;
                        }

                    }
               }else
                   s->find_remain_length=0;
            }
            //else if(s->current_command == SMB2_TREE_DISCONNECT &&)
        }

    }else{
        if(s->smb1_or_smb2 == IPPROTO_SMB1) {
            if(s->current_command == SMB1_READ_ANDX && s->request_or_response == SMB_RESPONSE) {
                s->file_content_begin = ((uint8_t *)s->header_begin);
                s->file_content_length = (s->remain_command_bytes > s->remain_recv_bytes)?s->remain_recv_bytes : s->remain_command_bytes;
            }
            if(s->current_command == SMB1_NT_CREATE_ANDX && s->request_or_response == SMB_REQUEST){
                s->filename_begin = (uint16_t *)(uint8_t *)s->header_begin;
                s->filename_length = (s->remain_command_bytes > s->remain_recv_bytes)?s->remain_recv_bytes : s->remain_command_bytes;
            }
        }else if (s->smb1_or_smb2 == IPPROTO_SMB2) {
            if(s->current_command == SMB2_READ && s->request_or_response == SMB_RESPONSE && s->current_tid == file_tid) {
                printf("smb2 long read command\n");
                uint16_t read_command_data_offset = 0;
                uint32_t read_command_data_length = (s->remain_command_bytes > s->remain_recv_bytes)?s->remain_recv_bytes : s->remain_command_bytes;

                s->file_content_begin = ((uint8_t *)s->header_begin);
                s->file_content_length = (s->remain_read_length > s->remain_recv_bytes)?s->remain_recv_bytes : s->remain_read_length;
                s->remain_read_length -=s->file_content_length;
                printf("file tid is 0x%x\n",file_tid);
                printf("current tid is 0x%x\n",s->current_tid);
                printf("file content length is %d\n",s->file_content_length);

                // fp_d = fopen("logs.txt","a+");
                // fprintf(fp_d,"%s","long command\n");
                // fprintf(fp_d,"%s %u %s","file tid is 0x",file_tid,"\n");
                // fprintf(fp_d,"%s %u %s","current tid is ",s->current_tid," \n");
                // fprintf(fp_d,"%s %u %s","file content length is ",s->file_content_length," bytes\n");
                // fprintf(fp_d,"%s %u %s","remain_read_length is",s->remain_read_length," bytes\n\n");
                // fclose(fp_d);
            }else if(s->current_command == SMB2_CREAT && s->request_or_response == SMB_REQUEST && s->current_tid == file_tid) {
                s->filename_begin = (uint16_t *)s->header_begin;
                s->filename_length = (s->remain_recv_bytes > s->filename_remain_length)?s->filename_remain_length : s->remain_recv_bytes;

            }else if(s->current_command == SMB2_FIND && s->request_or_response == SMB_RESPONSE &&s->current_tid==file_tid){
                uint32_t find_cpy_length;
                if(s->find_remain_length > 0){

                    find_cpy_length = (s->remain_recv_bytes>s->find_remain_length) ? s->find_remain_length : s->remain_recv_bytes;
                    memcpy(s->findCommand + s->find_length-s->find_remain_length,(char*)s->header_begin,find_cpy_length);
                    s->find_remain_length -= find_cpy_length;

                    if(s->find_remain_length <= 0){
                        setNameNode(s->name_list,s->findCommand);
                    }
                }
            }
        }

    }
    
}

void update_message(smb_session_p s)
{
    // move the pointer to next smb header 
    if(s->is_compounded_command == 0) {
        s->if_have_netbiosHeader = 1;
        s->nethdr = (netbios_header_p) s->header_begin;
        s->smbhdr = (smb_header_p) (s->header_begin + NETBIOS_HEADER_LENGTH);
        s->netbios_command_length = ((uint32_t)s->nethdr->lenhigh)<<16 | ((uint32_t)s->nethdr->lenmiddle)<<8 | ((uint32_t)s->nethdr->lenlow);
        printf("This netbios command length is %d\n",s->netbios_command_length);
    }else{
        s->smbhdr = (smb_header_p) s->header_begin;
    }


    if(s->smbhdr->Protocol[0] == 0xff) {
        s->smb1_or_smb2 = IPPROTO_SMB1;
        s->header_buffer_size = SMB1_HEADER_LENGTH + NETBIOS_HEADER_LENGTH;
        s->current_command = s->smbhdr->Command;

    }else if(s->smbhdr->Protocol[0] == 0xfe) {
        s->smb1_or_smb2 = IPPROTO_SMB2;
        s->smb2hdr = (smb2_header_p) (s->smbhdr);
        s->header_buffer_size = SMB2_HEADER_LENGTH + NETBIOS_HEADER_LENGTH;
        s->current_command = s->smb2hdr->Command;
        s->current_tid = s->smb2hdr->TreeID;
        if(s->request_or_response == SMB_RESPONSE) {
            s->command_status = s->smb2hdr->Status.Status;
        }else{
            s->command_status = NT_STATUS_SUCCESS;
        }
    }


    if(s->smb1_or_smb2 == IPPROTO_SMB2 && s->smb2hdr->NextCommand != 0x0 && s->is_compounded_command == 0) {
        printf("This is a compounded command\n");
        s->is_compounded_command = 1;
    }


    // update the current smb command length
    if(s->smb1_or_smb2 == IPPROTO_SMB2 && s->smb2hdr->NextCommand != 0x0 && s->is_compounded_command == 1) {
        s->smb_command_length = s->smb2hdr->NextCommand;
        s->before_command_length += s->smb_command_length;
        printf("It's a smb command of the compounded command\n");
        printf("This smb command length is %u bytes\n",s->smb_command_length);
    }else if(s->smb1_or_smb2 == IPPROTO_SMB2 && s->smb2hdr->NextCommand == 0x0 && s->is_compounded_command == 1) {
        printf("This is the last smb command of the compounded command\n");
        printf("The sum of previous smb commands is %d bytes\n",s->before_command_length);
        s->smb_command_length = s->netbios_command_length - s->before_command_length;
        printf("This smb command length is %d bytes\n",s->smb_command_length);
        s->before_command_length = 0;
    }else if(s->is_compounded_command == 0) {
        printf("This is a normal netbios command\n");
        s->smb_command_length = s->netbios_command_length;
    }

    if(s->smb1_or_smb2 == IPPROTO_SMB2 && s->smb2hdr->NextCommand == 0x0 && s->is_compounded_command == 1) {
       printf("The compounded netbios command finished\n");
        s->is_compounded_command = 0;
    }
    
    
}

void check_header(smb_session_p s)
{
    //judge whether this command is "session keep alive" command
    if(s->nethdr->type != 0x00 && s->if_have_netbiosHeader == 1){
            printf("The netbios header type don't equal to 0\n");
            s->header_begin = (uint8_t *)s->header_begin + NETBIOS_HEADER_LENGTH + s->smb_command_length;
            s->remain_recv_bytes -= NETBIOS_HEADER_LENGTH + s->smb_command_length;
            s->if_need_continue = 1;
    }else{
        s->if_need_continue = 0;
    }
    // judge whether the command header is correct         
    if(s->smbhdr->Protocol[1] != 0x53 || s->smbhdr->Protocol[2] != 0x4d || s->smbhdr->Protocol[3] != 0x42) {
        printf("error:Smb header incorrect\n");
        s->if_need_break = 1;
    }else{
        s->if_need_break = 0;
    }
    //check if the command header is complete
    if((s->smb1_or_smb2 == IPPROTO_SMB1 && s->current_command == SMB1_READ_ANDX && s->request_or_response == SMB_RESPONSE && s->remain_recv_bytes < (s->smb_command_length > 64 ? 64 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB1 && s->current_command == SMB1_NT_CREATE_ANDX && s->request_or_response == SMB_REQUEST && s->remain_recv_bytes < (s->smb_command_length > 88 ? 88 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_READ && s->request_or_response == SMB_RESPONSE && s->remain_recv_bytes < (s->smb_command_length > 84 ? 84 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_READ && s->request_or_response == SMB_REQUEST && s->remain_recv_bytes < (s->smb_command_length > 116 ? 116 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_TREE_CONNECT && s->request_or_response == SMB_RESPONSE && s->remain_recv_bytes < (s->smb_command_length > 84 ? 84 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_CREAT && s->request_or_response == SMB_REQUEST && s->remain_recv_bytes < (s->smb_command_length > 124 ? 124 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_CREAT && s->request_or_response == SMB_RESPONSE && s->remain_recv_bytes < (s->smb_command_length > 156 ? 156 : s->smb_command_length))
        || (s->smb1_or_smb2 == IPPROTO_SMB2 && s->current_command == SMB2_CLOSE && s->request_or_response == SMB_REQUEST && s->remain_recv_bytes < (s->smb_command_length > 92 ? 92 : s->smb_command_length))
        ) {
        printf("command header incomplete\n");
        s->is_header_uncomplete = 1;
        s->buffer_offset = s->remain_recv_bytes;
        s->remain_recv_bytes = 0;
        s->if_need_break = 1;
    }
    print_smbhdr(s);
}

void smb_handle(smb_session_p s)
{   
    //----------------------------processing receiveis_header_uncompleted data-------------------------
    while(s->remain_recv_bytes > 0) {
            /*----------------------------------solve header------------------------------*/
        // received data don't contain last smb command data
        if(s->is_last_command == 0) {
            update_message(s);
            check_header(s);
            if(s->if_need_break == 1){
                printf("break\n\n");
                s->if_need_break = 0;
                break;
            }
            if(s->if_need_continue == 1){
                printf("continue\n\n");
                s->if_need_continue = 0;
                continue;
            }

            record_logs(s);

            if(s->if_have_netbiosHeader == 1) {
                s->remain_recv_bytes -= NETBIOS_HEADER_LENGTH;
                s->if_have_netbiosHeader = 0;
            }
            //record the positon and length message about file
            

            s->remain_recv_bytes -= s->smb_command_length;

            // case 1 that the header of next smb command  is complete
            if(s->remain_recv_bytes >= s->header_buffer_size){
                printf("The next smb header is complete\n");
                s->header_begin = ((uint8_t *)s->smbhdr) + s->smb_command_length;
                printf("------------------next command-----------------------\n");
                //solve the given command such as READ_ANX��NT_CREAT
                
            // case 2 that current smb command still  has many bytes waiting to be received
            }else if(s->remain_recv_bytes < 0){
                printf("Current smb command still has many bytes waitting to be received\n");
                s->is_last_command = 1;
                s->remain_command_bytes = -s->remain_recv_bytes;
                printf("The remain command bytes is %d bytes\n",s->remain_command_bytes);
            // case 3 that the header of next smb command  is uncomplete
            }else if(s->remain_recv_bytes > 0 && s->remain_recv_bytes < s->header_buffer_size){
                printf("The next smb command header is uncomplete\n");
                s->is_header_uncomplete = 1;
                s->header_begin = ((uint8_t *)s->smbhdr) + s->smb_command_length;
                s->buffer_offset = s->remain_recv_bytes;
                s->remain_recv_bytes = 0;
                break;
            }

            
        // received data contain last smb command data
        }else{
            //record the positon and length message about file
            record_logs(s);

            printf("This is the data of last smb command\n");
            // case 1 that current smb command still  has many bytes waiting to be received
            if(s->remain_command_bytes > s->remain_recv_bytes) {
                s->remain_command_bytes -= s->remain_recv_bytes;
                s->remain_recv_bytes = 0;
                printf("Current smb command don't finish\n");
            // case 2 that current smb command finished its data transferring                     s->header_begin = ((uint8_t *)s->smbhdr) + s->smb_command_length;
            }else{
                printf("Current smb command has already finished\n");
                printf("The last command data is %d bytes\n",s->remain_command_bytes);
                s->remain_recv_bytes -= s->remain_command_bytes;
                s->header_begin = ((uint8_t *)s->header_begin) + s->remain_command_bytes;
                s->remain_command_bytes = 0;
                s->is_last_command = 0;
                if(s->remain_recv_bytes < s->header_buffer_size && s->remain_recv_bytes > 0){
                    printf("The next smb command header is uncomplete\n");
                    s->is_header_uncomplete = 1;
                    s->buffer_offset = s->remain_recv_bytes;
                    s->remain_recv_bytes = 0;
                    break;
                }
            }

        }
        /*-----------------------------------solve header-----------------------------*/
        /*--------------------------------------solve data---------------------------------------*/
        printf("SMB data processing\n");
        if(s->command_status == NT_STATUS_SUCCESS) {
            filecontent_handle(s);
            filename_handle(s);
        }

        
        /*--------------------------------------solve data---------------------------------------*/

    }

}

static void *_handler1(void *args)
{
    FILE * fp_d;
    struct _handler_args *s_args;
    uint8_t buf[BUF_SIZE];
    ssize_t nbytes;
    s_args = args;

    smb_session_t t; 
    smb_session_p s;
    s = &t;

    s->name_list = NULL;
    s->findCommand=NULL;

    memset((uint8_t *)s,0,sizeof(smb_session_t));

    char header_buffer[HEADER_BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    s->request_or_response = SMB_RESPONSE;

    fp_d = fopen("logs.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);

    fp_d = fopen("C_A_recv_data.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);
    
    fp_d = fopen("C_A_send_data.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);

    fp_d = fopen("write_offset.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);

    fp_d = fopen("logs1.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);

    fp_d = fopen("read_list.txt","w+");
    fprintf(fp_d,"%s","reset\n");
    fclose(fp_d);

    fp_d = fopen("creat_list.txt","w+");
    fprintf(fp_d,"%s","no node\n\n\n");
    fclose(fp_d);

    read_list_head = (read_command_node *)malloc(sizeof(read_command_node));
    memset(read_list_head,0,sizeof(read_command_node));
    read_list_head->next = NULL;

    creat_list_head = (creat_command_node *)malloc(sizeof(creat_command_node));
    memset(creat_list_head,0,sizeof(creat_command_node));
    creat_list_head->next = NULL;

    while (1)
    {
        
        nbytes = recv(s_args->srcsock, buf + s->buffer_offset, BUF_SIZE - s->buffer_offset, 0);

        fp_d = fopen("C_A_recv_data.txt","a+");
        fwrite(buf + s->buffer_offset,nbytes , 1 , fp_d);
        fprintf(fp_d,"%s","################");
        fclose(fp_d);

        nbytes += s->buffer_offset;
        s->remain_recv_bytes = nbytes;
        s->buffer_offset = 0;
         
        if (nbytes < 0)
        {
            perror("response Failed to recv data");
            break;
        }
        else if(nbytes == 0)
            continue;

        printf("recv data from C to A :%ld bytes\n",nbytes);

        // for(int i = 0; i < ((nbytes > 100)?100:nbytes) ; i++) {
        //     printf("0x%x ",*(buf + i));
        // }a
//----------------------------processing received data-------------------------
        s->header_begin = (uint8_t *)buf;
        smb_handle(s);
//------------------------send received data-------------------------
        SEND_DATA:
        printf("recv data from C to A :%ld bytes\n",nbytes);
        fp_d = fopen("C_A_send_data.txt","a+");
        fwrite(buf,((size_t) nbytes - s->buffer_offset),1,fp_d);
        fprintf(fp_d,"%s","################");
        fclose(fp_d);

        if (send(s_args->dstsock, buf, ((size_t) nbytes - s->buffer_offset), 0) != nbytes - s->buffer_offset)
        {
            perror("Failed to send data");
        }else{
            printf("succeed in sending %lu bytes data\n\n\n\n\n",((size_t) nbytes - s->buffer_offset));
        }
        if(s->is_header_uncomplete == 1){
            memmove(buf ,(uint8_t *)s->header_begin ,s->buffer_offset);
            s->is_header_uncomplete = 0;
        }
    }

    if (s_args->srcsock)
    {
        close(s_args->srcsock);
    }

    if (s_args->dstsock)
    {
        close(s_args->dstsock);
    }
}

static void *_handler2(void *args)
{
    smb_header_p smbhdr;
    netbios_header_p nethdr;
    struct _handler_args *s_args;
    uint8_t buf[BUF_SIZE];
    ssize_t nbytes;
    s_args = args;

    smb_session_t t; 
    smb_session_p s;
    s = &t;
    memset((uint8_t *)s,0,sizeof(smb_session_t));

    FILE * fp_d;


    int fd;
    int res;

    char header_buffer[HEADER_BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    s->request_or_response = SMB_REQUEST;
    while (1)
    {
        nbytes = recv(s_args->srcsock, buf + s->buffer_offset, BUF_SIZE - s->buffer_offset, 0);

        nbytes += s->buffer_offset;
        s->remain_recv_bytes = nbytes; 
        printf("buffer offset is %u bytes\n",s->buffer_offset);
        s->buffer_offset = 0;       
        if (nbytes < 0)
        {
            perror("request Failed to recv data");
            break;
        }
        else if(nbytes == 0)
            continue;

        printf("recv data from A to C :%ld bytes\n", nbytes);
        printf("s->is_last_command is %d\n",s->is_last_command);
        // for(int i = 0; i < ((nbytes > 100)?100:nbytes) ; i++) {
        //     printf("0x%x ",*(buf + i));
        // }
//----------------------------processing received data-------------------------
        s->header_begin = (uint8_t *)buf;
        smb_handle(s);
//------------------------send received data-------------------------
        SEND_DATA:
        if (send(s_args->dstsock, buf, ((size_t) nbytes - s->buffer_offset), 0) != nbytes - s->buffer_offset)
        {
            perror("Failed to send data");
        }else{
            printf("succeed in sending %ld bytes data\n\n\n\n\n",((size_t) nbytes - s->buffer_offset));
        }
        if(s->is_header_uncomplete == 1){
            memmove(buf ,(uint8_t *)s->header_begin ,s->buffer_offset);
            s->is_header_uncomplete = 0;
        }

    }

    if (s_args->srcsock)
    {
        close(s_args->srcsock);
    }
  
    if (s_args->dstsock)
    {
        close(s_args->dstsock);
    }
}



void print_smbhdr(smb_session_p s)
{
    printf("This smb command length is :%d bytes\n",s->smb_command_length);
    printf("protocol: %c%c%c%c\n", s->smbhdr->Protocol[0], s->smbhdr->Protocol[1], s->smbhdr->Protocol[2], s->smbhdr->Protocol[3]);

    if(s->smb1_or_smb2 == IPPROTO_SMB1){
        printf("cmd: 0x%x ", s->current_command);
        switch(s->current_command){
            case SMB1_NT_CREATE_ANDX:
                printf("      NT create Andx\n");
                break;
            case SMB1_READ_ANDX:
                printf("      Read Andx\n");
                break;
            case SMB1_CLOSE:
                printf("      close connection\n");
                break;
            default:
                printf("      other command\n");
                break;
        }
        printf("status: 0x%x\n", s->smbhdr->Status.Status);
        printf("flags: 0x%x\n", s->smbhdr->Flags);
        printf("flags2: 0x%x\n", s->smbhdr->Flags2);
        printf("tid: %x\n", s->smbhdr->Tid);
        printf("pidlow: %x\n", s->smbhdr->Pid);
        printf("uid: %x\n", s->smbhdr->Uid);
        printf("mid: %x\n", s->smbhdr->Mid);
    }else{
        printf("cmd: 0x%x ", s->current_command);
        switch(s->current_command){
            case SMB2_NEGOTIATE_PROTOCOL:
                printf("      Negotiate protocol\n");
                break;
            case SMB2_GET_INFO:
                printf("      Get Information\n");
                break;
            case SMB2_SESSION_SETUP:
                printf("      Session setup\n");
                break;
            case SMB2_TREE_CONNECT:
                printf("      Tree connect\n");
                break;
            case SMB2_TREE_DISCONNECT:
                printf("      Tree disconnect\n");
                break;
            case SMB2_CREAT:
                printf("      Create\n");
                break;
            case SMB2_CLOSE:
                printf("      Close\n");
                break;    
            case SMB2_FIND:
                printf("      Find\n");
                break;
            case SMB2_READ:
                printf("      Read\n");
                break;
            default:
                printf("      other command\n");
                break;
        }
        printf("Header length 0x%x\n",s->smb2hdr->HeaderLength);
        printf("status: 0x%x\n", s->smb2hdr->Status.Status);
        printf("flags: 0x%x\n", s->smb2hdr->Flags);
        printf("tree id: 0x%x\n", s->smb2hdr->TreeID);
        printf("process id: 0x%x\n", s->smb2hdr->ProcessID);
        printf("session id: 0x%lx\n", s->smb2hdr->SessionID);
        printf("message id: 0x%lx\n", s->smb2hdr->MessageID);   
    }

}
int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,  
        int outSize)  
{  
    assert(pOutput != NULL);  
    assert(outSize >= 6);  
  
    if ( unic <= 0x0000007F )  
    {  
        // * U-00000000 - U-0000007F:  0xxxxxxx  
        *pOutput     = (unic & 0x7F);  
        return 1;  
    }  
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
    {  
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
        *(pOutput+1) = (unic & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;  
        return 2;  
    }  
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
    {  
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
        *(pOutput+2) = (unic & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;  
        return 3;  
    }  
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
    {  
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+3) = (unic & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;  
        return 4;  
    }
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
    {  
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+4) = (unic & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;  
        return 5;  
    }  
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
    {  
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+5) = (unic & 0x3F) | 0x80;  
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;  
        return 6;  
    }  
  
    return 0;  
}  

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        printf("Usage: %s <listen_ip> <listen_port> <to_ip>\n", argv[0]);
        exit(0);
    }

    pthread_mutex_init(&mutex_creat_list,NULL);
    pthread_mutex_init(&mutex_read_list,NULL);

    char *listen_ip;
    int listen_port;
    char *to_ip;

    listen_ip = argv[1];
    listen_port = atoi(argv[2]);
    to_ip = argv[3];

    proxy_server_t server;

    proxy_server_init(&server, listen_ip, (uint16_t) listen_port, to_ip);


    proxy_server_start(&server);

    //freeNameList(short_name_list);
}