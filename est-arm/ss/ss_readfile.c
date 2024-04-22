/*******************************************************************************
 *
 * Chronos: A Timing Analyzer for Embedded Software
 * =============================================================================
 * http://www.comp.nus.edu.sg/~rpembed/chronos/
 *
 * Copyright (C) 2005 Xianfeng Li
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * $Id: ss_readfile.c,v 1.2 2006/06/24 08:55:06 lixianfe Exp $
 *
 ******************************************************************************/

#include <stdio.h>
#include <errno.h>

#include "ecoff.h"
#include "symbol.h"
#include "../cfg.h"
#include "../common.h"

char *load_elf_strtabl(FILE *file, long offset, size_t size);
void decode_inst(de_inst_t *de_inst, md_inst_t inst);
void dump_syms(struct sym_sym_t **syms, int size);

// variables from readfile.c
extern prog_t prog;
addr_t *procs_addr;
int num_procs;

/* amount of tail padding added to all loaded text segments */
#define TEXT_TAIL_PADDING 128

/* program text (code) segment base */
md_addr_t ld_text_base = 0;
/* program text (code) size in bytes */
unsigned int ld_text_size = 0;
unsigned text_entry;
unsigned text_offset;
extern struct sym_sym_t **sym_textsyms;
extern int sym_ntextsyms;

// locate the start & end addr of the user code by excluding library functions,
// in addition, locate the main() entrance
static void
lookup_addr(char *fname)
{
  int i, size = 0, max_num_procs = 16;
  struct sym_sym_t *sym;

  sym_loadsyms(fname, 1);
  // dump_syms(sym_textsyms, sym_ntextsyms);
  // sym_dumptextsyms(stdout, 1);
  prog.start_addr = sym_textsyms[0]->addr;
  procs_addr = (md_addr_t *)calloc(max_num_procs, sizeof(md_addr_t));
  for (i = 0; i < sym_ntextsyms; i++)
  {
    sym = sym_textsyms[i];
    if (strcmp(sym->name, "_start") == 0)
    {
      prog.entry_addr = sym->addr;
    }
    if (num_procs >= max_num_procs)
    {
      max_num_procs *= 2;
      procs_addr = (md_addr_t *)realloc(procs_addr,
                                        max_num_procs * sizeof(md_addr_t));
    }
    procs_addr[num_procs++] = sym->addr;
    prog.code_size += sym->size;
  }
  prog.end_addr = prog.start_addr + prog.code_size;
}

// locate & read the text header of the program
// static void
// read_text_head(FILE *fp)
// {
//     long		    pos;
//     struct ecoff_filehdr    fhdr;
//     struct ecoff_aouthdr    ahdr;
//     struct ecoff_scnhdr	    shdr;
//     unsigned		    text_size;
//     int			    i;
//     enum md_opcode	    op;

//     md_init_decoder();

//     // locate the text section
//     fseek(fp, 0, SEEK_SET);
//     fread(&fhdr, sizeof(fhdr), 1, fp);
//     fread(&ahdr, sizeof(ahdr), 1, fp);
//     for (i=0; i<fhdr.f_nscns; i++) {
// 	fread(&shdr, sizeof(shdr), 1, fp);
// 	if (shdr.s_flags != ECOFF_STYP_TEXT)
// 	    continue;
// 	text_size = shdr.s_size;
// 	text_offset = shdr.s_scnptr;
// 	text_entry = shdr.s_vaddr;
// 	ld_text_size = ((shdr.s_vaddr + shdr.s_size) - MD_TEXT_BASE)
// 	    + TEXT_TAIL_PADDING;
//     }
//     text_offset += prog.start_addr - text_entry;
//     ld_text_base = MD_TEXT_BASE;
// }

// locate & read the text header of the program

static void read_text_head(FILE *file)
{
  struct elf_filehdr ehdr;  // elf 文件头
  struct elf_scnhdr scnhdr; // 段表头部
  int read_count = 0;
  int section_index = 0;    // 段下标
  int nums_of_sections = 0; // 段的数目
  int text_sec_index = 0;   // .text 段在段表中的下标
  errno = 0;
  char *secstrtbl = NULL; // 段表字符串表
  char *strtb = NULL;     // 字符串表
  int strtb_count = 0;
  unsigned text_size = 0;
  // 读取 elf 文件的文件头
  if (fread(&ehdr, sizeof(struct elf_filehdr), 1, file) < 1)
  {
    printf("read elf header filaed: %s\n", strerror(errno));
    return;
  }

  nums_of_sections = ehdr.e_shnum;
  // 定位到段表的开始处
  if (fseek(file, (long)ehdr.e_shoff, SEEK_SET) != 0)
  {
    printf("seek elf section filaed: %s\n", strerror(errno));
    return;
  }
  // 读取段表字符串表
  while (read_count = fread(&scnhdr, sizeof(struct elf_scnhdr), 1, file))
  {
    if (read_count < 1)
    {
      printf("read elf section filaed: %s\n", strerror(errno));
      return;
    }
    if (section_index == ehdr.e_shstrndx)
    {
      secstrtbl = load_elf_strtabl(file, scnhdr.sh_offset, scnhdr.sh_size);
      break;
    }
    section_index++;
  }
  assert(secstrtbl);
  fseek(file, (long)ehdr.e_shoff, SEEK_SET);
  section_index = -1;
  // 循环读取段表中各个段的信息，查找到 .text 退出循环；
  while ((section_index++) < nums_of_sections - 1)
  {
    read_count = fread(&scnhdr, sizeof(struct elf_scnhdr), 1, file);
    if (read_count < 1)
    {
      printf("read elf section filaed: %s\n", strerror(errno));
      return;
    }
    if (scnhdr.sh_type == SHT_PROGBITS &&
        strcmp(secstrtbl + scnhdr.sh_name, ".text") == 0)
    {
      text_sec_index = section_index;
      break;
    }
  }
  text_size = scnhdr.sh_size;
  text_offset = scnhdr.sh_offset;
  text_entry = scnhdr.sh_addr;
  ld_text_size = ((scnhdr.sh_addr + scnhdr.sh_size) - MD_TEXT_BASE) + TEXT_TAIL_PADDING;
  text_offset += prog.start_addr - text_entry;
  ld_text_base = MD_TEXT_BASE;
}

// read an instruction from the object file, decode it and store the decoded instr in
// the ith entry of the decode instruction sequence
static void
read_inst(FILE *fp, addr_t addr, int i)
{
  md_inst_t inst;
  int op;

  fread(&inst, sizeof(inst), 1, fp);
  prog.code[i].addr = addr;
  decode_inst(&prog.code[i], inst);
}

#define NOP 0
// read the obj code, decode the inst and store them in prog
void read_code_ss(char *fname)
{
  FILE *fp;
  int i;
  addr_t addr;

  lookup_addr(fname);
  fp = fopen(fname, "rb");
  md_init_decoder();
  read_text_head(fp);
  prog.num_inst = prog.code_size / sizeof(md_inst_t);
  prog.code = (de_inst_t *)calloc(prog.num_inst, sizeof(de_inst_t));
  CHECK_MEM(prog.code);
  addr = prog.start_addr;
  // read the text
  fseek(fp, text_offset, SEEK_SET);
  for (i = 0; i < prog.num_inst; i++)
  {
    read_inst(fp, addr, i);
    addr += sizeof(md_inst_t);
  }
  // the linker may leave an nop instr at the end of the user object text for
  // aligning purpose, e.g, if the text ends at 0x400308, then an nop is inserted
  // at 0x400308, and the library code starts at 0x400310; if this happens, the
  // CFG will be awkard, so we leave it out and adjust the end_addr, prog size etc.
  if (prog.code[prog.num_inst - 1].op_enum == NOP)
  {
    prog.num_inst--;
    prog.code_size -= sizeof(md_inst_t);
    prog.end_addr -= sizeof(md_inst_t);
  }

  fclose(fp);
}
