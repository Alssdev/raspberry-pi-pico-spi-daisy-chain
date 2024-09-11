//=================================================================================//

#include "slave.h"
#include "frame.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "lib/list.h"
#include "pico/sem.h"
#include "pico/stdlib.h"
#include "pico/sem.h"

//=================================================================================//

semaphore_t receive_sema;     // syncronization
struct list receive_list;

//=================================================================================//

void spiReceiveISR () {
  // create a new frame
  struct frame *f = malloc (sizeof *f);

  // only one byte is read at a time
  uint8_t in_buf[1];

  // read byte using SPI
  spi_read_blocking (spi0, 0, (uint8_t *)f, sizeof *f);

  if (sem_try_acquire (&receive_sema)) {
    // add it to receive_list;
    struct receive_elem *elem = malloc (sizeof *elem);
    elem->f = f;
    list_push_back (&receive_list, &elem->elem);
    printf ("{ from: %d, data: %s }\n", f->from, f->data);
    sem_release (&receive_sema);

  } else {
    // TODO
  }
  // }
}

//=================================================================================//

void slave_init (void) {
  // enable SPI0 at 1MHz
  spi_init (spi0, BAUDRATE);
  spi_set_slave (spi0, true);

  // assign SPI functions to default SPI pint.
  // for more info read RP2040 Datasheet(2.19.2. Function Select).
  gpio_set_function (16, GPIO_FUNC_SPI);
  gpio_set_function (17, GPIO_FUNC_SPI);
  gpio_set_function (18, GPIO_FUNC_SPI);
  // gpio_set_function (19, GPIO_FUNC_SPI);

  // Enable the RX FIFO interrupt (RXIM)
  spi0_hw->imsc = 1 << 2;

  // Enable the SPI interrupt
  irq_set_enabled (SPI0_IRQ, 1);

  // Attach the interrupt handler
  irq_set_exclusive_handler (SPI0_IRQ, spiReceiveISR);

  // init list
  sem_init (&receive_sema, 1, 1);
  list_init (&receive_list);
}

