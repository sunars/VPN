#include "capture_network_traffic.h"
#include "capture_network_traffic.c"
#include "/home/mrhx/Unity/src/unity.h"


void setUP(){}
void tearDown(){}

int v_nic = open_network_tunnel();

void test_make_packet_buffer(){
    Packet_Buffer* packet_buffer = make_packet_buffer();
    packet_buffer = read_packets(v_nic, packet_buffer);
    for(size_t i = 0; i < PACKET_SIZE-1; i++){
        printf("%02x", packet_buffer[i]);
    }
}

int main(){
    UNITY_BEGIN();




    return UNITY_END();
}
