#ifndef _TESTBED_HPP_
#define _TESTBED_HPP_

void sys_Init(void);
// system initialization
void tb_start(int target, int ncyc, int pat_base, int gld_base, uint32_t mut_cfg=0);
void tb_start(int target, int ncyc, uint32_t mut_cfg=0);
// start test bed
bool tb_clear();
bool mytb_clear(const int startIndex=0);
// clear test code
void clock_config();
// config clock
#endif
