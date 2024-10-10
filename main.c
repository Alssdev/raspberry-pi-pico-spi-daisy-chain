#include "main.h"
#include "frame.h"
#include "slave.h"
#include "master.h"

#include "lib/list.h"

#include "pico/sem.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include <stdlib.h>
#include <string.h>

#define SLEEP_TIME_MS 50

int main (void) {
  // Enable UART
  stdio_init_all ();

  led_init();
  slave_init ();
  master_init ();

  // example of data from Driver Layer
  // ============================================================
  char *data = "I'm Oscar!";
  struct frame *f = malloc (sizeof *f);
  f->to = 0x5;
  f->from = 0x1;
  f->type = message;
  for (unsigned int i = 0; i < strlen(data) + 1; i++) {
    f->data[i] = data[i];
  }
  struct receive_elem *elem = malloc(sizeof *elem);
  elem->f = f;

  sem_acquire_blocking (&receive_sema);
  list_push_back (&receive_list, &elem->elem);
  sem_release (&receive_sema);

  sleep_ms(2 * 1000);
  // ============================================================

  while (1) {
    elem = NULL;

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
    led_ON();
    sleep_ms (SLEEP_TIME_MS);
    led_OFF();
    sleep_ms (SLEEP_TIME_MS);
  }
}

void led_init (void) {
  gpio_init(25);
  gpio_set_dir(25, GPIO_OUT);
}

void led_ON (void) {
  gpio_put(25, 1);
}

void led_OFF (void) {
  gpio_put(25, 0);
}
