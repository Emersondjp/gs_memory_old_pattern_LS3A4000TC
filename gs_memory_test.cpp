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

#define ONCE_TEST_MAX 100 // Need < 1024

int pat_cnt=0;
int gld_cnt=0;

template <class T1, class T2> bool write_patgld( const int index, const T1& pat, const T2& gld ){
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
}

bool randomTest_8w6r( gs_regfile_128x64_8sw6sr & rf86 ){
  tb_rf8w6r_in_t  pat, tmp_pat;
  tb_rf8w6r_out_t gld, tmp_gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));
  memset(&tmp_pat, 0, sizeof(tmp_pat));
  memset(&tmp_gld, 0, sizeof(tmp_gld));

  int i=0;
  // Read all port at first cycle
  int addr = random()&0x7F;
  if(rf86.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00ffffffffffffull;
    pat.ren5_0 = 0x3f;
    pat.raddr[5] = addr;
    pat.raddr[4] = addr;
    pat.raddr[3] = addr;
    pat.raddr[2] = addr;
    pat.raddr[1] = addr;
    pat.raddr[0] = addr;
    gld.q[0] = rf86.get_out0();
    gld.q[1] = rf86.get_out1();
    gld.q[2] = rf86.get_out2();
    gld.q[3] = rf86.get_out3();
    gld.q[4] = rf86.get_out4();
    gld.q[5] = rf86.get_out5();
    //write_pattern(i, pat);
    //write_golden( i, gld);
    write_patgld(i, pat, gld);
    i++;
  } else exit(-1);

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00ffffffffffffull;
    pat.wen7_0 = random()&0xff;
    pat.ren5_0 = random()&0x3f;
    pat.waddr[7] = random()&0x7f;
    pat.waddr[6] = random()&0x7f;
    pat.waddr[5] = random()&0x7f;
    pat.waddr[4] = random()&0x7f;
    pat.waddr[3] = random()&0x7f;
    pat.waddr[2] = random()&0x7f;
    pat.waddr[1] = random()&0x7f;
    pat.waddr[0] = random()&0x7f;
    pat.raddr[5] = random()&0x7f;
    pat.raddr[4] = random()&0x7f;
    pat.raddr[3] = random()&0x7f;
    pat.raddr[2] = random()&0x7f;
    pat.raddr[1] = random()&0x7f;
    pat.raddr[0] = random()&0x7f;
    pat.d[7] = random() + (0x100000000ull*random());
    pat.d[6] = random() + (0x100000000ull*random());
    pat.d[5] = random() + (0x100000000ull*random());
    pat.d[4] = random() + (0x100000000ull*random());
    pat.d[3] = random() + (0x100000000ull*random());
    pat.d[2] = random() + (0x100000000ull*random());
    pat.d[1] = random() + (0x100000000ull*random());
    pat.d[0] = random() + (0x100000000ull*random());
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

      //printf("Output vec program produced...\n");
      //rf86_print_vec2( i, pat, gld );
      //printf("End output...\n");

      //write_pattern(i, pat);
      //write_golden( i, gld);
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing REGFILE_8W6R_V
  printf("\n\nStart testing REGFILE_8W6R_V ...\n");
  tb_start(REGFILE_8W6R_V, ONCE_TEST_MAX-1, 0xf0000000);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_rf8w6r_in_t  tmp_pat;
    tb_rf8w6r_out_t tmp_gld;
    tb_rf8w6r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf86_print_vec2( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing REGFILE_8W6R_H
  printf("\n\nStart testing REGFILE_8W6R_H ...\n");
  tb_start(REGFILE_8W6R_H, ONCE_TEST_MAX-1, 0xf0000000);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_rf8w6r_in_t  tmp_pat;
    tb_rf8w6r_out_t tmp_gld;
    tb_rf8w6r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf86_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

int randomTest_4w4r( gs_regfile_128x64_4sw4sr & rf44 ){

  tb_rf4w4r_in_t  pat;
  tb_rf4w4r_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int i=0;
  // Read all port at first cycle
  int addr = random()&0x7F;
  if(rf44.read(addr)){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.ren3_0 = 0x0f;
    pat.raddr[3] = addr;
    pat.raddr[2] = addr;
    pat.raddr[1] = addr;
    pat.raddr[0] = addr;
    gld.q[0] = rf44.get_out0();
    gld.q[1] = rf44.get_out1();
    gld.q[2] = rf44.get_out2();
    gld.q[3] = rf44.get_out3();
    //write_pattern(i, pat);
    //write_golden( i, gld);
    write_patgld(i, pat, gld);
    i++;
  } else exit(-1);

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x00000000ffffffffull;
    pat.wen3_0 = random()&0x0f;
    pat.ren3_0 = random()&0x0f;
    pat.waddr[3] = random()&0x7f;
    pat.waddr[2] = random()&0x7f;
    pat.waddr[1] = random()&0x7f;
    pat.waddr[0] = random()&0x7f;
    pat.raddr[3] = random()&0x7f;
    pat.raddr[2] = random()&0x7f;
    pat.raddr[1] = random()&0x7f;
    pat.raddr[0] = random()&0x7f;
    pat.d[3] = random() + (0x100000000ull*random());
    pat.d[2] = random() + (0x100000000ull*random());
    pat.d[1] = random() + (0x100000000ull*random());
    pat.d[0] = random() + (0x100000000ull*random());
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
      //write_pattern(i, pat);
      //write_golden( i, gld);
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing REGFILE_4W4R_V
  printf("\n\nStart testing REGFILE_4W4R_V ...\n");
  tb_start(REGFILE_4W4R_V, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_rf4w4r_in_t  tmp_pat;
    tb_rf4w4r_out_t tmp_gld;
    tb_rf4w4r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf44_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing REGFILE_4W4R_H
  printf("\n\nStart testing REGFILE_4W4R_H ...\n");
  tb_start(REGFILE_4W4R_H, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_rf4w4r_in_t  tmp_pat;
    tb_rf4w4r_out_t tmp_gld;
    tb_rf4w4r_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      rf44_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

bool randomTest_3w5r( gs_cp0q_ram_64x128_3sw5sr & cp35 ){

  tb_cp0q_ram_old_in_t  pat;
  tb_cp0q_ram_old_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int i=0;
  // Read all port at first cycle
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
    gld.q0 = cp35.get_out0L();
    gld.q1 = cp35.get_out1L();
    gld.q2 = cp35.get_out2L();
    gld.q3 = cp35.get_out3L();
    gld.q4 = cp35.get_out4L();
    //write_pattern(i, pat);
    //write_golden( i, gld);
    write_patgld(i, pat, gld);
    i++;
  } else exit(-1);

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x0000ffffffffffull;
    pat.wvalue0[0] = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.wvalue0[1] = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.wvalue1[0] = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.wvalue1[1] = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.waddr0 = random()&0x7f;
    pat.waddr1 = random()&0x7f;
    pat.raddr0 = random()&0x3f;
    pat.raddr1 = random()&0x3f;
    pat.raddr2 = random()&0x3f;
    pat.raddr3 = random()&0x3f;
    pat.raddr4 = random()&0x3f;
    pat.ren    = random()&0x1f;
    pat.wmask0 = random()&0xfffful;
    pat.wmask1 = random()&0xfffful;
    pat.osel   = random()&0x1;
    pat.wen2   = (((uint64_t)random())<<32) + (uint64_t)random();

    if( cp35.operate( !(pat.waddr0&0x40), pat.waddr0&0x3f, pat.wvalue0[0], pat.wvalue0[1], pat.wmask0,
          !(pat.waddr1&0x40), pat.waddr1&0x3f, pat.wvalue1[0], pat.wvalue1[1], pat.wmask1,
          pat.wen2,
          pat.ren&0x01, pat.raddr0, pat.ren&0x02, pat.raddr1,
          pat.ren&0x04, pat.raddr2, pat.ren&0x08, pat.raddr3,
          pat.ren&0x10, pat.raddr4) ){
      gld.q0 = pat.osel ? cp35.get_out0H() :  cp35.get_out0L();
      gld.q1 = pat.osel ? cp35.get_out1H() :  cp35.get_out1L();
      gld.q2 = pat.osel ? cp35.get_out2H() :  cp35.get_out2L();
      gld.q3 = pat.osel ? cp35.get_out3H() :  cp35.get_out3L();
      gld.q4 = pat.osel ? cp35.get_out4H() :  cp35.get_out4L();
      //write_pattern(i, pat);
      //write_golden( i, gld);
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing CP0Q_RAM_OLD_V
  printf("\n\nStart testing CP0Q_RAM_OLD_V ...\n");
  tb_start(CP0Q_RAM_OLD_V, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cp0q_ram_old_in_t  tmp_pat;
    tb_cp0q_ram_old_out_t tmp_gld;
    tb_cp0q_ram_old_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp35_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing CP0Q_RAM_OLD_H
  printf("\n\nStart testing CP0Q_RAM_OLD_H ...\n");
  tb_start(CP0Q_RAM_OLD_H, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cp0q_ram_old_in_t  tmp_pat;
    tb_cp0q_ram_old_out_t tmp_gld;
    tb_cp0q_ram_old_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp35_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

bool randomTest_2w5r( gs_cp0q_ram_48x64_2sw5sr & cp25 ){

  tb_cp0q_ram_in_t  pat;
  tb_cp0q_ram_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int i=0;
  // Read all port at first cycle
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
    gld.q0 = cp25.get_out0();
    gld.q1 = cp25.get_out1();
    gld.q2 = cp25.get_out2();
    gld.q3 = cp25.get_out3();
    gld.q4 = cp25.get_out4();
    //write_pattern(i, pat);
    //write_golden( i, gld);
    write_patgld(i, pat, gld);
    i++;
  } else exit(-1);

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x0000ffffffffffull;
    pat.wvalue0 = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.wvalue1 = (((uint64_t)random())<<32) + (uint64_t)random();
    uint8_t waddr0 = random() % 48;
    uint8_t waddr1 = random() % 48;
    uint8_t raddr0 = random() % 48;
    uint8_t raddr1 = random() % 48;
    uint8_t raddr2 = random() % 48;
    uint8_t raddr3 = random() % 48;
    uint8_t raddr4 = random() % 48;
    pat.waddr0 = 0x01ull << waddr0;
    pat.waddr1 = 0x01ull << waddr1;
    pat.raddr0 = 0x01ull << raddr0;
    pat.raddr1 = 0x01ull << raddr1;
    pat.raddr2 = 0x01ull << raddr2;
    pat.raddr3 = 0x01ull << raddr3;
    pat.raddr4 = 0x01ull << raddr4;
    pat.ren4_0 = random()&0x1f;
    pat.wen1_0 = random()&0x03;

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
      //write_pattern(i, pat);
      //write_golden( i, gld);
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing CP0Q_RAM_V
  printf("\n\nStart testing CP0Q_RAM_V ...\n");
  tb_start(CP0Q_RAM_V, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cp0q_ram_in_t  tmp_pat;
    tb_cp0q_ram_out_t tmp_gld;
    tb_cp0q_ram_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp25_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing CP0Q_RAM_H
  printf("\n\nStart testing CP0Q_RAM_H ...\n");
  tb_start(CP0Q_RAM_H, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cp0q_ram_in_t  tmp_pat;
    tb_cp0q_ram_out_t tmp_gld;
    tb_cp0q_ram_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cp25_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

bool randomTest_cam464v( gs_cam_464v_64x64_1wrs & cam464v ){

  tb_cam64x64_in_t  pat;
  tb_cam64x64_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int i=0;
  // Read all port at first cycle
  cam464v.operate( /*se*/true, 0,0, /*valid*/0x00ull, false, 0, 0, 0, 0, 0, false, 0);
  i=cam464v_search(i, 0, 0, 0x00ull, cam464v.get_match(), cam464v.get_out(), cam464v.get_hit());

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x1ffffull;
    pat.valid = (((uint64_t)(random()|random()))<<32) + (uint64_t)(random()|random());
    pat.wd    = (((uint64_t)random())<<32) + (uint64_t)random();
    pat.wvpn  = (((uint64_t)random()&0xffff)<<32) + (uint64_t)random();
    pat.wasid = random()&0x3ff;
    pat.wen   = random()&0x1;
    pat.ren   = random()&0x1;
    pat.sen   = random()&0x1;
    pat.addr  = random()&0x3f;
    pat.mask  = random()&0x3ffff;
    pat.G     = (random()&0xf) < 0x4 ? 0x1 : 0x0;

    // determin sasid & svpn
    uint8_t tmp_addr = random()&0x3f;
    if( (random()&0xf) < 0xa ){
      pat.sasid = cam464v.get_asid( tmp_addr );
    } else {
      pat.sasid = random()&0x3ff;
    }

    uint32_t tmp_mask   = random()&0x3ffff;
    uint64_t inner_svpn = cam464v.get_vpn(tmp_addr);
    if( (random()&0xff) < 0xf8 ){
      unsigned int tmp = random()&0xf ;
      if( tmp < 6 ){
        pat.svpn = inner_svpn ;
      } else if( tmp < 14 ) {
        pat.svpn  = inner_svpn ^ (pat.mask & tmp_mask);
      } else if( tmp < 15 ) {
        pat.svpn  = inner_svpn ^ (pat.mask | tmp_mask);
      } else {
        uint64_t t = (((uint64_t)random()&0xffff)<<32) + (uint64_t)random();
        pat.svpn = inner_svpn ^ (t);
      }
    } else {
      pat.svpn  = (((uint64_t)random()&0xffff)<<32) + (uint64_t)random();
    }

    if( cam464v.operate( pat.sen, pat.svpn, pat.sasid, pat.valid,
          pat.wen, pat.wd, pat.wvpn, pat.mask, pat.wasid, pat.G,
          pat.ren, pat.addr) ){
      gld.match = cam464v.get_match();
      gld.rd    = cam464v.get_out();
      gld.hit   = cam464v.get_hit();
      //write_pattern(i, pat);
      //write_golden( i, gld);
      //cam464v_print_vec( i, pat, gld );
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing CAM_64X64_V
  printf("\n\nStart testing CAM_64X64_V ...\n");
  tb_start(CAM_64X64_V, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cam64x64_in_t  tmp_pat;
    tb_cam64x64_out_t tmp_gld;
    tb_cam64x64_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cam464v_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing CAM_64X64_H
  printf("\n\nStart testing CAM_64X64_H ...\n");
  tb_start(CAM_64X64_H, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_cam64x64_in_t  tmp_pat;
    tb_cam64x64_out_t tmp_gld;
    tb_cam64x64_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      cam464v_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

bool randomTest_cambtb( gs_cam_btb_30x96_1w1s & cambtb ){

  tb_btbcam_in_t  pat;
  tb_btbcam_out_t gld;
  memset(&pat, 0, sizeof(pat));
  memset(&gld, 0, sizeof(gld));

  int i=0;
  // Read all port at first cycle
  cambtb.operate( /*se*/true, 0, /*valid*/0x00,0x00,0x00, false, 0, 0, 0, 0, 0);
  i=btbcam_search(i, 0, 0x00,0x00,0x00, cambtb.get_match31_00(), cambtb.get_match63_32(), cambtb.get_match95_64(), cambtb.get_out(), cambtb.get_hit());

  while( i < ONCE_TEST_MAX ){
    memset(&pat, 0, sizeof(pat));
    memset(&gld, 0, sizeof(gld));
    pat.cmp_mask = 0x1fffffull;
    pat.sen   = random()&0x1;
    pat.valid[0] = random();
    pat.valid[1] = random();
    pat.valid[2] = random();
    int tmp = random() % 96;
    pat.addr[0] = (tmp<32)&&(tmp>= 0) ? 0x1<<(tmp- 0) : 0x00;
    pat.addr[1] = (tmp<64)&&(tmp>=32) ? 0x1<<(tmp-32) : 0x00;
    pat.addr[2] = (tmp<96)&&(tmp>=64) ? 0x1<<(tmp-64) : 0x00;
    pat.wd    = (((uint64_t)random()&0x3ffff)<<32) + (uint64_t)random();
    pat.wen   = random()&0x1;
    pat.wvpn  = random() & 0x3fffffff;
    if(random() % 7 == 0){
      pat.svpn = random() & 0x3fffffff;
    } else {
      pat.svpn = cambtb.get_svpn( random()%96 );
    }

    if( cambtb.operate( pat.sen, pat.svpn, pat.valid[0], pat.valid[1], pat.valid[2],
          pat.wen, pat.addr[0], pat.addr[1], pat.addr[2], pat.wd, pat.wvpn) ){
      gld.match[0] = cambtb.get_match31_00();
      gld.match[1] = cambtb.get_match63_32();
      gld.match[2] = cambtb.get_match95_64();
      uint64_t rd  = cambtb.get_out();
      gld.rd[0]    = rd&0xffffffff; 
      gld.rd[1]    = (rd>>32)&0x3fff; 
      gld.hit   = cambtb.get_hit();
      //write_pattern(i, pat);
      //write_golden( i, gld);
      //btbcam_print_vec( i, pat, gld );
      write_patgld(i, pat, gld);
      i++;
    }
  };

  // Start test
  uint32_t st=0;
  // Testing BTBCAM_1W1S_V
  printf("\n\nStart testing BTBCAM_1W1S_V ...\n");
  tb_start(BTBCAM_1W1S_V, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_btbcam_in_t  tmp_pat;
    tb_btbcam_out_t tmp_gld;
    tb_btbcam_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      btbcam_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  // Testing BTBCAM_1W1S_H
  printf("\n\nStart testing BTBCAM_1W1S_H ...\n");
  tb_start(BTBCAM_1W1S_H, ONCE_TEST_MAX-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(st&0x2){
#ifdef PRINT_DETAIL
    printf("\nAll recent %d operations :\n", ONCE_TEST_MAX);
    tb_btbcam_in_t  tmp_pat;
    tb_btbcam_out_t tmp_gld;
    tb_btbcam_out_t out;
    memset( &tmp_pat, 0, sizeof(tmp_pat) );
    memset( &tmp_gld, 0, sizeof(tmp_gld) );
    for( int k=0; k<ONCE_TEST_MAX; k++ ){
      read_pattern(k, tmp_pat);
      read_golden( k, tmp_gld);
      btbcam_print_vec( k, tmp_pat, tmp_gld );
    }
    printf("\n\n");
    for (int i=0; i<ONCE_TEST_MAX; i++) {
      read_outbuf(i, out);
      printf("outbuf[%d]: ", i);
      dump_buffer(out);
    }
    printf("\n\n");
  }
#endif
  printf("Done, test %s\n", st&0x02 ? "FAILED":"PASSED");
  if(!tb_clear()) return false;

  return true;
}

