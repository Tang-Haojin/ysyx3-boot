#include "ysyx3.h"

asm(".option norvc");

/*
__attribute__((aligned(4))) void htoa(uint64_t i, char *string) {
  uint64_t power = 0, j = 0;
  j = i;
  for (power = 1; j >= 16; j /= 16)
    power *= 16;
  for (; power > 0; power /= 16) {
    int digit = i / power;
    *string++ = (digit < 10 ? '0' : 'A' - 10) + digit;
    i %= power;
  }
  *string = '\0';
}
*/

__attribute__((aligned(4))) void send_char(char c) {
  while ((*(volatile char *)(UART_BASE + 5) & 0b100000) == 0);
  *(char *)UART_BASE = c;
}

__attribute__((aligned(4))) void print(char *str) {
  while (*str) {
    send_char(*str++);
  }
}
