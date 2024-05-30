#include <string.h>
#include <stdio.h>

struct{
  int start;
  int end;
  char buf[20][128];
} iringbuffer;

void init_iringbuffer() {
    iringbuffer.start = iringbuffer.end = 0;
}

void iringbuffer_write(const char* s) {
  strcpy(iringbuffer.buf[iringbuffer.end], s);
  iringbuffer.end = (iringbuffer.end + 1) % 20;
  if(iringbuffer.end == iringbuffer.start) {
    iringbuffer.start = (iringbuffer.start + 1) % 20;
  }
}

void iringbuffer_display() {
  int idx = iringbuffer.start;
  do {
    printf("%s\n", iringbuffer.buf[idx]);
    idx = (idx + 1) % 20;
  } while(idx != iringbuffer.end);
  printf("%s  <--\n", iringbuffer.buf[iringbuffer.end]);
}