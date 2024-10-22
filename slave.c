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
  // buffers 
  uint8_t header[4];
  uint8_t *data;

  // read first 4 Bytes (frame header: to, from, length, checksum)
  spi_read_blocking (spi0, 0, header, 4);

  // validate checksum
  uint8_t valid = header[0] + header[1] + header[2] + header[3]; // to + from + length + checksum
  while (valid != 0) {
    printf("checksum error\n");
    header[0] = header[1];
    header[1] = header[2];
    header[2] = header[3];

    spi_read_blocking (spi0, 0, &header[3], 1);
    
    // try agin
    valid = header[0] + header[1] + header[2] + header[3]; // to + from + length + checksum
  }

  // create frame 
  struct frame *f = malloc(sizeof *f);
  f->to = header[0];
  f->from = header[1];
  f->length = header[2];
  f->header_checksum = header[3];

  // read data
  f->data = malloc(f->length);
  spi_read_blocking (spi0, 0, f->data, f->length);

  if (sem_try_acquire (&receive_sema)) {
    // add it to receive_list;
    struct receive_elem *elem = malloc (sizeof *elem);
    elem->f = f;
    list_push_back (&receive_list, &elem->elem);
    sem_release (&receive_sema);
  } else {
    // TODO
  }
}

//=================================================================================//

void slave_init (void) {
  // enable SPI0 at 10kHz
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

