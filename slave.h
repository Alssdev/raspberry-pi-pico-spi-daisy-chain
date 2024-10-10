#ifndef SLAVE_H
#define SLAVE_H

#include <stdint.h>
#include "lib/list.h"
#include "pico/sem.h"

/* receive list */
extern semaphore_t receive_sema;     // synchronization
extern struct list receive_list;

struct receive_elem {
  struct list_elem elem; 
  struct frame *f;
};

void slave_init (void);

#endif // !SLAVE_H
