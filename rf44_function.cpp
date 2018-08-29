#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "rf44_function.hpp"

int rf44_myoperate( int index,
  const bool wen0, uint8_t waddr0, uint8_t in0, 
  const bool wen1, uint8_t waddr1, uint8_t in1, 
  const bool wen2, uint8_t waddr2, uint8_t in2, 
  const bool wen3, uint8_t waddr3, uint8_t in3, 
  const bool ren0, uint8_t raddr0, uint8_t out0, 
  const bool ren1, uint8_t raddr1, uint8_t out1, 
  const bool ren2, uint8_t raddr2, uint8_t out2, 
  const bool ren3, uint8_t raddr3, uint8_t out3 ){
  /*
   */
  // Testify data effectiveness
  assert( raddr0>= RF44_ADDR_MIN && raddr0<=RF44_ADDR_MAX ); // make sure raddr0 is in the right range.
  assert( raddr1>= RF44_ADDR_MIN && raddr1<=RF44_ADDR_MAX ); // make sure raddr1 is in the right range.
  assert( raddr2>= RF44_ADDR_MIN && raddr2<=RF44_ADDR_MAX ); // make sure raddr2 is in the right range.
  assert( raddr3>= RF44_ADDR_MIN && raddr3<=RF44_ADDR_MAX ); // make sure raddr3 is in the right range.
  assert( waddr0>= RF44_ADDR_MIN && waddr0<=RF44_ADDR_MAX ); // make sure waddr0 is in the right range.
  assert( waddr1>= RF44_ADDR_MIN && waddr1<=RF44_ADDR_MAX ); // make sure waddr1 is in the right range.
  assert( waddr2>= RF44_ADDR_MIN && waddr2<=RF44_ADDR_MAX ); // make sure waddr2 is in the right range.
  assert( waddr3>= RF44_ADDR_MIN && waddr3<=RF44_ADDR_MAX ); // make sure waddr3 is in the right range.

  // Make sure no write confilict
  if( wen0&&wen1 ) assert( not ( waddr0 == waddr1 ));
  if( wen0&&wen2 ) assert( not ( waddr0 == waddr2 ));
  if( wen0&&wen3 ) assert( not ( waddr0 == waddr3 ));
  if( wen1&&wen2 ) assert( not ( waddr1 == waddr2 ));
  if( wen1&&wen3 ) assert( not ( waddr1 == waddr3 ));
  if( wen2&&wen3 ) assert( not ( waddr2 == waddr3 ));

  tb_rf4w4r_in_t  pat;
  tb_rf4w4r_out_t gld;
  memset( &pat, 0, sizeof(pat) );
  memset( &gld, 0, sizeof(gld) );
  
  // Write Part
  if( wen0 ) {
    pat.wen3_0  |= 0x1;
    pat.waddr[0] = waddr0;
    pat.d[0]     = 0x0101010101010101ull * in0;
  }
  if( wen1 ) {
    pat.wen3_0  |= 0x2;
    pat.waddr[1] = waddr1;
    pat.d[1]     = 0x0101010101010101ull * in1;
  }
  if( wen2 ) {
    pat.wen3_0  |= 0x4;
    pat.waddr[2] = waddr2;
    pat.d[2]     = 0x0101010101010101ull * in2;
  }
  if( wen3 ) {
    pat.wen3_0  |= 0x8;
    pat.waddr[3] = waddr3;
    pat.d[3]     = 0x0101010101010101ull * in3;
  }

  // Read Part
  pat.cmp_mask|= 0x00ull;
  if( ren0 ) {
    pat.ren3_0  |= 0x1;
    pat.raddr[0] = raddr0;
    gld.q[0]     = 0x0101010101010101ull * out0;
    pat.cmp_mask|= 0xffull;
  }
  if( ren1 ) {
    pat.ren3_0  |= 0x2;
    pat.raddr[1] = raddr1;
    gld.q[1]     = 0x0101010101010101ull * out1;
    pat.cmp_mask|= 0xffull<<8;
  }
  if( ren2 ) {
    pat.ren3_0  |= 0x4;
    pat.raddr[2] = raddr2;
    gld.q[2]     = 0x0101010101010101ull * out2;
    pat.cmp_mask|= 0xffull<<16;
  }
  if( ren3 ) {
    pat.ren3_0  |= 0x8;
    pat.raddr[3] = raddr3;
    gld.q[3]     = 0x0101010101010101ull * out3;
    pat.cmp_mask|= 0xffull<<24;
  }

#ifndef DEBUG_PROGRAM
  write_pattern( index, pat );
  write_golden(  index, gld );
#endif

  return index+1;
}

int rf44_read( const int index, const uint8_t addr, const uint8_t expected_data ){
  /* index : 0~1024, entry number of 1024 memory
   * output_sel : high 64 bits to output if true, otherwise low 64 bits
   * addr  : read address
   * expected_data : expected output data
   */
  return rf44_myoperate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
      true, addr, expected_data, true, addr, expected_data, true, addr, expected_data, true, addr, expected_data );
}

int rf44_write( int index, int wport, uint8_t addr, const uint8_t data ){
  /*
   * Write data into regfile_4w4r
   * index : 0~1024, entry number of 1024 memory
   */
  if(wport==0) return rf44_myoperate( index, true, addr, data, false, 0, 0, false, 0, 0, false, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  else if(wport==1) return rf44_myoperate( index, false, 0, 0, true, addr, data, false, 0, 0, false, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  else if(wport==2) return rf44_myoperate( index, false, 0, 0, false, 0, 0, true, addr, data, false, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  else if(wport==3) return rf44_myoperate( index, false, 0, 0, false, 0, 0, false, 0, 0, true, addr, data,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  
  printf("**INFO** Specified WPORT not avalible, use W0 to write data in...\n");
  return rf44_myoperate( index, true, addr, data, false, 0, 0, false, 0, 0, false, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
}

int rf44_memset( int index, const uint8_t data, int wport ){
  /*
   * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
   * index : entry number of 1024 memory
   * wport : write port to use (0~7)
   * data  : data to write into, low 8-bit repeated to 64-bit
   */
  int addr;
  for( addr=RF44_ADDR_MIN; addr<=RF44_ADDR_MAX; addr++){
    rf44_write( index, /*wport*/wport, addr, data );
    index++;
  }
  return index;
}

void rf44_print_vec( int index, const tb_rf4w4r_in_t &pat, const tb_rf4w4r_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
  if( pat.wen3_0&(0x1<<0) ){
    printf("     W0     : waddr0=%#04X, input_data0=%#018lX;\n", pat.waddr[0], pat.d[0] );
  } 
  if( pat.wen3_0&(0x1<<1) ){
    printf("     W1     : waddr1=%#04X, input_data1=%#018lX;\n", pat.waddr[1], pat.d[1] );
  } 
  if( pat.wen3_0&(0x1<<2) ){
    printf("     W2     : waddr2=%#04X, input_data2=%#018lX;\n", pat.waddr[2], pat.d[2] );
  } 
  if( pat.wen3_0&(0x1<<3) ){
    printf("     W3     : waddr3=%#04X, input_data3=%#018lX;\n", pat.waddr[3], pat.d[3] );
  } 

  //if( pat.ren3_0 ) {
  //  printf("     Read cmp_mask : %#018lX;\n", pat.cmp_mask );
  //}
  if( pat.ren3_0&(0x1<<0) ){
    printf("     R0     : raddr0=%#04X, expected_data0=%#018lX;\n", pat.raddr[0], gld.q[0] );
  } 
  if( pat.ren3_0&(0x1<<1) ){
    printf("     R1     : raddr1=%#04X, expected_data1=%#018lX;\n", pat.raddr[1], gld.q[1] );
  } 
  if( pat.ren3_0&(0x1<<2) ){
    printf("     R2     : raddr2=%#04X, expected_data2=%#018lX;\n", pat.raddr[2], gld.q[2] );
  } 
  if( pat.ren3_0&(0x1<<3) ){
    printf("     R3     : raddr3=%#04X, expected_data3=%#018lX;\n", pat.raddr[3], gld.q[3] );
  } 

  fflush(stdout);
}

void rf44_print_vec2( int index, const tb_rf4w4r_in_t &pat, const tb_rf4w4r_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
  printf(" wen0=%d, waddr0=%#04X, input_data0=%#018lX;\n", pat.wen3_0&(0x1<<0)?1:0, pat.waddr[0], pat.d[0] );
  printf(" wen1=%d, waddr1=%#04X, input_data1=%#018lX;\n", pat.wen3_0&(0x1<<1)?1:0, pat.waddr[1], pat.d[1] );
  printf(" wen2=%d, waddr2=%#04X, input_data2=%#018lX;\n", pat.wen3_0&(0x1<<2)?1:0, pat.waddr[2], pat.d[2] );
  printf(" wen3=%d, waddr3=%#04X, input_data3=%#018lX;\n", pat.wen3_0&(0x1<<3)?1:0, pat.waddr[3], pat.d[3] );

  printf("  ren0=%d, raddr0=%#04X, expected_data0=%#018lX;\n", pat.ren3_0&(0x1<<0)?1:0, pat.raddr[0], gld.q[0] );
  printf("  ren1=%d, raddr1=%#04X, expected_data1=%#018lX;\n", pat.ren3_0&(0x1<<1)?1:0, pat.raddr[1], gld.q[1] );
  printf("  ren2=%d, raddr2=%#04X, expected_data2=%#018lX;\n", pat.ren3_0&(0x1<<2)?1:0, pat.raddr[2], gld.q[2] );
  printf("  ren3=%d, raddr3=%#04X, expected_data3=%#018lX;\n", pat.ren3_0&(0x1<<3)?1:0, pat.raddr[3], gld.q[3] );

  fflush(stdout);
}

