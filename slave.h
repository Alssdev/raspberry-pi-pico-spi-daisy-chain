#include <stdint.h>
#include "lib/list.h"
#include "pico/sem.h"
#include "pico/sync.h"


/* frame definitions */
enum frame_type { message };
struct frame {
  enum frame_type type;
  uint8_t to;                 // assigned in class
  uint8_t from;               // assigned in class
  char data[99];              // actual data
};

/* receive list */
semaphore_t receive_sema;
struct list receive_list;
struct receive_elem {
  struct list_elem elem; 
  struct frame *f;
};

void init_configs (void);
