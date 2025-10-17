#include "watcher.h"

#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/inotify.h>
#include <sys/types.h>

extern volatile sig_atomic_t stop;

int watcher(message_handler_fptr handler) {
  int fd = inotify_init();
  if (fd < 0) {
    perror("inotify_init");
    return -1;
  }

  int wd = inotify_add_watch(fd, POLICY_FILE_PATH, 
                              IN_CLOSE_WRITE | IN_MODIFY | IN_MOVE_SELF | IN_DELETE_SELF);
  if (wd == -1) {
    perror("inotify_add_watch");
    close(fd);
    return -1;
  }

  char buffer[4096] __attribute__ ((aligned(__alignof__(struct inotify_event))));
  const struct inotify_event *event;
  ssize_t size;

  while (!stop) {
    size = read(fd, buffer, sizeof(buffer));
    if (size == -1) {
      if (errno == EAGAIN || errno == EINTR)
        continue;

      perror("read");
      break;
    }

    for (char *ptr = buffer; ptr < buffer + size;) {
      event = (const struct inotify_event *) ptr;
      ptr += sizeof(struct inotify_event) + event->len;

      printf("[INFO] policy file changed (mask: 0x%x)\n", event->mask);
      
      if((*handler)(event->mask) == -1) {
        printf("[ERR] handler failed\n");
        goto cleanup;
      }
      usleep(100000);
    }
  }

cleanup:
  inotify_rm_watch(fd, wd);
  close(fd);
  return 0;
}
