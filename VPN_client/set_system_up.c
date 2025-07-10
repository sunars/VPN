#include "set_system_up.h"

/**
* open_network_tunnel() simply opens a tun special file, no device drivers needed as tun interface is given to us by linux.
*/ 
static int open_network_tunnel(){
    int v_nic = open("/dev/net/tun", O_RDWR);
    if(v_nic < 0){
        fprintf(stderr, "error creating tunnel for VPN.\n");
        exit(1);
    }
    return v_nic;
}

/**
* execute_command() takes the command to execute as string. It spawns a new shell and executes the command.
*/
static void execute_command(char* command){
    switch(fork()){

        case -1:
            /* error creating child process. */
            fprintf(stderr, "error forking process to execute the setup command.\n");
            exit(0);
        case 0: /* child process */
            char* argv[] = {"/bin/bash", "-c", command, NULL};
            execv("/bin/bash", argv);
            
            /* successful program execution never reaches this step */
            fprintf(stderr, "error in executing the command as separate process.\n");
            exit(EXIT_FAILURE);
        default:
            wait(NULL);
            break;

    }
}

/**
* the function traverses the network interfaces on the client machine and extracts the IP of the interafce being used to route the network traffic. heap memory is allocated to return the IP address.
*/
static char* get_client_IP(){
    
    char* IP = malloc(INET_ADDR_LEN);
    if(IP == NULL){
        fprintf(stderr, "error allocating memory to return the client's IP.\n");
        exit(EXIT_FAILURE);
    }

    struct ifaddrs *ifaddr, *ifa;
    
    /* obtain a linked list of all interfaces */
    if((getifaddrs(&ifaddr)) == -1){
        fprintf(stderr, "error getting the network interfaces on the system\n");
        exit(EXIT_FAILURE);
    }

    /* traverse through the linked list to get the correct IP address */
    for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next){
        /* if a certain interface doesn't have any address, just ignore it */
        if((!ifa->ifa_addr) || (strcmp((ifa->ifa_name), "lo") == 0)) {
            continue;
        }
        
        short family;

        family = ifa->ifa_addr->sa_family;
        if(family == AF_INET){
            struct in_addr the_ip;
            the_ip = ((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            if((inet_ntop(family, &the_ip, IP, INET_ADDR_LEN)) == NULL){
                fprintf(stderr, "erroe getting IP address from the network interfaces.\n");
                exit(1);
            }
            freeifaddrs(ifaddr);
            return IP;
        }
    }

    return NULL;
}

