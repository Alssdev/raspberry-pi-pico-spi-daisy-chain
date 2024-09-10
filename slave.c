//=================================================================================//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/regs/spi.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "slave.h"

//=================================================================================//

#define MHz           1000000
#define kHz           1000
#define LED_GPIO      15
#define PACKET_SIZE   sizeof (struct packet)
#define CHUNK_SIZE    1

uint8_t in_buf[2];
uint8_t off = 0;
//=================================================================================//

void spiReceiveISR () {
  // gpio_put (LED_GPIO, 1);

  // read packet
  spi_read_blocking (spi_default, 0, in_buf + off++, CHUNK_SIZE);
  if (off > 1) {
    printf("%c", (char) in_buf[1]);
    off = 0;
  }
  // gpio_put (LED_GPIO, 0);
}

//=================================================================================//

int main (void) {
  // Enable UART
  stdio_init_all ();
  sleep_ms (2 * 1000);
  printf ("CC8 project - slave mode\n");

  // enable SPI0 at 1MHz
  spi_init (spi_default, 1 * kHz);
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

  while (1) {
    tight_loop_contents();
  }
}

