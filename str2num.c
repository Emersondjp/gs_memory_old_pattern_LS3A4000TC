#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
unsigned int str2num(unsigned char *s){
  unsigned int num = 0;
  unsigned int radix;
  unsigned int chr;
  
  unsigned int i;
  if((*(s)=='0') && (*(s+1)=='x' || *(s+1)=='X')){
    i = 2;
    radix = 16;
  }else if(*(s)=='0'){
    i = 1;
    radix = 8;
  }else{
    i = 0;
    radix = 10;
  }
  
  for(; *(s+i)!='\0'; i++){
    chr = *(s+i);
    switch(radix){
      case 16:
        if(chr>='0' && chr<='9')      num = num*radix + (chr - '0');
        else if(chr>='a' && chr<='f') num = num*radix + (chr - 'a' + 10);
        else if(chr>='A' && chr<='F') num = num*radix + (chr - 'A' + 10);
        else printf("illegal hex number...\n");
        break;
      case 10:
      case 8:
        if(chr>='0' && chr<('0'+radix)) num = num*radix + (chr - '0');
        else printf("illegal number...\n");
        break;
      default:
      break;
    }
  }
  return num;
}
