#ifndef CAPTURE_NETWORK_TRAFFIC_H_
#define CAPTURE_NETWORK_TRAFFIC_H_


#include "set_system_up.h"


#define PACKET_SIZE 1382


/* a buffer to hold all incoming packets from client's machine.*/
typedef struct Packet_Buffer{
    uint8_t* buffer;           
}Packet_Buffer;


Packet_Buffer* make_packet_buffer(); /* makes and returns a packet buffer to hold incoming packets from client. */
Packet_Buffer* read_packets(int v_nic, Packet_Buffer* packet_buffer); /* reads packets from the tunnel and puts it in the packet_buffer and returns it. */





#endif
