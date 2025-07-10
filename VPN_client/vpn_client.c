<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <ifaddrs.h>


#define PACKET_SIZE 1500
#define VPN_SERVER_IP "198.51.100.5"	/* fake one just for testing */
#define VPN_SERVER_PORT 65000


=======
#include "vpn_client.h"
>>>>>>> 17d321e (initial phase of the project)

/**
 * the function allocates memory to hold client's IP.
 * struct ifaddrs is used to get information on interfaces and IP on client's machine.
 */
char* get_client_IP(){
<<<<<<< HEAD
=======
    
>>>>>>> 17d321e (initial phase of the project)
	struct ifaddrs *ifaddr, *ifa;
	char* client_IP = malloc(INET_ADDRSTRLEN);
	if(client_IP == NULL){
		fprintf(stderr, "memory allocation for client_IP failed in get_client_IP()\n");
		exit(EXIT_FAILURE);
	}	

<<<<<<< HEAD
	if(getifaddrs(&ifaddr) == -1){
=======
	if(getifaddrs(ifaddr) == -1){
>>>>>>> 17d321e (initial phase of the project)
		fprintf(stderr, "error getting client's network interface details.\n");
		exit(EXIT_FAILURE);
	}	

	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
		if(!ifa->ifa_addr){
			continue;
		}
		short family = ifa->ifa_addr->sa_family;
		if(family == AF_INET){
			void* src;
			src = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
			inet_ntop(family, src, client_IP, sizeof(client_IP));
		}	
	}
          
	freeifaddrs(ifaddr);
	return client_IP;
}	

/**
 * execute_setup_command() takes a pointer to the commands, parses them into tokens, forks processes and executes them.
 * a new bash shell is spawned and we rely on bash to parse and interpret the commands.
 */

void execute_command(char* command){
	switch(fork()){
	case -1:
		exit(EXIT_FAILURE);
	case 0:	// child process
		char* argv[] = {"/bin/bash", "-c", command, NULL};
		execv("/bin/bash", argv);
		fprintf(stderr, "error executing the commands.\n");
<<<<<<< HEAD
		exit(1);
=======
		exit(EXIT_SUCCESS);
>>>>>>> 17d321e (initial phase of the project)
	default:
		wait(NULL);
		break;
	}

}	

<<<<<<< HEAD
typedef struct IP_H{
	
}IP_H;



int packet_handler(int net_if){
	char* buffer = malloc(4500);
	if(buffer == NULL){
		fprintf(stderr, "error allocating buffer to hold the packets.\n");
		exit(EXIT_FAILURE);
	}
	
	int sock_to_server;
	sock_to_server = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
=======
uint16_t calculate_checksum(N_Packet* new_packet){
    uint16_t sum = 0;
    uint16_t header[20];
    memcpy(header,new_packet->new_header, sizeof(new_packet->new_header));
    for(uint16_t i = 0; i < 10; i++){
        sum += header[i];

        if(sum > 0xffff){
            sum = (sum & 0xffff) + 0x1;
        }
    }

    return ~((uint16_t) sum);
}



N_Packet* make_new_packet(uint8_t* packet){
    
    N_Packet* new_packet = malloc(sizeof(N_Packet));  /** malloc here */
    if(new_packet != NULL){
        new_packet->new_header = malloc(20);
        if(new_packet->new_header != NULL){
            new_packet->new_header->version = 4;
            new_packet->new_header->ihl = 5;
            new_packet->new_header->tos = 0;
            new_packet->new_header->packet_length = htons(1480);
            new_packet->new_header->identification = packet[2];
            new_packet->new_header->flag = htons(0b010 << 13);
            new_packet->new_header->ttl = 100;
            new_packet->new_header->protocol = IPPROTO_TCP;
            new_packet->new_header->checksum = 0;
            if((inet_pton(AF_INET, get_client_IP(), &(new_packet->new_header->src_ip))) < 0){
                fprintf(stderr, "couldn't get client's IP for new packet's header.\n");
                exit(EXIT_FAILURE);
            }
            if((inet_pton(AF_INET,VPN_SERVER_IP, &(new_packet->new_header->dst_ip))) <0){
                fprintf(stderr, "couldn't get server's IP for new packet's header.\n");
                exit(EXIT_FAILURE);
            }
            new_packet->packet = malloc(sizeof(PACKET_SIZE));
            if(new_packet->packet == NULL){
                fprintf(stderr, "allocation of space for packet inside new packet failed.\n");
                exit(EXIT_FAILURE);
            }
            new_packet->packet = packet;

            return new_packet;

        }
    }   
    return NULL;
}


uint8_t* create_packet_buffer(){
    uint8_t* packet_buffer = malloc(PACKET_SIZE * 3);
    if(packet_buffer == NULL){
        fprintf(stderr, "error creating buffer to hold packets.\n");
        exit(EXIT_FAILURE);
    }
    return packet_buffer;
}


void non_blocking_read_write(int fd){
    
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags < 0){
        fprintf(stderr, "error in getting flags from the file descriptor\n");
        exit(1);
    }
    if((fcntl(fd, F_SETFL, flags | O_NONBLOCK)) < 0){
        fprintf(stderr, "error setting the fd to non-blocking mode.\n");
        exit(1);
    }

    fd_set readfds, writefds;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);

    int max_fd = fd;

    FD_SET(fd, &readfds);
    FD_SET(fd, &writefds);

    if((select(max_fd + 1, &readfds, &writefds, NULL, NULL)) > 0){
        while()
    }

}


int packet_handler(int net_if){
	
	int sock_to_server;
	sock_to_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
>>>>>>> 17d321e (initial phase of the project)
	if(sock_to_server < 0){
		fprintf(stderr, "error creating server socket.\n");
		exit(EXIT_FAILURE);
	}
<<<<<<< HEAD
=======

>>>>>>> 17d321e (initial phase of the project)
	struct sockaddr_in server;
	memset(&server, 0, sizeof(struct sockaddr));

	server.sin_family = AF_INET;
	server.sin_port = htons(VPN_SERVER_PORT);
	if((inet_pton(AF_INET, VPN_SERVER_IP, &server.sin_addr)) <= 0){
		fprintf(stderr, "error assigning IP to the server address.\n");
		exit(EXIT_FAILURE);
	}	

	if((connect(sock_to_server, (struct sockaddr*) &server, sizeof(struct sockaddr_in))) < 0){
		fprintf(stderr, "error connecting to the server.\n");
		exit(EXIT_FAILURE);
	}	
<<<<<<< HEAD

	size_t num_read, num_written;
	while((num_read = read(net_if,buffer,4500)) > 0){
		unsigned int bytes_processed = 0;
		if(bytes_processed < num_read){

			char* processing_buffer = mem
			while((num_written = write(sock_to_server, buffer + bytes_processed, PACKET_SIZE)) != PACKET_SIZE){
				bytes_processed += PACKET_SIZE;
			}	
	
		}	
	}	
	
}	


=======
    
    uint8_t* packet_buffer = create_packet_buffer();

	size_t num_read, num_sent;
	while((num_read = read(net_if,packet_buffer,(PACKET_SIZE*3))) > 0){
		uint8_t bytes_processed = 0;
		while(bytes_processed < num_read){

			uint8_t processing_buffer[PACKET_SIZE];

            memcpy(processing_buffer, packet_buffer + bytes_processed, PACKET_SIZE);

            N_Packet* packet_to_send = make_new_packet(processing_buffer);
            uint16_t checksum= calculate_checksum(packet_to_send);
            packet_to_send->new_header[10] = checksum;
            
            uint8_t* send_buffer[]
            memcpy(processing_buffer, packet_to_send, PACKET_SIZE);
            
			if((num_sent = send(sock_to_server, processing_buffer, PACKET_SIZE, 0)) != PACKET_SIZE){
			    fprintf(stderr, "error sending bytes into the socket for transmission.\n");
                exit(EXIT_FAILURE);
			}	
	        bytes_processed += PACKET_SIZE;
            free_packet(packet_to_send);
		}	
	}	
	
    free(packet_buffer);
    packet_buffer == NULL;
    
    close(sock_to_server);

    return 0;
}	

void free_packet(N_Packet* new_packet){
    free(new_packet->new_header);
    free(new_packet->packet);
    free(new_packet);
    new_packet->new_header = NULL;
    new_packet->packet = NULL;
    new_packet = NULL;
}
>>>>>>> 17d321e (initial phase of the project)


int main(int argc, char* argv[]){
	
	int virtual_NIC;
	virtual_NIC = open("/dev/net/tun", O_RDWR);
	if(virtual_NIC < 0){
		fprintf(stderr, "error creating virtual network interface\n");
		exit(1);
	}	
	
	char* client_IP = get_client_IP();

	char* set_default_route_command = "ip route add default dev tun";
	char* set_tun_up_command = "ip link set tun up";
	char route_to_vpn_server_command[51];
	
	snprintf(route_to_vpn_server_command, 51, "ip route add %s via %s", VPN_SERVER_IP, client_IP);
	execute_command(set_default_route_command);
	execute_command(route_to_vpn_server_command);
	execute_command(set_tun_up_command);
	
<<<<<<< HEAD
	

	free(client_IP);
=======
    if((packet_handler(virtual_NIC)) < 0){
        fprintf(stderr, "error sending packets to the VPN server.\n");
        exit(EXIT_FAILURE);
    }	


    close(virtual_NIC);
	free(client_IP);
    client_IP = NULL;
>>>>>>> 17d321e (initial phase of the project)
	return 0;
}	
