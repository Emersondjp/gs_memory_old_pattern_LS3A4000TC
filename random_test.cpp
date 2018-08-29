//
// INCLUDES
//
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "global.h"
#include "types.h"
#include "spi.h"
#include "testbed.hpp"
#include "gs_memory_test.hpp"
#include "gs_memory_class.hpp"
#include "rf86_function.hpp"
#include "rf44_function.hpp"
#include "cp35_function.hpp"
#include "cp25_function.hpp"
#include "cam464v_function.hpp"
#include "btbcam_function.hpp"

#define TEST_TIMES 500

extern unsigned int str2num(unsigned char *s);
extern int pat_cnt;
extern int gld_cnt;

bool gs_memory_initial(
    gs_regfile_128x64_8sw6sr& rf86,
    gs_regfile_128x64_4sw4sr& rf44,
    gs_cp0q_ram_64x128_3sw5sr& cp35,
    gs_cp0q_ram_48x64_2sw5sr& cp25,
    gs_cam_464v_64x64_1wrs& cam464v,
    gs_cam_btb_30x96_1w1s& cambtb){
  // Initial all gs_memory

  printf("In Initial Stage...\n");
  // Begin initialization of virtual memory
  rf86.memset(0x00ull);
  rf44.memset(0x00ull);
  cp35.memset(0xffff, 0x00ull, 0x00ull);
  cp25.memset(0x00);
  cam464v.memset(0x00, 0x00, 0x00, false, 0x00);
  cambtb.memset(0x00, 0x00);
  // End initialization of virtual memory

  // Begin initialization of testchip
  printf("  REGFILE_8W6R Initial Stage...\n");
  int index=0;
  uint32_t st=0;
  index = rf86_memset(index, 0, 0x00);
  index = rf86_read(index, 0x00, 0x00ull);
  tb_start(REGFILE_8W6R_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(REGFILE_8W6R_V, index-1, 0xf0000000);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("  REGFILE_4W4R Initial Stage...\n");
  index = 0;
  index = rf44_memset(index, 0x00, 0);
  index = rf44_read(index, 0x00, 0x00ull);
  tb_start(REGFILE_4W4R_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(REGFILE_4W4R_V, index-1, 0xf0000000);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("  CP0Q_RAM_OLD Initial Stage...\n");
  index = 0;
  index = cp35_memset(index, 0x00, 0xffff, 0);
  index = cp35_read(index, 0x00, 0x00ull, 0);
  tb_start(CP0Q_RAM_OLD_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(CP0Q_RAM_OLD_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("  CP0Q_RAM Initial Stage...\n");
  index = 0;
  index = cp25_memset(index, 0, 0x00ull);
  index = cp25_read(index, 0, 0x00ull);
  tb_start(CP0Q_RAM_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(CP0Q_RAM_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("  CAM_64X64 Initial Stage...\n");
  index = 0;
  index = cam464v_memset(index, 0x00ull, 0x00ull, 0x00, 0x00, false);
  index = cam464v_read(index, 63, 0x00ull);
  tb_start(CAM_64X64_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(CAM_64X64_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("  BTBCAM_1W1S Initial Stage...\n");
  index = 0;
  index = btbcam_memset2(index, 0x00ull, 0x00);
  index = btbcam_search(index, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3fffffffffffull, false);
  tb_start(BTBCAM_1W1S_H, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  tb_start(BTBCAM_1W1S_V, index-1);
  while(((st=status_read()) & 0x01) == 0) usleep(10);
  if(!tb_clear()) return false;
  fflush(stdout);

  printf("End of Initial Stage...\n\n");
  // End initialization of testchip
  return true;
}

int main(int argc, char* argv[])
{
  uint32_t st;
  sys_Init();
  cfg_write(0x03); // cpu off, reset assert
  cfg_write(0x02); // cpu off
  clock_config();

  int seed=0; // random seed
  if( argc == 1 ){
    printf("No random seed specified, use ctime() to get!\n");
    seed = (int)time(NULL);
  } else {
    seed = str2num((unsigned char*)argv[1]);
  }
  srand( seed );
  printf("Random Seed is : %d .\n\n", seed);

  gs_regfile_128x64_8sw6sr  rf86;
  gs_regfile_128x64_4sw4sr  rf44;
  gs_cp0q_ram_64x128_3sw5sr cp35;
  gs_cp0q_ram_48x64_2sw5sr  cp25;
  gs_cam_464v_64x64_1wrs    cam464v;
  gs_cam_btb_30x96_1w1s     cambtb;

  // Initial 
  if( !gs_memory_initial(rf86, rf44, cp35, cp25, cam464v, cambtb) ){
    printf("**ERROR** : Initial Error!\n");
    fflush(stdout);
    return -1;
  }
  fflush(stdout);

  int rf86_cnt=0, rf44_cnt=0, cp35_cnt=0, cp25_cnt=0, cam464v_cnt=0, cambtb_cnt=0;

  for( int i=0; i<TEST_TIMES; i++){
#ifdef TEST_RF86
    if( ! randomTest_8w6r( rf86 ) ){
      printf("\n********************** 8W6R ERROR OUT *********************\n");
      rf86_cnt++;
      //return -2;
    } else {
      printf("\n********************** 8W6R PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

#ifdef TEST_RF44
    if( ! randomTest_4w4r( rf44 ) ){
      printf("\n********************** 4W4R ERROR OUT *********************\n");
      rf44_cnt++;
      //return -3;
    } else {
      printf("\n********************** 4W4R PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

#ifdef TEST_CP35
    if( ! randomTest_3w5r( cp35 ) ){
      printf("\n********************** 3W5R ERROR OUT *********************\n");
      cp35_cnt++;
      //return -4;
    } else {
      printf("\n********************** 3W5R PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

#ifdef TEST_CP25
    if( ! randomTest_2w5r( cp25 ) ){
      printf("\n********************** 2W5R ERROR OUT *********************\n");
      cp25_cnt++;
      //return -5;
    } else {
      printf("\n********************** 2W5R PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

#ifdef TEST_CAM464V
    if( ! randomTest_cam464v( cam464v ) ){
      printf("\n******************** 464V_CAM ERROR OUT *******************\n");
      cam464v_cnt++;
      //return -6;
    } else {
      printf("\n********************** 464V_CAM PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

#ifdef TEST_CAMBTB
    if( ! randomTest_cambtb( cambtb ) ){
      printf("\n******************** BTB_CAM ERROR OUT *******************\n");
      cambtb_cnt++;
      //return -7;
    } else {
      printf("\n********************** BTB_CAM PASS %d *********************\n", i);
    }
    fflush(stdout);
#endif

  }

#ifdef DUMP_MEM
  // Begin dump
  rf86.dump();
  rf44.dump();
  cp35.dump();
  cp25.dump();
  cam464v.dump();
  cambtb.dump();
  // End dump
#endif

  printf("\n\n");
  printf("\n************** Begin of Conclude *************");
  printf("\n**     REGFILE_8W6R : %3d                   **", rf86_cnt);
  printf("\n**     REGFILE_4W4R : %3d                   **", rf44_cnt);
  printf("\n**    CP0Q_RAM_3W5R : %3d                   **", cp35_cnt);
  printf("\n**    CP0Q_RAM_2W5R : %3d                   **", cp25_cnt);
  printf("\n**         CAM_464V : %3d                   **", cam464v_cnt);
  printf("\n**         CAM_BTB  : %3d                   **", cambtb_cnt);
  printf("\n**********************************************");
  printf("\n**         pat_buf  : %3d                   **", pat_cnt);
  printf("\n**         gld_buf  : %3d                   **", gld_cnt);
  printf("\n************** End   of Conclude *************\n");

  return 0;
}
