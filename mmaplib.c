#include <sys/mman.h>   
#include <stdio.h>   
#include <fcntl.h>   
#include <sys/types.h>   
#include <sys/stat.h>   
#include <unistd.h>   
#define IOREG_BASE  (0x1fe00000)   
#define SYSTEM_LEN  0x2000   
#define DEV_MEM_CTL   "/dev/mem"   
static int dev_mem = -1;  
typedef volatile unsigned int vuint_t;
typedef volatile unsigned char vuchar_t;
vuchar_t *pMem_map = NULL;  /*mmap address for cfgreg*/  
vuint_t  *GPIO_DATA = NULL;
vuint_t  *GPIO_EN   = NULL;
vuchar_t *SPI_BASE  = NULL;
int mem_open(void)  
{  
  if(dev_mem < 0){  
    dev_mem = open(DEV_MEM_CTL, O_RDWR|O_SYNC);  
    if(dev_mem < 0){  
      printf("open %s error/n", DEV_MEM_CTL);  
      return -1;  
    }  
  }  
  return 0;  
}  
int mmap_init(void)  
{  
  unsigned long phyaddr = IOREG_BASE;  
   if(pMem_map == NULL){  
    pMem_map = (vuchar_t*)mmap((void *)phyaddr, SYSTEM_LEN,
              PROT_READ|PROT_WRITE|PROT_EXEC, MAP_SHARED,  
              dev_mem, phyaddr);  
//    if(pMem_map != (void *)phyaddr){  
//      printf("mem failed/n");  
//      return -1;  
//    }  
    printf("pMem_map = %x\n", pMem_map);
    GPIO_DATA = (vuint_t*)(pMem_map+0x11c);
    GPIO_EN   = (vuint_t*)(pMem_map+0x120);
    SPI_BASE  = pMem_map+0x1f0;
  }  
  else{  
    return 0;  
  }  
}
static int mmap_free(void)  
{  
  if(pMem_map){  
    munmap((void*)pMem_map, SYSTEM_LEN);  
    pMem_map = NULL;  
  }  
}  
static void mem_close(void)  
{  
  if(dev_mem > 0) {  
    close(dev_mem);  
    dev_mem = -1;  
  }  
}  
void mem_free_all(void)  
{  
  mmap_free();    
  mem_close();  
}  
/*
int main(int argc, char *argv[])  
{  
  int ret = -1;   
  unsigned long phr_add = 1;  
  if((ret = mem_open()) < 0)  
  {  
    return -1;  
  }  
  ret = mmap_init();  
  if(ret < 0)  
  {  
    return -1;  
  }  
  ret = ReadRegister(1);  
  if(ret < 0)  
  {  
    return -1;  
  }  
  mem_free_all();  
  return 0;  
}   
*/

