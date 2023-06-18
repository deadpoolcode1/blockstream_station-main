#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "libs/database.hpp"
static int read_initial_configuration();
int state_machine();
int main (void)
{
  zmq::context_t context(1);
    //  Socket to talk to server
    printf ("starting main…\n");
    zmq::socket_t sock (context, ZMQ_REQ);
    sock.connect("ipc:///tmp/test");
    char msg[] = "get_conf";
    char buffer[256] = {0x00};
    int request_nbr;
    read_initial_configuration();
    state_machine();
    return 0;
}

/*read data from configuration manager
intial configuration is kept in mysql file
configuration is kept in object.
*/
static int read_initial_configuration()
{
  database_on_get();
  database_on_set(25,45);
  database_on_get();
  return 0;
}

int state_machine()
{
  return 0;
}

int state_discovery()
{
  return 0;
}

int state_power_sequence()
{
  return 0;
}

int state_monitor()
{
  return 0;
}
