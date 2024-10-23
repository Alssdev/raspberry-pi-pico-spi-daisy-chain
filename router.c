#include "router.h"
#include "frame.h"
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

  sleep_ms(1000);

  slave_init ();
  master_init ();

  // example of data from Drvier Layer
  // ============================================================
  char *data = "Probando probando 123";
  struct frame *f = malloc (sizeof *f);
  f->to = 0x4;
  f->from = 0x2;
  f->length = 21 + 1;   // + 1 because of \0 at the end of a string
  f->header_checksum = ((f->to + f->from + f->length) ^ 0xFF) + 0x1;    // checksum is ALWAYS calculated the same

  // reserve memory and read
  f->data = malloc (f->length);
  for (int8_t i = 0; i < f->length; i++) {
    f->data[i] = data[i];
  }

  // add to list
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
      // elem->f->from++;
      struct frame *f = elem->f;
      printf ("(%d, %d, %d, %d, %s)\n", f->to, f->from, f->length, f->header_checksum, f->data);
      master_propagate (f);

      // free resources
      free (elem->f->data);
      free (elem->f);
      free (elem);
    }

    // this is optional.
    sleep_ms (10);
  }
}
