#include "router.h"
#include "lib/list.h"
#include "master.h"
#include "pico/sem.h"
#include "pico/stdio.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/time.h"
#include "slave.h"

int main (void) {
  // Enable UART
  stdio_init_all ();

  slave_init ();
  master_init ();

  // example of data from Drvier Layer
  // ============================================================
  char *data = "I'm Poseidon!";
  struct frame *f = malloc (sizeof *f);
  f->to = 0x5;
  f->from = 0x1;
  f->type = message;
  for (int8_t i = 0; i < 14; i++) {
    f->data[i] = data[i];
  }
  struct receive_elem *elem = malloc(sizeof *elem);
  elem->f = f;

  sem_acquire_blocking (&receive_sema);
  list_push_back (&receive_list, &elem->elem);
  sem_release (&receive_sema);

  sleep_ms(1 * 1000);
  // ============================================================

  while (1) {
    struct receive_elem *elem = NULL;

    // read a frame from receive buffer
    sem_acquire_blocking (&receive_sema);
    if (!list_empty (&receive_list))
      elem = list_entry (list_pop_front (&receive_list), struct receive_elem, elem);
    sem_release (&receive_sema);

    // send frame to next router
    if (elem != NULL) {
      elem->f->from++;
      master_propagate (elem->f);

      // free resources
      free (elem->f);
      free (elem);
    }

    // this is optional.
    sleep_us (500);
  }
}
