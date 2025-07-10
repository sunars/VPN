#include "capture_network_traffic.h"

/* the function allocates memory and initializes a packet buffer */
Packet_Buffer* make_packet_buffer(){
    Packet_Buffer packet_buffer = calloc(1, sizeof(Packet_Buffer));
    if(packet_buffer != NULL){
        packet_buffer->buffer = calloc(1, PACKET_SIZE);
        if(packet_buffer->buffer != NULL){
            return packet_buffer;
        }

    }
    return NULL:
}

/* read from v_nic into the packet_buffer and return the packet_buffer */
Packet_Buffer* read_packets(int v_nic, Packet_Buffer* packet_buffer){
    /* validate the inputs */
    if((v_nic > 0) && (packet_buffer != NULL)){
        size_t packets_read;
        if((packets_read = read(v_nic, packet_buffer->buffer, PACKET_SIZE)) == PACKET_SIZE){
            
            return packet_buffer;
        }
    }
    return NULL;
}


