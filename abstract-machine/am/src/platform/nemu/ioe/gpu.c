#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  int vdata = inl(VGACTL_ADDR);
  int width = vdata >> 16;
  int height = vdata & 0xFFFF;
  int vmemsz = width * height * sizeof(uint32_t);

  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = width, .height = height,
    .vmemsz = vmemsz
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y;
  int w = ctl->w, h = ctl->h;
  int width = inl(VGACTL_ADDR) >> 16;
  uint32_t *pixels = (uint32_t*)ctl->pixels;

  int px, py;
  for(int j = 0; j < h; ++j) {
    for(int i = 0; i < w; ++i) {
      px = x + i;
      py = y + j;
      outl(FB_ADDR + (py * width + px) * sizeof(uint32_t), pixels[j * w + i]);
    }
  }

  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
