#ifndef _CP25_FUNCTION_
#define _CP25_FUNCTION_

/* Define necessary parameters of CP0Q_2W5R
 */
#define CP25_ADDR_MIN 0
#define CP25_ADDR_MAX 47
#define CP25_DATA_MIN 0
#define CP25_DATA_MAX 63
#define CP25_WPORT_MIN 0
#define CP25_WPORT_MAX 1
#define CP25_RPORT_MIN 0
#define CP25_RPORT_MAX 4

int cp25_memset( int start, int wport, uint64_t data );
  /*
 * Write specified data to memory entry, using specified wport
 * start : start index of 1024 memory entry
 * wport : 0/1 writing port number
 * data  : data to write into, low 8-bit repeated to 64-bit
 */

int cp25_write( int start, int wport, const unsigned int addr, const unsigned int data );
  /*
 * Write specified data to memory entry, using specified wport
 * start : start index of 1024 memory entry
 * wport : 0/1 writing port number
 * addr  : address of writing entry, should transform to onehot format
 * data  : data to write, low 8-bit repeated to 64-bit
 */

int cp25_read( int start, const unsigned int addr, const unsigned int expected_data );
  /*
 * Read specified entry of memory, using specified wport and configuration
 * start : start index of 1024 memory entry
 * addr  : address of writing entry, should transform to onehot format
 * data  : data to write, low 8-bit repeated to 64-bit
 */

int cp25_operate( int start, bool we0, const unsigned int wad0, const unsigned int wdata0,
                             bool we1, const unsigned int wad1, const unsigned int wdata1,
                             bool re0, const unsigned int rad0, const unsigned int rdata0,
                             bool re1, const unsigned int rad1, const unsigned int rdata1,
                             bool re2, const unsigned int rad2, const unsigned int rdata2,
                             bool re3, const unsigned int rad3, const unsigned int rdata3,
                             bool re4, const unsigned int rad4, const unsigned int rdata4 );
  /*
 * All kinds of operating
 * start : start index of 1024 memory entry
 * we0    / we1    : enable signal of corespond wport
 * wad0   / wad1   : write address of each wport, should transform to onehot format
 * waddr0 / waddr1 : address of writing entry, should transform to onehot format
 * wdata0 / wdata1 : data to write, low 8-bit repeated to 64-bit
 * re0 / re1 / re2 / re3 / re4 : enable signal of corespond rport
 * raddr0 / raddr1 / raddr2 / raddr3 / raddr4 : address of read operation
 * rdata0 / rdata1 / rdata2 / rdata3 / rdata4 : expected output data of read operation
 */

void cp25_print_vec( int index, const tb_cp0q_ram_in_t &pat );
void cp25_print_vec( int index, const tb_cp0q_ram_in_t &pat, const tb_cp0q_ram_out_t &gld );
  /*
 * Print vector information.
 */

void cp25_test_spi();
  /*
 * Test for pattern buffer, golden buffer and out buffer.
 */
#endif
