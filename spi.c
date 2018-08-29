#include<stdio.h>
#include<stdint.h>
#include "spi.h"


extern unsigned char*SPI_BASE;

volatile unsigned char*SPI_SPCR= NULL;
volatile unsigned char*SPI_SPSR= NULL;
volatile unsigned char*SPI_FIFO= NULL;
volatile unsigned char*SPI_SPER= NULL;

#define SPI_TXEMPTY (*SPI_SPSR & 0x04)
#define SPI_RXEMPTY (*SPI_SPSR & 0x01)

void spi_init()
{
#ifndef NO_HARDWARE
  SPI_SPCR = SPI_BASE + 0;
  SPI_SPSR = SPI_BASE + 1;
  SPI_FIFO = SPI_BASE + 2;
  SPI_SPER = SPI_BASE + 3;
  *SPI_SPCR = 0x0;
  *SPI_SPCR = 0x5d; // {int_en, spi, rsvd, mstr, cpol, cpha, spr[1:0]}
  *SPI_SPER = 0x1;
/*
 * 8Mhz : *SPI_SPCR = 0x5c; *SPI_SPER=0x01;
 * 4Mhz : *SPI_SPCR = 0x5c; *SPI_SPER=0x01;
 * 2Mhz : *SPI_SPCR = 0x5e; *SPI_SPER=0x0;
 * 1Mhz : *SPI_SPCR = 0x5f; *SPI_SPER=0x0;
 * 500KHz : *SPI_SPCR = 0x5d; *SPI_SPER=0x1;
 * 250KHz : *SPI_SPCR = 0x5d; *SPI_SPER=0x2;
 * 125KHz : *SPI_SPCR = 0x5d; *SPI_SPER=0x3;
 * 64KHz  : *SPI_SPCR = 0x5e/0x5f; *SPI_SPER=0x0;
 * 32KHz  : *SPI_SPCR = 0x5e/0x5f; *SPI_SPER=0x1;
 * 16KHz  : *SPI_SPCR = 0x5e/0x5f; *SPI_SPER=0x2;
 * 8KHz   : *SPI_SPCR = 0x5e/0x5f; *SPI_SPER=0x3;
 */
  while(!SPI_RXEMPTY) *SPI_FIFO; // read till empty
#endif
}

// 4byte transfer
void spi_write_read_4(uint8_t* buf)
{
#ifndef NO_HARDWARE
  int i;
  for (i=0; i<4; i++) *SPI_FIFO = buf[i];
  while(!SPI_TXEMPTY) ; // loop
  for (i=0; i<3; i++) buf[i] = *SPI_FIFO; // read first 3bytes
  while(SPI_RXEMPTY) ;
  buf[3] = *SPI_FIFO;
#endif
}
// 1byte transfer
void spi_write_read_1(uint8_t* buf)
{
#ifndef NO_HARDWARE
  int i;
  *SPI_FIFO = buf[0];
  while(SPI_RXEMPTY) ;
  buf[0] = *SPI_FIFO;
#endif
}


void spi_cs_down()
{
#ifndef NO_HARDWARE
  *(SPI_BASE+5) = (0x0<<5) | (0x1<<1);
#endif
}

void spi_cs_up()
{
#ifndef NO_HARDWARE
  *(SPI_BASE+5) = (0x1<<5) | (0x1<<1);
#endif
}


uint32_t axi_read4(uint32_t addr)
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[5];
  spi_cs_down();
  cmd_buf[0] = 0xa4;        // Mread32 with 1 dummy byte
  cmd_buf[1] = addr & 0xff;
  cmd_buf[2] =(addr >> 8) & 0xff;
  cmd_buf[3] =(addr >>16) & 0xff;
  spi_write_read_4(cmd_buf);
  spi_write_read_1(cmd_buf);
  spi_write_read_4(cmd_buf); // reuse cmd_buf
  spi_cs_up();
  return *((uint32_t*)cmd_buf);
#endif
}

void axi_read4n(uint32_t addr, uint32_t buf[], int n)
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[5];
  int i;
  spi_cs_down();
  cmd_buf[0] = 0xa4;        // Mread32 with 1 dummy byte
  cmd_buf[1] = addr & 0xff;
  cmd_buf[2] =(addr >> 8) & 0xff;
  cmd_buf[3] =(addr >>16) & 0xff;
  spi_write_read_4(cmd_buf);
  spi_write_read_1(cmd_buf);
  for (i=0; i<n; i++) {
    spi_write_read_4(cmd_buf); // reuse cmd_buf
    buf[i] = *((uint32_t*)cmd_buf);
  }
  spi_cs_up();
#endif
  return ;
}

void axi_write4(uint32_t addr, uint32_t data)
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[5];
  spi_cs_down();
  cmd_buf[0] = 0xac;        // Mwrite32
  cmd_buf[1] = addr & 0xff;
  cmd_buf[2] =(addr >> 8) & 0xff;
  cmd_buf[3] =(addr >>16) & 0xff;
  spi_write_read_4(cmd_buf);
  spi_write_read_4((uint8_t*)&data);    // little endian
  spi_cs_up();
#endif
}

void axi_write4n(uint32_t addr, uint32_t data[], int n)
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[5];
  int i;
  spi_cs_down();
  cmd_buf[0] = 0xac;        // Mwrite32
  cmd_buf[1] = addr & 0xff;
  cmd_buf[2] =(addr >> 8) & 0xff;
  cmd_buf[3] =(addr >>16) & 0xff;
  spi_write_read_4(cmd_buf);
  for (i=0; i<n; i++) {
    spi_write_read_4((uint8_t*)(data+i));// little endian
  }
  spi_cs_up();
#endif
}


uint32_t cfg_read()
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[4];
  int i;
  spi_cs_down();
  cmd_buf[0] = 0xb0;        // Cfg_read
  spi_write_read_4(cmd_buf);
  spi_cs_up();
  return (*((uint32_t*)cmd_buf)) >> 8; // cmd_buf shall be dword aligned
#endif
}

void cfg_write(uint32_t data)
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[4];
  spi_cs_down();
  cmd_buf[0] = 0xb8;        // Cfg_write
  cmd_buf[1] = data & 0xff;
  cmd_buf[2] =(data >> 8) & 0xff;
  cmd_buf[3] =(data >>16) & 0xff;
  spi_write_read_4(cmd_buf);
  spi_cs_up();
#endif
}

uint32_t status_read()
{
#ifndef NO_HARDWARE
  uint8_t cmd_buf[4];
  int i;
  spi_cs_down();
  cmd_buf[0] = 0xb2;        // Status_read
  spi_write_read_4(cmd_buf);
  spi_cs_up();
  return (*((uint32_t*)cmd_buf)) >> 8; // cmd_buf shall be dword aligned
#endif
  return 0x01;
}

