#include "master.h"
#include "frame.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/stdio.h"
#include "pico/time.h"
#include <stdio.h>

void master_init () {
  // enable SPI0 at 1kHz
  spi_init (spi1, BAUDRATE);

  // assign SPI functions to default SPI pint.
  // for more info read RP2040 Datasheet(2.19.2. Function Select).
  // gpio_set_function (PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function (10, GPIO_FUNC_SPI);
  gpio_set_function (11, GPIO_FUNC_SPI);
  gpio_set_function (13, GPIO_FUNC_SPI); 
}

void master_propagate (struct frame *f) {
  // send headers
  spi_write_blocking (spi1, &f->to, 1);
  spi_write_blocking (spi1, &f->from, 1);
  spi_write_blocking (spi1, &f->length, 1);
  spi_write_blocking (spi1, &f->header_checksum, 1);

  // send data
  spi_write_blocking (spi1, f->data, f->length);
}
