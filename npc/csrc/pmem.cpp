#include <cstdint>

#define MEMORY_SIZE 1024
int memory[MEMORY_SIZE];

extern "C" int pmem_read(int raddr) {
  // 总是读取地址为`raddr & ~0x3u`的4字节返回
  return memory[raddr & ~0x3u];
}

extern "C" void pmem_write(int waddr, int wdata, char wmask) {
  // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变

    int aligned_addr = waddr & ~0x3u;

    int current_data = memory[aligned_addr / 4];

    for (int i = 0; i < 4; i++) {
        if (wmask & (1 << i)) {
            current_data = (current_data & ~(0xFF << (i * 8))) | (wdata & (0xFF << (i * 8)));
        }
    }
    
    memory[aligned_addr / 4] = current_data;
}

void load_img() {
  if (img_file == NULL) {
    Log("No image is given. Use the default build-in image.");
    return 4096; // built-in image size
  }

  FILE *fp = fopen(img_file, "rb");
  Assert(fp, "Can not open '%s'", img_file);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  Log("The image is %s, size = %ld", img_file, size);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(guest_to_host(RESET_VECTOR), size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}

