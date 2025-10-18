#ifndef KDEXG_DAEMON_COMMUNICATION_H
#define KDEXG_DAEMON_COMMUNICATION_H

#include <stdint.h>

int init_comm();
void exit_comm();

int comm_handler(uint32_t mask);

#endif
