/*
 * symbol.c - program symbol and line data routines
 *
 * This file is a part of the SimpleScalar tool suite written by
 * Todd M. Austin as a part of the Multiscalar Research Project.
 *
 * The tool suite is currently maintained by Doug Burger and Todd M. Austin.
 *
 * Copyright (C) 1994, 1995, 1996, 1997 by Todd M. Austin
 *
 * This source file is distributed "as is" in the hope that it will be
 * useful.  The tool set comes with no warranty, and no author or
 * distributor accepts any responsibility for the consequences of its
 * use.
 *
 * Everyone is granted permission to copy, modify and redistribute
 * this tool set under the following conditions:
 *
 *    This source code is distributed for non-commercial use only.
 *    Please contact the maintainer for restrictions applying to
 *    commercial use.
 *
 *    Permission is granted to anyone to make or distribute copies
 *    of this source code, either as received or modified, in any
 *    medium, provided that all copyright notices, permission and
 *    nonwarranty notices are preserved, and that the distributor
 *    grants the recipient permission for further redistribution as
 *    permitted by this document.
 *
 *    Permission is granted to distribute this file in compiled
 *    or executable form under the same conditions that apply for
 *    source code, provided that either:
 *
 *    A. it is accompanied by the corresponding machine-readable
 *       source code,
 *    B. it is accompanied by a written offer, with no time limit,
 *       to give anyone a machine-readable copy of the corresponding
 *       source code in return for reimbursement of the cost of
 *       distribution.  This written offer must permit verbatim
 *       duplication by anyone, or
 *    C. it is distributed by someone who received only the
 *       executable form, and is accompanied by a copy of the
 *       written offer of source code that they received concurrently.
 *
 * In other words, you are welcome to use, share and improve this
 * source file.  You are forbidden to forbid anyone else to use, share
 * and improve what you give them.
 *
 * INTERNET: dburger@cs.wisc.edu
 * US Mail:  1210 W. Dayton Street, Madison, WI 53706
 *
 * $Id: symbol.c,v 1.1.1.1 2000/11/29 14:53:54 cu-cs Exp $
 *
 * $Log: symbol.c,v $
 * Revision 1.1.1.1  2000/11/29 14:53:54  cu-cs
 * Grand unification of arm sources.
 *
 *
 * Revision 1.1.2.1  2000/07/21 18:35:00  taustin
 * More progress on the SimpleScalar/ARM target.
 *
 * Revision 1.1.1.1  2000/05/26 15:22:27  taustin
 * SimpleScalar Tool Set
 *
 *
 * Revision 1.1  1998/08/27 16:54:29  taustin
 * Initial revision
 *
 * Revision 1.1  1998/05/06  01:08:39  calder
 * Initial revision
 *
 * Revision 1.2  1997/04/16  22:11:50  taustin
 * added standalone loader support
 *
 * Revision 1.1  1997/03/11  01:34:45  taustin
 * Initial revision
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#include "host.h"
#include "misc.h"
#include "loader.h"
#include "symbol.h"
// #include "elf.h"

/* symbol database in no particular order */
struct sym_sym_t *sym_db = NULL;

/* all symbol sorted by address */
int sym_nsyms = 0;
struct sym_sym_t **sym_syms = NULL;

/* all symbols sorted by name */
struct sym_sym_t **sym_syms_by_name = NULL;

/* text symbols sorted by address */
int sym_ntextsyms = 0;
struct sym_sym_t **sym_textsyms = NULL;

/* text symbols sorted by name */
struct sym_sym_t **sym_textsyms_by_name = NULL;

/* data symbols sorted by address */
int sym_ndatasyms = 0;
struct sym_sym_t **sym_datasyms = NULL;

/* data symbols sorted by name */
struct sym_sym_t **sym_datasyms_by_name = NULL;

/* symbols loaded? */
static int syms_loaded = FALSE;

#ifdef PRINT_SYMS
/* convert BFD symbols flags to a printable string */
static char *                 /* symbol flags string */
flags2str(unsigned int flags) /* bfd symbol flags */
{
  static char buf[256];
  char *p;

  if (!flags)
    return "";

  p = buf;
  *p = '\0';

  if (flags & BSF_LOCAL)
  {
    *p++ = 'L';
    *p++ = '|';
  }
  if (flags & BSF_GLOBAL)
  {
    *p++ = 'G';
    *p++ = '|';
  }
  if (flags & BSF_DEBUGGING)
  {
    *p++ = 'D';
    *p++ = '|';
  }
  if (flags & BSF_FUNCTION)
  {
    *p++ = 'F';
    *p++ = '|';
  }
  if (flags & BSF_KEEP)
  {
    *p++ = 'K';
    *p++ = '|';
  }
  if (flags & BSF_KEEP_G)
  {
    *p++ = 'k';
    *p++ = '|';
  }
  if (flags & BSF_WEAK)
  {
    *p++ = 'W';
    *p++ = '|';
  }
  if (flags & BSF_SECTION_SYM)
  {
    *p++ = 'S';
    *p++ = '|';
  }
  if (flags & BSF_OLD_COMMON)
  {
    *p++ = 'O';
    *p++ = '|';
  }
  if (flags & BSF_NOT_AT_END)
  {
    *p++ = 'N';
    *p++ = '|';
  }
  if (flags & BSF_CONSTRUCTOR)
  {
    *p++ = 'C';
    *p++ = '|';
  }
  if (flags & BSF_WARNING)
  {
    *p++ = 'w';
    *p++ = '|';
  }
  if (flags & BSF_INDIRECT)
  {
    *p++ = 'I';
    *p++ = '|';
  }
  if (flags & BSF_FILE)
  {
    *p++ = 'f';
    *p++ = '|';
  }

  if (p == buf)
    panic("no flags detected");

  *--p = '\0';
  return buf;
}
#endif /* PRINT_SYMS */

/* qsort helper function */
static int
acmp(struct sym_sym_t **sym1, struct sym_sym_t **sym2)
{
  return (int)((*sym1)->addr - (*sym2)->addr);
}

/* qsort helper function */
static int
ncmp(struct sym_sym_t **sym1, struct sym_sym_t **sym2)
{
  return strcmp((*sym1)->name, (*sym2)->name);
}

#define RELEVANT_SCOPE(SYM)                                                                                                   \
  (                                /* global symbol */                                                                        \
   ((SYM)->flags & BSF_GLOBAL) || (/* local symbol */                                                                         \
                                   (((SYM)->flags & (BSF_LOCAL | BSF_DEBUGGING)) == BSF_LOCAL) && (SYM)->name[0] != '$') ||   \
   (                 /* compiler local */                                                                                     \
    load_locals && ((/* basic block idents */                                                                                 \
                     ((SYM)->flags & (BSF_LOCAL | BSF_DEBUGGING)) == (BSF_LOCAL | BSF_DEBUGGING) && (SYM)->name[0] == '$') || \
                    (/* local constant idents */                                                                              \
                     ((SYM)->flags & (BSF_LOCAL | BSF_DEBUGGING)) == (BSF_LOCAL) && (SYM)->name[0] == '$'))))

#if 0
/* load symbols out of FNAME */
void
sym_loadsyms(char *fname,	/* file name containing symbols */
	     int load_locals)	/* load local symbols */
{
#if 1 // XXX
  int i, debug_cnt;
  int len;
  FILE *fobj;
  struct ecoff_filehdr fhdr;
  struct ecoff_aouthdr ahdr;
  struct ecoff_symhdr_t symhdr;
  char *strtab = NULL;
  struct ecoff_EXTR *extr;

  if (syms_loaded)
    {
      /* symbols are already loaded */
      /* FIXME: can't handle symbols from multiple files */
      return;
    }

  errno = 0;
  /* load the program into memory, try both endians */
#if defined(__CYGWIN32__) || defined(_MSC_VER)
  fobj = fopen(fname, "rb");
#else
  fobj = fopen(fname, "rb");
#endif
	printf("%s\n",strerror(errno));
  if (!fobj)
    fatal("cannot open executable `%s'", fname);
 if (fread(&fhdr, sizeof(struct ecoff_filehdr), 1, fobj) < 1)
    fatal("cannot read header from executable `%s'", fname);
	
  /* little-endian of target */
  if (fhdr.f_magic != 0x457f && fhdr.f_nscns != 0x464c)
    fatal("bad magic number in executable `%s'", fname);

  if (fread(&ahdr, sizeof(struct ecoff_aouthdr), 1, fobj) < 1)
    fatal("cannot read AOUT header from executable `%s'", fname);

  /* seek to the beginning of the symbolic header */
  fseek(fobj, (long)fhdr.f_symptr, 0);

  if (fread(&symhdr, sizeof(struct ecoff_symhdr_t), 1, fobj) < 1)
    fatal("could not read symbolic header from executable `%s'", fname);

  if (symhdr.magic != ECOFF_magicSym)
    fatal("bad magic number (0x%x) in symbolic header", symhdr.magic);

  /* allocate space for the string table */
  len = symhdr.issMax + symhdr.issExtMax;
  strtab = (char *)calloc(len, sizeof(char));
  if (!strtab)
    fatal("out of virtual memory");

  /* read all the symbol names into memory */
  fseek(fobj, (long)symhdr.cbSsExtOffset /* cbSsOffset */, 0);
  if (fread(strtab, len, 1, fobj) < 0)
    fatal("error while reading symbol table names");

  /* allocate symbol space */
  len = symhdr.isymMax + symhdr.iextMax;
  if (len <= 0)
    fatal("`%s' has no text or data symbols", fname);
  sym_db = (struct sym_sym_t *)calloc(len, sizeof(struct sym_sym_t));
  if (!sym_db)
    fatal("out of virtual memory");

  /* allocate space for the external symbol entries */
  extr =
    (struct ecoff_EXTR *)calloc(symhdr.iextMax, sizeof(struct ecoff_EXTR));
  if (!extr)
    fatal("out of virtual memory");

  fseek(fobj, (long)symhdr.cbExtOffset, 0);
  if (fread(extr, sizeof(struct ecoff_EXTR), symhdr.iextMax, fobj) < 0)
    fatal("error reading external symbol entries");

  sym_nsyms = 0; sym_ndatasyms = 0; sym_ntextsyms = 0;

  /* convert symbols to internal format */
  for (i=0; i < symhdr.iextMax; i++)
    {
      int str_offset;

      str_offset = symhdr.issMax + extr[i].asym.iss;

#if 0
      printf("ext %2d: ifd = %2d, iss = %3d, value = %8x, st = %3x, "
	     "sc = %3x, index = %3x\n",
	     i, extr[i].ifd,
	     extr[i].asym.iss, extr[i].asym.value,
	     extr[i].asym.st, extr[i].asym.sc,
	     extr[i].asym.index);
      printf("       %08x %2d %2d %s\n",
	     extr[i].asym.value,
	     extr[i].asym.st,
	     extr[i].asym.sc,
	     &strtab[str_offset]);
#endif

      switch (extr[i].asym.st)
	{
	case ECOFF_stGlobal:
	case ECOFF_stStatic:
	  /* from data segment */
	  sym_db[sym_nsyms].name = mystrdup(&strtab[str_offset]);
	  sym_db[sym_nsyms].seg = ss_data;
	  sym_db[sym_nsyms].initialized = /* FIXME: ??? */TRUE;
	  sym_db[sym_nsyms].pub = /* FIXME: ??? */TRUE;
	  sym_db[sym_nsyms].local = /* FIXME: ??? */FALSE;
	  sym_db[sym_nsyms].addr = extr[i].asym.value;
	  sym_nsyms++;
	  sym_ndatasyms++;
	  break;

	case ECOFF_stProc:
	case ECOFF_stStaticProc:
	case ECOFF_stLabel:
	  /* from text segment */
	  sym_db[sym_nsyms].name = mystrdup(&strtab[str_offset]);
	  sym_db[sym_nsyms].seg = ss_text;
	  sym_db[sym_nsyms].initialized = /* FIXME: ??? */TRUE;
	  sym_db[sym_nsyms].pub = /* FIXME: ??? */TRUE;
	  sym_db[sym_nsyms].local = /* FIXME: ??? */FALSE;
	  sym_db[sym_nsyms].addr = extr[i].asym.value;
	  sym_nsyms++;
	  sym_ntextsyms++;
	  break;

	default:
	  /* FIXME: ignored... */;
#if 0
	  fprintf(stderr, "** skipping: %s...\n", &strtab[str_offset]);
	  break;
#endif
	}
    }
  free(extr);

  /* done with the executable, close it */
  if (fclose(fobj))
    fatal("could not close executable `%s'", fname);

  /*
   * generate various sortings
   */

  /* all symbols sorted by address and name */
  sym_syms =
    (struct sym_sym_t **)calloc(sym_nsyms, sizeof(struct sym_sym_t *));
  if (!sym_syms)
    fatal("out of virtual memory");

  sym_syms_by_name =
    (struct sym_sym_t **)calloc(sym_nsyms, sizeof(struct sym_sym_t *));
  if (!sym_syms_by_name)
    fatal("out of virtual memory");

  for (debug_cnt=0, i=0; i<sym_nsyms; i++)
    {
      sym_syms[debug_cnt] = &sym_db[i];
      sym_syms_by_name[debug_cnt] = &sym_db[i];
      debug_cnt++;
    }
  /* sanity check */
  if (debug_cnt != sym_nsyms)
    panic("could not locate all symbols");

  /* sort by address */
  qsort(sym_syms, sym_nsyms, sizeof(struct sym_sym_t *), (void *)acmp);

  /* sort by name */
  qsort(sym_syms_by_name, sym_nsyms, sizeof(struct sym_sym_t *), (void *)ncmp);

  /* text segment sorted by address and name */
  sym_textsyms =
    (struct sym_sym_t **)calloc(sym_ntextsyms, sizeof(struct sym_sym_t *));
  if (!sym_textsyms)
    fatal("out of virtual memory");

  sym_textsyms_by_name =
    (struct sym_sym_t **)calloc(sym_ntextsyms, sizeof(struct sym_sym_t *));
  if (!sym_textsyms_by_name)
    fatal("out of virtual memory");

  for (debug_cnt=0, i=0; i<sym_nsyms; i++)
    {
      if (sym_db[i].seg == ss_text)
	{
	  sym_textsyms[debug_cnt] = &sym_db[i];
	  sym_textsyms_by_name[debug_cnt] = &sym_db[i];
	  debug_cnt++;
	}
    }
  /* sanity check */
  if (debug_cnt != sym_ntextsyms)
    panic("could not locate all text symbols");

  /* sort by address */
  qsort(sym_textsyms, sym_ntextsyms, sizeof(struct sym_sym_t *), (void *)acmp);

  /* sort by name */
  qsort(sym_textsyms_by_name, sym_ntextsyms,
	sizeof(struct sym_sym_t *), (void *)ncmp);

  /* data segment sorted by address and name */
  sym_datasyms =
    (struct sym_sym_t **)calloc(sym_ndatasyms, sizeof(struct sym_sym_t *));
  if (!sym_datasyms)
    fatal("out of virtual memory");

  sym_datasyms_by_name =
    (struct sym_sym_t **)calloc(sym_ndatasyms, sizeof(struct sym_sym_t *));
  if (!sym_datasyms_by_name)
    fatal("out of virtual memory");

  for (debug_cnt=0, i=0; i<sym_nsyms; i++)
    {
      if (sym_db[i].seg == ss_data)
	{
	  sym_datasyms[debug_cnt] = &sym_db[i];
	  sym_datasyms_by_name[debug_cnt] = &sym_db[i];
	  debug_cnt++;
	}
    }
  /* sanity check */
  if (debug_cnt != sym_ndatasyms)
    panic("could not locate all data symbols");
      
  /* sort by address */
  qsort(sym_datasyms, sym_ndatasyms, sizeof(struct sym_sym_t *), (void *)acmp);

  /* sort by name */
  qsort(sym_datasyms_by_name, sym_ndatasyms,
	sizeof(struct sym_sym_t *), (void *)ncmp);

  /* compute symbol sizes */
  for (i=0; i<sym_ntextsyms; i++)
    {
      sym_textsyms[i]->size =
	(i != (sym_ntextsyms - 1)
	 ? (sym_textsyms[i+1]->addr - sym_textsyms[i]->addr)
	 : ((ld_text_base + ld_text_size) - sym_textsyms[i]->addr));
    }
#if 0
  for (i=0; i<sym_ndatasyms; i++)
    {
      sym_datasyms[i]->size =
	(i != (sym_ndatasyms - 1)
	 ? (sym_datasyms[i+1]->addr - sym_datasyms[i]->addr)
	 : ((ld_data_base + ld_data_size) - sym_datasyms[i]->addr));
    }
#endif
  /* symbols are now available for use */
  syms_loaded = TRUE;
#endif
}
#endif

///////////////////////////////////////////////////////////////////
// ARM ELF
///////////////////////////////////////////////////////////////////
// 将符号表读取出来；
// 该函数将字符串表从文件中读取出来；

void dump_syms(struct sym_sym_t **syms, int size);
void sym_loadsyms__elf(FILE *file);
void sym_alloc_sort(struct sym_sym_t ***sym_datasyms,
                    struct sym_sym_t ***sym_datasyms_by_name, int sym_ndatasyms, enum sym_seg_t seg);

char *load_elf_strtabl(FILE *file, long offset, size_t size)
{
  char *temp = malloc(sizeof(char) + size);
  long seek = ftell(file);
  fseek(file, offset, SEEK_SET);
  if (fread(temp, size, 1, file) < 1)
  {
    printf("read elf string table filaed: %s\n", strerror(errno));
    return NULL;
  }
  fseek(file, seek, SEEK_SET);
  return temp;
}

char *strcopy2heap(char *s)
{
  char *buf;
  if (!(buf = (char *)malloc(strlen(s) + 1)))
    return NULL;
  strcpy(buf, s);
  return buf;
}

void sym_loadsyms(char *filename, int local)
{
  if (syms_loaded)
  {
    /* symbols are already loaded */
    /* FIXME: can't handle symbols from multiple files */
    return;
  }

  FILE *fobj = fopen(filename, "rb");
  if (!fobj)
  {
    printf(" %s\n", strerror(errno));
  }
  sym_loadsyms__elf(fobj);
  // 关闭可执行文件
  if (fclose(fobj))
  {
  }
  syms_loaded = TRUE;
}

void sym_loadsyms__elf(FILE *file)
{

  struct elf_filehdr ehdr;  // elf 文件头
  struct elf_scnhdr scnhdr; // 段表头部
  struct Elf32_Sym symtb;   // 符号表
  int read_count = 0;
  long symtb_offset = 0;    // 符号表文件偏移
  int section_index = 0;    // 段下标
  int nums_of_sections = 0; // 段的数目
  int data_sec_index = 0;   // .data 段在段表中的下标
  int text_sec_index = 0;   // .text 段在段表中的下标
  errno = 0;
  char *secstrtbl = NULL; // 段表字符串表
  char *strtb = NULL;     // 字符串表
  int strtb_count = 0;
  struct elf_scnhdr symtbl_hdr; // 段表头部

  int debug_cnt = 0;
  int i = 0;

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
  // 循环读取段表中各个段的信息，查找到符号表和字符串表后退出循环；
  while (section_index++ < nums_of_sections - 1)
  {
    read_count = fread(&scnhdr, sizeof(struct elf_scnhdr), 1, file);
    if (read_count < 1)
    {
      printf("read elf section filaed: %s\n", strerror(errno));
      return;
    }
    // 记录符号表段头
    if (scnhdr.sh_type == SHT_SYMTAB &&
        strcmp(secstrtbl + scnhdr.sh_name, ".symtab") == 0)
    {
      symtbl_hdr = scnhdr;
      symtb_offset = scnhdr.sh_offset;
    }
    // 读取字符串表
    if (scnhdr.sh_type == SHT_STRTAB &&
        strcmp(secstrtbl + scnhdr.sh_name, ".strtab") == 0)
    {
      strtb = load_elf_strtabl(file, scnhdr.sh_offset, scnhdr.sh_size);
    }
    if (strcmp(secstrtbl + scnhdr.sh_name, ".data") == 0)
    {
      data_sec_index = section_index;
    }
    if (strcmp(secstrtbl + scnhdr.sh_name, ".text") == 0)
    {
      text_sec_index = section_index;
    }
    // 读取成功则退出循环；
    // if(symtb_offset && strtb) break;
  }
  assert(symtb_offset && strtb);
  // 定位到符号表的开始处
  if (fseek(file, symtb_offset, SEEK_SET) != 0)
  {
    printf("seek symbol table filaed: %s\n", strerror(errno));
    return;
  }
  // 计算符号表中符号的数目,并为符号表分配空间；
  read_count = symtbl_hdr.sh_size / symtbl_hdr.sh_entsize;
  sym_db = (struct sym_sym_t *)calloc(read_count, sizeof(struct sym_sym_t));
  // 读取符号表
  while (read_count--)
  {
    if (fread(&symtb, sizeof(struct Elf32_Sym), 1, file) < 1)
    {
      printf("read elf symbol table filaed: %s\n", strerror(errno));
      return;
    }
    // TODO:将所有的符号解析到相应的数据结构中
    // 符号所属的段
    if (symtb.st_shndx == SHN_ABS ||
        symtb.st_shndx == SHN_COMMON ||
        symtb.st_shndx == SHN_UNDEF)
    {
      continue;
    }
    // if(SYM_TYPE(symtb.st_info) == STT_OBJECT){
    if (data_sec_index == symtb.st_shndx)
    {
      sym_db[sym_nsyms].name = strcopy2heap(strtb + symtb.st_name);
      sym_db[sym_nsyms].seg = ss_data;
      // 符号的地址
      sym_db[sym_nsyms].addr = symtb.st_value;
      // 符号的大小
      sym_db[sym_nsyms].size = symtb.st_size;
      // 符号是否为局部变量
      sym_db[sym_nsyms].local = SYM_INFO(symtb.st_info);
      sym_db[sym_nsyms].initialized = TRUE;
      sym_db[sym_nsyms].pub = TRUE;
      sym_ndatasyms++;
      sym_nsyms++;
      //}else if(SYM_TYPE(symtb.st_info) == STT_FUNC){
    }
    else if (text_sec_index == symtb.st_shndx && (symtb.st_info & 0xf) == 2)
    {
      sym_db[sym_nsyms].name = strcopy2heap(strtb + symtb.st_name);
      sym_db[sym_nsyms].seg = ss_text;
      // 符号的地址
      sym_db[sym_nsyms].addr = symtb.st_value;
      // 符号的大小
      sym_db[sym_nsyms].size = symtb.st_size;
      // 符号是否为局部变量
      sym_db[sym_nsyms].local = SYM_INFO(symtb.st_info);
      sym_db[sym_nsyms].initialized = TRUE;
      sym_db[sym_nsyms].pub = TRUE;
      sym_ntextsyms++;
      sym_nsyms++;
    }
  }

  // 释放不在使用的堆空间；
  if (strtb)
  {
    free(strtb);
  }
  if (secstrtbl)
  {
    free(secstrtbl);
  }
  /* all symbols sorted by address */
  sym_syms =
      (struct sym_sym_t **)calloc(sym_nsyms, sizeof(struct sym_sym_t *));
  /* all symbols sorted by name */
  sym_syms_by_name =
      (struct sym_sym_t **)calloc(sym_nsyms, sizeof(struct sym_sym_t *));

  for (debug_cnt = 0, i = 0; i < sym_nsyms; i++)
  {
    sym_syms[debug_cnt] = &sym_db[i];
    sym_syms_by_name[debug_cnt] = &sym_db[i];
    debug_cnt++;
  }
  assert(debug_cnt == i);
  /* sort by address */
  qsort(sym_syms, sym_nsyms,
        sizeof(struct sym_sym_t *), (void *)acmp);
  /* sort by name */
  qsort(sym_syms_by_name, sym_nsyms,
        sizeof(struct sym_sym_t *), (void *)ncmp);

  /* allocate space and sort by name/addr for .text and .data symbols*/
  sym_alloc_sort(&sym_textsyms, &sym_textsyms_by_name, sym_ntextsyms, ss_text);
  sym_alloc_sort(&sym_datasyms, &sym_datasyms_by_name, sym_ndatasyms, ss_data);

  /* compute symbol sizes */
#if 0
  for (i=0; i<sym_ntextsyms; i++){
      sym_textsyms[i]->size =
	(i != (sym_ntextsyms - 1)
	 ? (sym_textsyms[i+1]->addr - sym_textsyms[i]->addr)
	 : ((ld_text_base + ld_text_size) - sym_textsyms[i]->addr));
    }
  syms_loaded = TRUE;
#endif

  // dump_syms(sym_syms, sym_nsyms);
  // dump_syms(sym_datasyms, sym_ndatasyms);
  // dump_syms(sym_textsyms, sym_ntextsyms);
  // dump_sym_db(sym_db);
  // exit(0);

} // end of load_symbol_table

void sym_alloc_sort(struct sym_sym_t ***sym_datasyms,
                    struct sym_sym_t ***sym_datasyms_by_name, int sym_ndatasyms, enum sym_seg_t seg)
{

  int debug_cnt = 0;
  int i = 0;
  /* data segment sorted by address and name */
  *sym_datasyms =
      (struct sym_sym_t **)calloc(sym_ndatasyms, sizeof(struct sym_sym_t *));
  *sym_datasyms_by_name =
      (struct sym_sym_t **)calloc(sym_ndatasyms, sizeof(struct sym_sym_t *));

  for (debug_cnt = 0, i = 0; i < sym_nsyms; i++)
  {
    if (sym_db[i].seg == seg)
    {
      (*sym_datasyms)[debug_cnt] = &sym_db[i];
      (*sym_datasyms_by_name)[debug_cnt] = &sym_db[i];
      debug_cnt++;
    }
  }
  assert(debug_cnt == sym_ndatasyms);
  /* sort by address */
  qsort(*sym_datasyms, sym_ndatasyms, sizeof(struct sym_sym_t *), (void *)acmp);
  /* sort by name */
  qsort(*sym_datasyms_by_name, sym_ndatasyms,
        sizeof(struct sym_sym_t *), (void *)ncmp);
}

void dump_syms(struct sym_sym_t **syms, int size)
{
  int i = 0;
  while (i < size)
  {
    printf("seg: %d | addr: 0x%x  | size: %d | name: %s\n", syms[i]->seg, syms[i]->addr, syms[i]->size, syms[i]->name);
    i++;
  }
}

////////////////////////////////////////////////////////////////////////////

/* dump symbol SYM to output stream FD */
void sym_dumpsym(struct sym_sym_t *sym, /* symbol to display */
                 FILE *fd)              /* output stream */
{
  myfprintf(fd,
            "sym `%s': %s seg, init-%s, pub-%s, local-%s, addr=0x%08p, size=%d\n",
            sym->name,
            sym->seg == ss_data ? "data" : "text",
            sym->initialized ? "y" : "n",
            sym->pub ? "y" : "n",
            sym->local ? "y" : "n",
            sym->addr,
            sym->size);
}

/* dump all symbols to output stream FD */
void sym_dumpsyms(FILE *fd) /* output stream */
{
  int i;

  for (i = 0; i < sym_nsyms; i++)
    sym_dumpsym(sym_syms[i], fd);
}

/* dump all symbol state to output stream FD */
void sym_dumpstate(FILE *fd) /* output stream */
{
  int i;

  if (fd == NULL)
    fd = stderr;

  fprintf(fd, "** All symbols sorted by address:\n");
  for (i = 0; i < sym_nsyms; i++)
    sym_dumpsym(sym_syms[i], fd);

  fprintf(fd, "\n** All symbols sorted by name:\n");
  for (i = 0; i < sym_nsyms; i++)
    sym_dumpsym(sym_syms_by_name[i], fd);

  fprintf(fd, "** Text symbols sorted by address:\n");
  for (i = 0; i < sym_ntextsyms; i++)
    sym_dumpsym(sym_textsyms[i], fd);

  fprintf(fd, "\n** Text symbols sorted by name:\n");
  for (i = 0; i < sym_ntextsyms; i++)
    sym_dumpsym(sym_textsyms_by_name[i], fd);

  fprintf(fd, "** Data symbols sorted by address:\n");
  for (i = 0; i < sym_ndatasyms; i++)
    sym_dumpsym(sym_datasyms[i], fd);

  fprintf(fd, "\n** Data symbols sorted by name:\n");
  for (i = 0; i < sym_ndatasyms; i++)
    sym_dumpsym(sym_datasyms_by_name[i], fd);
}

/* bind address ADDR to a symbol in symbol database DB, the address must
   match exactly if EXACT is non-zero, the index of the symbol in the
   requested symbol database is returned in *PINDEX if the pointer is
   non-NULL */
struct sym_sym_t *              /* symbol found, or NULL */
sym_bind_addr(md_addr_t addr,   /* address of symbol to locate */
              int *pindex,      /* ptr to index result var */
              int exact,        /* require exact address match? */
              enum sym_db_t db) /* symbol database to search */
{
  int nsyms, low, high, pos;
  struct sym_sym_t **syms;

  switch (db)
  {
  case sdb_any:
    syms = sym_syms;
    nsyms = sym_nsyms;
    break;
  case sdb_text:
    syms = sym_textsyms;
    nsyms = sym_ntextsyms;
    break;
  case sdb_data:
    syms = sym_datasyms;
    nsyms = sym_ndatasyms;
    break;
  default:
    panic("bogus symbol database");
  }

  /* any symbols to search? */
  if (!nsyms)
  {
    if (pindex)
      *pindex = -1;
    return NULL;
  }

  /* binary search symbol database (sorted by address) */
  low = 0;
  high = nsyms - 1;
  pos = (low + high) >> 1;
  while (!(/* exact match */
           (exact && syms[pos]->addr == addr)
           /* in bounds match */
           || (!exact && syms[pos]->addr <= addr && addr < (syms[pos]->addr + MAX(1, syms[pos]->size)))))
  {
    if (addr < syms[pos]->addr)
      high = pos - 1;
    else
      low = pos + 1;
    if (high >= low)
      pos = (low + high) >> 1;
    else
    {
      if (pindex)
        *pindex = -1;
      return NULL;
    }
  }

  /* bound! */
  if (pindex)
    *pindex = pos;
  return syms[pos];
}

/* bind name NAME to a symbol in symbol database DB, the index of the symbol
   in the requested symbol database is returned in *PINDEX if the pointer is
   non-NULL */
struct sym_sym_t *              /* symbol found, or NULL */
sym_bind_name(char *name,       /* symbol name to locate */
              int *pindex,      /* ptr to index result var */
              enum sym_db_t db) /* symbol database to search */
{
  int nsyms, low, high, pos, cmp;
  struct sym_sym_t **syms;

  switch (db)
  {
  case sdb_any:
    syms = sym_syms_by_name;
    nsyms = sym_nsyms;
    break;
  case sdb_text:
    syms = sym_textsyms_by_name;
    nsyms = sym_ntextsyms;
    break;
  case sdb_data:
    syms = sym_datasyms_by_name;
    nsyms = sym_ndatasyms;
    break;
  default:
    panic("bogus symbol database");
  }

  /* any symbols to search? */
  if (!nsyms)
  {
    if (pindex)
      *pindex = -1;
    return NULL;
  }

  /* binary search symbol database (sorted by name) */
  low = 0;
  high = nsyms - 1;
  pos = (low + high) >> 1;
  while (!(/* exact string match */ !(cmp = strcmp(syms[pos]->name, name))))
  {
    if (cmp > 0)
      high = pos - 1;
    else
      low = pos + 1;
    if (high >= low)
      pos = (low + high) >> 1;
    else
    {
      if (pindex)
        *pindex = -1;
      return NULL;
    }
  }

  /* bound! */
  if (pindex)
    *pindex = pos;
  return syms[pos];
}
