#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#define CHAIN_LENGTH 426661
#define RF_OFFSET 146297
#define RF_OFFSET1 370580
#define RF_NUM 8
#define RF_SIZE 64
//#define RF_SIZE 8
#define TB_REG_BASE    0x00e30000
#define TB_PATRAM_BASE 0x00f00000
#define TB_GLDRAM_BASE 0x00f20000
#define TB_OUTBUF_BASE 0x00f30000
#define TB_REG(n) (TB_REG_BASE + 4 * n)
#define TB_PAT(n) (TB_PATRAM_BASE + 128 * n)
#define TB_GLD(n) (TB_GLDRAM_BASE +  64 * n)
#define TB_OUT(n) (TB_OUTBUF_BASE +  64 * n)
#define size4(t) ((sizeof(t)+3)/4)

#define CLKCFG 0x00e20000

#define REGFILE_4W2R    0
#define CAM_64X64_V     1
#define CAM_64X64_H     2

#define CP0Q_RAM_V      4
#define BTBCAM_1W1S_V   5
#define CP0Q_RAM_H      6
#define BTBCAM_1W1S_H   7
#define CP0Q_RAM_OLD_V  8
#define REGFILE_4W4R_V  9
#define REGFILE_8W6R_V  10

#define CP0Q_RAM_OLD_H  12
#define REGFILE_8W6R_H  13
#define REGFILE_4W4R_H  14
#define MUT_TEST        15


int  mem_open(void);
int  mmap_init(void);
void mem_free_all(void);

#endif
