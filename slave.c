#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "pico/binary_info/code.h"
#include "pico/time.h"
#include <stdint.h>
#include "slave.h"

#define BUF_LEN         20

int main() {
  init_spi_slave();

  char buf_out[BUF_LEN] = "12345";
  char buf_in[BUF_LEN];

  while (1) {
    // send and read
    spi_write_read_blocking(spi_default, buf_out, buf_in, BUF_LEN);
  }
}

void init_spi_slave(void) {
#if !defined(spi_default) || !defined(PICO_DEFAULT_SPI_SCK_PIN) || !defined(PICO_DEFAULT_SPI_TX_PIN) || !defined(PICO_DEFAULT_SPI_RX_PIN) || !defined(PICO_DEFAULT_SPI_CSN_PIN)
  #warning spi/spi_slave example requires a board with SPI pins
  puts("Default SPI pins were not defined");
#else
  // Enable SPI 0 at 1 MHz and connect to GPIOs
  spi_init(spi_default, 1000 * 1000);
  spi_set_slave(spi_default, true);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI);
  // Make the SPI pins available to picotool
  bi_decl(bi_4pins_with_func(PICO_DEFAULT_SPI_RX_PIN, PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));
#endif
}

