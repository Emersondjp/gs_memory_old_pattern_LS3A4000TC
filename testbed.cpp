//
// INCLUDES
//
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "testbed.hpp"

void sys_Init(void)
{
#ifndef NO_HARDWARE
  // First we need to init the parallel port
  if (mem_open()<0) exit(1);
  if (mmap_init()<0) exit(1);

  spi_init();
#endif 
  return;
}

void tb_start(int target, int ncyc, int pat_base, int gld_base, uint32_t mut_cfg)
{
#ifndef NO_HARDWARE
  if( ncyc <= 0 ){
    printf("*WARNING* : test cycle <= 0, retuned without testing.\n");
    return;
  }
  testbed_regs_t regs;
  regs.testcfg.b.tid    = target;
  regs.testcfg.b.ncyc   = ncyc;
  regs.patbase.b.pat_base = pat_base;
  regs.patbase.b.gld_base = gld_base;
  axi_write4(TB_REG(0), regs.testcfg.d32);
  axi_write4(TB_REG(3), regs.patbase.d32);
  axi_write4(TB_REG(4), mut_cfg); // start
  axi_write4(TB_REG(2), 1); // start
#endif
}

void tb_start(int target, int ncyc, uint32_t mut_cfg)
{
  tb_start(target, ncyc, 0, 0, mut_cfg);
}

bool tb_clear()
{
#ifndef NO_HARDWARE
  testbed_regs_t regs;
  regs.teststs.d32 = axi_read4(TB_REG(1));
  regs.testctl.d32 = axi_read4(TB_REG(2));
  //printf("\nerr %d, ecyc %d, ecnt %d\n\n", regs.teststs.b.err, regs.teststs.b.ecyc-2, regs.teststs.b.ecnt);
  bool err = regs.teststs.b.err;
  int  ecyc = regs.teststs.b.ecyc;
  printf("**RESULT** err %d, ecyc %d, ecnt %d.\n", regs.teststs.b.err, regs.teststs.b.ecyc, regs.teststs.b.ecnt);
  axi_write4(TB_REG(2), 0); // clear

  // if Error, print pat/gld/out buffer.
  if(err){
    printf("\n");
    tb_rf8w6r_in_t tmp_pat;
    tb_rf8w6r_out_t    out;
    for (int i=0; i<=ecyc; i++) {
      read_pattern(i, tmp_pat);
      printf("patbuf[%4d]: ", i);
      dump_buffer(tmp_pat);
    }
    printf("\n");

    for (int i=0; i<=ecyc; i++) {
      read_golden(i, out);
      printf("gldbuf[%4d]: ", i);
      dump_buffer(out);
    }
    printf("\n");

    for (int i=0; i<=ecyc; i++) {
      read_outbuf(i, out);
      printf("outbuf[%4d]: ", i);
      dump_buffer(out);
    }
    printf("\n");
  }

  return !err;
#endif
}

bool mytb_clear(int startIndex)
{
#ifndef NO_HARDWARE
  testbed_regs_t regs;
  regs.teststs.d32 = axi_read4(TB_REG(1));
  regs.testctl.d32 = axi_read4(TB_REG(2));
  bool err = regs.teststs.b.err;
  int  ecyc = regs.teststs.b.ecyc;
  printf("**RESULT** err %d, ecyc %d, ecnt %d.\n", regs.teststs.b.err, startIndex+regs.teststs.b.ecyc, regs.teststs.b.ecnt);
  //printf("start=%d\n", regs.testctl.b.start);
  axi_write4(TB_REG(2), 0); // start

  // if Error, print pat/gld/out buffer.
  if(err){
    printf("\n");
    tb_rf8w6r_in_t tmp_pat;
    tb_rf8w6r_out_t    out;
    for (int i=0; i<=ecyc; i++) {
      read_pattern(i, tmp_pat);
      printf("patbuf[%4d]: ", i);
      dump_buffer(tmp_pat);
    }
    printf("\n");

    for (int i=0; i<=ecyc; i++) {
      read_golden(i, out);
      printf("gldbuf[%4d]: ", i);
      dump_buffer(out);
    }
    printf("\n");

    for (int i=0; i<=ecyc; i++) {
      read_outbuf(i, out);
      printf("outbuf[%4d]: ", i);
      dump_buffer(out);
    }
    printf("\n");
  }
  return !err;
#endif
}

void clock_config()
{
#ifndef NO_HARDWARE
  clkcfg_reg_t clkcfg;
  clkcfg.d32 = axi_read4(CLKCFG);

  if (clkcfg.b.eno == 1) {
    // switch to ref clk before power down pll
    //return;
    clkcfg.b.eno = 0;
    axi_write4(CLKCFG, clkcfg.d32);// write config, power down
    clkcfg.d32 = axi_read4(CLKCFG);// delay
    clkcfg.d32 = axi_read4(CLKCFG);// delay
    clkcfg.d32 = axi_read4(CLKCFG);// delay
  }
  clkcfg.d32 = 0;
  clkcfg.b.pdn = 0;
  clkcfg.b.eno = 0;
  clkcfg.b.bypass = 0;
  clkcfg.b.sel = 0;  // 0: gspll
  clkcfg.b.idf = 0;
  clkcfg.b.ldf = 60; // 33MHz ref clk
  clkcfg.b.odf = 1;  // 0: 1, 1: 2, 2: 4
  clkcfg.b.cpudiv = 2; // 2: div4, 1: div2, 0: div1
  clkcfg.b.busdiv = 2;
  clkcfg.b.tbdiv  = 1;
  printf("cfg and power down, CLKCFG = 0x%08x\n", clkcfg.d32);
  axi_write4(CLKCFG, clkcfg.d32);// write config, power down
  clkcfg.d32 = axi_read4(CLKCFG);// delay
  clkcfg.d32 = axi_read4(CLKCFG);// delay
  clkcfg.d32 = axi_read4(CLKCFG);// delay
  clkcfg.d32 = axi_read4(CLKCFG);// delay
  clkcfg.d32 = axi_read4(CLKCFG);// delay
  clkcfg.b.pdn = 1;
  printf("power up, CLKCFG = 0x%08x\n", clkcfg.d32);
  axi_write4(CLKCFG, clkcfg.d32);// write config, power up
  clkcfg.d32 = axi_read4(CLKCFG);
  for (int i=0; i<100; i++) {
    clkcfg.d32 = axi_read4(CLKCFG);// wait lock
  }
  printf("CLKCFG = 0x%08x\n", clkcfg.d32);
  while (!clkcfg.b.lock) {
    printf("Waiting lock...\n");
    clkcfg.d32 = axi_read4(CLKCFG);// wait lock
  }
  printf("locked\n");
  clkcfg.b.eno = 1;
  axi_write4(CLKCFG, clkcfg.d32);// write config, power up
  printf("switch to pll\n");
#endif
}

