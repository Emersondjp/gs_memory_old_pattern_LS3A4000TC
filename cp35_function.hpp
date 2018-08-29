#ifndef _CP35_FUNCTION_HPP_
#define _CP35_FUNCTION_HPP_

#define CP35_ADDR_MIN 0
#define CP35_ADDR_MAX 63

int cp35_operate( int index,
   uint8_t waddr0, uint64_t wvalue0, uint32_t wmask0, // Write Port-0
   uint8_t waddr1, uint64_t wvalue1, uint32_t wmask1, // Write Port-1
   uint64_t wen2,                                     // Write Port-2
   uint8_t raddr0, uint8_t raddr1, uint8_t raddr2, uint8_t raddr3, uint8_t raddr4, uint8_t ren,  // Read
   uint8_t ren2_bypass, uint8_t ren3_bypass,                                    // bypass enable obsolete
   uint8_t tbypass0, uint8_t tbypass1, uint8_t tin0, uint8_t tin1, uint8_t tin2,// test bypass
   uint64_t q0, uint64_t q1, uint64_t q2, uint64_t q3, uint64_t q4);  // Output

int cp35_myoperate( int index, const bool output_sel,
                    bool wen0, uint8_t waddr0, const uint8_t wvalue0, const uint32_t wmask0,
                    bool wen1, uint8_t waddr1, const uint8_t wvalue1, const uint32_t wmask1,
                    const uint64_t wen2,
                    const bool ren0, const uint8_t raddr0, const uint64_t q0,
                    const bool ren1, const uint8_t raddr1, const uint64_t q1,
                    const bool ren2, const uint8_t raddr2, const uint64_t q2,
                    const bool ren3, const uint8_t raddr3, const uint64_t q3,
                    const bool ren4, const uint8_t raddr4, const uint64_t q4 );

int cp35_read( const int index, const uint8_t addr, const uint64_t expected_data, const bool output_sel=false );
  /* index : 0~1024, entry number of 1024 memory
   * output_sel : high 64 bits to output if true, otherwise low 64 bits
   * addr  : read address
   * expected_data : expected output data
   */

int cp35_readH( const int index, const uint8_t addr, const uint64_t expected_data );

int cp35_readL( const int index, const uint8_t addr, const uint64_t expected_data );

int cp35_write( int index, int wport, uint8_t addr, const uint8_t wvalue, const uint32_t wmask=0xffff );
  /*
   * Write data into cp0q_3w5r
   * index : 0~1024, entry number of 1024 memory
   */
int cp35_write_vec( int index,  uint64_t vec, const uint8_t wvalue );
  /*
   * Write data into cp0q_3w5r through W2
   * index : entry number of 1024 memory
   * wvalue: data to write into, low 8-bit repeated to 64-bit
   */

int cp35_memset( int index, const uint8_t wvalue, const uint32_t wmask=0xffff, int wport=0 );
  /*
   * Write data into addr entry of cp0q_3w5r, return finished index number of 1024 memory
   * index : entry number of 1024 memory
   * wport : write port to use (0~1)
   * data  : data to write into, low 8-bit repeated to 64-bit
   */

void cp35_print_vec( int index, const tb_cp0q_ram_old_in_t &pat, const tb_cp0q_ram_old_out_t &gld );
  /*
   * Print vector information.
   */

#endif
