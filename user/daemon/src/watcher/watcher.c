#include "watcher.h"

#include "defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/types.h>

int watcher(message_handler_fptr handler) {
  int fd = inotify_init();
  if (fd < 0) {
    perror("inotify_init");
    return -1;
  }

  int wd = inotify_add_watch(fd, POLICY_FILE_PATH, IN_MODIFY | IN_CREATE);
  if (wd == -1) {
    perror("inotify_add_watch");
    close(fd);
    return -1;
  }

  char buffer[4096];
  const struct inotify_event *event;
  ssize_t size;

  while (true) {
    size = read(fd, buffer, sizeof(buffer));
    if (size == -1 && errno != EAGAIN) {
      perror("read");
      inotify_rm_watch(fd, wd);
      close(fd);
      return -1;
    }

    if (size <= 0) continue;

    for (char *ptr = buffer; ptr < buffer + size
    ; ptr += sizeof(struct inotify_event) + event->len) {
      event = (const struct inotify_event *) ptr;

      if((*handler)(event->mask) == -1) {
        inotify_rm_watch(fd, wd);
        close(fd);
        return -1;
      }
    }
  }

  inotify_rm_watch(fd, wd);
  close(fd);

  return 0;
}
