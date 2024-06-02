#include <cstdint>

#define MEMORY_SIZE 4096

uint32_t pmem[MEMORY_SIZE] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)
  0x0102c503,  // lbu a0,16(t0)
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data 
};  // built-in image

#define CONFIG_MBASE 0x80000000

uint32_t guest_to_host(uint32_t addr) { return addr - CONFIG_MBASE; }

extern "C" int pmem_read(int raddr) {
  // 总是读取地址为`raddr & ~0x3u`的4字节返回
  return pmem[guest_to_host(raddr) & ~0x3u];
}

extern "C" void pmem_write(int waddr, int wdata, char wmask) {
  // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变

  int aligned_addr = guest_to_host(waddr) & ~0x3u;
  int current_data = pmem[aligned_addr / 4];

  for (int i = 0; i < 4; i++) {
    if (wmask & (1 << i)) {
      current_data = (current_data & ~(0xFF << (i * 8))) | (wdata & (0xFF << (i * 8)));
    }
  }
  
  pmem[aligned_addr / 4] = current_data;
}

// void init_mem() {
//   IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
//   Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
// }


