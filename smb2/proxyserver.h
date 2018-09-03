/* Proxy Server Declaration */
#ifndef __SHENGE
#define __SHENGE

#include <stdint.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <iconv.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <wchar.h>
#include <math.h>
#include"short_name_list.h"

#define SMB1_NT_CREATE_ANDX 0xa2
#define SMB1_READ_ANDX 0x2e
#define SMB1_CLOSE 0x04
#define SMB2_NEGOTIATE_PROTOCOL 0x0
#define SMB2_SESSION_SETUP 0x1
#define SMB2_SESSION_LOGOFF 0x2
#define SMB2_TREE_CONNECT 0x3
#define SMB2_TREE_DISCONNECT 0x4
#define SMB2_CREAT 0x5
//#define SMB2_CLOSE 0x6
//#define SMB2_FLUSH 0x7
//#define SMB2_READ 0x8
//#define SMB2_WRITE 0x9
//#define SMB2_LOCK 0xa
#define SMB2_IOCTL 0xb
//#define SMB2_CANCEL 0xc
#define SMB2_KEEP_ALIVE 0xd
//#define SMB2_FIND 0xe
//#define SMB2_NOTIFY 0xf
#define SMB2_GET_INFO 0x10
#define SMB2_SET_INFO 0x11
//#define SMB2_BREAK 0x12



#define IPPROTO_SMB1 0x0
#define IPPROTO_SMB2 0x1

#define SMB_REQUEST 0x1
#define SMB_RESPONSE 0x0


#define BUF_SIZE 4000
#define HEADER_BUF_SIZE 32 //the header length of command "Read andx" 

#define SMB1_HEADER_LENGTH 32
#define SMB2_HEADER_LENGTH 64
#define NETBIOS_HEADER_LENGTH 4

#define NT_STATUS_SUCCESS 0x00000000

#define CREAT_RESPONSE_ALLOCATION_SIZE_FIELD_OFFSET 104
#define CREAT_RESPONSE_END_FILE_FIELD_OFFSET 112

#define PHYSICAL_DISK 1
typedef struct proxy_server_s
{
    uint32_t listen_ip;
    uint16_t listen_port;
    uint32_t to_ip;

    int _sock;
    struct sockaddr_in listen_addr;
    struct sockaddr_in to_addr;
} proxy_server_t;

typedef volatile struct file_offset_s{
    uint64_t mid;
    uint64_t file_offset;
}file_offset_t;

typedef volatile struct file_message_s{
    uint32_t tid;
    uint64_t fid_low;
    uint64_t fid_high;
    uint64_t size;
}file_message_t;

typedef struct creat_command_s {
    uint64_t creat_mid;
    uint64_t fid_low;
    uint64_t fid_high;
    uint8_t *file_name;
    uint16_t filename_length;
    FILE *fp;
    struct creat_command_s *next;
}creat_command_node;

typedef struct read_command_s {
    uint64_t read_mid;
    uint64_t fid_low;
    uint64_t fid_high;
    uint64_t read_offset;
    struct read_command_s *next;
}read_command_node;

typedef struct file_attribute_s {
    uint32_t read_only : 1;
    uint32_t hidden : 1;
    uint32_t system : 1;
    uint32_t Volume_id : 1;
    uint32_t directory : 1;
    uint32_t archive : 1;
    uint32_t device : 1;
    uint32_t normal : 1;
    uint32_t reserved : 24;
}file_attribute_t ,*file_attribute_p;


typedef proxy_server_t *proxy_server_p;

//static void _on_accept(proxy_server_p server, int sock, struct sockaddr_in *addr);
struct _on_accept_arg_s
{
    int sock;
    proxy_server_p server;
    struct sockaddr_in addr;
};

struct _handler_args
{
    int srcsock;
    int dstsock;
};

/* SMB Declaration */

typedef struct netbios_header_s
{
    uint8_t type;
    uint8_t lenhigh;
    uint8_t lenmiddle;
    uint8_t lenlow;
}__attribute__((packed)) netbios_header_t;
typedef netbios_header_t *netbios_header_p;
 
 
typedef struct  {
    uint8_t Protocol[4];                // Contains 0xFF,'SMB'
    uint8_t Command;                 // Command code
    union {
        struct {
            uint8_t ErrorClass;         // Error class
            uint8_t Reserved;           // Reserved for future use
            uint16_t Error;             // Error code
        } DosError;
        uint32_t Status;                 // 32-bit error code
    } Status;
    uint8_t Flags;                      // Flags
    uint16_t Flags2;                    // More flags
    union {
        uint16_t Pad[6];                // Ensure section is 12 bytes long
        struct {
            uint16_t PidHigh;           // High part of PID
            uint32_t  Unused;            // Not used
            uint32_t  Unused2;
    } Extra;
    };
    uint16_t Tid;                       // Tree identifier
    uint16_t Pid;                       // Caller's process id
    uint16_t Uid;                       // Unauthenticated user id
    uint16_t Mid;                       // multiplex id
}__attribute__((packed)) smb_header_t;
typedef smb_header_t *smb_header_p;

typedef struct  {
    uint8_t Protocol[4];                // Contains 0xFE,'SMB'
    uint16_t HeaderLength;                 // Command code
    uint16_t CreditCharge;
    union {
        struct {
            uint8_t ErrorClass;         // Error class
            uint8_t Reserved;           // Reserved for future use
            uint16_t Error;             // Error code
        } DosError;
        uint32_t Status;                 // 32-bit error code
    } Status;
    uint16_t Command;
    uint16_t CreditGranted;
    uint32_t Flags;
    uint32_t NextCommand;               // For a compounded request, it will be set to the offset, in bytes, from the beginning of this SMB2 header to the start of the subsequent 8-byte aligned. 
    uint64_t MessageID;                 // A value that identifies a message request and response uniquely across all messages that are sent on the same SMB 2 Protocol transport connection.
    uint32_t ProcessID;
    uint32_t TreeID;                    // identifies the tree connect for the command.
    uint64_t SessionID;                 // Uniquely identifies the established session for the command.
    uint64_t SianatureLow;
    uint64_t SianatureHigh;
}__attribute__((packed)) smb2_header_t;
typedef smb2_header_t *smb2_header_p;

typedef struct smb_session_s {
    smb_header_p smbhdr;
    smb2_header_p smb2hdr;
    netbios_header_p nethdr;
    uint8_t smb1_or_smb2;           // judge whether protocol is smb1 or smb2
    uint8_t is_last_command;        // mark the last command hasn't been tranferred comppletely
    int32_t remain_recv_bytes;      // record the remaining data length waiting to be processed
    int32_t remain_command_bytes;   // record the remaining data length of last command
    uint32_t netbios_command_length;  // record the current netbios command length
    uint32_t smb_command_length;      // record the current smb command length
    uint8_t current_command;
    uint32_t file_content_remain_length;
    uint8_t is_header_uncomplete;
    uint32_t buffer_offset;
    uint8_t *header_begin;
    uint8_t if_need_break;
    uint8_t if_need_continue;
    uint8_t header_buffer_size;     //record the size we prepared for header buffer,in smb1 it's the sum of smb1 header length and netbios header length
    uint8_t is_compounded_command;
    uint8_t if_have_netbiosHeader;
    uint32_t before_command_length; //record the sum of all the smb command except the last one in a compounded netbios command
    uint32_t command_status;        // record the current command status


    uint32_t fd;
    FILE * fp;
    uint8_t request_or_response;    //judge whether the data is  from server or client,0----data from server   1-----data from client

    uint8_t *file_content_begin;
    uint32_t file_content_length;
    uint64_t file_content_offset;       // record the file content offset of the read command
    uint32_t remain_read_length;
    uint32_t current_tid;                   // record the tree id of current command
    uint32_t file_tid;                      // record the tree id of the transmitted file
    creat_command_node *last_creat_list_node;
    read_command_node *last_read_list_node;

    uint64_t current_fid_low;
    uint64_t current_fid_high;

    uint16_t filename_total_length;
    uint32_t filename_offset;
    uint16_t filename_remain_length;
    uint16_t filename_length;        //record the length of the file's name which will be dumped
    uint16_t *filename_begin; 
    uint8_t *filename_utf_8;
    uint8_t *filename_unicode;
    uint32_t filename_storage_offset;
    uint8_t if_already_malloc;

    //find命令使用
    //uint32_t find_get_length;
    uint32_t find_length;
    int find_remain_length;
    NameList name_list;
    uint8_t  *findCommand;
} smb_session_t;


typedef struct smb_session_s *smb_session_p;


static void *_on_accept(void *args);

static void _create_server(proxy_server_p server);

void proxy_server_start(proxy_server_p server);

void print_addr(char *info, uint32_t addr);

void proxy_server_init(proxy_server_p server, char *listen_ip, uint16_t listen_port,
                       char *to_ip);

void print_smbhdr(smb_session_p s);

int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,  
        int outSize);

void smb_handle(smb_session_p s);       // handle smb data

void filecontent_handle(smb_session_p s); // handle file content data

void filename_handle(smb_session_p s);  // handle file name data

void record_logs(smb_session_p s);      //record the positon and length message about file

void check_header(smb_session_p s);     //check whether smb header is correct

static void *_handler1(void *args);     // handle the data from computer C in yellow zone to computer A in green zone 

static void *_handler2(void *args);     // handle the data from computer A in green zone to computer C in yellow zone 
/* Proxy Server Implementation */

int print_read_list_node(read_command_node * head);

int print_creat_list_node(creat_command_node * head);

read_command_node  * creat_read_list_node(read_command_node * head, smb2_header_p smb2hdr);

creat_command_node * creat_creat_list_node(creat_command_node * head, smb2_header_p smb2hdr);

uint64_t pop_read_list_node(read_command_node * head, uint64_t mid , smb_session_p s);

int del_creat_list_node_by_mid(creat_command_node * head, uint64_t mid);

int del_creat_list_node_by_fid(creat_command_node * head, smb2_header_p smb2hdr);

int add_fid_creat_list_node(creat_command_node * head, smb2_header_p smb2hdr,NameList name_list);

FILE * find_creat_list_node_by_fid(creat_command_node * head, uint64_t fid_low, uint64_t fid_high);

#endif

