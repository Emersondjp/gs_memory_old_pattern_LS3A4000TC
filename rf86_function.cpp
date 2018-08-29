#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "rf86_function.hpp"

int rf86_write( int index, unsigned int wport, unsigned int addr, uint8_t data ){
  /*
 * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
 * index : entry number of 1024 memory
 * wport : write port to use (0~7)
 * addr  : address of writing entry, should transform to onehot format
 * data  : data to write into, low 8-bit repeated to 64-bit
 */
  switch( wport ) {
    case 0 : return rf86_operate( index, true, addr, data, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 1 : return rf86_operate( index, false, 0, 0, true, addr, data, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 2 : return rf86_operate( index, false, 0, 0, false, 0, 0, true, addr, data, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 3 : return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, true, addr, data,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
             
    case 4 : return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         true, addr, data, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 5 : return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, true, addr, data, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 6 : return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, true, addr, data, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    case 7 : return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, true, addr, data,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);

    default : printf("Writing operation wport is ilegal, write through W0 as default!\n");
             return rf86_operate( index, true, addr, data, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                                         false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0);
  }

}

int rf86_memset( int index, unsigned int wport, uint8_t data ){
  /*
 * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
 * index : entry number of 1024 memory
 * wport : write port to use (0~7)
 * data  : data to write into, low 8-bit repeated to 64-bit
 */
  int t_index = index;
  for( int i=0; i<=RF86_ADDR_MAX-RF86_ADDR_MIN; i++){
    t_index = rf86_write( t_index, wport, i, data );
  }

  return t_index;
}

int rf86_read( int index, const unsigned int addr, const uint8_t expexted_data ){
  /*
  * Read specified entry of regfile_8w6r, using all read-port
  * start : start index of 1024 memory entry
  * addr  : address of read entry
  * data  : data expexted, low 8-bit repeated to 64-bit
 */
  return rf86_operate( index, false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                              false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
                              true,  addr, expexted_data, true,  addr, expexted_data,
                              true,  addr, expexted_data, true,  addr, expexted_data,
                              true,  addr, expexted_data, true,  addr, expexted_data);
}

int rf86_operate( int index, const bool we0, const uint16_t wad0, const uint8_t d0,
                             const bool we1, const uint16_t wad1, const uint8_t d1,
                             const bool we2, const uint16_t wad2, const uint8_t d2,
                             const bool we3, const uint16_t wad3, const uint8_t d3,
                             const bool we4, const uint16_t wad4, const uint8_t d4,
                             const bool we5, const uint16_t wad5, const uint8_t d5,
                             const bool we6, const uint16_t wad6, const uint8_t d6,
                             const bool we7, const uint16_t wad7, const uint8_t d7,
                             const bool re0, const uint16_t rad0, const uint8_t q0,
                             const bool re1, const uint16_t rad1, const uint8_t q1,
                             const bool re2, const uint16_t rad2, const uint8_t q2,
                             const bool re3, const uint16_t rad3, const uint8_t q3,
                             const bool re4, const uint16_t rad4, const uint8_t q4,
                             const bool re5, const uint16_t rad5, const uint8_t q5 ){
  // Verifying input
  /* printf("wad0 = 0x%02x, wad1 = 0x%02x, wad2 = 0x%02x, wad3 = 0x%02x, wad4 = 0x%02x, wad5 = 0x%02x, wad6 = 0x%02x, wad7 = 0x%02x,\nrad0 = 0x%02x, rad1 = 0x%02x, rad2 = 0x%02x, rad3 = 0x%02x, rad4 = 0x%02x, rad5 = 0x%02x.\n\n\n",
          wad0, wad1, wad2, wad3, wad4, wad5, wad6, wad7, rad0, rad1, rad2, rad3, rad4, rad5); */

  assert( wad0 >= RF86_ADDR_MIN && wad0 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad1 >= RF86_ADDR_MIN && wad1 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad2 >= RF86_ADDR_MIN && wad2 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad3 >= RF86_ADDR_MIN && wad3 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad4 >= RF86_ADDR_MIN && wad4 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad5 >= RF86_ADDR_MIN && wad5 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad6 >= RF86_ADDR_MIN && wad6 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( wad7 >= RF86_ADDR_MIN && wad7 <= RF86_ADDR_MAX );  // Checking Writing Address
  assert( rad0 >= RF86_ADDR_MIN && rad0 <= RF86_ADDR_MAX );  // Checking Reading Address
  assert( rad1 >= RF86_ADDR_MIN && rad1 <= RF86_ADDR_MAX );  // Checking Reading Address
  assert( rad2 >= RF86_ADDR_MIN && rad2 <= RF86_ADDR_MAX );  // Checking Reading Address
  assert( rad3 >= RF86_ADDR_MIN && rad3 <= RF86_ADDR_MAX );  // Checking Reading Address
  assert( rad4 >= RF86_ADDR_MIN && rad4 <= RF86_ADDR_MAX );  // Checking Reading Address
  assert( rad5 >= RF86_ADDR_MIN && rad5 <= RF86_ADDR_MAX );  // Checking Reading Address

  rf8w6r_in  pat;
  rf8w6r_out gld;
  memset( &pat, 0, sizeof(pat) ); // Clear before using
  memset( &gld, 0, sizeof(gld) ); // Clear before using

  // Write Part
  pat.wen7_0  |= we0? 0x1 << 0 : 0;
  pat.wen7_0  |= we1? 0x1 << 1 : 0;
  pat.wen7_0  |= we2? 0x1 << 2 : 0;
  pat.wen7_0  |= we3? 0x1 << 3 : 0;
  pat.wen7_0  |= we4? 0x1 << 4 : 0;
  pat.wen7_0  |= we5? 0x1 << 5 : 0;
  pat.wen7_0  |= we6? 0x1 << 6 : 0;
  pat.wen7_0  |= we7? 0x1 << 7 : 0;

  pat.waddr[0] = wad0;
  pat.waddr[1] = wad1;
  pat.waddr[2] = wad2;
  pat.waddr[3] = wad3;
  pat.waddr[4] = wad4;
  pat.waddr[5] = wad5;
  pat.waddr[6] = wad6;
  pat.waddr[7] = wad7;

  pat.d[0] = 0x0101010101010101ull*(d0&0xff);
  pat.d[1] = 0x0101010101010101ull*(d1&0xff);
  pat.d[2] = 0x0101010101010101ull*(d2&0xff);
  pat.d[3] = 0x0101010101010101ull*(d3&0xff);
  pat.d[4] = 0x0101010101010101ull*(d4&0xff);
  pat.d[5] = 0x0101010101010101ull*(d5&0xff);
  pat.d[6] = 0x0101010101010101ull*(d6&0xff);
  pat.d[7] = 0x0101010101010101ull*(d7&0xff);

  // Read Part
  pat.cmp_mask |= re0? 0xffull << 8*0 : 0;
  pat.cmp_mask |= re1? 0xffull << 8*1 : 0;
  pat.cmp_mask |= re2? 0xffull << 8*2 : 0;
  pat.cmp_mask |= re3? 0xffull << 8*3 : 0;
  pat.cmp_mask |= re4? 0xffull << 8*4 : 0;
  pat.cmp_mask |= re5? 0xffull << 8*5 : 0;

  pat.ren5_0 |= re0? 0x1 << 0 : 0;
  pat.ren5_0 |= re1? 0x1 << 1 : 0;
  pat.ren5_0 |= re2? 0x1 << 2 : 0;
  pat.ren5_0 |= re3? 0x1 << 3 : 0;
  pat.ren5_0 |= re4? 0x1 << 4 : 0;
  pat.ren5_0 |= re5? 0x1 << 5 : 0;

  pat.raddr[0] = rad0;
  pat.raddr[1] = rad1;
  pat.raddr[2] = rad2;
  pat.raddr[3] = rad3;
  pat.raddr[4] = rad4;
  pat.raddr[5] = rad5;

  gld.q[0] = 0x0101010101010101ull*(q0&0xffull);
  gld.q[1] = 0x0101010101010101ull*(q1&0xffull);
  gld.q[2] = 0x0101010101010101ull*(q2&0xffull);
  gld.q[3] = 0x0101010101010101ull*(q3&0xffull);
  gld.q[4] = 0x0101010101010101ull*(q4&0xffull);
  gld.q[5] = 0x0101010101010101ull*(q5&0xffull);

  write_golden( index, gld );
  write_pattern( index, pat );

  return index+1;
}

void rf86_print_vec( int index, const tb_rf8w6r_in_t &pat ){
  /*
 * Print vector information.
 */
  printf("Index %04d : \n", index);
  for(int i=RF86_WPORT_MIN; i<=RF86_WPORT_MAX; i++){
    printf("    wen%d -> %d, waddr%d -> 0x%02X, wdata%d -> %#018lX;\n", i, bool(pat.wen7_0&(0x1<<i)), i, pat.waddr[i], i, pat.d[i]);
  }

  if (pat.cmp_mask == 0x0) return;
  else printf("    compare mask is %#018X, while no golden data given.\n", pat.cmp_mask );
}

void rf86_print_vec( int index, const tb_rf8w6r_in_t &pat, const tb_rf8w6r_out_t &gld ){
  /*
 * Print vector information.
 */
  printf("Index %04d : \n", index);
  for(int i=RF86_WPORT_MIN; i<=RF86_WPORT_MAX; i++){
    if(pat.wen7_0&(0x1<<i))
      printf("    wen%d -> %d, waddr%d -> 0x%02X, wdata%d -> %#018lX;\n", i, bool(pat.wen7_0&(0x1<<i)), i, pat.waddr[i], i, pat.d[i]);
  }

  if (pat.cmp_mask == 0x0) return;
  assert( &gld!=NULL );
  for(int i=RF86_RPORT_MIN; i<=RF86_RPORT_MAX; i++){
    if(pat.ren5_0&(0x1<<i))
      printf("    ren%d -> %d, raddr%d -> 0x%02X, expt_d%d -> %#018lX;\n", i, bool(pat.ren5_0&(0x1<<i)),  i, pat.raddr[i], i, gld.q[i]);
  }
}

void rf86_print_vec2( int index, const tb_rf8w6r_in_t &pat, const tb_rf8w6r_out_t &gld ){
  /*
 * Print vector information.
 */
  printf("Index %04d : \n", index);
  for(int i=RF86_WPORT_MIN; i<=RF86_WPORT_MAX; i++){
    printf("    wen%d -> %d, waddr%d -> 0x%02X, wdata%d -> %#018lX;\n", i, bool(pat.wen7_0&(0x1<<i)), i, pat.waddr[i], i, pat.d[i]);
  }

  printf("    compare_mask -> %016lX.\n", pat.cmp_mask);
  for(int i=RF86_RPORT_MIN; i<=RF86_RPORT_MAX; i++){
    printf("    ren%d -> %d, raddr%d -> 0x%02X, expt_d%d -> %#018lX;\n", i, bool(pat.ren5_0&(0x1<<i)),  i, pat.raddr[i], i, gld.q[i]);
  }
}

void rf86_print_cfg(uint32_t cfg){
  /*
 * Print Configuration Code of 8W6R
 */
  printf("\n**************** REGFILE_8W6R Adj Code Information ****************\n");
  printf("RDLY_S    = 0x%X;\n", (cfg>> 0)&0x7); //RDLY_S    = cfg[2:0]
  printf("WWL_DLY_S = 0x%X;\n", (cfg>> 4)&0x7); //WWL_DLY_S = cfg[6:4];
  printf("CK2_DLY_S = 0x%X;\n", (cfg>> 8)&0x7); //CK2_DLY_S = cfg[10:8];
  printf("BPGEN_S   = 0x%X;\n", (cfg>>12)&0x3); //BPGEN_S   = cfg[13:12];
  printf("RPC_S     = 0x%X;\n", (cfg>>16)&0x3); //RPC_S     = cfg[17:16];
  printf("PW1_S     = 0x%X;\n", (cfg>>20)&0x3); //PW1_S     = cfg[21:20];
  printf("PW2_S     = 0x%X;\n", (cfg>>24)&0x3); //PW2_S     = cfg[25:24];
  printf("PC_S      = 0x%X;\n", (cfg>>28)&0xf); //PC_S      = cfg[31:28];
  printf("*******************************************************************\n\n");
}
