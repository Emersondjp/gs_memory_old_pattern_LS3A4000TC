#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "cam464v_function.hpp"

int cam464v_operate( int index, uint8_t ren, uint8_t addr,
                     uint8_t wen, uint64_t wd, uint64_t wvpn, uint32_t wasid, uint32_t mask, uint8_t G,
                     uint8_t sen, uint64_t svpn, uint32_t sasid, uint64_t valid,
                     uint64_t match, uint64_t rd, uint8_t hit ){
  /*
   * wd, rd will be extended from 0xAB to 0xABABABABABABABAB, if <= 0xff
   */

  assert( addr>= CAM464V_ADDR_MIN && addr<=CAM464V_ADDR_MAX ); // make sure addr is in the right range.
  assert( not ( ren&&sen ) ); // assert read and search not happend at the same time.
  assert( not ( ren&&wen ) ); // assert read and search not happend at the same time.
  // if write and search happend the same time, make sure write entry not valid.
  if( wen && sen ) assert( not ( valid & (0x1ull<<addr) ) );
  tb_cam64x64_in_t  pat;
  tb_cam64x64_out_t gld;
  
  memset( &pat, 0, sizeof(pat) ); // Clear before using;
  memset( &gld, 0, sizeof(gld) ); // Clear before using;

  // Write Part
  if( wen ) {
    pat.addr  = addr;
    pat.wen   = wen;
    pat.wd    = wd;
    pat.wvpn  = wvpn;
    pat.wasid = wasid;
    pat.mask  = mask;
    pat.G     = G;
  }
  // Search Part
  if( sen ){
    pat.sen   = sen;
    pat.svpn  = svpn;
    pat.sasid = sasid;
    pat.valid = valid;
    gld.match = match;
    gld.rd    = rd;
    gld.hit   = hit;
    pat.cmp_mask = 0x1ffff;
  }

  // Read Part
  if( ren ) {
    pat.ren   = ren;
    pat.addr  = addr;
    gld.rd    = rd;
    pat.cmp_mask = 0xff00 ;
  }
  
  // Write into 1024 ram
  write_pattern( index, pat );
  write_golden( index, gld );

  return index+1;
}

int cam464v_read( int index, uint8_t addr, uint64_t expected_data ){
  /* index : 0~1024, entry number of 1024 memory
   * addr  : read address
   * expected_data : expected output data
   */
  return cam464v_operate( index, /*ren*/1, /*addr*/addr,
                    /*wen*/0, /*wd*/0, /*wvpn*/0, /*wasid*/0, /*mask*/0, /*G*/0,
                    /*sen*/0, /*svpn*/0, /*sasid*/0, /*valid*/0,
                    /*match*/0, /*rd*/expected_data, /*hit*/0 );
}

int cam464v_write( int index, uint8_t addr, uint64_t wd, uint64_t wvpn, uint32_t wasid, uint32_t mask, uint8_t G ){
  /*
   * Write data into cam_464v
   * index : 0~1024, entry number of 1024 memory
   */
  return cam464v_operate( index, /*ren*/false, /*addr*/addr,
      /*wen*/0x1, /*wd*/wd, /*wvpn*/wvpn, /*wasid*/wasid, /*mask*/mask, /*G*/G);
}

int cam464v_search( int index, uint64_t svpn, uint16_t sasid, uint32_t valid,
                     uint64_t match, uint64_t rd, uint8_t hit ){
 /*
  * Search for match entry in cam_464v
  * index : 0~1024, entry number of 1024 memory
  */
  return cam464v_operate( index, /*ren*/0, /*addr*/0,
                    /*wen*/0, /*wd*/0, /*wvpn*/0, /*wasid*/0, /*mask*/0, /*G*/0,
                    /*sen*/1, /*svpn*/svpn, /*sasid*/sasid, /*valid*/valid,
                    /*match*/match, /*rd*/rd, /*hit*/hit );
}

int cam464v_memset( int index, uint64_t wd, uint64_t wvpn, uint32_t wasid, uint32_t mask, uint8_t G ){
  /*
   * Write data into addr entry of cam464v, return finished index number of 1024 memory
   * index : entry number of 1024 memory
   * wport : write port to use (0~7)
   * data  : data to write into, low 8-bit repeated to 64-bit
   */
  int addr;
  for( addr=CAM464V_ADDR_MIN; addr<=CAM464V_ADDR_MAX; addr++){
    index = cam464v_write( index, addr, wd, wvpn, wasid, mask, G );
  }
  return index;
}

void cam464v_print_vec( int index, const tb_cam64x64_in_t &pat, const tb_cam64x64_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
  if( pat.ren ){
    printf("     Read     : addr=%#04X, expected_data=%#018lX;\n", pat.addr, gld.rd );
  } 
  if( pat.wen ){
    printf("     Write    : addr=%#04X, wd=%#018lX, wvpn=%#014lX, wasid=%#05X, mask=%#010X, G=%d;\n", pat.addr, pat.wd, pat.wvpn, pat.wasid, pat.mask, pat.G );
  }
  if( pat.sen ){
    printf("     Search   : svpn=%#014lX, sasid=%#05X, valid=%#018lX;\n", pat.svpn, pat.sasid, pat.valid );
    printf("     Expected : hit=%x, rd  =%#018lX, match=%#018lX;\n", gld.hit, gld.rd, gld.match );
  }
  fflush(stdout);
}

void cam464v_print_vec2( int index, const tb_cam64x64_in_t &pat, const tb_cam64x64_out_t &gld ){
  /*
   * Print vector information.
   */
  printf("Index %04d : \n", index);
    printf("     Read     : ren=%d, addr=%#04X, expected_data=%#018lX;\n", pat.ren, pat.addr, gld.rd );
    printf("     Write    : wen=%d, addr=%#04X, wd=%#018lX, wvpn=%#014lX, wasid=%#05X, mask=%#010X, G=%#04X;\n", pat.wen, pat.addr, pat.wd, pat.wvpn, pat.wasid, pat.mask, pat.G );
    printf("     Search   : sen=%d, svpn=%#014lX, sasid=%#05X, valid=%#018lX;\n", pat.sen, pat.svpn, pat.sasid, pat.valid );
    printf("     Expected : hit=%x, rd  =%#018lX, match=%#018lX;\n", gld.hit, gld.rd, gld.match );
  fflush(stdout);
}

