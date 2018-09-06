#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<time.h>
#include<string.h>
#include "spi.h"
#include "global.h"
#include "types.h"
#include "testbed.hpp"
#include "gs_memory_class.hpp"
#include "rf86_function.hpp"
#include "rf44_function.hpp"
#include "cp35_function.hpp"
#include "cp25_function.hpp"
#include "cam464v_function.hpp"
#include "btbcam_function.hpp"

int pat_cnt=0;
int gld_cnt=0;
extern uint32_t rf86_cfg;

template <class T1, class T2> bool write_patgld( const int index, const T1& pat, const T2& gld ){
  // {{{1
#ifndef NO_HARDWARE
  T1 cpy_pat, tmp_pat;
  T2 cpy_gld, tmp_gld;
  memset( &tmp_pat, 0 , sizeof(T1) );
  memset( &tmp_gld, 0 , sizeof(T2) );
  do{
    memcpy( &cpy_pat, &pat, sizeof(T1) );
    write_pattern( index, cpy_pat );

    memset( &tmp_pat, 0 , sizeof(T1) );
    read_pattern( index, tmp_pat );
    if( memcmp( &pat, &tmp_pat, sizeof(T1) ) != 0 ){
      printf("*ERROR* : pattern-ram write error detected!\n");
      pat_cnt++;
      continue;
    }else break;
  }while(1);

  do{
    memcpy( &cpy_gld, &gld, sizeof(T2) );
    write_golden( index, cpy_gld );

    memset( &tmp_gld, 0 , sizeof(T2) );
    read_golden( index, tmp_gld );
    if( memcmp( &gld, &tmp_gld, sizeof(T2) ) != 0 ){
      printf("*ERROR* : golden-ram write error detected!\n");
      gld_cnt++;
      continue;
    }else break;
  }while(1);

#endif
} // }}}1

inline unsigned int doTest_rf86( const int index, tb_rf8w6r_in_t& pat, tb_rf8w6r_out_t&gld, gs_regfile_128x64_8sw6sr& rf86 ){
  // {{{1
  if( rf86.operate(pat.wen7_0&0x01, pat.waddr[0],pat.d[0],
        pat.wen7_0&0x02, pat.waddr[1], pat.d[1],
        pat.wen7_0&0x04, pat.waddr[2], pat.d[2],
        pat.wen7_0&0x08, pat.waddr[3], pat.d[3],
        pat.wen7_0&0x10, pat.waddr[4], pat.d[4],
        pat.wen7_0&0x20, pat.waddr[5], pat.d[5],
        pat.wen7_0&0x40, pat.waddr[6], pat.d[6],
        pat.wen7_0&0x80, pat.waddr[7], pat.d[7],
        pat.ren5_0&0x01, pat.raddr[0], pat.ren5_0&0x02, pat.raddr[1],
        pat.ren5_0&0x04, pat.raddr[2], pat.ren5_0&0x08, pat.raddr[3],
        pat.ren5_0&0x10, pat.raddr[4], pat.ren5_0&0x20, pat.raddr[5]) ){
    gld.q[0] = rf86.get_out0();
    gld.q[1] = rf86.get_out1();
    gld.q[2] = rf86.get_out2();
    gld.q[3] = rf86.get_out3();
    gld.q[4] = rf86.get_out4();
    gld.q[5] = rf86.get_out5();

    write_patgld(index, pat, gld);

    return index+1;
  } else exit(-1);
} // }}}1
bool oldPatternTest_8w6r( gs_regfile_128x64_8sw6sr& rf86 ){
  // {{{1
  tb_rf8w6r_in_t  pat, tmp_pat;
  tb_rf8w6r_out_t gld, tmp_gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));
  memset(&tmp_pat, 0, sizeof(tmp_pat));
  memset(&tmp_gld, 0, sizeof(tmp_gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  int addr = 0x36;
  if(rf86.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    //pat.cmp_mask = 0x00ffffffffffffull;
    pat.ren5_0 = 0x3f;
    pat.raddr[5] = addr;
    pat.raddr[4] = addr;
    pat.raddr[3] = addr;
    pat.raddr[2] = addr;
    pat.raddr[1] = addr;
    pat.raddr[0] = addr;
    index = doTest_rf86(index, pat, gld, rf86);
    index = doTest_rf86(index, pat, gld, rf86);
  } else exit(-1); // }}}2

  if(1){ //  begin 预充
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00ffffffffffffull;
    pat.waddr[0] = 0x33;
    pat.waddr[1] = 0x44; // make sure waddr0 != waddr1
    pat.raddr[0] = pat.waddr[0];
    pat.raddr[1] = pat.waddr[0];
    pat.raddr[2] = pat.waddr[0];
    pat.raddr[3] = pat.waddr[0];
    pat.raddr[4] = pat.waddr[0];
    pat.raddr[5] = pat.waddr[0];
    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0x0000000000000000ull;
    pat.wen7_0 = 0x3;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    pat.ren5_0 = 0x3f;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.raddr[0] = pat.waddr[1];
    pat.raddr[1] = pat.waddr[1];
    pat.raddr[2] = pat.waddr[1];
    pat.raddr[3] = pat.waddr[1];
    pat.raddr[4] = pat.waddr[1];
    pat.raddr[5] = pat.waddr[1];
    index = doTest_rf86(index, pat, gld, rf86);

    pat.raddr[0] = pat.waddr[0];
    pat.raddr[1] = pat.waddr[0];
    pat.raddr[2] = pat.waddr[0];
    pat.raddr[3] = pat.waddr[0];
    pat.raddr[4] = pat.waddr[0];
    pat.raddr[5] = pat.waddr[0];
    index = doTest_rf86(index, pat, gld, rf86);

    pat.raddr[0] = pat.waddr[1];
    pat.raddr[1] = pat.waddr[1];
    pat.raddr[2] = pat.waddr[1];
    pat.raddr[3] = pat.waddr[1];
    pat.raddr[4] = pat.waddr[1];
    pat.raddr[5] = pat.waddr[1];
    index = doTest_rf86(index, pat, gld, rf86);
  } // }}}2

  if(1){ // begin 写入（bypass + couple + farest entry)
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00ffffffffffffull;

    // WPORT-0
    // {{{3
    pat.waddr[0] = 0x7f;
    pat.waddr[1] = 0x7e;
    pat.waddr[2] = 0x7d;
    pat.waddr[3] = 0x7c;
    pat.waddr[4] = 0x7b;
    pat.waddr[5] = 0x7a;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.raddr[0] = 0x7f;
    pat.raddr[1] = 0x7f;
    pat.raddr[2] = 0x7f;
    pat.raddr[3] = 0x7f;
    pat.raddr[4] = 0x7f;
    pat.raddr[5] = 0x7f;
    pat.d[0] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    pat.ren5_0 = 0x3f;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-1
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7f;
    pat.waddr[2] = 0x7d;
    pat.waddr[3] = 0x7c;
    pat.waddr[4] = 0x7b;
    pat.waddr[5] = 0x7a;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-2
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7f;
    pat.waddr[3] = 0x7c;
    pat.waddr[4] = 0x7b;
    pat.waddr[5] = 0x7a;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-3
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7c;
    pat.waddr[3] = 0x7f;
    pat.waddr[4] = 0x7b;
    pat.waddr[5] = 0x7a;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-4
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7c;
    pat.waddr[3] = 0x7b;
    pat.waddr[4] = 0x7f;
    pat.waddr[5] = 0x7a;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-5
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7c;
    pat.waddr[3] = 0x7b;
    pat.waddr[4] = 0x7a;
    pat.waddr[5] = 0x7f;
    pat.waddr[6] = 0x79;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-6
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7c;
    pat.waddr[3] = 0x7b;
    pat.waddr[4] = 0x7a;
    pat.waddr[5] = 0x79;
    pat.waddr[6] = 0x7f;
    pat.waddr[7] = 0x78;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3
    // WPORT-7
    // {{{3
    pat.waddr[0] = 0x7e;
    pat.waddr[1] = 0x7d;
    pat.waddr[2] = 0x7c;
    pat.waddr[3] = 0x7b;
    pat.waddr[4] = 0x7a;
    pat.waddr[5] = 0x79;
    pat.waddr[6] = 0x78;
    pat.waddr[7] = 0x7f;
    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0xffffffffffffffffull;
    pat.d[1] = 0xffffffffffffffffull;
    pat.d[2] = 0xffffffffffffffffull;
    pat.d[3] = 0xffffffffffffffffull;
    pat.d[4] = 0xffffffffffffffffull;
    pat.d[5] = 0xffffffffffffffffull;
    pat.d[6] = 0xffffffffffffffffull;
    pat.d[7] = 0x0000000000000000ull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.d[0] = 0x0000000000000000ull;
    pat.d[1] = 0x0000000000000000ull;
    pat.d[2] = 0x0000000000000000ull;
    pat.d[3] = 0x0000000000000000ull;
    pat.d[4] = 0x0000000000000000ull;
    pat.d[5] = 0x0000000000000000ull;
    pat.d[6] = 0x0000000000000000ull;
    pat.d[7] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);
    // }}}3

  } // begin 写入（bypass + couple + farest entry)  
  if(1){ // begin 读取Glitch
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.waddr[0] = 0x00;
    pat.waddr[1] = 0x01;
    pat.waddr[2] = 0x02;
    pat.waddr[3] = 0x04;
    pat.waddr[4] = 0x08;
    pat.waddr[5] = 0x10;
    pat.waddr[6] = 0x20;
    pat.waddr[7] = 0x40;
    pat.d[0] = 0xffffffffffffffffull;
    pat.wen7_0 = 0xff;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.ren5_0 = 0x3f;
    // Addr+1 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x01;
    pat.raddr[1] = 0x01;
    pat.raddr[2] = 0x01;
    pat.raddr[3] = 0x01;
    pat.raddr[4] = 0x01;
    pat.raddr[5] = 0x01;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+2 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x02;
    pat.raddr[1] = 0x02;
    pat.raddr[2] = 0x02;
    pat.raddr[3] = 0x02;
    pat.raddr[4] = 0x02;
    pat.raddr[5] = 0x02;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+4 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x04;
    pat.raddr[1] = 0x04;
    pat.raddr[2] = 0x04;
    pat.raddr[3] = 0x04;
    pat.raddr[4] = 0x04;
    pat.raddr[5] = 0x04;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+8 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x08;
    pat.raddr[1] = 0x08;
    pat.raddr[2] = 0x08;
    pat.raddr[3] = 0x08;
    pat.raddr[4] = 0x08;
    pat.raddr[5] = 0x08;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+16 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x10;
    pat.raddr[1] = 0x10;
    pat.raddr[2] = 0x10;
    pat.raddr[3] = 0x10;
    pat.raddr[4] = 0x10;
    pat.raddr[5] = 0x10;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+32 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x20;
    pat.raddr[1] = 0x20;
    pat.raddr[2] = 0x20;
    pat.raddr[3] = 0x20;
    pat.raddr[4] = 0x20;
    pat.raddr[5] = 0x20;
    index = doTest_rf86(index, pat, gld, rf86);
    // 
    // Addr+64 
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.raddr[4] = 0x00;
    pat.raddr[5] = 0x00;
    index = doTest_rf86(index, pat, gld, rf86);
    pat.raddr[0] = 0x40;
    pat.raddr[1] = 0x40;
    pat.raddr[2] = 0x40;
    pat.raddr[3] = 0x40;
    pat.raddr[4] = 0x40;
    pat.raddr[5] = 0x40;
    index = doTest_rf86(index, pat, gld, rf86);
  } // end 读取Glitch }}}2

  if(1){ // begin 读远端RPC关闭过早问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.waddr[0] = 0x7F;
    pat.waddr[1] = 0x00;
    pat.d[0] = 0xffffffffffffffffull;
    pat.wen7_0 = 0x3;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.wen7_0 = 0x0;
    pat.ren5_0 = 0xf;
    pat.raddr[0] = 0x7F;
    pat.raddr[1] = 0x7F;
    pat.raddr[2] = 0x7F;
    pat.raddr[3] = 0x7F;
    pat.raddr[4] = 0x7F;
    pat.raddr[5] = 0x7F;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.raddr[0] = 0x0;
    pat.raddr[1] = 0x0;
    pat.raddr[2] = 0x0;
    pat.raddr[3] = 0x0;
    pat.raddr[4] = 0x0;
    pat.raddr[5] = 0x0;
    index = doTest_rf86(index, pat, gld, rf86);

    pat.raddr[0] = 0x7F;
    pat.raddr[1] = 0x7F;
    pat.raddr[2] = 0x7F;
    pat.raddr[3] = 0x7F;
    pat.raddr[4] = 0x7F;
    pat.raddr[5] = 0x7F;
    index = doTest_rf86(index, pat, gld, rf86);

  } // end 读Glitch问题 }}}2

  // Start test
  uint32_t st=0;
  bool flag=true;
  // Testing REGFILE_8W6R_V
  // {{{2
  printf("\n\nStart testing REGFILE_8W6R_V ...\n");
  tb_start(REGFILE_8W6R_V, index-1, rf86_cfg);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_rf8w6r_in_t  tmp_pat;
    tb_rf8w6r_out_t tmp_gld;
    tb_rf8w6r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf86_print_vec2( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) flag=false;
  // }}}2

  // Testing REGFILE_8W6R_H
  // {{{2
  printf("\n\nStart testing REGFILE_8W6R_H ...\n");
  tb_start(REGFILE_8W6R_H, index-1, rf86_cfg);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_rf8w6r_in_t  tmp_pat;
    tb_rf8w6r_out_t tmp_gld;
    tb_rf8w6r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf86_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) flag=false;
  // }}}2

  return flag;
} // }}}1

inline unsigned int doTest_rf44( const int index, tb_rf4w4r_in_t& pat, tb_rf4w4r_out_t& gld, gs_regfile_128x64_4sw4sr& rf44 ){
  // {{{1
  if( rf44.operate(pat.wen3_0&0x01, pat.waddr[0], pat.d[0],
        pat.wen3_0&0x02, pat.waddr[1], pat.d[1],
        pat.wen3_0&0x04, pat.waddr[2], pat.d[2],
        pat.wen3_0&0x08, pat.waddr[3], pat.d[3],
        pat.ren3_0&0x01, pat.raddr[0], pat.ren3_0&0x02, pat.raddr[1],
        pat.ren3_0&0x04, pat.raddr[2], pat.ren3_0&0x08, pat.raddr[3]) ){
    gld.q[0] = rf44.get_out0();
    gld.q[1] = rf44.get_out1();
    gld.q[2] = rf44.get_out2();
    gld.q[3] = rf44.get_out3();
    write_patgld(index, pat, gld);
    return index+1;
  } else exit(-1);
} // }}}1
int oldPatternTest_4w4r( gs_regfile_128x64_4sw4sr & rf44 ){
  // {{{1
  tb_rf4w4r_in_t  pat;
  tb_rf4w4r_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  int addr = 0x33;
  if(rf44.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.ren3_0 = 0x0f;
    pat.raddr[3] = addr;
    pat.raddr[2] = addr;
    pat.raddr[1] = addr;
    pat.raddr[0] = addr;
    index = doTest_rf44(index, pat, gld, rf44);
  } else exit(-1); // }}}2

  if(1){ // begin 预充
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));

    pat.cmp_mask = 0x00000000ffffffffull;
    pat.wen3_0 = 0x03;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0x0000000000000000ull;
    pat.waddr[0] = 0x33;
    pat.waddr[1] = 0x44; // make sure waddr0 != waddr1
    pat.raddr[0] = pat.waddr[0];
    pat.raddr[1] = pat.waddr[0];
    pat.raddr[2] = pat.waddr[0];
    pat.raddr[3] = pat.waddr[0];
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    pat.ren3_0   = 0x0f;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.raddr[0] = pat.waddr[1];
    pat.raddr[1] = pat.waddr[1];
    pat.raddr[2] = pat.waddr[1];
    pat.raddr[3] = pat.waddr[1];
    index = doTest_rf44(index, pat, gld, rf44);
  } // end 预充  }}}2

  if(1){ // begin 写入（bypass + couple + farest entry)
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.waddr[0] = 0x7F;
    pat.waddr[1] = 0x7F;
    pat.waddr[2] = 0x7F;
    pat.waddr[3] = 0x7F;
    pat.raddr[0] = 0x7F;
    pat.raddr[1] = 0x7F;
    pat.raddr[2] = 0x7F;
    pat.raddr[3] = 0x7F;
    pat.ren3_0 = 0xf;

    // WPORT-0 {{{3
    pat.wen3_0 = 0x01;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0x0;
    pat.d[2] = 0x0;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x01;
    pat.d[0] = 0x0;
    pat.d[1] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[2] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[3] = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x01;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0x0;
    pat.d[2] = 0x0;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    // end WPORT-1 }}}3

    // begin WPORT-1 {{{3
    pat.wen3_0 = 0x02;
    pat.d[0] = 0x0;
    pat.d[1] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[2] = 0x0;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x02;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0x0;
    pat.d[2] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[3] = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x02;
    pat.d[0] = 0x0;
    pat.d[1] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[2] = 0x0;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    // end WPORT-1 }}}3

    // begin WPORT-2 {{{3
    pat.wen3_0 = 0x04;
    pat.d[0] = 0x0;
    pat.d[1] = 0x0;
    pat.d[2] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x04;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[2] = 0x0;
    pat.d[3] = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x04;
    pat.d[0] = 0x0;
    pat.d[1] = 0x0;
    pat.d[2] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    // end WPORT-2 }}}3

    // begin WPORT-3 {{{3
    pat.wen3_0 = 0x08;
    pat.d[0] = 0x0;
    pat.d[1] = 0x0;
    pat.d[2] = 0x0;
    pat.d[3] = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x08;
    pat.d[0] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[1] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[2] = 0xFFFFFFFFFFFFFFFFull;
    pat.d[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x08;
    pat.d[0] = 0x0;
    pat.d[1] = 0x0;
    pat.d[2] = 0x0;
    pat.d[3] = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0   = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    // end WPORT-3 }}}3

  } // end 写入 }}}2

  if(1){ // begin 读Glitch问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.waddr[0] = 0x00;
    pat.waddr[1] = 0x01;
    pat.waddr[2] = 0x02;
    pat.waddr[3] = 0x04;
    pat.d[0] = 0xffffffffffffffffull;
    pat.wen3_0 = 0xf;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.waddr[0] = 0x08;
    pat.waddr[1] = 0x10;
    pat.waddr[2] = 0x20;
    pat.waddr[3] = 0x40;
    pat.d[0] = 0x0000000000000000ull;
    pat.wen3_0 = 0xf;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.ren3_0 = 0xf;
    // Addr+1 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x01;
    pat.raddr[1] = 0x01;
    pat.raddr[2] = 0x01;
    pat.raddr[3] = 0x01;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+2 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x02;
    pat.raddr[1] = 0x02;
    pat.raddr[2] = 0x02;
    pat.raddr[3] = 0x02;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+4 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x04;
    pat.raddr[1] = 0x04;
    pat.raddr[2] = 0x04;
    pat.raddr[3] = 0x04;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+8 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x08;
    pat.raddr[1] = 0x08;
    pat.raddr[2] = 0x08;
    pat.raddr[3] = 0x08;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+16 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.ren3_0 = 0xf;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x10;
    pat.raddr[1] = 0x10;
    pat.raddr[2] = 0x10;
    pat.raddr[3] = 0x10;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+32 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.ren3_0 = 0xf;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x20;
    pat.raddr[1] = 0x20;
    pat.raddr[2] = 0x20;
    pat.raddr[3] = 0x20;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3
    // Addr+64 {{{3
    pat.raddr[0] = 0x00;
    pat.raddr[1] = 0x00;
    pat.raddr[2] = 0x00;
    pat.raddr[3] = 0x00;
    pat.ren3_0 = 0xf;
    index = doTest_rf44(index, pat, gld, rf44);
    pat.raddr[0] = 0x40;
    pat.raddr[1] = 0x40;
    pat.raddr[2] = 0x40;
    pat.raddr[3] = 0x40;
    index = doTest_rf44(index, pat, gld, rf44);
    // }}}3

  } // end 读Glitch问题 }}}2

  if(1){ // begin 读远端RPC关闭过早问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.waddr[0] = 0x7F;
    pat.waddr[1] = 0x00;
    pat.d[0] = 0xffffffffffffffffull;
    pat.wen3_0 = 0x3;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.wen3_0 = 0x0;
    pat.ren3_0 = 0xf;
    pat.raddr[0] = 0x7F;
    pat.raddr[1] = 0x7F;
    pat.raddr[2] = 0x7F;
    pat.raddr[3] = 0x7F;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.raddr[0] = 0x0;
    pat.raddr[1] = 0x0;
    pat.raddr[2] = 0x0;
    pat.raddr[3] = 0x0;
    index = doTest_rf44(index, pat, gld, rf44);

    pat.raddr[0] = 0x7F;
    pat.raddr[1] = 0x7F;
    pat.raddr[2] = 0x7F;
    pat.raddr[3] = 0x7F;
    index = doTest_rf44(index, pat, gld, rf44);

  } // end 读远端RPC关闭过早问题 }}}2

  // Start test
  uint32_t st=0;
  // Testing REGFILE_4W4R_V
  // {{{2
  printf("\n\nStart testing REGFILE_4W4R_V ...\n");
  tb_start(REGFILE_4W4R_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_rf4w4r_in_t  tmp_pat;
    tb_rf4w4r_out_t tmp_gld;
    tb_rf4w4r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf44_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  // Testing REGFILE_4W4R_H
  // {{{2
  printf("\n\nStart testing REGFILE_4W4R_H ...\n");
  tb_start(REGFILE_4W4R_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_rf4w4r_in_t  tmp_pat;
    tb_rf4w4r_out_t tmp_gld;
    tb_rf4w4r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf44_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  return true;
} // }}}1

inline unsigned int doTest_cp35( const int index, tb_cp0q_ram_old_in_t& pat, tb_cp0q_ram_old_out_t& gld, gs_cp0q_ram_64x128_3sw5sr& cp35){
  // {{{1
  if( cp35.operate( !(pat.waddr0&0x40), pat.waddr0&0x3f, pat.wvalue0[0], pat.wvalue0[1], pat.wmask0,
        !(pat.waddr1&0x40), pat.waddr1&0x3f, pat.wvalue1[0], pat.wvalue1[1], pat.wmask1,
        pat.wen2,
        pat.ren&0x01, pat.raddr0&0x3f, pat.ren&0x02, pat.raddr1&0x3f,
        pat.ren&0x04, pat.raddr2&0x3f, pat.ren&0x08, pat.raddr3&0x3f,
        pat.ren&0x10, pat.raddr4&0x3f) ){
    gld.q0 = pat.osel ? cp35.get_out0H() :  cp35.get_out0L();
    gld.q1 = pat.osel ? cp35.get_out1H() :  cp35.get_out1L();
    gld.q2 = pat.osel ? cp35.get_out2H() :  cp35.get_out2L();
    gld.q3 = pat.osel ? cp35.get_out3H() :  cp35.get_out3L();
    gld.q4 = pat.osel ? cp35.get_out4H() :  cp35.get_out4L();
    write_patgld(index, pat, gld);
    return index+1;
  } else exit(-1);
} // }}}1
bool oldPatternTest_3w5r( gs_cp0q_ram_64x128_3sw5sr & cp35 ){
  // {{{1
  tb_cp0q_ram_old_in_t  pat;
  tb_cp0q_ram_old_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  int addr = random()&0x3F;
  if(cp35.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.ren    = 0x1f;
    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.raddr4 = addr;
    pat.raddr3 = addr;
    pat.raddr2 = addr;
    pat.raddr1 = addr;
    pat.raddr0 = addr;
    index = doTest_cp35(index, pat, gld, cp35);
  } else exit(-1); // }}}2

  if(1){ // begin 预充
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.cmp_mask = 0x0000ffffffffffull;
    pat.wmask0 = 0xffff;
    pat.wmask1 = 0xffff;
    uint8_t addr1 = 0x20;
    uint8_t addr2 = 0x3f; // make sure addr0 != addr1
    pat.waddr0 = addr1;
    pat.waddr1 = addr2;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    pat.wvalue1[0] = 0x0000000000000000ull;
    pat.wvalue1[1] = 0x0000000000000000ull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.ren = 0x1f;
    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.raddr0 = addr1;
    pat.raddr1 = addr1;
    pat.raddr2 = addr1;
    pat.raddr3 = addr1;
    pat.raddr4 = addr1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.osel = 1;

    pat.raddr0 = addr2;
    pat.raddr1 = addr2;
    pat.raddr2 = addr2;
    pat.raddr3 = addr2;
    pat.raddr4 = addr2;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.raddr0 = addr1;
    pat.raddr1 = addr1;
    pat.raddr2 = addr1;
    pat.raddr3 = addr1;
    pat.raddr4 = addr1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.raddr0 = addr2;
    pat.raddr1 = addr2;
    pat.raddr2 = addr2;
    pat.raddr3 = addr2;
    pat.raddr4 = addr2;
    index = doTest_cp35(index, pat, gld, cp35);

  } // end 预充 }}}2

  if(1){ // begin 写入（耦合）
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.wmask0 = 0xffff;
    pat.wmask1 = 0xffff;
    pat.cmp_mask = 0x000000ffffffffffull;

    // WPORT-0
    // {{{3
    pat.waddr0 = 0x3f;
    pat.waddr1 = 0x3e;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.wvalue1[0] = 0xffffffffffffffffull;
    pat.wvalue1[1] = 0xffffffffffffffffull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x3f;
    pat.waddr1 = 0x3e;
    pat.ren = 0x0;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    pat.wvalue1[0] = 0x0000000000000000ull;
    pat.wvalue1[1] = 0x0000000000000000ull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x3f;
    pat.waddr1 = 0x3e;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.wvalue1[0] = 0xffffffffffffffffull;
    pat.wvalue1[1] = 0xffffffffffffffffull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // WPORT-1
    // {{{3
    pat.ren = 0x00;
    pat.waddr0 = 0x3e;
    pat.waddr1 = 0x3f;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.wvalue1[0] = 0xffffffffffffffffull;
    pat.wvalue1[1] = 0xffffffffffffffffull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x3e;
    pat.waddr1 = 0x3f;
    pat.ren = 0x0;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    pat.wvalue1[0] = 0x0000000000000000ull;
    pat.wvalue1[1] = 0x0000000000000000ull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x3e;
    pat.waddr1 = 0x3f;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.wvalue1[0] = 0xffffffffffffffffull;
    pat.wvalue1[1] = 0xffffffffffffffffull;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    pat.osel = 0;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 1;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3

  } // end 写入（耦合） }}}2

  if(1){ // begin 读取Glitch
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );

    // 初始设置
    // {{{3
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.ren = 0x0;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wmask0 = 0xffff;
    pat.wmask1 = 0xffff;

    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    pat.waddr0 = 0x00;
    pat.waddr1 = 0x01;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.waddr0 = 0x02;
    pat.waddr1 = 0x04;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x08;
    pat.waddr1 = 0x10;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x20;
    pat.waddr1 = 0x40;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x40;
    // }}}3
    // Addr+1 {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x01;
    pat.raddr1 = 0x01;
    pat.raddr2 = 0x01;
    pat.raddr3 = 0x01;
    pat.raddr4 = 0x01;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x01;
    pat.raddr1 = 0x01;
    pat.raddr2 = 0x01;
    pat.raddr3 = 0x01;
    pat.raddr4 = 0x01;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // Addr+2 {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x02;
    pat.raddr1 = 0x02;
    pat.raddr2 = 0x02;
    pat.raddr3 = 0x02;
    pat.raddr4 = 0x02;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x02;
    pat.raddr1 = 0x02;
    pat.raddr2 = 0x02;
    pat.raddr3 = 0x02;
    pat.raddr4 = 0x02;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // Addr+4  {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x04;
    pat.raddr1 = 0x04;
    pat.raddr2 = 0x04;
    pat.raddr3 = 0x04;
    pat.raddr4 = 0x04;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x04;
    pat.raddr1 = 0x04;
    pat.raddr2 = 0x04;
    pat.raddr3 = 0x04;
    pat.raddr4 = 0x04;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // Addr+8  {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x08;
    pat.raddr1 = 0x08;
    pat.raddr2 = 0x08;
    pat.raddr3 = 0x08;
    pat.raddr4 = 0x08;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x08;
    pat.raddr1 = 0x08;
    pat.raddr2 = 0x08;
    pat.raddr3 = 0x08;
    pat.raddr4 = 0x08;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // Addr+16 {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x10;
    pat.raddr1 = 0x10;
    pat.raddr2 = 0x10;
    pat.raddr3 = 0x10;
    pat.raddr4 = 0x10;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x10;
    pat.raddr1 = 0x10;
    pat.raddr2 = 0x10;
    pat.raddr3 = 0x10;
    pat.raddr4 = 0x10;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3
    // Addr+32  {{{3
    pat.ren = 0x1f;
    pat.osel = 0x0;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x20;
    pat.raddr1 = 0x20;
    pat.raddr2 = 0x20;
    pat.raddr3 = 0x20;
    pat.raddr4 = 0x20;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.osel = 0x1;
    pat.raddr0 = 0x00;
    pat.raddr1 = 0x00;
    pat.raddr2 = 0x00;
    pat.raddr3 = 0x00;
    pat.raddr4 = 0x00;
    index = doTest_cp35(index, pat, gld, cp35);
    pat.raddr0 = 0x20;
    pat.raddr1 = 0x20;
    pat.raddr2 = 0x20;
    pat.raddr3 = 0x20;
    pat.raddr4 = 0x20;
    index = doTest_cp35(index, pat, gld, cp35);
    // }}}3

  } // end 读取Glitch }}}2

  if(1){ // begin 读取远端时序失配
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.wmask0 = 0xffffffff;
    pat.wmask1 = 0xffffffff;
    pat.cmp_mask = 0x000000ffffffffffull;

    pat.waddr0 = 0x3f;
    pat.waddr1 = 0x40;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    index = doTest_cp35(index, pat, gld, cp35);
    
    pat.waddr0 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.osel = 0x1;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.waddr0 = 0x3f;
    pat.waddr1 = 0x40;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    index = doTest_cp35(index, pat, gld, cp35);
    
    pat.waddr0 = 0x40;
    pat.ren = 0x1f;
    pat.raddr0 = 0x3f;
    pat.raddr1 = 0x3f;
    pat.raddr2 = 0x3f;
    pat.raddr3 = 0x3f;
    pat.raddr4 = 0x3f;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.osel = 0x1;
    index = doTest_cp35(index, pat, gld, cp35);

  } // end 读取远端时序失配 }}}2

  if(1){ // begin W2写入失败
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.wmask0 = 0xffff;
    pat.wmask1 = 0xffff;

    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.wen2 = 0xffffffffffffffffull;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.wen2 = 0x0000000000000000ull;
    pat.ren = 0x1f;
    pat.osel = 0x0;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }
    pat.osel = 0x1;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }

    pat.wvalue0[0] = 0xffffffffffffffffull;
    pat.wvalue0[1] = 0xffffffffffffffffull;
    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.wen2 = 0xffffffffffffffffull;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.wen2 = 0x0000000000000000ull;
    pat.ren = 0x1f;
    pat.osel = 0x0;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }
    pat.osel = 0x1;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }

    pat.wvalue0[0] = 0x0000000000000000ull;
    pat.wvalue0[1] = 0x0000000000000000ull;
    pat.waddr0 = 0x40;
    pat.waddr1 = 0x40;
    pat.wen2 = 0xffffffffffffffffull;
    pat.ren = 0x00;
    pat.raddr0 = 0x40;
    pat.raddr1 = 0x40;
    pat.raddr2 = 0x40;
    pat.raddr3 = 0x40;
    pat.raddr4 = 0x40;
    index = doTest_cp35(index, pat, gld, cp35);

    pat.wen2 = 0x0000000000000000ull;
    pat.ren = 0x1f;
    pat.osel = 0x0;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }
    pat.osel = 0x1;
    for(int i=0;i<64;i++){
      pat.raddr0 = i;
      pat.raddr1 = i;
      pat.raddr2 = i;
      pat.raddr3 = i;
      pat.raddr4 = i;
      index = doTest_cp35(index, pat, gld, cp35);
    }

  } // end W2写入失败 }}}2

  // Start test
  uint32_t st=0;
  bool flag=true;
  // Testing CP0Q_RAM_OLD_V
  // {{{2
  printf("\n\nStart testing CP0Q_RAM_OLD_V ...\n");
  tb_start(CP0Q_RAM_OLD_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cp0q_ram_old_in_t  tmp_pat;
    tb_cp0q_ram_old_out_t tmp_gld;
    tb_cp0q_ram_old_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp35_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) flag=false;
  // }}}2

  // Testing CP0Q_RAM_OLD_H
  // {{{2
  printf("\n\nStart testing CP0Q_RAM_OLD_H ...\n");
  tb_start(CP0Q_RAM_OLD_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cp0q_ram_old_in_t  tmp_pat;
    tb_cp0q_ram_old_out_t tmp_gld;
    tb_cp0q_ram_old_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp35_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) flag=false;
  // }}}2

  return flag;
} // }}}1

inline unsigned int doTest_cp25( const int index, tb_cp0q_ram_in_t&  pat, tb_cp0q_ram_out_t& gld, gs_cp0q_ram_48x64_2sw5sr& cp25){
  // {{{1
  uint8_t waddr0=0, waddr1=0, raddr0=0, raddr1=0, raddr2=0, raddr3=0, raddr4=0;
  while(pat.waddr0 && !(pat.waddr0&(0x01ull<<waddr0)) && (waddr0<48) ) waddr0++;
  while(pat.waddr1 && !(pat.waddr1&(0x01ull<<waddr1)) && (waddr1<48) ) waddr1++;
  while(pat.raddr0 && !(pat.raddr0&(0x01ull<<raddr0)) && (raddr0<48) ) raddr0++;
  while(pat.raddr1 && !(pat.raddr1&(0x01ull<<raddr1)) && (raddr1<48) ) raddr1++;
  while(pat.raddr2 && !(pat.raddr2&(0x01ull<<raddr2)) && (raddr2<48) ) raddr2++;
  while(pat.raddr3 && !(pat.raddr3&(0x01ull<<raddr3)) && (raddr3<48) ) raddr3++;
  while(pat.raddr4 && !(pat.raddr4&(0x01ull<<raddr4)) && (raddr4<48) ) raddr4++;
  if( cp25.operate( pat.wen1_0&0x01, waddr0, pat.wvalue0, 
        pat.wen1_0&0x02, waddr1, pat.wvalue1,
        pat.ren4_0&0x01, raddr0, pat.ren4_0&0x02, raddr1,
        pat.ren4_0&0x04, raddr2, pat.ren4_0&0x08, raddr3,
        pat.ren4_0&0x10, raddr4) ){
    gld.q0 = cp25.get_out0();
    gld.q1 = cp25.get_out1();
    gld.q2 = cp25.get_out2();
    gld.q3 = cp25.get_out3();
    gld.q4 = cp25.get_out4();
    write_patgld(index, pat, gld);
    return index+1;
  } else exit(-1);
} // }}}1
bool oldPatternTest_2w5r( gs_cp0q_ram_48x64_2sw5sr & cp25 ){
  // {{{1
  tb_cp0q_ram_in_t  pat;
  tb_cp0q_ram_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  int addr = random() % 48;
  if(cp25.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.ren4_0 = 0x1f;
    pat.raddr4 = 0x1ull<<addr;
    pat.raddr3 = 0x1ull<<addr;
    pat.raddr2 = 0x1ull<<addr;
    pat.raddr1 = 0x1ull<<addr;
    pat.raddr0 = 0x1ull<<addr;
    index = doTest_cp25( index, pat, gld, cp25);
  } else exit(-1); // }}}2

  if(1){ // begin 预充
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.cmp_mask = 0x0000ffffffffffull;
    pat.wen1_0 = 0x03;
    pat.waddr0 = 0x01ull<<0x00;
    pat.waddr1 = 0x01ull<<0x2f;
    pat.wvalue1 = 0xffffffffffffffffull;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.ren4_0 = 0x1f;
    pat.wen1_0 = 0x00;
    pat.raddr0 = 0x01ull<<0x2f;
    pat.raddr1 = 0x01ull<<0x2f;
    pat.raddr2 = 0x01ull<<0x2f;
    pat.raddr3 = 0x01ull<<0x2f;
    pat.raddr4 = 0x01ull<<0x2f;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull<<0x00;
    pat.raddr1 = 0x01ull<<0x00;
    pat.raddr2 = 0x01ull<<0x00;
    pat.raddr3 = 0x01ull<<0x00;
    pat.raddr4 = 0x01ull<<0x00;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x03;
    pat.waddr0 = 0x01ull<<0x2f;
    pat.waddr1 = 0x01ull<<0x00;
    pat.ren4_0 = 0x00;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull<<0x00;
    pat.raddr1 = 0x01ull<<0x00;
    pat.raddr2 = 0x01ull<<0x00;
    pat.raddr3 = 0x01ull<<0x00;
    pat.raddr4 = 0x01ull<<0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull<<0x2f;
    pat.raddr1 = 0x01ull<<0x2f;
    pat.raddr2 = 0x01ull<<0x2f;
    pat.raddr3 = 0x01ull<<0x2f;
    pat.raddr4 = 0x01ull<<0x2f;
    index = doTest_cp25(index, pat, gld, cp25);
  } // end 预充 }}}2

  if(1){ // begin 写入（耦合）
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );
    pat.cmp_mask = 0x000000ffffffffffull;

    pat.waddr0 = 0x01ull<<0x2f;
    pat.waddr1 = 0x01ull<<0x2e;
    pat.wen1_0 = 0x03;
    pat.wvalue0 = 0x0000000000000000ull;
    pat.wvalue1 = 0xffffffffffffffffull;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull<<0x2e;
    pat.raddr1 = 0x01ull<<0x2e;
    pat.raddr2 = 0x01ull<<0x2f;
    pat.raddr3 = 0x01ull<<0x2f;
    pat.raddr4 = 0x01ull<<0x2f;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wvalue0 = 0xffffffffffffffffull;
    pat.wvalue1 = 0x0000000000000000ull;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    pat.ren4_0 = 0x1f;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wvalue0 = 0x0000000000000000ull;
    pat.wvalue1 = 0xffffffffffffffffull;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    pat.ren4_0 = 0x1f;
    index = doTest_cp25(index, pat, gld, cp25);
  } // end 写入（耦合） }}}2

  if(1){ // begin 读取Glitch
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );

    // 初始设置
    // {{{3
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.ren4_0 = 0x00;
    pat.wen1_0 = 0x03;

    pat.wvalue0 = 0xffffffffffffffffull;
    pat.waddr0 = 0x01ull << 0x00;
    pat.waddr1 = 0x01ull << 0x01;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wvalue0 = 0x0000000000000000ull;
    pat.waddr0 = 0x01ull << 0x02;
    pat.waddr1 = 0x01ull << 0x04;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.waddr0 = 0x01ull << 0x08;
    pat.waddr1 = 0x01ull << 0x10;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.waddr0 = 0x01ull << 0x20;
    pat.waddr1 = 0x01ull << 0x2f;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    // }}}3
    // Addr+1 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x01;
    pat.raddr1 = 0x01ull << 0x01;
    pat.raddr2 = 0x01ull << 0x01;
    pat.raddr3 = 0x01ull << 0x01;
    pat.raddr4 = 0x01ull << 0x01;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
    // Addr+2 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x02;
    pat.raddr1 = 0x01ull << 0x02;
    pat.raddr2 = 0x01ull << 0x02;
    pat.raddr3 = 0x01ull << 0x02;
    pat.raddr4 = 0x01ull << 0x02;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
    // Addr+4 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x04;
    pat.raddr1 = 0x01ull << 0x04;
    pat.raddr2 = 0x01ull << 0x04;
    pat.raddr3 = 0x01ull << 0x04;
    pat.raddr4 = 0x01ull << 0x04;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
    // Addr+8 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x08;
    pat.raddr1 = 0x01ull << 0x08;
    pat.raddr2 = 0x01ull << 0x08;
    pat.raddr3 = 0x01ull << 0x08;
    pat.raddr4 = 0x01ull << 0x08;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
    // Addr+16 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x10;
    pat.raddr1 = 0x01ull << 0x10;
    pat.raddr2 = 0x01ull << 0x10;
    pat.raddr3 = 0x01ull << 0x10;
    pat.raddr4 = 0x01ull << 0x10;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
    // Addr+32 {{{3
    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x00;
    pat.raddr1 = 0x01ull << 0x00;
    pat.raddr2 = 0x01ull << 0x00;
    pat.raddr3 = 0x01ull << 0x00;
    pat.raddr4 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);
    pat.raddr0 = 0x01ull << 0x20;
    pat.raddr1 = 0x01ull << 0x20;
    pat.raddr2 = 0x01ull << 0x20;
    pat.raddr3 = 0x01ull << 0x20;
    pat.raddr4 = 0x01ull << 0x20;
    index = doTest_cp25(index, pat, gld, cp25);
    // }}}3
  } // end 读取Glitch }}}2

  if(1){ // begin 读取远端时序失配
    // {{{2
    memset( &pat, 0, sizeof(pat) );
    memset( &gld, 0, sizeof(gld) );

    // 初始设置
    // {{{3
    pat.cmp_mask = 0x000000ffffffffffull;
    pat.ren4_0 = 0x00;
    pat.wen1_0 = 0x03;

    pat.wvalue0 = 0xffffffffffffffffull;
    pat.waddr0 = 0x01ull << 0x2f;
    pat.waddr1 = 0x01ull << 0x00;
    index = doTest_cp25(index, pat, gld, cp25);

    pat.wen1_0 = 0x00;
    // }}}3

    pat.ren4_0 = 0x1f;
    pat.raddr0 = 0x01ull << 0x2f;
    pat.raddr1 = 0x01ull << 0x2f;
    pat.raddr2 = 0x01ull << 0x2f;
    pat.raddr3 = 0x01ull << 0x2f;
    pat.raddr4 = 0x01ull << 0x2f;
    index = doTest_cp25(index, pat, gld, cp25);
    index = doTest_cp25(index, pat, gld, cp25);
    index = doTest_cp25(index, pat, gld, cp25);

  } // end 读取远端时序失配 }}}2

  // Start test
  uint32_t st=0;
  // Testing CP0Q_RAM_V
  // {{{2
  printf("\n\nStart testing CP0Q_RAM_V ...\n");
  tb_start(CP0Q_RAM_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cp0q_ram_in_t  tmp_pat;
    tb_cp0q_ram_out_t tmp_gld;
    tb_cp0q_ram_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp25_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  // Testing CP0Q_RAM_H
  // {{{2
  printf("\n\nStart testing CP0Q_RAM_H ...\n");
  tb_start(CP0Q_RAM_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cp0q_ram_in_t  tmp_pat;
    tb_cp0q_ram_out_t tmp_gld;
    tb_cp0q_ram_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp25_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  return true;
} // }}}1

inline unsigned int doTest_cam464v(const int index, tb_cam64x64_in_t& pat, tb_cam64x64_out_t& gld, gs_cam_464v_64x64_1wrs& cam464v){
  // {{{1
  if( cam464v.operate( pat.sen, pat.svpn, pat.sasid, pat.valid,
        pat.wen, pat.wd, pat.wvpn, pat.mask, pat.wasid, pat.G,
        pat.ren, pat.addr) ){
    gld.match = cam464v.get_match();
    gld.rd    = cam464v.get_out();
    gld.hit   = cam464v.get_hit();
    //cam464v_print_vec( i, pat, gld );
    write_patgld(index, pat, gld);
    return index+1;
  } else exit(-1);
} // }}}1
bool oldPatternTest_cam464v( gs_cam_464v_64x64_1wrs & cam464v ){
  // {{{1
  tb_cam64x64_in_t  pat;
  tb_cam64x64_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  cam464v.operate( /*se*/true, 0,0, /*valid*/0x00ull, false, 0, 0, 0, 0, 0, false, 0);
  index=cam464v_search(index, 0, 0, 0x00ull, cam464v.get_match(), cam464v.get_out(), cam464v.get_hit());
  // }}}2

  if(1){ // begin 预充
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));

    pat.cmp_mask = 0x1ffffull;
    pat.wen = 0x1;
    pat.addr = 0x00;
    pat.wd = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x3f;
    pat.wd = 0x0000000000000000ull;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.wen = 0x0;

    pat.ren = 0x1;
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x3f;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.ren = 0x0;

    pat.wen = 0x1;
    pat.addr = 0x3f;
    pat.wd = 0xFFFFFFFFFFFFFFFFull;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x00;
    pat.wd = 0x0000000000000000ull;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.wen = 0x0;

    pat.ren = 0x1;
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x3f;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.ren = 0x0;
  } // end 预充  }}}2

  if(1){ // begin 写入（couple + farest entry)
    // {{{2
    // No other WPORT to couple.
  } // end 写入 }}}2

  if(1){ // begin 读Glitch问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x000000000001ffffull;
    pat.wen = 0x1;

    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);

    pat.wd = 0xffffffffffffffffull;
    pat.addr = 0x01;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x02;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x04;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x08;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x10;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x20;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.wen = 0x0;

    pat.ren = 0x1;
    // Addr+1 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x01;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3
    // Addr+2 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x02;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3
    // Addr+4 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x04;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3
    // Addr+8 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x08;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3
    // Addr+16 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x10;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3
    // Addr+32 {{{3
    pat.addr = 0x00;
    index = doTest_cam464v(index, pat, gld, cam464v);
    pat.addr = 0x20;
    index = doTest_cam464v(index, pat, gld, cam464v); // }}}3

  } // end 读Glitch问题 }}}2

  if(1){ // begin 读远端RPC关闭过早问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x000000000001ffffull;
    pat.addr = 0x3F;
    pat.wen = 0x1;
    index = doTest_cam464v(index, pat, gld, cam464v);

    pat.wen = 0x0;
    pat.ren = 0x1;
    index = doTest_cam464v(index, pat, gld, cam464v);
    index = doTest_cam464v(index, pat, gld, cam464v);
    index = doTest_cam464v(index, pat, gld, cam464v);

    pat.ren = 0x0;
    pat.sen = 0x1;
    pat.valid = 0x01ull << 63;
    index = doTest_cam464v(index, pat, gld, cam464v);
    index = doTest_cam464v(index, pat, gld, cam464v);
    index = doTest_cam464v(index, pat, gld, cam464v);

  } // end 读远端RPC关闭过早问题 }}}2

  // Start test
  uint32_t st=0;
  // Testing CAM_64X64_V
  // {{{2
  printf("\n\nStart testing CAM_64X64_V ...\n");
  tb_start(CAM_64X64_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cam64x64_in_t  tmp_pat;
    tb_cam64x64_out_t tmp_gld;
    tb_cam64x64_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cam464v_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  // Testing CAM_64X64_H
  // {{{2
  printf("\n\nStart testing CAM_64X64_H ...\n");
  tb_start(CAM_64X64_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_cam64x64_in_t  tmp_pat;
    tb_cam64x64_out_t tmp_gld;
    tb_cam64x64_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cam464v_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  return true;
} // }}}1

inline unsigned int doTest_cambtb( const int index, tb_btbcam_in_t& pat, tb_btbcam_out_t& gld, gs_cam_btb_30x96_1w1s& cambtb){
  // {{{1
  if( cambtb.operate( pat.sen, pat.svpn, pat.valid[0], pat.valid[1], pat.valid[2],
        pat.wen, pat.addr[0], pat.addr[1], pat.addr[2], pat.wd, pat.wvpn) ){
    gld.match[0] = cambtb.get_match31_00();
    gld.match[1] = cambtb.get_match63_32();
    gld.match[2] = cambtb.get_match95_64();
    uint64_t rd  = cambtb.get_out();
    gld.rd[0]    = rd&0xffffffff; 
    gld.rd[1]    = (rd>>32)&0x3fff; 
    gld.hit   = cambtb.get_hit();
    //btbcam_print_vec( i, pat, gld );
    write_patgld(index, pat, gld);
    return index+1;
  } else exit(-1);
} // }}}1
bool oldPatternTest_cambtb( gs_cam_btb_30x96_1w1s & cambtb ){
  // {{{1
  tb_btbcam_in_t  pat;
  tb_btbcam_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int index=0;
  // Read all port at first cycle
  // {{{2
  cambtb.operate( /*se*/true, 0, /*valid*/0x00,0x00,0x00, false, 0, 0, 0, 0, 0);
  index=btbcam_search(index, 0, 0x00,0x00,0x00, cambtb.get_match31_00(), cambtb.get_match63_32(), cambtb.get_match95_64(), cambtb.get_out(), cambtb.get_hit()); // }}}2

  if(1){ // begin 预充
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));

    pat.cmp_mask = 0x1fffffull;
    pat.wen = 0x1;
    pat.addr[0] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x00;
    pat.addr[2] = 0x80000000;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.wen = 0x0;
    pat.sen = 0x1;
    pat.valid[0] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x00;
    pat.valid[2] = 0x80000000;
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);
  } // end 预充  }}}2

  if(1){ // begin 写入（couple + farest entry)
    // {{{2
    // No other WPORT to couple.
  } // end 写入 }}}2

  if(1){ // begin 读Glitch问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000001fffffull;
    pat.sen = 0x0;
    pat.wen = 0x1;

    pat.addr[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.wd = 0x00003fffffffffffull;
    pat.addr[0] = 0x01 << 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x01 << 0x02;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x01 << 0x04;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x01 << 0x08;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x01 << 0x10;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[0] = 0x00;
    pat.addr[1] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[1] = 0x00;
    pat.addr[2] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.addr[2] = 0x00;
    pat.wen = 0x0;

    pat.sen = 0x1;
    pat.valid[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x01 << 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x01 << 0x02;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x01 << 0x04;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x01 << 0x08;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x01 << 0x00;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x01 << 0x10;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.valid[0] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x00;
    pat.valid[1] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[1] = 0x00;

    pat.valid[0] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
    pat.valid[0] = 0x00;
    pat.valid[2] = 0x01;
    index = doTest_cambtb(index, pat, gld, cambtb);
  } // end 读Glitch问题 }}}2

  if(1){ // begin 读远端RPC关闭过早问题
    // {{{2
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000001fffffull;
    pat.addr[0] = 0x1;
    pat.wen = 0x1;
    index = doTest_cambtb(index, pat, gld, cambtb);

    pat.wen = 0x0;
    pat.sen = 0x1;
    pat.valid[0] = 0x1;
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);
    index = doTest_cambtb(index, pat, gld, cambtb);

  } // end 读远端RPC关闭过早问题 }}}2

  // Start test
  uint32_t st=0;
  // Testing BTBCAM_1W1S_V
  // {{{2
  printf("\n\nStart testing BTBCAM_1W1S_V ...\n");
  tb_start(BTBCAM_1W1S_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_btbcam_in_t  tmp_pat;
    tb_btbcam_out_t tmp_gld;
    tb_btbcam_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      btbcam_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  // Testing BTBCAM_1W1S_H
  // {{{2
  printf("\n\nStart testing BTBCAM_1W1S_H ...\n");
  tb_start(BTBCAM_1W1S_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", index);
    tb_btbcam_in_t  tmp_pat;
    tb_btbcam_out_t tmp_gld;
    tb_btbcam_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<index; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      btbcam_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<index; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;
  // }}}2

  return true;
} // }}}1

