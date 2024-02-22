/*
 * symbol.h - program symbol and line data interfaces
 *
 * This file is a part of the SimpleScalar tool suite written by
 * Todd M. Austin as a part of the Multiscalar Research Project.
 *
 * The tool suite is currently maintained by Doug Burger and Todd M. Austin.
 *
 * Copyright (C) 1994, 1995, 1996, 1997, 1998 by Todd M. Austin
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
 * $Id: symbol.h,v 1.1.1.1 2000/11/29 14:53:54 cu-cs Exp $
 *
 * $Log: symbol.h,v $
 * Revision 1.1.1.1  2000/11/29 14:53:54  cu-cs
 * Grand unification of arm sources.
 *
 *
 * Revision 1.1.1.1  2000/05/26 15:18:58  taustin
 * SimpleScalar Tool Set
 *
 *
 * Revision 1.2  1998/08/27 16:45:17  taustin
 * implemented host interface description in host.h
 * added target interface support
 * moved target-dependent definitions to target files
 * added support for register and memory contexts
 *
 * Revision 1.1  1997/03/11  01:34:37  taustin
 * Initial revision
 *
 *
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdio.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "ecoff.h"

#define ECOFF_ALPHAMAGIC 0603

/* #define PRINT_SYMS */

/* Fields in the e_ident array.  The EI_* macros are indices into the
   array.  The macros under each EI_* macro are the values the byte
   may have.  */

#define EI_MAG0 0    /* File identification byte 0 index */
#define ELFMAG0 0x7f /* Magic number byte 0 */

#define EI_MAG1 1   /* File identification byte 1 index */
#define ELFMAG1 'E' /* Magic number byte 1 */

#define EI_MAG2 2   /* File identification byte 2 index */
#define ELFMAG2 'L' /* Magic number byte 2 */

#define EI_MAG3 3   /* File identification byte 3 index */
#define ELFMAG3 'F' /* Magic number byte 3 */

/* Legal values for e_type (object file type).  */

#define ET_NONE 0        /* No file type */
#define ET_REL 1         /* Relocatable file */
#define ET_EXEC 2        /* Executable file */
#define ET_DYN 3         /* Shared object file */
#define ET_CORE 4        /* Core file */
#define ET_NUM 5         /* Number of defined types.  */
#define ET_LOPROC 0xff00 /* Processor-specific */
#define ET_HIPROC 0xffff /* Processor-specific */

/* Legal values for e_machine (architecture).  */

#define EM_NONE 0         /* No machine */
#define EM_M32 1          /* AT&T WE 32100 */
#define EM_SPARC 2        /* SUN SPARC */
#define EM_386 3          /* Intel 80386 */
#define EM_68K 4          /* Motorola m68k family */
#define EM_88K 5          /* Motorola m88k family */
#define EM_486 6          /* Intel 80486 */
#define EM_860 7          /* Intel 80860 */
#define EM_MIPS 8         /* MIPS R3000 big-endian */
#define EM_S370 9         /* Amdahl */
#define EM_MIPS_RS4_BE 10 /* MIPS R4000 big-endian */
#define EM_PARISC 15      /* HPPA */
#define EM_SPARC32PLUS 18 /* Sun's "v8plus" */
#define EM_PPC 20         /* PowerPC */
#define EM_ARM 40         /* ARM */
#define EM_SPARCV9 43     /* SPARC v9 64-bit */

/* Special section indices.  */

#define SHN_UNDEF 0          /* Undefined section */
#define SHN_LORESERVE 0xff00 /* Start of reserved indices */
#define SHN_LOPROC 0xff00    /* Start of processor-specific */
#define SHN_HIPROC 0xff1f    /* End of processor-specific */
#define SHN_ABS 0xfff1       /* Associated symbol is absolute */
#define SHN_COMMON 0xfff2    /* Associated symbol is common */
#define SHN_HIRESERVE 0xffff /* End of reserved indices */

/* Legal values for sh_type (section type).  */

#define SHT_NULL 0                 /* Section header table entry unused */
#define SHT_PROGBITS 1             /* Program data */
#define SHT_SYMTAB 2               /* Symbol table */
#define SHT_STRTAB 3               /* String table */
#define SHT_RELA 4                 /* Relocation entries with addends */
#define SHT_HASH 5                 /* Symbol hash table */
#define SHT_DYNAMIC 6              /* Dynamic linking information */
#define SHT_NOTE 7                 /* Notes */
#define SHT_NOBITS 8               /* Program space with no data (bss) */
#define SHT_REL 9                  /* Relocation entries, no addends */
#define SHT_SHLIB 10               /* Reserved */
#define SHT_DYNSYM 11              /* Dynamic linker symbol table */
#define SHT_NUM 12                 /* Number of defined types.  */
#define SHT_LOSUNW 0x6ffffffd      /* Sun-specific low bound.  */
#define SHT_GNU_verdef 0x6ffffffd  /* Version definition section.  */
#define SHT_GNU_verneed 0x6ffffffe /* Version needs section.  */
#define SHT_GNU_versym 0x6fffffff  /* Version symbol table.  */
#define SHT_HISUNW 0x6fffffff      /* Sun-specific high bound.  */
#define SHT_LOPROC 0x70000000      /* Start of processor-specific */
#define SHT_HIPROC 0x7fffffff      /* End of processor-specific */
#define SHT_LOUSER 0x80000000      /* Start of application-specific */
#define SHT_HIUSER 0x8fffffff      /* End of application-specific */

// 符号表信息定义
#define STB_LOCAL 0x000000;  // 局部符号
#define STB_GLOBAL 0x0000001 // 全局符号
#define STT_OBJECT 0x0000001 // 数据符号
#define STT_FUNC 0x0000002   // 函数符号或者其他可执行代码

// 获取一个 unsigned char 的低四位
#define SYM_TYPE(type) GET_TYPE(type)
#define GET_TYPE(type) ((unsigned int)(type) & 0xf)

// 获取一个 unsigned char 的高四位
#define SYM_INFO(info) GET_INFO(info)
#define GET_INFO(info) ((unsigned int)(info) >> 4)

/* Legal values for sh_flags (section flags).  */

#define SHF_WRITE (1 << 0)      /* Writable */
#define SHF_ALLOC (1 << 1)      /* Occupies memory during execution */
#define SHF_EXECINSTR (1 << 2)  /* Executable */
#define SHF_MASKPROC 0xf0000000 /* Processor-specific */

#define EI_NIDENT (16)

// elf文件头数据结构
struct elf_filehdr
{
  unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
  half_t e_type;                    /* Object file type */
  half_t e_machine;                 /* Architecture */
  word_t e_version;                 /* Object file version */
  word_t e_entry;                   /* Entry point virtual address */
  word_t e_phoff;                   /* Program header table file offset */
  word_t e_shoff;                   /* Section header table file offset */
  word_t e_flags;                   /* Processor-specific flags */
  half_t e_ehsize;                  /* ELF header size in bytes */
  half_t e_phentsize;               /* Program header table entry size */
  half_t e_phnum;                   /* Program header table entry count */
  half_t e_shentsize;               /* Section header table entry size */
  half_t e_shnum;                   /* Section header table entry count */
  half_t e_shstrndx;                /* Section header string table index */
};
// 段表数据结构
struct elf_scnhdr
{
  word_t sh_name;      /* Section name (string tbl index) */
  word_t sh_type;      /* Section type */
  word_t sh_flags;     /* Section flags */
  word_t sh_addr;      /* Section virtual addr at execution */
  word_t sh_offset;    /* Section file offset */
  word_t sh_size;      /* Section size in bytes */
  word_t sh_link;      /* Link to another section */
  word_t sh_info;      /* Additional section information */
  word_t sh_addralign; /* Section alignment */
  word_t sh_entsize;   /* Entry size if section holds table */
};

// 符号表数据结构
struct Elf32_Sym
{
  word_t st_name;
  word_t st_value;
  word_t st_size;
  unsigned char st_info;
  unsigned char st_other;
  half_t st_shndx;
};

enum sym_seg_t
{
  ss_data, /* data segment symbol */
  ss_text, /* text segment symbol */
  ss_NUM
};

/* internal program symbol format */
struct sym_sym_t
{
  char *name;         /* symbol name */
  enum sym_seg_t seg; /* symbol segment */
  int initialized;    /* initialized? (if data segment) */
  int pub;            /* externally visible? */
  int local;          /* compiler local symbol? */
  md_addr_t addr;     /* symbol address value */
  int size;           /* bytes to next symbol */
};

/* symbol database in no particular order */
extern struct sym_sym_t *sym_db;

/* all symbol sorted by address */
extern int sym_nsyms;
extern struct sym_sym_t **sym_syms;

/* all symbols sorted by name */
extern struct sym_sym_t **sym_syms_by_name;

/* text symbols sorted by address */
extern int sym_ntextsyms;
extern struct sym_sym_t **sym_textsyms;

/* text symbols sorted by name */
extern struct sym_sym_t **sym_textsyms_by_name;

/* data symbols sorted by address */
extern int sym_ndatasyms;
extern struct sym_sym_t **sym_datasyms;

/* data symbols sorted by name */
extern struct sym_sym_t **sym_datasyms_by_name;

/* load symbols out of FNAME */
void sym_loadsyms(char *fname,      /* file name containing symbols */
                  int load_locals); /* load local symbols */

/* dump symbol SYM to output stream FD */
void sym_dumpsym(struct sym_sym_t *sym, /* symbol to display */
                 FILE *fd);             /* output stream */

/* dump all symbols to output stream FD */
void sym_dumpsyms(FILE *fd); /* output stream */

/* dump all symbol state to output stream FD */
void sym_dumpstate(FILE *fd); /* output stream */

/* symbol databases available */
enum sym_db_t
{
  sdb_any,  /* search all symbols */
  sdb_text, /* search text symbols */
  sdb_data, /* search data symbols */
  sdb_NUM
};

/* bind address ADDR to a symbol in symbol database DB, the address must
   match exactly if EXACT is non-zero, the index of the symbol in the
   requested symbol database is returned in *PINDEX if the pointer is
   non-NULL */
struct sym_sym_t *               /* symbol found, or NULL */
sym_bind_addr(md_addr_t addr,    /* address of symbol to locate */
              int *pindex,       /* ptr to index result var */
              int exact,         /* require exact address match? */
              enum sym_db_t db); /* symbol database to search */

/* bind name NAME to a symbol in symbol database DB, the index of the symbol
   in the requested symbol database is returned in *PINDEX if the pointer is
   non-NULL */
struct sym_sym_t *               /* symbol found, or NULL */
sym_bind_name(char *name,        /* symbol name to locate */
              int *pindex,       /* ptr to index result var */
              enum sym_db_t db); /* symbol database to search */

#endif /* SYMBOL_H */
