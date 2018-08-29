#ifndef _RF44_FUNCTION_HPP_
#define _RF44_FUNCTION_HPP_

#define RF44_ADDR_MIN 0
#define RF44_ADDR_MAX 127


int rf44_myoperate( int index,
  const bool wen0, uint8_t waddr0, uint8_t in0, 
  const bool wen1, uint8_t waddr1, uint8_t in1, 
  const bool wen2, uint8_t waddr2, uint8_t in2, 
  const bool wen3, uint8_t waddr3, uint8_t in3, 
  const bool ren0, uint8_t raddr0, uint8_t out0, 
  const bool ren1, uint8_t raddr1, uint8_t out1, 
  const bool ren2, uint8_t raddr2, uint8_t out2, 
  const bool ren3, uint8_t raddr3, uint8_t out3 );

int rf44_read( const int index, const uint8_t addr, const uint8_t expected_data );
  /* index : 0~1024, entry number of 1024 memory
   * addr  : read address
   * expected_data : expected output data
   */

int rf44_write( int index, int wport, uint8_t addr, const uint8_t data );
  /*
   * Write data into regfile_4w4r
   * index : 0~1024, entry number of 1024 memory
   */

int rf44_memset( int index, const uint8_t data, int wport=0 );
  /*
   * Write data into addr entry of cp0q_3w5r, return finished index number of 1024 memory
   * index : entry number of 1024 memory
   * wport : write port to use (0~3)
   * data  : data to write into, low 8-bit repeated to 64-bit
   */

void rf44_print_vec( int index, const tb_rf4w4r_in_t &pat, const tb_rf4w4r_out_t &gld );
  /*
   * Print vector information.
   */

void rf44_print_vec2( int index, const tb_rf4w4r_in_t &pat, const tb_rf4w4r_out_t &gld );
  /*
   * Print vector information.
   */

#endif
