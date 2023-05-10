#include "ysyx3.h"

#define load_data(src_start, dst_start, dst_end) \
  load_data((int *)(src_start), (int *)(dst_start), (int *)(dst_end))

__ALIGN __NORET void init2() {
  print("loading yuquan_sbi.dtb\n");
  // [0x30000800:0x30001000] -> [0x80000000:0x80000800]
  load_data(0x30000800, 0x80000000, 0x80000800);
  print("loading yuquan_linux.dtb\n");
  // [0x30001000:0x30001800] -> [0xA0000000:0xA0000800]
  load_data(0x30001000, 0xA0000000, 0xA0000800);
  print("loading fw_jump.bin\n");
  // [0x30001800:0x30017000] -> [0x80200000:0x80215800]
  load_data(0x30001800, 0x80200000, 0x80215800);
  print("loading Image\n");
  // [0x30017000:0x31000000] -> [0x82000000:0x82FE9000]
  load_data(0x30017000, 0x82000000, 0x82FE9000);
#ifdef USE_256_FLASH
  spi_init();
  spi_txrx_1(NULL, 0x06);
  spi_txrx_2(NULL, 0xC5, 0x01);
  spi_txrx_1(NULL, 0x04);
  // [0x31000000:0x32000000] -> [0x82FE9000:0x83FE9000]
  load_data(0x31000000, 0x82FE9000, 0x83FE9000);
#endif
  print("all loaded\n");

  // flush cache
  asm volatile("fence.i");
  // jump to opensbi
  ((void (*)(uint64_t mhartid, uint64_t dtb))0x80200000UL)(0, 0x80000000UL);
}

__ALIGN __NORET void init() {
  // serial init
  asm volatile(
    ".option norvc\n\t"
    "lui	a4,	0x10000\n\t"
    "lbu	a5,	3(a4)\n\t"
    "ori	a5,	a5,	-128\n\t"
    "andi	a5,	a5,	255\n\t"
    "sb	a5,	3(a4)\n\t"
    "li	a5,	13\n\t"
    "sb	a5,	0(a4)\n\t"
    "li	a5,	3\n\t"
    "sb	a5,	3(a4)");
  print("Compiled at " __DATE__ " " __TIME__ "\n");

  // [0x30000000:0x30000800] -> [0x80080000:0x80080800]
  print("loading lowlevelboot.bin\n");
  load_data(0x30000000, 0x80080000, 0x80080800);
  asm volatile("fence.i");
  ((void (*)(void))0x8008000CUL)();
}
