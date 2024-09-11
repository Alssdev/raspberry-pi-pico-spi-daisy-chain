//=================================================================================//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware/address_mapped.h"
#include "hardware/gpio.h"
#include "hardware/regs/spi.h"
#include "hardware/spi.h"
#include "lib/list.h"
#include "pico/binary_info.h"
#include "pico/sem.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "slave.h"

//=================================================================================//

#define MHz           1000000
#define kHz           1000
#define LED_GPIO      15
#define PACKET_SIZE   sizeof (struct packet)
#define CHUNK_SIZE    1

//=================================================================================//

struct frame *f;
uint8_t fi;           // frame index --> f[fi]

//=================================================================================//

void spiReceiveISR () {
  // create a new frame
  if (fi == 0)
    f = malloc(sizeof *f);

  // only one byte is read at a time
  uint8_t in_buf[1];

  // read byte using SPI
  spi_read_blocking (spi_default, 0, in_buf, CHUNK_SIZE);

  // place this new byte into frame
  ((uint8_t*)f)[fi++] = in_buf[0];

  // is frame completely read?
  if (fi >= sizeof(struct frame)) {
    fi = 0;

    if (sem_try_acquire (&receive_sema)) {
      // add it to receive_list;
      struct receive_elem *elem = malloc (sizeof *elem);
      elem->f = f;
      list_push_back (&receive_list, &elem->elem);

      sem_release (&receive_sema);
    }
  }
}

//=================================================================================//

int main (void) {
  // USB serial, SPI, interruptions, lists...
  init_configs ();

  while (1) {
    struct receive_elem *elem = NULL;

    sem_acquire_blocking (&receive_sema);
    if (!list_empty (&receive_list)) {
      elem = list_entry (list_pop_front (&receive_list), struct receive_elem, elem);
      printf("{ to: %d, from: %d, type: %d, data: \"%s\" }\n", elem->f->to,elem->f->from,elem->f->type,elem->f->data);
    }
    sem_release (&receive_sema);
  
    if (elem != NULL) {
      free (elem->f);
      free (elem);
    }
    sleep_ms (1);
  }
}

//=================================================================================//

void init_configs (void) {
  sem_init (&receive_sema, 1, 1);

  // Enable UART
  stdio_init_all ();
  printf ("CC8 project - slave mode\n");

  // enable SPI0 at 1MHz
  spi_init (spi_default, 10 * kHz);
  spi_set_slave (spi_default, true);

  // enable LED
  gpio_init (LED_GPIO);
  gpio_set_dir (LED_GPIO, GPIO_OUT);

  // assign SPI functions to default SPI pint.
  // for more info read RP2040 Datasheet(2.19.2. Function Select).
  gpio_set_function (PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function (PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);

  // Enable the RX FIFO interrupt (RXIM)
  spi0_hw->imsc = 1 << 2;

  // Enable the SPI interrupt
  irq_set_enabled (SPI0_IRQ, 1);

  // Attach the interrupt handler
  irq_set_exclusive_handler (SPI0_IRQ, spiReceiveISR);

  // init list
  list_init (&receive_list);
}

