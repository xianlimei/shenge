#pragma once
#include <stdint.h>
#include <stdio.h>
#include "proxyserver.h"

typedef struct read_command_s {
    uint64_t read_mid;
    uint64_t fid_low;
    uint64_t fid_high;
    uint64_t read_offset;
    struct read_command_s *next;
}read_command_node;

int print_read_list_node(read_command_node * head);
int creat_read_list_node(volatile read_command_node * head, smb2_header_p smb2hdr);
uint64_t pop_read_list_node(read_command_node * head, uint64_t mid);