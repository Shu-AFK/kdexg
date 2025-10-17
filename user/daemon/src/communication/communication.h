#ifndef KDEXG_DAEMON_COMMUNICATION_H
#define KDEXG_DAEMON_COMMUNICATION_H

#include <stdint.h>

int comm_init();
void comm_exit();

int comm_handler(uint32_t mask);

#endif
