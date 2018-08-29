#include<assert.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<stdio.h>

#include "gs_memory_class.hpp"

//-------------------------------------------------------------------

// Begin of gs_memory
bool gs_memory::check(){
  return false;
}

bool gs_memory::operate(){
  return false;
}

void gs_memory::dump(){
  printf("**WARNING**: dump function not complete yet.\n\n");
}

// End of gs_memory

//-------------------------------------------------------------------

// Begin of gs_regfile_128x64_8sw6sr
gs_regfile_128x64_8sw6sr::gs_regfile_128x64_8sw6sr( uint64_t val ) {
  this->memset( val );
  this->out0 = val;
  this->out1 = val;
  this->out2 = val;
  this->out3 = val;
  this->out4 = val;
  this->out5 = val;
}

bool gs_regfile_128x64_8sw6sr::check(
    bool we0, uint8_t wad0, bool we1, uint8_t wad1,
    bool we2, uint8_t wad2, bool we3, uint8_t wad3,
    bool we4, uint8_t wad4, bool we5, uint8_t wad5,
    bool we6, uint8_t wad6, bool we7, uint8_t wad7,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1,
    bool re2, uint8_t rad2, bool re3, uint8_t rad3,
    bool re4, uint8_t rad4, bool re5, uint8_t rad5){

  if( wad0 > 0x7F ) return false;
  if( wad1 > 0x7F ) return false;
  if( wad2 > 0x7F ) return false;
  if( wad3 > 0x7F ) return false;
  if( wad4 > 0x7F ) return false;
  if( wad5 > 0x7F ) return false;
  if( wad6 > 0x7F ) return false;
  if( wad7 > 0x7F ) return false;

  if( rad0 > 0x7F ) return false;
  if( rad1 > 0x7F ) return false;
  if( rad2 > 0x7F ) return false;
  if( rad3 > 0x7F ) return false;
  if( rad4 > 0x7F ) return false;
  if( rad5 > 0x7F ) return false;

  if( we0 ){
    if( we1 && (wad0==wad1) ) return false;
    if( we2 && (wad0==wad2) ) return false;
    if( we3 && (wad0==wad3) ) return false;
    if( we4 && (wad0==wad4) ) return false;
    if( we5 && (wad0==wad5) ) return false;
    if( we6 && (wad0==wad6) ) return false;
    if( we7 && (wad0==wad7) ) return false;
  }

  if( we1 ){
    if( we2 && (wad1==wad2) ) return false;
    if( we3 && (wad1==wad3) ) return false;
    if( we4 && (wad1==wad4) ) return false;
    if( we5 && (wad1==wad5) ) return false;
    if( we6 && (wad1==wad6) ) return false;
    if( we7 && (wad1==wad7) ) return false;
  }

  if( we2 ){
    if( we3 && (wad2==wad3) ) return false;
    if( we4 && (wad2==wad4) ) return false;
    if( we5 && (wad2==wad5) ) return false;
    if( we6 && (wad2==wad6) ) return false;
    if( we7 && (wad2==wad7) ) return false;
  }

  if( we3 ){
    if( we4 && (wad3==wad4) ) return false;
    if( we5 && (wad3==wad5) ) return false;
    if( we6 && (wad3==wad6) ) return false;
    if( we7 && (wad3==wad7) ) return false;
  }

  if( we4 ){
    if( we5 && (wad4==wad5) ) return false;
    if( we6 && (wad4==wad6) ) return false;
    if( we7 && (wad4==wad7) ) return false;
  }

  if( we5 ){
    if( we6 && (wad5==wad6) ) return false;
    if( we7 && (wad5==wad7) ) return false;
  }

  if( we6 ){
    if( we7 && (wad6==wad7) ) return false;
  }

  return true;
}

bool gs_regfile_128x64_8sw6sr::operate(
    bool we0, uint8_t wad0, uint64_t in0, bool we1, uint8_t wad1, uint64_t in1,
    bool we2, uint8_t wad2, uint64_t in2, bool we3, uint8_t wad3, uint64_t in3,
    bool we4, uint8_t wad4, uint64_t in4, bool we5, uint8_t wad5, uint64_t in5,
    bool we6, uint8_t wad6, uint64_t in6, bool we7, uint8_t wad7, uint64_t in7,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1,
    bool re2, uint8_t rad2, bool re3, uint8_t rad3,
    bool re4, uint8_t rad4, bool re5, uint8_t rad5){

  if( not this->check(we0, wad0, we1, wad1, we2, wad2, we3, wad3,
        we4, wad4, we5, wad5, we6, wad6, we7, wad7,
        re0, rad0, re1, rad1, re2, rad2, re3, rad3,
        re4, rad4, re5, rad5) ) return false;
  
  //Write Part
  if(we0) this->mem[wad0] = in0;
  if(we1) this->mem[wad1] = in1;
  if(we2) this->mem[wad2] = in2;
  if(we3) this->mem[wad3] = in3;
  if(we4) this->mem[wad4] = in4;
  if(we5) this->mem[wad5] = in5;
  if(we6) this->mem[wad6] = in6;
  if(we7) this->mem[wad7] = in7;

  //Read Part
  if(re0) this->out0 = this->mem[rad0];
  if(re1) this->out1 = this->mem[rad1];
  if(re2) this->out2 = this->mem[rad2];
  if(re3) this->out3 = this->mem[rad3];
  if(re4) this->out4 = this->mem[rad4];
  if(re5) this->out5 = this->mem[rad5];

  return true;
}

bool gs_regfile_128x64_8sw6sr::memset(uint64_t val){
  for(int i=0; i<128; i++){
    this->mem[i] = val;
  }
  return true;
}

bool gs_regfile_128x64_8sw6sr::write(){
  return false;
}

bool gs_regfile_128x64_8sw6sr::read(const uint8_t addr){
  return this->operate( false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
      false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
      true, addr, true, addr, true, addr, true, addr, true, addr, true, addr);
}

uint64_t gs_regfile_128x64_8sw6sr::get_out0(){
  return this->out0;
}

uint64_t gs_regfile_128x64_8sw6sr::get_out1(){
  return this->out1;
}

uint64_t gs_regfile_128x64_8sw6sr::get_out2(){
  return this->out2;
}

uint64_t gs_regfile_128x64_8sw6sr::get_out3(){
  return this->out3;
}

uint64_t gs_regfile_128x64_8sw6sr::get_out4(){
  return this->out4;
}

uint64_t gs_regfile_128x64_8sw6sr::get_out5(){
  return this->out5;
}

void gs_regfile_128x64_8sw6sr::dump(){
  uint16_t d0, d1, d2, d3;
  uint64_t d64;
  printf("\n\n--------------Begin of 8W6R dump--------------\n");
  for(int i=0; i<128; i++){
    d64 = this->mem[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    d3 = (d64 & 0xffff000000000000ull) >> 48;
    printf(" 0x%02x : %04x %04x %04x %04x .\n", i, d3, d2, d1, d0 );
  }
  fflush(stdout);
  printf("---------------End of 8W6R dump---------------\n\n");
}

// End of gs_regfile_128x64_8sw6sr

//-------------------------------------------------------------------

// Begin of gs_regfile_128x64_4sw4sr
gs_regfile_128x64_4sw4sr::gs_regfile_128x64_4sw4sr( uint64_t val ) {
  this->memset( val );
  for(int i=0; i<128; i++){
    this->mem[i] = val;
  }
  this->out0 = val;
  this->out1 = val;
  this->out2 = val;
  this->out3 = val;
}

bool gs_regfile_128x64_4sw4sr::check(
    bool we0, uint8_t wad0, bool we1, uint8_t wad1,
    bool we2, uint8_t wad2, bool we3, uint8_t wad3,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1,
    bool re2, uint8_t rad2, bool re3, uint8_t rad3){

  if( we0 && wad0 > 0x7F ) return false;
  if( we1 && wad1 > 0x7F ) return false;
  if( we2 && wad2 > 0x7F ) return false;
  if( we3 && wad3 > 0x7F ) return false;
  if( re0 && rad0 > 0x7F ) return false;
  if( re1 && rad1 > 0x7F ) return false;
  if( re2 && rad2 > 0x7F ) return false;
  if( re3 && rad3 > 0x7F ) return false;

  if( we0 ){
    if( we1 && (wad0==wad1) ) return false;
    if( we2 && (wad0==wad2) ) return false;
    if( we3 && (wad0==wad3) ) return false;
  }

  if( we1 ){
    if( we2 && (wad1==wad2) ) return false;
    if( we3 && (wad1==wad3) ) return false;
  }

  if( we2 ){
    if( we3 && (wad2==wad3) ) return false;
  }

  return true;
}

bool gs_regfile_128x64_4sw4sr::operate(
    bool we0, uint8_t wad0, uint64_t in0, bool we1, uint8_t wad1, uint64_t in1,
    bool we2, uint8_t wad2, uint64_t in2, bool we3, uint8_t wad3, uint64_t in3,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1,
    bool re2, uint8_t rad2, bool re3, uint8_t rad3){

  if( not this->check(we0, wad0, we1, wad1, we2, wad2, we3, wad3,
        re0, rad0, re1, rad1, re2, rad2, re3, rad3) ) return false;
  
  //Write Part
  if(we0) this->mem[wad0] = in0;
  if(we1) this->mem[wad1] = in1;
  if(we2) this->mem[wad2] = in2;
  if(we3) this->mem[wad3] = in3;

  //Read Part
  if(re0) this->out0 = this->mem[rad0];
  if(re1) this->out1 = this->mem[rad1];
  if(re2) this->out2 = this->mem[rad2];
  if(re3) this->out3 = this->mem[rad3];

  return true;
}

bool gs_regfile_128x64_4sw4sr::memset(uint64_t val){
  for(int i=0; i<128; i++){
    this->mem[i] = val;
  }
  return true;
}

bool gs_regfile_128x64_4sw4sr::write(){
  return false;
}

bool gs_regfile_128x64_4sw4sr::read(uint8_t addr){
  return this->operate(false, 0, 0, false, 0, 0, false, 0, 0, false, 0, 0,
      true, addr, true, addr, true, addr, true, addr);
}

uint64_t gs_regfile_128x64_4sw4sr::get_out0(){
  return this->out0;
}

uint64_t gs_regfile_128x64_4sw4sr::get_out1(){
  return this->out1;
}

uint64_t gs_regfile_128x64_4sw4sr::get_out2(){
  return this->out2;
}

uint64_t gs_regfile_128x64_4sw4sr::get_out3(){
  return this->out3;
}

void gs_regfile_128x64_4sw4sr::dump(){
  uint16_t d0, d1, d2, d3;
  uint64_t d64;
  printf("\n\n--------------Begin of 4W4R dump--------------\n");
  for(int i=0; i<128; i++){
    d64 = this->mem[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    d3 = (d64 & 0xffff000000000000ull) >> 48;
    printf(" 0x%02x : %04x %04x %04x %04x .\n", i, d3, d2, d1, d0 );
  }
  fflush(stdout);
  printf("---------------End of 4W4R dump---------------\n\n");
}

// End of gs_regfile_128x64_4sw4sr

//-------------------------------------------------------------------

// Begin of gs_cp0q_ram_64x128_3sw5sr
gs_cp0q_ram_64x128_3sw5sr::gs_cp0q_ram_64x128_3sw5sr( uint16_t mask, uint64_t val_L, uint64_t val_H ) {
  this->memset( mask, val_L, val_H );
  this->out0_L = val_L;
  this->out0_H = val_H;
  this->out1_L = val_L;
  this->out1_H = val_H;
  this->out2_L = val_L;
  this->out2_H = val_H;
  this->out3_L = val_L;
  this->out3_H = val_H;
  this->out4_L = val_L;
  this->out4_H = val_H;
}

bool gs_cp0q_ram_64x128_3sw5sr::check(
    bool we0, uint8_t wad0, bool we1, uint8_t wad1, uint64_t we2,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1, bool re2, uint8_t rad2,
    bool re3, uint8_t rad3, bool re4, uint8_t rad4 ){

  if( we0 && wad0 > 0x3F ) return false;
  if( we1 && wad1 > 0x3F ) return false;

  if( re0 && rad0 > 0x3F ) return false;
  if( re1 && rad1 > 0x3F ) return false;
  if( re2 && rad2 > 0x3F ) return false;
  if( re3 && rad3 > 0x3F ) return false;
  if( re4 && rad4 > 0x3F ) return false;

#ifndef CP35_STRICT_OFF
  if( re0 ){
    if( re1 && ( rad0 == rad1 ) ) return false;
    if( re2 && ( rad0 == rad2 ) ) return false;
    if( re3 && ( rad0 == rad3 ) ) return false;
    if( re4 && ( rad0 == rad4 ) ) return false;
    //if( we2 & ( 0x01ull<<rad0 ) ) return false;
  }

  if( re1 ){
    if( re2 && ( rad1 == rad2 ) ) return false;
    if( re3 && ( rad1 == rad3 ) ) return false;
    if( re4 && ( rad1 == rad4 ) ) return false;
    //if( we2 & ( 0x01ull<<rad1 ) ) return false;
  }

  if( re2 ){
    if( re3 && ( rad2 == rad3 ) ) return false;
    if( re4 && ( rad2 == rad4 ) ) return false;
    if( we0 && ( rad2 == wad0 ) ) return false;
    if( we1 && ( rad2 == wad1 ) ) return false;
    if( we2 & ( 0x01ull<<rad2 ) ) return false;
  }

  if( re3 ){
    if( re4 && ( rad3 == rad4 ) ) return false;
    if( we0 && ( rad3 == wad0 ) ) return false;
    if( we1 && ( rad3 == wad1 ) ) return false;
    if( we2 & ( 0x01ull<<rad3 ) ) return false;
  }

  if( re4 ){
    if( we0 && ( rad4 == wad0 ) ) return false;
    if( we1 && ( rad4 == wad1 ) ) return false;
    if( we2 & ( 0x01ull<<rad4 ) ) return false;
  }

#endif

  if( we0 ){
    if( we1 && (wad0==wad1) ) return false;
    if( we2 ) return false;
  }
  if( we1 ){
    if( we2 & ( 0x01ull << wad1 ) ) return false;
  }

  return true;
}

bool gs_cp0q_ram_64x128_3sw5sr::operate(
    bool we0, uint8_t wad0, uint64_t wvalue0_L, uint64_t wvalue0_H, uint32_t wmask0,
    bool we1, uint8_t wad1, uint64_t wvalue1_L, uint64_t wvalue1_H, uint32_t wmask1,
    uint64_t we2,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1, bool re2, uint8_t rad2,
    bool re3, uint8_t rad3, bool re4, uint8_t rad4 ){

  if( not this->check(we0, wad0, we1, wad1, we2,
        re0, rad0, re1, rad1, re2, rad2, re3, rad3, re4, rad4) ) return false;
  
  //Write Part
  if(we0){
    this->mem_L[wad0] = wvalue0_L;
    this->mem_H[wad0] = wvalue0_H;
    this->mask[wad0] = wmask0;
  }
  if(we1){
    this->mem_L[wad1] = wvalue1_L;
    this->mem_H[wad1] = wvalue1_H;
    this->mask[wad1] = wmask1;
  }
  if(we2){
    for(int i=0; i<64; i++){
      if(we2 & (0x01ull<<i)){
        //Write according to mask
        uint16_t mask = this->mask[i];
        uint64_t tmp_1 = 0x00ull, tmp_2 = 0x00ull;
        uint64_t tmp_3 = 0x00ull, tmp_4 = 0x00ull;
        for(int j=0; j<8; j++){
          if(mask&(0x01u<<j)){
            tmp_1 += ( wvalue0_L      & (0xffull<<(j*8)) );
          } else {
            tmp_2 += ( this->mem_L[i] & (0xffull<<(j*8)) );
          }
          if(mask&(0x01u<<(j+8))){
            tmp_3 += ( wvalue0_H      & (0xffull<<(j*8)) );
          } else {
            tmp_4 += ( this->mem_H[i] & (0xffull<<(j*8)) );
          }
        }
        this->mem_L[i] = tmp_1 + tmp_2;
        this->mem_H[i] = tmp_3 + tmp_4;
      }
    }
  }

  //Read Part
  if(re0){
    this->out0_L = this->mem_L[rad0];
    this->out0_H = this->mem_H[rad0];
  }
  if(re1){
    this->out1_L = this->mem_L[rad1];
    this->out1_H = this->mem_H[rad1];
  }
  if(re2){
    this->out2_L = this->mem_L[rad2];
    this->out2_H = this->mem_H[rad2];
  }
  if(re3){
    this->out3_L = this->mem_L[rad3];
    this->out3_H = this->mem_H[rad3];
  }
  if(re4){
    this->out4_L = this->mem_L[rad4];
    this->out4_H = this->mem_H[rad4];
  }

  return true;
}

bool gs_cp0q_ram_64x128_3sw5sr::memset(uint16_t mask, uint64_t val_L, uint64_t val_H){
  for(int i=0; i<64; i++){
    this->mem_L[i] = val_L;
    this->mem_H[i] = val_H;
    this->mask[i]  = mask;
  }
  this->out0_L = val_L;
  this->out0_H = val_H;
  this->out1_L = val_L;
  this->out1_H = val_H;
  this->out2_L = val_L;
  this->out2_H = val_H;
  this->out3_L = val_L;
  this->out3_H = val_H;
  this->out4_L = val_L;
  this->out4_H = val_H;
  return true;
}

bool gs_cp0q_ram_64x128_3sw5sr::write(){
  return false;
}

bool gs_cp0q_ram_64x128_3sw5sr::read(uint8_t addr){
  return
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      true, addr, false, addr, false, addr, false, addr, false, addr) && 
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      false, addr, true, addr, false, addr, false, addr, false, addr) && 
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      true, addr, false, addr, false, addr, false, addr, false, addr) && 
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      false, addr, false, addr, true, addr, false, addr, false, addr) && 
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      false, addr, false, addr, false, addr, true, addr, false, addr) && 
    this->operate( false, 0, 0, 0, 0, false, 0, 0, 0, 0, 0x00ull,
      false, addr, false, addr, false, addr, false, addr, true, addr);
}

uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out0H(){
  return this->out0_H;
}
uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out0L(){
  return this->out0_L;
}

uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out1H(){
  return this->out1_H;
}
uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out1L(){
  return this->out1_L;
}

uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out2H(){
  return this->out2_H;
}
uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out2L(){
  return this->out2_L;
}

uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out3H(){
  return this->out3_H;
}
uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out3L(){
  return this->out3_L;
}

uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out4H(){
  return this->out4_H;
}
uint64_t gs_cp0q_ram_64x128_3sw5sr::get_out4L(){
  return this->out4_L;
}

void gs_cp0q_ram_64x128_3sw5sr::dump(){
  uint16_t d0, d1, d2, d3, d4, d5, d6, d7, mask;
  uint64_t d64L, d64H;
  printf("\n\n--------------Begin of 3W5R dump--------------\n");
  for(int i=0; i<64; i++){
    d64L = this->mem_L[i];
    d64H = this->mem_H[i];
    mask = this->mask[i];
    d0 = (d64L & 0x000000000000ffffull) >>  0;
    d1 = (d64L & 0x00000000ffff0000ull) >> 16;
    d2 = (d64L & 0x0000ffff00000000ull) >> 32;
    d3 = (d64L & 0xffff000000000000ull) >> 48;
    d4 = (d64H & 0x000000000000ffffull) >>  0;
    d5 = (d64H & 0x00000000ffff0000ull) >> 16;
    d6 = (d64H & 0x0000ffff00000000ull) >> 32;
    d7 = (d64H & 0xffff000000000000ull) >> 48;
    printf(" 0x%02x : %04x %04x %04x %04x  %04x %04x %04x %04x  -- 0x%04x .\n", i, d7, d6, d5, d4, d3, d2, d1, d0, mask );
  }
  fflush(stdout);
  printf("---------------End of 3W5R dump---------------\n\n");
}

// End of gs_cp0q_ram_64x128_3sw5sr

//-------------------------------------------------------------------

// Begin of gs_cp0q_ram_48x64_2sw5sr
gs_cp0q_ram_48x64_2sw5sr::gs_cp0q_ram_48x64_2sw5sr( uint64_t val ) {
  this->memset( val );
  this->out0 = val;
  this->out1 = val;
  this->out2 = val;
  this->out3 = val;
  this->out4 = val;
}

bool gs_cp0q_ram_48x64_2sw5sr::check( bool we0, uint8_t wad0, bool we1, uint8_t wad1,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1, bool re2, uint8_t rad2,
    bool re3, uint8_t rad3, bool re4, uint8_t rad4){

  if( we0 && wad0 > 0x2F ) return false;
  if( we1 && wad1 > 0x2F ) return false;

  if( re0 && rad0 > 0x2F ) return false;
  if( re1 && rad1 > 0x2F ) return false;
  if( re2 && rad2 > 0x2F ) return false;
  if( re3 && rad3 > 0x2F ) return false;
  if( re4 && rad4 > 0x2F ) return false;

  if( we0 && we1 && (wad0==wad1) ) return false;

  if( re0 && re1 && (rad0==rad1) ) return false;

  if(we0){
    if(re0 && (rad0==wad0)) return false;
    if(re1 && (rad1==wad0)) return false;
    if(re2 && (rad2==wad0)) return false;
    if(re3 && (rad3==wad0)) return false;
    if(re4 && (rad4==wad0)) return false;
  }
  if(we1){
    if(re0 && (rad0==wad1)) return false;
    if(re1 && (rad1==wad1)) return false;
    if(re2 && (rad2==wad1)) return false;
    if(re3 && (rad3==wad1)) return false;
    if(re4 && (rad4==wad1)) return false;
  }

  return true;
}

bool gs_cp0q_ram_48x64_2sw5sr::operate(
    bool we0, uint8_t wad0, uint64_t in0, bool we1, uint8_t wad1, uint64_t in1,
    bool re0, uint8_t rad0, bool re1, uint8_t rad1, bool re2, uint8_t rad2,
    bool re3, uint8_t rad3, bool re4, uint8_t rad4){

  if( not this->check(we0, wad0, we1, wad1,
        re0, rad0, re1, rad1, re2, rad2, re3, rad3, re4, rad4) ) return false;
  
  //Write Part
  if(we0) this->mem[wad0] = in0;
  if(we1) this->mem[wad1] = in1;

  //Read Part
  if(re0) this->out0 = this->mem[rad0];
  if(re1) this->out1 = this->mem[rad1];
  if(re2) this->out2 = this->mem[rad2];
  if(re3) this->out3 = this->mem[rad3];
  if(re4) this->out4 = this->mem[rad4];

  return true;
}

bool gs_cp0q_ram_48x64_2sw5sr::memset(uint64_t val){
  for(int i=0; i<48; i++){
    this->mem[i] = val;
  }
  return true;
}

bool gs_cp0q_ram_48x64_2sw5sr::write(){
  return false;
}

bool gs_cp0q_ram_48x64_2sw5sr::read(uint8_t addr){
  return
    this->operate( false, 0, 0, false, 0, 0,
      true, addr, false, addr, false, addr, false, addr, false, addr) &&
    this->operate( false, 0, 0, false, 0, 0,
      false, addr, true, addr, false, addr, false, addr, false, addr) &&
    this->operate( false, 0, 0, false, 0, 0,
      false, addr, false, addr, true, addr, false, addr, false, addr) &&
    this->operate( false, 0, 0, false, 0, 0,
      false, addr, false, addr, false, addr, true, addr, false, addr) &&
    this->operate( false, 0, 0, false, 0, 0,
      false, addr, false, addr, false, addr, false, addr, true, addr);
}

uint64_t gs_cp0q_ram_48x64_2sw5sr::get_out0(){
  return this->out0;
}

uint64_t gs_cp0q_ram_48x64_2sw5sr::get_out1(){
  return this->out1;
}

uint64_t gs_cp0q_ram_48x64_2sw5sr::get_out2(){
  return this->out2;
}

uint64_t gs_cp0q_ram_48x64_2sw5sr::get_out3(){
  return this->out3;
}

uint64_t gs_cp0q_ram_48x64_2sw5sr::get_out4(){
  return this->out4;
}

void gs_cp0q_ram_48x64_2sw5sr::dump(){
  uint16_t d0, d1, d2, d3;
  uint64_t d64;
  printf("\n\n--------------Begin of 2W5R dump--------------\n");
  for(int i=0; i<48; i++){
    d64 = this->mem[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    d3 = (d64 & 0xffff000000000000ull) >> 48;
    printf(" 0x%02x : %04x %04x %04x %04x .\n", i, d3, d2, d1, d0 );
  }
  fflush(stdout);
  printf("---------------End of 2W5R dump---------------\n\n");
}

// End of gs_cp0q_ram_48x64_2sw5sr

//-------------------------------------------------------------------

// Begin of gs_cam_464v_64x64_1wrs
gs_cam_464v_64x64_1wrs::gs_cam_464v_64x64_1wrs(
    uint64_t vpn, uint32_t mask, uint16_t asid, bool g, uint64_t data){
  this->memset(vpn, mask, asid, g, data);
  this->match = 0x00ull;
  this->hit   = false;
  this->out   = data;
}

bool gs_cam_464v_64x64_1wrs::check( bool se, uint64_t svpn, uint16_t sasid, uint64_t valid,
    bool we, uint64_t wvpn, uint32_t wmask, uint16_t wasid,
    bool re, uint16_t addr){

  if( addr > 0x3F ) return false;
  if( se && re ) return false;
  if( we && re ) return false;
  if( se && we && ( (0x01ull<<addr) & valid ) ) return false;

  if( svpn  > (0x01ull << 48) ) return false;
  if( sasid > (0x01ul  << 10) ) return false;
  if( wvpn  > (0x01ull << 48) ) return false;
  if( wasid > (0x01ul  << 10) ) return false;
  if( wmask > (0x01ul  << 18) ) return false;

  return true;
}

bool gs_cam_464v_64x64_1wrs::operate( bool se, uint64_t svpn, uint16_t sasid, uint64_t valid,
    bool we, uint64_t wdata, uint64_t wvpn, uint32_t wmask, uint16_t wasid, bool g,
    bool re, uint16_t addr){

  if( not this->check( se, svpn, sasid, valid, we, wvpn, wmask, wasid, re, addr ) ) return false;

  if( se ){
    bool flag=false;
    int match_addr=-1;
    for(int i=0; i<64; i++){
      if( valid & (0x01ull<<i) ){
        if( ( this->cam_g[i] || (sasid == this->cam_asid[i]) ) &&
            ( (this->cam_mask[i]|this->cam_vpn[i]) == (this->cam_mask[i]|svpn) ) ){
          if( flag ) return false; // double match return false
          else{
            flag = true;
            match_addr = i;
          }
        }
      }
    }
    if( flag ){
      this->hit = true;
      this->match = (0x01ull << match_addr);
      this->out = this->ram[match_addr];
    } else {
      this->hit = false;
      this->match = 0x00ull;
      this->out = 0xffffffffffffffffull;
    }
  }
  if( re ){
    this->out = this->ram[addr];
  }
  if( we ){
    this->ram[addr]      = wdata;
    this->cam_vpn[addr]  = wvpn;
    this->cam_mask[addr] = wmask;
    this->cam_asid[addr] = wasid;
    this->cam_g[addr]    = g;
  }

  return true;
}

bool gs_cam_464v_64x64_1wrs::memset( uint64_t vpn, uint32_t mask, uint16_t asid, bool g, uint64_t data){
  if( not this->check( false, 0x00ull, 0x00, 0x00ull, false, vpn, mask, asid, false, 0x00 ) ) return false;
  for(int i=0; i<48; i++){
    this->cam_vpn[i]  = vpn;
    this->cam_mask[i] = mask;
    this->cam_asid[i] = asid;
    this->cam_g[i]    = g;
    this->ram[i]      = data;
  }
  return true;
}

uint16_t gs_cam_464v_64x64_1wrs::get_asid(const uint8_t addr){
  assert( addr < 64 );
  return this->cam_asid[addr];
}

uint64_t gs_cam_464v_64x64_1wrs::get_vpn(const uint8_t addr){
  assert( addr < 64 );
  return this->cam_vpn[addr];
}

uint64_t gs_cam_464v_64x64_1wrs::get_out(){
  return this->out;
}

uint64_t gs_cam_464v_64x64_1wrs::get_match(){
  return this->match;
}

bool gs_cam_464v_64x64_1wrs::get_hit(){
  return this->hit;
}

void gs_cam_464v_64x64_1wrs::dump(){
  uint16_t d0, d1, d2, d3;
  uint64_t d64;
  printf("\n\n--------------Begin of CAM_464V dump--------------\n");
  for(int i=0; i<64; i++){
    d64 = this->cam_vpn[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    printf(" 0x%02x : %04x %04x %04x , %05x, %03x, %d, ", i, d2, d1, d0,
        this->cam_mask[i], this->cam_asid[i], (this->cam_g[i]?1:0) );
    d64 = this->ram[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    d3 = (d64 & 0xffff000000000000ull) >> 48;
    printf("%04x %04x %04x %04x .\n", d3, d2, d1, d0);
  }
  fflush(stdout);
  printf("---------------End of CAM_464V dump---------------\n\n");
}

// End of gs_cam_464v_64x64_1wrs

//-------------------------------------------------------------------

// Begin of gs_cam_btb_30x96_1w1s
gs_cam_btb_30x96_1w1s::gs_cam_btb_30x96_1w1s( uint32_t vpn, uint64_t data){
  this->memset(vpn, data);
  this->out = data;
  this->match31_00 = 0x0ul;
  this->match63_32 = 0x0ul;
  this->match95_64 = 0x0ul;
}

bool gs_cam_btb_30x96_1w1s::check( bool se, uint32_t svpn,
    uint32_t valid31_00, uint32_t valid63_32, uint32_t valid95_64,
    bool we, uint32_t addr31_00, uint32_t addr63_32,
    uint32_t addr95_64, uint32_t wvpn, uint64_t data ){

  bool flag=false;
  for(int i=0;i<32;i++){ // make sure only one 1 in addr.
    if( addr31_00 & (0x01ul<<i) ){
      if(flag) return false;
      else flag=true;
    }
    if( addr63_32 & (0x01ul<<i) ){
      if(flag) return false;
      else flag=true;
    }
    if( addr95_64 & (0x01ul<<i) ){
      if(flag) return false;
      else flag=true;
    }
  }
  if( se && we ){
    if( addr31_00 & valid31_00  ) return false;
    if( addr63_32 & valid63_32  ) return false;
    if( addr95_64 & valid95_64  ) return false;
  }
  if( wvpn >= (0x01ul <<30) ) return false;
  if( data >= (0x01ull<<46) ) return false;

  return true;
}

bool gs_cam_btb_30x96_1w1s::operate( bool se, uint32_t svpn, uint32_t valid31_00, uint32_t valid63_32, uint32_t valid95_64,
    bool we, uint32_t addr31_00, uint32_t addr63_32, uint32_t addr95_64, uint64_t data, uint32_t wvpn ){

  if( not this->check( se, svpn, valid31_00, valid63_32, valid95_64, we,
        addr31_00, addr63_32, addr95_64, wvpn, data ) ) return false;

  int myaddr = 0;
  for(int i=0; i<32; i++){
    if( addr31_00 & (0x01ul<<i) ){ myaddr = i +  0; break; }
    if( addr63_32 & (0x01ul<<i) ){ myaddr = i + 32; break; }
    if( addr95_64 & (0x01ul<<i) ){ myaddr = i + 64; break; }
  }
  bool flag=false;
  int  match_addr=-1;
  if(se){
    for(int i=0; i<32; i++){
      if( valid31_00 & ( 0x01ul << i ) ){
        if( this->cam[i] == svpn ){
          if( !flag ){
            flag = true;
            match_addr = i; 
          } else return false;  // double match
        }
      }
      if( valid63_32 & ( 0x01ul << i ) ){
        if( this->cam[i+32] == svpn ){
          if( !flag ){
            flag = true;
            match_addr = i+32; 
          } else return false;  // double match
        }
      }
      if( valid95_64 & ( 0x01ul << i ) ){
        if( this->cam[i+64] == svpn ){
          if( !flag ){
            flag = true;
            match_addr = i+64; 
          } else return false;  // double match
        }
      }
    }

    if( flag ){
      this->hit = 0x1;
      this->out = this->ram[match_addr];
      this->match31_00 = 0x00ull;
      this->match63_32 = 0x00ul;
      this->match95_64 = 0x00ul;
      if( match_addr < 32 ) match31_00 = (0x01ull<<match_addr);
      else if(match_addr < 64) match63_32 = (0x01ull<<(match_addr-32));
      else if(match_addr < 96) match95_64 = (0x01ull<<(match_addr-64));
    } else {
      this->hit = 0x0;
      this->match31_00 = 0x00ul;
      this->match63_32 = 0x00ul;
      this->match95_64 = 0x00ul;
      this->out = 0x3fffffffffffull; // when no match, output will be all 1
    }
  }

  if(we){
    this->cam[myaddr] = wvpn;
    this->ram[myaddr] = data;
  }

  return true;
}

bool gs_cam_btb_30x96_1w1s::memset( uint32_t vpn, uint64_t data ){
  for(int i=0; i<96; i++){
    this->cam[i] = vpn;
    this->ram[i] = data;
  }
  return true;
}

bool gs_cam_btb_30x96_1w1s::memset2( uint32_t vpn, uint64_t data ){
  for(int i=0; i<96; i++){
    this->cam[i] = (vpn+i)&0x3ffffffful;
    this->ram[i] = (data+i)&0x00003fffffffffffull;
  }
  return true;
}

uint32_t gs_cam_btb_30x96_1w1s::get_svpn(const uint8_t addr){
  assert( addr < 96 );
  return this->cam[addr];
}

bool gs_cam_btb_30x96_1w1s::write(){
  return false;
}

bool gs_cam_btb_30x96_1w1s::read(const uint8_t addr){
  return false;
}

uint64_t gs_cam_btb_30x96_1w1s::get_out(){
  return this->out;
}

bool gs_cam_btb_30x96_1w1s::get_hit(){
  return this->hit;
}

uint32_t gs_cam_btb_30x96_1w1s::get_match31_00(){
  return this->match31_00;
}

uint32_t gs_cam_btb_30x96_1w1s::get_match63_32(){
  return this->match63_32;
}

uint32_t gs_cam_btb_30x96_1w1s::get_match95_64(){
  return this->match95_64;
}

void gs_cam_btb_30x96_1w1s::dump(){
  uint16_t d0, d1, d2, d3;
  uint64_t d64;
  printf("\n\n--------------Begin of CAM_BTB dump---------------\n");
  for(int i=0; i<96; i++){
    d64 = this->cam[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    printf(" 0x%02x : %04x %04x , ", i, d1, d0);

    d64 = this->ram[i];
    d0 = (d64 & 0x000000000000ffffull) >>  0;
    d1 = (d64 & 0x00000000ffff0000ull) >> 16;
    d2 = (d64 & 0x0000ffff00000000ull) >> 32;
    //d3 = (d64 & 0xffff000000000000ull) >> 48;
    //printf("%04x %04x %04x %04x .\n", d3, d2, d1, d0);
    printf("%04x %04x %04x .\n", d2, d1, d0);
  }
  fflush(stdout);
  printf("---------------End of CAM_BTB dump----------------\n\n");
}

// End of gs_cam_btb_30x96_1w1s

//-------------------------------------------------------------------

