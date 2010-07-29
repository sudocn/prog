#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t lowest_bit(uint8_t byte)
{
  uint8_t pos;
  for (pos=0; pos<8; pos++) {
    if (byte & 1)
      break;
    byte >>= 1;
  }
  return pos==8?0:pos;
}

int main(void)
{
  uint16_t i;
  for (i=0; i<256; i++) {
    if (i%16 == 0)
      printf("\n");
    printf("%d,", lowest_bit((uint8_t)i));
  }
  return 0;
}
