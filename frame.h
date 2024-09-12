#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdint.h>

#define BAUDRATE      10 * 1000
#define CHUNK_SIZE    1

/* frame definitions */
enum frame_type { message, ACK };
struct frame {
  uint8_t id;                 // id of the sent frame.
  enum frame_type type;
  uint8_t to;                 // assigned in class
  uint8_t from;               // assigned in class
  char data[99];              // actual data
};

#endif // !FRAME_H

