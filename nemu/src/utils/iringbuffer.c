#include <string.h>
#include <stdio.h>

struct{
  int start;
  int end;
  char buf[20][128];
  int is_empty;
} iringbuffer;

void init_iringbuffer() {
    iringbuffer.start = iringbuffer.end = 0;
    iringbuffer.is_empty = 1;
}

void iringbuffer_write(const char* s) {
  strcpy(iringbuffer.buf[iringbuffer.end], s);
  if(!iringbuffer.is_empty && iringbuffer.end == iringbuffer.start)
    iringbuffer.start = (iringbuffer.start + 1) % 20;
  iringbuffer.end = (iringbuffer.end + 1) % 20;
  iringbuffer.is_empty = 0;
}

void iringbuffer_display() {
  int idx = iringbuffer.start;
  do {
    printf("%s\n", iringbuffer.buf[idx]);
    idx = (idx + 1) % 20;
  } while(idx != iringbuffer.end);
}