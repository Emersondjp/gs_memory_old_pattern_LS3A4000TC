#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "types.h"
#include "global.h"
#include "spi.h"
#include "cp25_function.hpp"

//extern void axi_write4n(uint32_t addr, uint32_t data[], int n);
//extern void axi_read4n(uint32_t addr, uint32_t buf[], int n);

int cp25_memset( int start, int wport, uint64_t data ){
  /*
 * Write specified data to memory entry, using specified wport
 * start : start index of 1024 memory entry
 * wport : 0/1 writing port number
 * data  : data to write into, low 8-bit repeated to 64-bit
 */
  assert( wport >= CP25_WPORT_MIN && wport <= CP25_WPORT_MAX ); // Check Writing Port
  tb_cp0q_ram_in_t  pat;
  memset(&pat, 0, sizeof(pat));   // Clear pat before write

  // Set link point according to wport
  uint64_t * p_waddr;
  uint64_t * p_wvalue;
  if (wport == 0) {
    p_waddr  = &pat.waddr0;
    p_wvalue = &pat.wvalue0;
  } else if ( wport == 1 ) {
    p_waddr  = &pat.waddr1;
    p_wvalue = &pat.wvalue1;
  }

  for (int i=CP25_ADDR_MIN; i<=CP25_ADDR_MAX; i++) {
    memset(&pat, 0, sizeof(pat));   // Clear pat before write
    pat.cmp_mask   = 0;             // Disable compare
    *p_waddr = 0x1 << i; // Set Addr according to i
    *p_wvalue = 0x0101010101010101ull*(data&0xFF) ;
    pat.wen1_0     = 0x01<<wport;
#ifndef DEBUG_PROGRAM
    //assert( start+i-CP25_ADDR_MIN < 1024 ); // Make sure pat didn't exceed max pat num.
    write_pattern( start+i-CP25_ADDR_MIN, pat );
#endif
  }
  return start+CP25_ADDR_MAX-CP25_ADDR_MIN+1;
}

int cp25_write( int start, int wport, const unsigned int addr, const unsigned int data ){
  /*
 * Write specified data to memory entry, using specified wport
 * start : start index of 1024 memory entry
 * wport : 0/1 writing port number
 * addr  : address of writing entry, should transform to onehot format
 * data  : data to write, low 8-bit repeated to 64-bit
 */
  assert( wport >= CP25_WPORT_MIN && wport <= CP25_WPORT_MAX ); // Check Writing Port
  assert( addr  >= CP25_ADDR_MIN  && addr  <= CP25_ADDR_MAX );  // Check Writing Address

  tb_cp0q_ram_in_t  pat;
  memset(&pat, 0, sizeof(pat));   // Clear pat before write

  pat.cmp_mask   = 0;             // Disable compare

  // Set link point according to wport
  uint64_t * p_waddr;
  uint64_t * p_wvalue;
  if (wport == 0) {
    p_waddr  = &pat.waddr0;
    p_wvalue = &pat.wvalue0;
  } else if ( wport == 1 ) {
    p_waddr  = &pat.waddr1;
    p_wvalue = &pat.wvalue1;
  }

  *p_waddr = 0x1ull << addr; // Set WADDR according to addr
  *p_wvalue = 0x0101010101010101ull*(data&0xffull) ;
  pat.wen1_0     = 0x01<<wport;

#ifndef DEBUG_PROGRAM
  //assert( start+i-CP25_ADDR_MIN < 1024 ); // Make sure pat didn't exceed max pat num.
  write_pattern( start, pat );
#endif
  return start+1;
}

int cp25_read( int start, const unsigned int addr, const unsigned int expected_data ){
  /*
 * Read specified entry of memory, using all read-port
 * start : start index of 1024 memory entry
 * addr  : address of read entry, should transform to onehot format
 * expexted_data  : data expexted, low 8-bit repeated to 64-bit
 */
  assert( addr >= CP25_ADDR_MIN  && addr <= CP25_ADDR_MAX );  // Check Reading Address

  tb_cp0q_ram_in_t  pat;
  tb_cp0q_ram_out_t gld;

  memset(&pat, 0, sizeof(pat));   // Clear pat before using
  memset(&gld, 0, sizeof(gld));   // Clear gld before using

  pat.cmp_mask = 0x000000ffffffffffull;  // Compare all byte
  pat.ren4_0   = 0x1f;

  pat.raddr0 = 0x1ull << addr;
  pat.raddr1 = 0x1ull << addr;
  pat.raddr2 = 0x1ull << addr;
  pat.raddr3 = 0x1ull << addr;
  pat.raddr4 = 0x1ull << addr;

  gld.q0 = 0x0101010101010101ull*(expected_data&0xffull);
  gld.q1 = 0x0101010101010101ull*(expected_data&0xffull);
  gld.q2 = 0x0101010101010101ull*(expected_data&0xffull);
  gld.q3 = 0x0101010101010101ull*(expected_data&0xffull);
  gld.q4 = 0x0101010101010101ull*(expected_data&0xffull);

#ifndef DEBUG_PROGRAM
  //assert( start+i-CP25_ADDR_MIN < 1024 ); // Make sure pat didn't exceed max pat num.
  write_pattern( start, pat );
  write_golden( start, gld);
#endif
  return start+1;
}

int cp25_operate( int start, bool we0, const unsigned int wad0, const unsigned int wdata0,
                             bool we1, const unsigned int wad1, const unsigned int wdata1,
                             bool re0, const unsigned int rad0, const unsigned int rdata0,
                             bool re1, const unsigned int rad1, const unsigned int rdata1,
                             bool re2, const unsigned int rad2, const unsigned int rdata2,
                             bool re3, const unsigned int rad3, const unsigned int rdata3,
                             bool re4, const unsigned int rad4, const unsigned int rdata4 ){
  /*
 * All kinds of operating
 * start : start index of 1024 memory entry
 * we0    / we1    : enable signal of corespond wport
 * wad0   / wad1   : write address of each wport, should transform to onehot format
 * waddr0 / waddr1 : address of writing entry, should transform to onehot format
 * wdata0 / wdata1 : data to write, low 8-bit repeated to 64-bit
 * re0 / re1 / re2 / re3 / re4 : enable signal of corespond rport
 * raddr0 / raddr1 / raddr2 / raddr3 / raddr4 : address of read operation
 * rdata0 / rdata1 / rdata2 / rdata3 / rdata4 : expected output data of read operation
 */
  tb_cp0q_ram_in_t  pat;
  tb_cp0q_ram_out_t gld;
  memset(&pat, 0, sizeof(pat));   // Clear pat before using
  memset(&gld, 0, sizeof(gld));   // Clear gld before using
  
  pat.wen1_0 = (we1?0x2:0x0) + (we0?0x1:0x0);
  pat.ren4_0 = (re4?0x10:0x0) + (re3?0x8:0x0) + (re2?0x4:0x0) + (re1?0x2:0x0) + (re0?0x1:0x0);
  pat.waddr0 = 0x1 << wad0;
  pat.waddr1 = 0x1 << wad1;
  if(re0) {
    pat.cmp_mask |= 0x00000000000000ffull;
    pat.raddr0 = 0x1 << rad0;
    gld.q0     = 0x0101010101010101ull*(rdata0&0xffull);
  }
  if(re1) {
    pat.cmp_mask |= 0x000000000000ff00ull;
    pat.raddr1 = 0x1 << rad1;
    gld.q1     = 0x0101010101010101ull*(rdata1&0xffull);
  }
  if(re2) {
    pat.cmp_mask |= 0x0000000000ff0000ull;
    pat.raddr2 = 0x1 << rad2;
    gld.q2     = 0x0101010101010101ull*(rdata2&0xffull);
  }
  if(re3) {
    pat.cmp_mask |= 0x00000000ff000000ull;
    pat.raddr3 = 0x1 << rad3;
    gld.q3     = 0x0101010101010101ull*(rdata3&0xffull);
  }
  if(re4) {
    pat.cmp_mask |= 0x000000ff00000000ull;
    pat.raddr4 = 0x1 << rad4;
    gld.q4     = 0x0101010101010101ull*(rdata4&0xffull);
  }
#ifndef DEBUG_PROGRAM
  //assert( start+i-CP25_ADDR_MIN < 1024 ); // Make sure pat didn't exceed max pat num.
  write_pattern( start, pat );
  write_golden( start, gld);
#endif
  return start+1;
}

void cp25_print_vec( int index, const tb_cp0q_ram_in_t &pat ){
  /*
 * Print vector information.
 */
  printf("Index %04d : \n", index);
  printf("    wen0 -> %d, waddr0 -> %#018X, wvalue0 -> %#018lX;\n", pat.wen1_0&0x1?1:0, pat.waddr0, pat.wvalue0);
  printf("    wen1 -> %d, waddr1 -> %#018X, wvalue1 -> %#018lX;\n", pat.wen1_0&0x2?1:0, pat.waddr1, pat.wvalue1);

  if (pat.cmp_mask == 0x0) return;
  else printf("    compare mask is %#018X, while no golden data given.\n", pat.cmp_mask );
}

void cp25_print_vec( int index, const tb_cp0q_ram_in_t &pat, const tb_cp0q_ram_out_t &gld ){
  /*
 * Print vector information.
 */
  printf("Index %04d : \n", index);
  printf("    wen0 -> %d, waddr0 -> %#04X, wvalue0 -> %#018lX;\n", pat.wen1_0&0x1, pat.waddr0, pat.wvalue0);
  printf("    wen1 -> %d, waddr1 -> %#04X, wvalue1 -> %#018lX;\n", pat.wen1_0&0x2, pat.waddr1, pat.wvalue1);

  if (pat.cmp_mask == 0x0) return;
  assert( &gld!=NULL );
  printf("    ren0 -> %d, raddr0 -> %#04X, expt_d0 -> %#018lX;\n", bool(pat.ren4_0&0x1),  pat.raddr0, gld.q0);
  printf("    ren1 -> %d, raddr1 -> %#04X, expt_d1 -> %#018lX;\n", bool(pat.ren4_0&0x2),  pat.raddr1, gld.q1);
  printf("    ren2 -> %d, raddr2 -> %#04X, expt_d2 -> %#018lX;\n", bool(pat.ren4_0&0x4),  pat.raddr2, gld.q2);
  printf("    ren3 -> %d, raddr3 -> %#04X, expt_d3 -> %#018lX;\n", bool(pat.ren4_0&0x8),  pat.raddr3, gld.q3);
  printf("    ren4 -> %d, raddr4 -> %#04X, expt_d4 -> %#018lX;\n", bool(pat.ren4_0&0x10), pat.raddr4, gld.q4);
}

void cp25_test_spi(){
  /*
 * Test for pattern buffer and golden buffer.
 * Mention : Test result infer :
 *   Write Golden buffer before Pattern buffer.
 */
  printf("\n************ Test for buffer's correctness. ************\n");

  tb_cp0q_ram_in_t  pat;
  tb_cp0q_ram_out_t gld;

  tb_cp0q_ram_in_t  pat_out;
  tb_cp0q_ram_out_t gld_out;

  memset(&pat, 0, sizeof(pat));   // Clear pat before using
  memset(&gld, 0, sizeof(gld));   // Clear gld before using
  memset(&pat, 0, sizeof(pat_out));   // Clear pat_out before using
  memset(&gld, 0, sizeof(gld_out));   // Clear gld_out before using

  #define TEST_GLDBUF
  #ifdef TEST_GLDBUF
  printf("\n  ** Test for golden buffer: \n");
  for (int i=5; i<10; i++) {
    memset(&gld, 0, sizeof(gld));   // Clear gld before using
    gld.q0 = 0x0101010101010101ull*0x11;
    gld.q1 = 0x0101010101010101ull*0x22;
    gld.q2 = 0x0101010101010101ull*0x33;
    gld.q3 = 0x0101010101010101ull*0x44;
    gld.q4 = 0x0101010101010101ull*0x55;
    write_golden( i, gld);
    printf("Write Golden Buffer : %d\n",i);
  }
  for (int i=5; i<10; i++) {
    read_golden(i, gld_out);
    printf("Gldbuf[%d]: ", i);
    dump_buffer(gld_out);
  }
  printf("  ** End test for golden buffer!\n\n");
  #endif

  #define TEST_PATBUF
  #ifdef TEST_PATBUF
  printf("  ** Test for pattern buffer: \n");

  for (int i=0; i<5; i++) {
    memset(&pat, 0, sizeof(pat));   // Clear pat before using
    pat.cmp_mask = 0x000000ffffffffffull;  // Compare all byte
    pat.ren4_0   = 0x1f;
    pat.raddr0 = 0x01ull << i ;
    pat.raddr1 = 0x01ull << i*2 ;
    pat.raddr2 = 0x01ull << i*3 ;
    pat.raddr3 = 0x01ull << i*4 ;
    pat.raddr4 = 0x01ull << i*5 ;
    write_pattern( i, pat);
    printf("Write Pattern Buffer : %d\n",i);
  }
  for (int i=0; i<5; i++) {
    read_pattern(i, pat_out);
    printf("Patbuf[%d]: ", i);
    dump_buffer(pat_out);
  }
  printf("  ** End test for pattern buffer!\n\n");
  #endif

  printf("****************************\n");
}
