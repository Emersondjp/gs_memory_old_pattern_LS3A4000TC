#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "cp35_function.hpp"

int cp35_operate( int index, uint8_t osel,
   uint8_t waddr0, uint64_t wvalue0, uint32_t wmask0, // Write Port-0
   uint8_t waddr1, uint64_t wvalue1, uint32_t wmask1, // Write Port-1
   uint64_t wen2,                                     // Write Port-2
   uint8_t raddr0, uint8_t raddr1, uint8_t raddr2, uint8_t raddr3, uint8_t raddr4, uint8_t ren,  // Read
   uint8_t ren2_bypass, uint8_t ren3_bypass,                                    // bypass enable obsolete
   uint8_t tbypass0, uint8_t tbypass1, uint8_t tin0, uint8_t tin1, uint8_t tin2,// test bypass
   uint64_t q0, uint64_t q1, uint64_t q2, uint64_t q3, uint64_t q4){  // Output
  /*
   */
  // Testify data effectiveness
  assert( ren>= 0x0              && ren<=0x1f             ); // make sure ren is in the right range.
  assert( raddr0>= CP35_ADDR_MIN && raddr0<=CP35_ADDR_MAX ); // make sure raddr0 is in the right range.
  assert( raddr1>= CP35_ADDR_MIN && raddr1<=CP35_ADDR_MAX ); // make sure raddr1 is in the right range.
  assert( raddr2>= CP35_ADDR_MIN && raddr2<=CP35_ADDR_MAX ); // make sure raddr2 is in the right range.
  assert( raddr3>= CP35_ADDR_MIN && raddr3<=CP35_ADDR_MAX ); // make sure raddr3 is in the right range.
  assert( raddr4>= CP35_ADDR_MIN && raddr4<=CP35_ADDR_MAX ); // make sure raddr4 is in the right range.
  if(not (waddr0&0x40)) assert( wen2==0x00ull ) ;     // assert read and search not happend at the same time.
  if(not (waddr1&0x40)) {
    assert( not ( (0x1ull<<waddr1)&wen2 ) ); // assert read and search not happend at the same time.
    assert( not ( waddr0 == waddr1 ) );       // W0 W1 not write same entry
  }

  tb_cp0q_ram_old_in_t  pat;
  tb_cp0q_ram_old_out_t gld;
  memset( &pat, 0, sizeof(pat) );
  memset( &gld, 0, sizeof(gld) );
  
  pat.waddr0     = waddr0;
  pat.wvalue0[0] = wvalue0;
  pat.wvalue0[1] = wvalue0;
  pat.wmask0     = wmask0;

  pat.waddr1     = waddr1;
  pat.wvalue1[0] = wvalue1;
  pat.wvalue1[1] = wvalue1;
  pat.wmask1     = wmask1;

  pat.wen2 = wen2;

  pat.raddr0 = raddr0;
  pat.raddr1 = raddr1;
  pat.raddr2 = raddr2;
  pat.raddr3 = raddr3;
  pat.raddr4 = raddr4;

  pat.ren    = ren;
  pat.ren2_bypass = ren2_bypass;
  pat.ren3_bypass = ren3_bypass;
  pat.osel   = osel;
  pat.tbypass0 = tbypass0;
  pat.tbypass1 = tbypass1;
  pat.tin0   = tin0;
  pat.tin1   = tin1;
  pat.tin2   = tin2;

  // Set compare mask
  pat.cmp_mask  = 0x0ull;
  if(ren&0x01) { pat.cmp_mask |= 0x00000000ffull;   gld.q0 = q0; }
  if(ren&0x02) { pat.cmp_mask |= 0x000000ff00ull;   gld.q1 = q1; }
  if(ren&0x04) { pat.cmp_mask |= 0x0000ff0000ull;   gld.q2 = q2; }
  if(ren&0x08) { pat.cmp_mask |= 0x00ff000000ull;   gld.q3 = q3; }
  if(ren&0x10) { pat.cmp_mask |= 0xff00000000ull;   gld.q4 = q4; }
 
  write_pattern( index, pat );
  write_golden(  index, gld );

  return index+1;
}

int cp35_myoperate( int index, const bool output_sel,
                    bool wen0, uint8_t waddr0, const uint8_t wvalue0, const uint32_t wmask0,
                    bool wen1, uint8_t waddr1, const uint8_t wvalue1, const uint32_t wmask1,
                    const uint64_t wen2,
                    const bool ren0, uint8_t raddr0, const uint64_t q0,
                    const bool ren1, uint8_t raddr1, const uint64_t q1,
                    const bool ren2, uint8_t raddr2, const uint64_t q2,
                    const bool ren3, uint8_t raddr3, const uint64_t q3,
                    const bool ren4, uint8_t raddr4, const uint64_t q4 ){

  wen0 ? waddr0&=0x3f : waddr0 = 0x40;
  wen1 ? waddr1&=0x3f : waddr1 = 0x40;
  uint8_t ren=0x0;
  ren0 ? (ren|= 0x1)&&(raddr0&=0x3f) : (raddr0=0x00); 
  ren1 ? (ren|= 0x2)&&(raddr1&=0x3f) : (raddr1=0x00); 
  ren2 ? (ren|= 0x4)&&(raddr2&=0x3f) : (raddr2=0x00); 
  ren3 ? (ren|= 0x8)&&(raddr3&=0x3f) : (raddr3=0x00); 
  ren4 ? (ren|=0x10)&&(raddr4&=0x3f) : (raddr4=0x00); 

  uint64_t i_wvalue0 = 0x0101010101010101ull * wvalue0;
  uint64_t i_wvalue1 = 0x0101010101010101ull * wvalue1;
  return cp35_operate( index, output_sel, waddr0, i_wvalue0, wmask0, waddr1, i_wvalue1, wmask1, wen2,
      raddr0, raddr1, raddr2, raddr3, raddr4, ren, 0, 0, 0, 0, 0, 0, 0, q0, q1, q2, q3, q4 );
}

int cp35_read( const int index, const uint8_t addr, const uint64_t expected_data, const bool output_sel ){
  /* index : 0~1024, entry number of 1024 memory
   * output_sel : high 64 bits to output if true, otherwise low 64 bits
   * addr  : read address
   * expected_data : expected output data
   */
  return cp35_myoperate( index, output_sel, false, 0, 0, 0, false, 0, 0, 0, 0x0ull,
      true, addr, expected_data, true, addr, expected_data, true, addr, expected_data,
      true, addr, expected_data, true, addr, expected_data);
}

int cp35_readH( const int index, const uint8_t addr, const uint64_t expected_data ){
  return cp35_read( index, addr, expected_data, true );
}

int cp35_readL( const int index, const uint8_t addr, const uint64_t expected_data ){
  return cp35_read( index, addr, expected_data, false );
}

int cp35_write( int index, int wport, uint8_t addr, const uint8_t wvalue, const uint32_t wmask ){
  /*
   * Write data into cp0q_3w5r
   * index : 0~1024, entry number of 1024 memory
   */
  if(wport==0) return cp35_myoperate( index, false, true, addr, wvalue, wmask, false, 0, 0, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  else if(wport==1) return cp35_myoperate( index, false, false, 0, 0, 0, true, addr, wvalue, wmask, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  
  printf("**INFO** Specified WPORT not avalible, use W0 to write data in...\n");
  return cp35_myoperate( index, false, false, 0, 0, 0, true, addr, wvalue, wmask, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
}

int cp35_write_vec( int index,  uint64_t vec, const uint8_t wvalue ){
  /*
   * Write data into cp0q_3w5r
   * index : 0~1024, entry number of 1024 memory
   */
  return cp35_myoperate( index, false, false, 0, wvalue, 0, false, 0, 0, 0, vec,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
}

int cp35_memset( int index, const uint8_t wvalue, const uint32_t wmask, int wport ){
  /*
   * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
   * index : entry number of 1024 memory
   * wport : write port to use (0~7)
   * data  : data to write into, low 8-bit repeated to 64-bit
   */
  int addr;
  for( addr=CP35_ADDR_MIN; addr<=CP35_ADDR_MAX; addr++){
    cp35_write( index, /*wport*/wport, addr, wvalue, wmask );
    index++;
  }
  return index;
}

void cp35_print_vec( int index, const tb_cp0q_ram_old_in_t &pat, const tb_cp0q_ram_old_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
  if( pat.osel ) printf("     Output high 64 bits. \n");
  else printf("     Output low  64 bits. \n");
  if( pat.ren&(0x1<<0) ){
    printf("     Read     : addr0=%#04X, expected_data0=%#018lX;\n", pat.raddr0, gld.q0 );
  } 
  if( pat.ren&(0x1<<1) ){
    printf("     Read     : addr1=%#04X, expected_data1=%#018lX;\n", pat.raddr1, gld.q1 );
  } 
  if( pat.ren&(0x1<<2) ){
    printf("     Read     : addr2=%#04X, expected_data2=%#018lX;\n", pat.raddr2, gld.q2 );
  } 
  if( pat.ren&(0x1<<3) ){
    printf("     Read     : addr3=%#04X, expected_data3=%#018lX;\n", pat.raddr3, gld.q3 );
  } 
  if( pat.ren&(0x1<<4) ){
    printf("     Read     : addr4=%#04X, expected_data4=%#018lX;\n", pat.raddr4, gld.q4 );
  } 
  if( (pat.waddr0&0x40) == 0 ){
    printf("     Write    : addr0=%#04X, wvalue0=%#018lX_%016lX, wmask0=%#06lX;\n", pat.waddr0, pat.wvalue0[0], pat.wvalue0[1], pat.wmask0 );
  }
  if( (pat.waddr1&0x40) == 0 ){
    printf("     Write    : addr1=%#04X, wvalue1=%#018lX_%016lX, wmask1=%#06lX;\n", pat.waddr1, pat.wvalue1[0], pat.wvalue1[1], pat.wmask1 );
  }
  if( (pat.wen2) ){
    printf("     Vec Write: wen2=%#018lX, wvalue=%018lX_%016lX;\n", pat.wen2, pat.wvalue0[0], pat.wvalue0[1] );
  }
  fflush(stdout);
}

