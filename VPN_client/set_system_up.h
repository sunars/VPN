#ifndef SET_SYSTEM_UP_H_
#define SET_SYSTEM_UP_H_

#include "common_includes.h"

#define INET_ADDR_LEN 16 



static int open_network_tunnel();  /* opens virtual network interface in our system, i.e the tun interface in linux. */
static void execute_command(char* command);    /* forks a child process, creates a shell and executes the command */
static char* get_client_IP();  /* function to get IP address of the actual physical network interface on client machine. */



#endif
