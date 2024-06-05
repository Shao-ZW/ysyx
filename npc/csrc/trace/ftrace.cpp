#include <elf.h>
#include "common.h"

#define MAX_FTRACE_NUM   500
#define MAX_FUNC_NUM     100
#define MAX_FUNCNAME_LEN 20
#define ELF_ST_TYPE(x) MUXDEF(CONFIG_ISA64, ELF64_ST_TYPE(x), ELF32_ST_TYPE(x))

typedef MUXDEF(CONFIG_ISA64, Elf64_Ehdr, Elf32_Ehdr) Ehdr;
typedef MUXDEF(CONFIG_ISA64, Elf64_Shdr, Elf32_Shdr) Shdr;
typedef MUXDEF(CONFIG_ISA64, Elf64_Sym, Elf32_Sym) Sym;

typedef struct {
  char func_name[MAX_FUNCNAME_LEN];
  int func_size;
  vaddr_t func_addr;
} func_info;

typedef struct {
  int type; // 1 call 0 ret
  vaddr_t inst_addr;
  func_info* func;
} ftrace_entry;

static int func_cnt = 0;
static int ftrace_cnt = 0;
static func_info funcs[MAX_FUNC_NUM];
static ftrace_entry ftraces[MAX_FTRACE_NUM];

void init_ftrace(const char *elf_file) {
  if(elf_file == NULL)
    return;

  FILE *fp = fopen(elf_file, "rb");
  Assert(fp, "Can not open '%s'", elf_file);

  // read elf header
  Ehdr elf_header;
  int ret = fread(&elf_header, sizeof(Ehdr), 1, fp);
  assert(ret == 1);

  // read section header
  Shdr section_headers[elf_header.e_shnum];
  fseek(fp, elf_header.e_shoff, SEEK_SET);
  ret = fread(section_headers, sizeof(Shdr), elf_header.e_shnum, fp);
  assert(ret == elf_header.e_shnum);

  // find symbol table and string table
  Shdr *symtab_entry = NULL;
  Shdr *strtab_entry = NULL;
  for(int i = 0; i < elf_header.e_shnum; ++i) {
    if(section_headers[i].sh_type == SHT_SYMTAB) 
      symtab_entry = &section_headers[i];
    else if(strtab_entry == NULL && section_headers[i].sh_type == SHT_STRTAB)
      strtab_entry = &section_headers[i];
  }

  // read symbol table and get function info
  int symbols_num = symtab_entry->sh_size / sizeof(Sym);
  Sym symtab[symbols_num];
  fseek(fp, symtab_entry->sh_offset, SEEK_SET);
  ret = fread(symtab, sizeof(Sym), symbols_num, fp);
  assert(ret == symbols_num);

  for(int i = 0; i < symbols_num; ++i) {
    if(ELF_ST_TYPE(symtab[i].st_info) == STT_FUNC) {
      int name_offset = symtab[i].st_name;
      fseek(fp, strtab_entry->sh_offset + name_offset, SEEK_SET);
      ret = fscanf(fp, "%s", funcs[func_cnt].func_name);
      assert(ret == 1);
      funcs[func_cnt].func_size = symtab[i].st_size;
      funcs[func_cnt++].func_addr = symtab[i].st_value;
    }
  }

  fclose(fp);
}

void ftrace_add(int type, vaddr_t func_addr, vaddr_t inst_addr) {
  ftraces[ftrace_cnt].type = type;
  ftraces[ftrace_cnt].inst_addr = inst_addr;

  if(type == 0) { // ret
    for(int i = 0; i < func_cnt; ++i) {
      if(funcs[i].func_addr <= inst_addr && funcs[i].func_addr + funcs[i].func_size > inst_addr) {
        ftraces[ftrace_cnt++].func = &funcs[i];
        break;
      }
    }
  } else {  // call
    for(int i = 0; i < func_cnt; ++i) {
      if(funcs[i].func_addr == func_addr) {
        ftraces[ftrace_cnt++].func = &funcs[i];
        break;
      }
    }
  }
}

void ftrace_display() {
  printf(ANSI_FMT("FTRACE", ANSI_FG_YELLOW)":\n");

  int space_cnt = 0;
  for(int i = 0; i < ftrace_cnt; ++i) {
    if(ftraces[i].type == 0) {
      space_cnt--;
      printf(FMT_WORD ": %*s%s [%s]\n", ftraces[i].inst_addr, space_cnt, "",
      "ret", ftraces[i].func->func_name);
    }
    else {
      printf(FMT_WORD ": %*s%s [%s@" FMT_WORD "]\n", ftraces[i].inst_addr, space_cnt, "",
      "call", ftraces[i].func->func_name, ftraces[i].func->func_addr);
      space_cnt++;
    }
  }
}
