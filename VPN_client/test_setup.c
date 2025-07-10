#include "set_system_up.h"
#include "set_system_up.c"
#include "/home/mrhx/Unity/src/unity.h"

#define COMMAND_LEN 46




void setUp(){}

void tearDown(){}

void test_open_network_tunnel(){
    int tun = open_network_tunnel();
    TEST_ASSERT_TRUE((tun > 0));
}

void test_execute_command(){
    char command_buffer[COMMAND_LEN];
    memset(command_buffer, 0, sizeof(command_buffer));
    fprintf(stdout, "please pass command to the program.\n");
    if((fgets(command_buffer, 16, stdin)) == NULL){
        fprintf(stderr, "error reading user input commands");
        exit(1);
    }
    execute_command(command_buffer);
}

void test_get_client_IP(){
    
    char* IP = get_client_IP();

    TEST_ASSERT_NOT_NULL(IP);
    fprintf(stdout,"The IP of this machine is %s\n", IP);
}

int main(int argc, char* argv[]){
    UNITY_BEGIN();

    RUN_TEST(test_open_network_tunnel);
    RUN_TEST(test_execute_command);
    RUN_TEST(test_get_client_IP);


    return UNITY_END();
}
