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
/**
 * the function allocates memory to hold client's IP.
 * struct ifaddrs is used to get information on interfaces and IP on client's machine.
 */
char* get_client_IP(){
	struct ifaddrs *ifaddr, *ifa;
	char* client_IP = malloc(INET_ADDRSTRLEN);
	if(client_IP == NULL){
		fprintf(stderr, "memory allocation for client_IP failed in get_client_IP()\n");
		exit(EXIT_FAILURE);
	}	

	if(getifaddrs(&ifaddr) == -1){
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
		exit(1);
	default:
		wait(NULL);
		break;
	}

}	

int packet_handler(int net_if){
	char* buffer = malloc(4500);
	if(buffer == NULL){
		fprintf(stderr, "error allocating buffer to hold the packets.\n");
		exit(EXIT_FAILURE);
	}
	
	int sock_to_server;
	sock_to_server = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
	if(sock_to_server < 0){
		fprintf(stderr, "error creating server socket.\n");
		exit(EXIT_FAILURE);
	}
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

	size_t num_read, num_written;
	while((num_read = read(net_if,buffer,4500)) > 0){
		unsigned int bytes_processed = 0;
		if(bytes_processed < num_read){
			while((num_written = write(sock_to_server, buffer + bytes_processed, PACKET_SIZE)) != PACKET_SIZE){
				bytes_processed += PACKET_SIZE;
			}	
	
		}	
	}	
	
}	




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
	
	

	free(client_IP);
	return 0;
}	
