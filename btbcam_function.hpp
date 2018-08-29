#ifndef _BTB_CAM_FUNCTION_HPP_
#define _BTB_CAM_FUNCTION_HPP_

#define BTB_CAM_ADDR_MIN 0
#define BTB_CAM_ADDR_MAX 95

int btbcam_operate( int index, uint8_t sen=0, uint32_t svpn=0,
    uint32_t valid31_00=0, uint32_t valid63_32=0, uint32_t valide95_64=0,
    uint8_t wen=0, uint32_t addr31_00=0, uint32_t addr63_32=0, uint32_t addr95_64=0,
    uint64_t wd=0, uint32_t wvpn=0,
    uint32_t match31_00=0, uint32_t match63_32=0, uint32_t match95_64=0,
    uint64_t rd=0, uint8_t hit=0 );
  /*
   * wd, rd will be extended from 0xAB to 0xABABABABABABABAB, if <= 0xff
   * cmp_mask is determind by wen/sen
   */

int btbcam_write( int index, uint8_t addr, uint64_t wd, uint32_t wvpn );
/*
 * Write data into btb_cam
 * index : 0~1024, entry number of 1024 memory
 */

int btbcam_memset( int index, uint64_t wd, uint32_t wvpn );
int btbcam_memset2( int index, uint64_t wd, uint32_t wvpn );

int btbcam_search( int index, uint32_t svpn,
    uint32_t valid31_00, uint32_t valid63_32, uint32_t valid95_64,
    uint32_t match31_00, uint32_t match63_32, uint32_t match95_64,
    uint64_t rd, bool hit );

void btbcam_print_vec( const int index, const tb_btbcam_in_t &pat, const tb_btbcam_out_t &gld );
void btbcam_print_vec2( const int index, const tb_btbcam_in_t &pat, const tb_btbcam_out_t &gld );

#endif
