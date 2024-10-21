#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdint.h>

#define BAUDRATE      10 * 1000
#define CHUNK_SIZE    1
#define MSS           99

/* frame definitions */
struct frame {
  uint8_t to;                 // assigned in class
  uint8_t from;               // assigned in class
  uint8_t length;             // data's length
  uint8_t header_checksum;    // to + from + length
  uint8_t *data;              // actual data
};

#endif // !FRAME_H

