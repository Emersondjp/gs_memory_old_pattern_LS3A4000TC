#ifndef _SPI_H
#define _SPI_H
#include<stdio.h>
#include<stdint.h>
#include "global.h"


#define SPI_TXEMPTY (*SPI_SPSR & 0x04)
#define SPI_RXEMPTY (*SPI_SPSR & 0x01)

void spi_init();
// 4byte transfer
void spi_write_read_4(uint8_t* buf);
// 1byte transfer
void spi_write_read_1(uint8_t* buf);
void spi_cs_down();
void spi_cs_up();
uint32_t axi_read4(uint32_t addr);
void axi_read4n(uint32_t addr, uint32_t buf[], int n);
void axi_write4(uint32_t addr, uint32_t data);
void axi_write4n(uint32_t addr, uint32_t data[], int n);

uint32_t cfg_read();
void cfg_write(uint32_t data);
uint32_t status_read();

template <class T> void read_pattern(int base, T& pat)
{
  axi_read4n(TB_PAT(base), (uint32_t*)&pat, size4(T));
}
template <class T> void write_pattern(int base, T& pat)
{
  axi_write4n(TB_PAT(base), (uint32_t*)&pat, size4(T));
}
template <class T> void read_golden(int base, T& gld)
{
  axi_read4n(TB_GLD(base), (uint32_t*)&gld, size4(T));
}
template <class T> void write_golden(int base, T& gld)
{
  axi_write4n(TB_GLD(base), (uint32_t*)&gld, size4(T));
}
template <class T> void read_outbuf(int base, T& buf)
{
  axi_read4n(TB_OUT(base), (uint32_t*)&buf, size4(T));
}
template <class T> void dump_buffer(T& buf)
{
  uint8_t *ptr = (uint8_t*)&buf;
  for (int i=sizeof(T)-1; i>=0; i--) {
    printf("%02x", ptr[i]);
    if ((i&0x3)==0) printf(" ");
  }
  printf("\n");
}

#endif
