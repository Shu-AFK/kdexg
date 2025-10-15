#ifndef KDEXG_DAEMON_WATCHER_H
#define KDEXG_DAEMON_WATCHER_H

#include <stdint.h>

typedef int (*message_handler_fptr)(uint32_t mask);
int watcher(message_handler_fptr handler);

#endif
