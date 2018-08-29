#ifndef _RF86_FUNCTION_HPP_
#define _RF86_FUNCTION_HPP_

/*
 * Define necessary parameters of CP0Q_2W5R
 */

#define RF86_WPORT_MAX 7
#define RF86_WPORT_MIN 0
#define RF86_RPORT_MAX 5
#define RF86_RPORT_MIN 0
#define RF86_ADDR_MAX 127
#define RF86_ADDR_MIN 0
#define RF86_DATA_MAX 63
#define RF86_DATA_MIN 0

int rf86_write( const int index, const unsigned int wport, const unsigned int addr, const uint8_t data );
  /*
 * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
 * index : entry number of 1024 memory
 * wport : write port to use (0~7)
 * addr  : address of writing entry, should transform to onehot format
 * data  : data to write into, low 8-bit repeated to 64-bit
 */
int rf86_memset( const int index, const unsigned int wport, const uint8_t data );
  /*
 * Write data into addr entry of regfile_8w6r, return finished index number of 1024 memory
 * index : entry number of 1024 memory
 * wport : write port to use (0~7)
 * data  : data to write into, low 8-bit repeated to 64-bit
 */
int rf86_read( const int index, const unsigned int addr, const uint8_t exp_d );
  /*
  * Read specified entry of regfile_8w6r, using all read-port
  * start : start index of 1024 memory entry
  * addr  : address of read entry
  * data  : data expexted, low 8-bit repeated to 64-bit
 */
int rf86_operate( const int index, const bool we0, const uint16_t wad0, const uint8_t d0,
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
                             const bool re5, const uint16_t rad5, const uint8_t q5 );

void rf86_print_vec( const int index, const tb_rf8w6r_in_t &pat );
  /*
 * Print vector information.
 */
void rf86_print_vec( const int index, const tb_rf8w6r_in_t &pat, const tb_rf8w6r_out_t &gld );
  /*
 * Print vector information.
 */
void rf86_print_vec2( const int index, const tb_rf8w6r_in_t &pat, const tb_rf8w6r_out_t &gld );
  /*
 * Print vector information.
 */
void rf86_print_cfg(const uint32_t cfg);
  /*
 * Print Configuration Code of 8W6R
 */

#endif
