#ifndef __SLAVE_H__
#define __SLAVE_H__

#include <stdint.h>
#include "lib/list.h"
#include "pico/sem.h"

/* receive list */
extern semaphore_t receive_sema;     // syncronization
extern struct list receive_list;

struct receive_elem {
  struct list_elem elem; 
  struct frame *f;
};

void slave_init (void);

#endif // !SLAVE_H

