#ifndef _CAM464V_FUNCTION_HPP_
#define _CAM464V_FUNCTION_HPP_

#define CAM464V_ADDR_MIN 0
#define CAM464V_ADDR_MAX 63

int cam464v_operate( int index, uint8_t ren=0, uint8_t addr=0,
                     uint8_t wen=0, uint64_t wd=0, uint64_t wvpn=0, uint32_t wasid=0, uint32_t mask=0, uint8_t G=0,
                     uint8_t sen=0, uint64_t svpn=0, uint32_t sasid=0, uint64_t valid=0,
                     uint64_t match=0, uint64_t rd=0, uint8_t hit=0);
  /*
   * wd, rd will be extended from 0xAB to 0xABABABABABABABAB, if <= 0xff
   * cmp_mask is determind by ren/wen/sen
   */

int cam464v_read( int index, uint8_t addr, uint64_t expected_data );
/* index : 0~1024, entry number of 1024 memory
 * addr  : read address
 * expected_data : expected output data
 **/

int cam464v_write( int index, uint8_t addr, uint64_t wd, uint64_t wvpn, uint16_t wasid, uint32_t mask, uint8_t G );
/*
 * Write data into cam_464v
 * index : 0~1024, entry number of 1024 memory
 */

int cam464v_memset( int index, uint64_t wd, uint64_t wvpn, uint32_t wasid, uint32_t mask, uint8_t G );
/*
 * set all entries.
 */

int cam464v_search( int index, uint64_t svpn, uint16_t sasid, uint32_t valid,
                     uint64_t match, uint64_t rd, uint8_t hit ); // hit, match, rd to compare

void cam464v_print_vec( const int index, const tb_cam64x64_in_t &pat, const tb_cam64x64_out_t &gld );

#endif
