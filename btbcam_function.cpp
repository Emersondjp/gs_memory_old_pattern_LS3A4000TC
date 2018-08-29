#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "btbcam_function.hpp"

int btbcam_operate( int index, uint8_t sen, uint32_t svpn,
    uint32_t valid31_00, uint32_t valid63_32, uint32_t valid95_64,
    uint8_t wen, uint32_t addr31_00, uint32_t addr63_32, uint32_t addr95_64,
    uint64_t wd, uint32_t wvpn,
    uint32_t match31_00, uint32_t match63_32, uint32_t match95_64,
    uint64_t rd, uint8_t hit ){
  /*
   * wd, rd will be extended from 0xAB to 0xABABABABABABABAB, if EXTEND_DATA defined
   */

  bool flag=false;
  for(int i=0;i<32;i++){ // make sure only one 1 in addr.
    if( addr31_00 & (0x01ul<<i) ){
      if(flag) {
        printf("**ERROR** : In BTB_CAM, more one address effective, abort!\n");
        exit(-1);
      }else flag=true;
    }
    if( addr63_32 & (0x01ul<<i) ){
      if(flag) {
        printf("**ERROR** : In BTB_CAM, more one address effective, abort!\n");
        exit(-1);
      }else flag=true;
    }
    if( addr95_64 & (0x01ul<<i) ){
      if(flag) {
        printf("**ERROR** : In BTB_CAM, more one address effective, abort!\n");
        exit(-1);
      }else flag=true;
    }
  }
  if( sen && wen ){
    assert( not(addr31_00 & valid31_00)  );
    assert( not(addr63_32 & valid63_32)  );
    assert( not(addr95_64 & valid95_64)  );
  }
  assert( wvpn < (0x01ul <<30) );
  assert( wd   < (0x01ull<<46) );

  // if write and search happend the same time, make sure write entry not valid.
  tb_btbcam_in_t  pat;
  tb_btbcam_out_t gld;
  
  memset( &pat, 0, sizeof(pat) ); // Clear before using;
  memset( &gld, 0, sizeof(gld) ); // Clear before using;

  // Write Part
  if( wen ) {
    pat.wen   = wen ? 0x1 : 0x0;
    pat.wd    = wd;
    pat.wvpn  = wvpn;
  }
  // Search Part
  if( sen ){
    pat.sen   = sen;
    pat.svpn  = svpn;
    pat.valid[0] = valid31_00;
    pat.valid[1] = valid63_32;
    pat.valid[2] = valid95_64;
    gld.match[0] = match31_00;
    gld.match[1] = match63_32;
    gld.match[2] = match95_64;
    gld.rd[0]    = rd&0xffffffff;
    gld.rd[1]    = (rd>>32)&0x3fff;
    gld.hit      = hit;
    pat.cmp_mask = 0x1fffff;
  }

  // Write into 1024 ram
  write_pattern( index, pat );
  write_golden( index, gld );

  return index+1;
}


int btbcam_write( int index, uint8_t addr, uint64_t wd, uint32_t wvpn ){
  /*
   * Write data into btbcam
   * index : 0~1024, entry number of 1024 memory
   */
  uint32_t addr31_00, addr63_32, addr95_64;
  addr31_00 = ( addr >= 0  && addr < 32 ) ? 0x01<<addr : 0x00;
  addr63_32 = ( addr >= 32 && addr < 64 ) ? 0x01<<(addr-32) : 0x00;
  addr95_64 = ( addr >= 64 && addr < 96 ) ? 0x01<<(addr-64) : 0x00;

  return btbcam_operate( index, 0, 0, 0x00, 0x00, 0x00,
      true, addr31_00, addr63_32, addr95_64, wd, wvpn,
      0x00, 0x00, 0x00, 0x00, false);
}

int btbcam_search( int index, uint32_t svpn,
    uint32_t valid31_00, uint32_t valid63_32, uint32_t valid95_64,
    uint32_t match31_00, uint32_t match63_32, uint32_t match95_64,
    uint64_t rd, bool hit ){
 /*
  * Search for match entry in btbcam
  * index : 0~1024, entry number of 1024 memory
  */
  return btbcam_operate( index, /*sen*/true, /*svpn*/svpn,
    valid31_00, valid63_32, valid95_64, /*wen*/false,
    /*addr31_00*/0x00, /*addr63_32*/0x00, /*addr95_64*/0x00,
    /*wd*/0x00ull, /*wvpn*/0x00,
    match31_00, match63_32, match95_64, rd, (hit?0x1:0x0) );
}

int btbcam_memset( int index, uint64_t wd, uint32_t wvpn ){
  /*
   * Write data into addr entry , return finished index number of 1024 memory
   * index : entry number of 1024 memory
   */
  int addr;
  for( addr=BTB_CAM_ADDR_MIN; addr<=BTB_CAM_ADDR_MAX; addr++){
    index = btbcam_write( index, addr, wd, wvpn );
  }
  return index;
}

int btbcam_memset2( int index, uint64_t wd, uint32_t wvpn ){
  /*
   * Write data into addr entry , return finished index number of 1024 memory
   * index : entry number of 1024 memory
   */
  int addr;
  for( addr=BTB_CAM_ADDR_MIN; addr<=BTB_CAM_ADDR_MAX; addr++){
    index = btbcam_write( index, addr, (wd+addr)&0x00003fffffffffffull, (wvpn+addr)&0x3ffffffful );
  }
  return index;
}

void btbcam_print_vec( int index, const tb_btbcam_in_t &pat, const tb_btbcam_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
  if( pat.wen ){
    printf("     Write    : addr=0x%016lX_%016lX_%016lX, wd=%#014lX, wvpn=%#010lX;\n", pat.addr[2], pat.addr[1], pat.addr[0], pat.wd, pat.wvpn );
  }
  if( pat.sen ){
    printf("     Search   : svpn=%#010lX, valid=0x%016lX_%016lX_%016lX;\n", pat.svpn, pat.valid[2], pat.valid[1], pat.valid[0] );
    printf("     Expected : hit=%x, rd  =0x%08lX_%08lX, match=0x%016lX_%016lX_%016lX;\n", gld.hit, gld.rd[1], gld.rd[0], gld.match[2], gld.match[1], gld.match[0] );
  }
  fflush(stdout);
}

void btbcam_print_vec2( int index, const tb_btbcam_in_t &pat, const tb_btbcam_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
    printf("     Write    : wen=%d, addr=%#04X, wd=%#018lX, wvpn=%#014lX;\n", pat.wen, pat.addr, pat.wd, pat.wvpn );
    printf("     Search   : sen=%d, svpn=%#014lX, valid=0x%016lX_%016lX_%016lX;\n", pat.sen, pat.svpn, pat.valid[2], pat.valid[1], pat.valid[0] );
    printf("     Expected : hit=%x, rd  =0x%08lX_%08lX, match=0x%016lX_%016lX_%016lX;\n", gld.hit, gld.rd[1], gld.rd[0], gld.match[2], gld.match[1], gld.match[0] );
  fflush(stdout);
}

