#ifndef VPN_CLIENT_H_
#define VPN_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>


#define PACKET_SIZE 1460
#define VPN_SERVER_IP "198.51.100.5"    /* dummy IP for testing */
#define VPN_SERVER_PORT 65000


typedef struct __attribute__((packed)){
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    /* ihl is actually a value when divided by 32-bit, condensed so that we can store it in 4 bits, example: suppose we have 20 bytes of header length, what would the value be when it is divided by 32-bit? That's the value we store. */
    uint8_t ihl:4;
    uint8_t version:4;
#else
    uint8_t version:4;
    uint8_t ihl:4;
#endif
    uint8_t tos;
    uint16_t packet_length;
    uint16_t identification;
    uint16_t flag:3, offset:13;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dst_ip;
}IP_H;

typedef struct __attribute__((packed)){
    IP_H* new_header;
    uint8_t* packet;
}N_Packet;


char* get_client_IP();
void execute_command(char* command);
uint16_t calculate_checksum(N_Packet* new_packet);
N_Packet* make_new_packet(uint8_t* packet);
void non_blocking_read_write(int fd);
int packet_handler(int net_if);
void free_packet(N_Packet* packet);
uint8_t* create_packet_buffer();

#endif
