#include "ysyx3.h"

static inline void writel_cpu(uint32_t val, volatile void *addr) {
  asm volatile("sw %0, 0(%1)" : : "r" (val), "r" (addr));
}

static inline uint32_t readl_cpu(const volatile void *addr) {
  uint32_t val;
  asm volatile("lw %0, 0(%1)" : "=r" (val) : "r" (addr));
  return val;
}

#define __io_br()
#define __io_ar(v) asm volatile ("fence i,r" : : : "memory")
#define __io_bw()  asm volatile ("fence w,o" : : : "memory")
#define __io_aw()

#define readl(c)	({ uint32_t __v; __io_br(); __v = readl_cpu(c); __io_ar(__v); __v; })
#define writel(v, c)	({ __io_bw(); writel_cpu((v), (c)); __io_aw(); })

// spi

#define base ((void *)SPI_BASE)

static struct {
  int count;
  unsigned int ctrl;
  const unsigned char *tx;
  unsigned char *rx;
} hw;

static inline uint32_t spi_div(int divisor) {
  uint32_t bits = (divisor / 2 - 1) << 16;
  if (bits > SPI_CTL_DIV_MASK)
    bits = SPI_CTL_DIV_4;
  return bits;
}

static inline uint32_t byte_swap(uint32_t val) {
  union {
    uint32_t val;
    uint8_t byte[4];
  } u;
  u.val = val;
  return (u.byte[0] << 24) | (u.byte[1] << 16) | (u.byte[2] << 8) | u.byte[3];
}

static inline uint32_t spi_read(uint32_t reg) {
  return readl(base + reg);
}

static inline void spi_wait() {
  while (spi_read(SPI_REG_CTL) & SPI_CTL_GO);
}

static inline void spi_write(uint32_t reg, uint32_t val) {
  writel(val, base + reg);
}

static inline void nutshell_spi_set_cs(char is_high) {
  uint32_t mask = (1u << 0) << 24;
  is_high ? (hw.ctrl &= ~mask) : (hw.ctrl |= mask);
  spi_write(SPI_REG_CTL, hw.ctrl);
}

static inline void nutshell_spi_tx_word() {
  spi_write(SPI_REG_D0, hw.tx ? hw.tx[hw.count] : 0);

  uint32_t ctrl = hw.ctrl | 8;
  spi_write(SPI_REG_CTL, ctrl);
  ctrl |= SPI_CTL_GO;
  spi_write(SPI_REG_CTL, ctrl);
}

static inline void nutshell_spi_rx_word() {
  if (hw.rx)
    hw.rx[hw.count] = spi_read(SPI_REG_D0);
}

__ALIGN void spi_txrx(int len, unsigned char *rx_buf, const unsigned char *tx_buf) {
  hw.tx = tx_buf;
  hw.rx = rx_buf;
  hw.count = 0;

  nutshell_spi_set_cs(0);
  while (hw.count < len) {
    nutshell_spi_tx_word();
    spi_wait();
    nutshell_spi_rx_word();
    hw.count++;
  }
  nutshell_spi_set_cs(1);
}

__ALIGN void spi_init() {
  /* program defaults into the registers */
  hw.ctrl = byte_swap(spi_read(SPI_REG_CTL));
  hw.ctrl = SPI_CTL_TX_NEGEDGE | spi_div(4);
  spi_write(SPI_REG_CTL, hw.ctrl);
  // char buf[10] = {};
  // htoa(spi_read(SPI_REG_CTL), buf);
  // print("spi.ctrl: 0x");
  // print(buf);
  // print("\n");
}
