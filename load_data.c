#include "ysyx3.h"

__attribute__((aligned(4))) void load_data(volatile int *src_start, int *dst_start, int *dst_end) {
  asm volatile(".option norvc");
  while (dst_start < dst_end) {
    // char buffer[16];
    // print("current dst: 0x");
    // htoa((uint64_t)dst_start, buffer);
    // print(buffer);
    // print(", current src: 0x");
    // htoa((uint64_t)src_start, buffer);
    // print(buffer);
    // print("\n");
    *dst_start++ = *src_start++;
  }
  // print("finished!\n");
}
