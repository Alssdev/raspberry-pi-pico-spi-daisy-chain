#include "debug.h"

// TODO: Improve. For exmample, turning on a red led.
void ASSERT (bool expression) {
  while (!expression) {};
}
