#include <cstdint>
#include <cstring>

#define MEMORY_SIZE 4096
#define paddr_t uint32_t

static const uint32_t img [] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)
  0x0102c503,  // lbu a0,16(t0)
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data 
};  // built-in image

uint8_t pmem[MEMORY_SIZE];

#define CONFIG_MBASE 0x80000000

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }

extern "C" uint32_t pmem_read(paddr_t raddr) {
  // 总是读取地址为`raddr & ~0x3u`的4字节返回

  return *(uint32_t*)guest_to_host(raddr & ~0x3u);
}

extern "C" void pmem_write(paddr_t waddr, uint32_t wdata, char wmask) {
  // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变

  uint8_t* p = guest_to_host(waddr & ~0x3u);

  for (int i = 0; i < 4; i++) {
    if (wmask & 1) {
      *p = (uint8_t)(wdata & 0xFF); 
    }
    wdata >>= 8;
    wmask <<= 1;
    p++;
  }
}

#define RESET_VECTOR 0x80000000

void init_mem() {
  //IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));
  //Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}


