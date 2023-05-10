#ifndef __YSYX3_H__
#define __YSYX3_H__

#include <stddef.h>

#define __ALIGN __attribute__((aligned(4)))
#define __NORET __attribute__((naked)) __attribute__((noreturn))

typedef unsigned long  uint64_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

#define SPI_CTL_SS         0x01000000
#define SPI_CTL_DIV_4      0x00010000  // divide by 4
#define SPI_CTL_CPOL       0x00008000
#define SPI_CTL_RD_ENDIAN  0x00004000
#define SPI_CTL_ASS        0x00002000
#define SPI_CTL_IE         0x00001000
#define SPI_CTL_LSB        0x00000800
#define SPI_CTL_TX_NEGEDGE 0x00000400
#define SPI_CTL_RX_NEGEDGE 0x00000200
#define SPI_CTL_GO         0x00000100
#define SPI_CTL_RES_1      0x00000080
#define SPI_CTL_CHAR_LEN   0x00000040 // 0x40 = 64bit

#define SPI_REG_D0   0x00
#define SPI_REG_CTL  0x10
#define SPI_REG_DIV  0x14
#define SPI_REG_CS   0x18

#define FLASH_BASE 0x30000000UL
#define UART_BASE  0x10000000UL
#define SPI_BASE   0x10001000UL

#define SPI_CHAR_LEN_MASK 0x7FU
#define SPI_GO_BUSY_MASK  0x100U

#define SPI_CTL_SS_MASK    0xff000000
#define SPI_CTL_DIV_MASK   0x00ff0000
#define SPI_CTL_LEN_MASK   0x0000007f

void send_char(char c);
void print(char *str);
void load_data(volatile int *src_start, int *dst_start, int *dst_end);
// void htoa(uint64_t i, char *string);
void spi_init(void);
void spi_txrx(int len, unsigned char *rx_buf, const unsigned char *tx_buf);

#define spi_txrx_1(rx_buf, tx0)        \
  do                                   \
  {                                    \
    unsigned char tx_buf[1] = {(tx0)}; \
    spi_txrx(1, (rx_buf), tx_buf);     \
  } while (0)

#define spi_txrx_2(rx_buf, tx0, tx1)          \
  do                                          \
  {                                           \
    unsigned char tx_buf[2] = {(tx0), (tx1)}; \
    spi_txrx(2, (rx_buf), tx_buf);            \
  } while (0)

#endif // __YSYX3_H__
