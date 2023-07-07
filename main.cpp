#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "zmq_server/zmq_server.h"
#include "libs/database.hpp"

static int read_initial_configuration();
int state_machine();
int main (void)
{
    zmq::context_t context(1);
    ZmqServer zmqServer(context);
    zmqServer.Start();
    printf ("starting main…\n");
    // Wait for some time and then stop the server
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
  while (true)
  {
    sleep(1);
  }
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
