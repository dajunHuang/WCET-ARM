/*
 * sim-safe.c - functional simulator implementation
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
 * $Id: sim-depchk.c,v 1.1.1.1 2000/11/29 14:53:54 cu-cs Exp $
 *
 * $Log: sim-depchk.c,v $
 * Revision 1.1.1.1  2000/11/29 14:53:54  cu-cs
 * Grand unification of arm sources.
 *
 *
 * Revision 1.1.2.2  2000/09/22 01:58:11  taustin
 * Finished UOP flow generation support.
 *
 * Revision 1.1.2.1  2000/09/19 17:25:31  taustin
 * Completed UOP decomposition code, all UOPs now take nominally one cycle on a
 * simple pipeline, including loads and stores.  Initial testing has the code
 * working fine, however, extensive random testing will occur when sim-fuzz.c
 * gets extended to support multiple functional units...
 *
 * Revision 1.1.1.1.2.9  2000/09/05 13:56:02  taustin
 * SimpleScalar/ARM fuzz buster - random co-simulation tester, very handy!
 *
 * Revision 1.1.1.1.2.8  2000/08/30 21:23:39  taustin
 * Fixed r15 update semantics (now catches r15 updated to value of PC, this happens
 * when a tail recursive function returns to itself - oye!)...
 *
 * Revision 1.1.1.1.2.7  2000/08/29 14:18:09  taustin
 * Fixed word and qword size fp register accesses.
 *
 * Revision 1.1.1.1.2.6  2000/08/26 21:04:14  taustin
 * Added -trigger:inst <N> option to enable verbose dumps mid-execution.
 *
 * Revision 1.1.1.1.2.5  2000/08/26 06:53:53  taustin
 * Simplified SimpleScalar/ARM PC handling - seems to work...
 *
 * Revision 1.1.1.1.2.4  2000/08/22 18:38:49  taustin
 * More progress on the SimpleScalar/ARM target.
 *
 * Revision 1.1.2.3  2000/08/12 20:15:29  taustin
 * Fixed loader problems.
 * Improved heap management.
 * Added improved bogus address checks.
 *
 * Revision 1.1.2.2  2000/08/02 09:44:54  taustin
 * Fixed stat system call emulation.
 *
 * Revision 1.1.2.1  2000/08/02 08:51:57  taustin
 * SimpleScalar/ARM co-simulation component, based on the ARMulator.
 *
 * Revision 1.1.1.1.2.3  2000/07/28 21:37:08  taustin
 * Added ld_text_bound and ld_data_bound to loader exports.
 *
 * Revision 1.1.1.1.2.2  2000/07/26 05:01:32  taustin
 * Added more debug output.
 *
 * Revision 1.1.1.1.2.1  2000/07/21 18:30:55  taustin
 * More progress on the SimpleScalar/ARM target.
 *
 * Revision 1.1.1.1  2000/05/26 15:18:58  taustin
 * SimpleScalar Tool Set
 *
 *
 * Revision 1.9  1999/12/31 18:53:24  taustin
 * quad_t naming conflicts removed
 *
 * Revision 1.8  1999/12/13 18:47:13  taustin
 * cross endian execution support added
 *
 * Revision 1.7  1998/08/31 17:11:01  taustin
 * added register checksuming support, viewable with "-v" flag
 *
 * Revision 1.6  1998/08/27 16:38:25  taustin
 * implemented host interface description in host.h
 * added target interface support
 * added support for register and memory contexts
 * instruction predecoding moved to loader module
 * Alpha target support added
 * added support for qword's
 * added fault support
 * added option ("-max:inst") to limit number of instructions analyzed
 * added target-dependent myprintf() support
 *
 * Revision 1.5  1997/03/11  17:14:57  taustin
 * updated copyright
 * long/int tweaks made for ALPHA target support
 * supported added for non-GNU C compilers
 *
 * Revision 1.4  1997/01/06  16:06:28  taustin
 * updated comments
 * opt_reg_header() call now prints simulator overview message
 * access variable now generalized to is_write boolean flag
 *
 * Revision 1.3  1996/12/27  15:54:04  taustin
 * updated comments
 * integrated support for options and stats packages
 * added sim_init() code
 *
 * Revision 1.1  1996/12/05  18:52:32  taustin
 * Initial revision
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "host.h"
#include "misc.h"
#include "machine.h"
#include "regs.h"
#include "memory.h"
#include "loader.h"
#include "syscall.h"
#include "dlite.h"
#include "options.h"
#include "stats.h"
#include "sim.h"

#include "armdefs.h"
#include "armemu.h"

/*
 * This file implements a functional simulator.  This functional simulator is
 * the simplest, most user-friendly simulator in the simplescalar tool set.
 * Unlike sim-fast, this functional simulator checks for all instruction
 * errors, and the implementation is crafted for clarity rather than speed.
 */

/* simulated registers */
static struct regs_t regs;

/* simulated memory */
static struct mem_t *mem = NULL;

/* track number of UOPs executed */
static counter_t sim_num_uops = 0;

/* track number of refs */
static counter_t sim_num_refs = 0;

/* maximum number of inst's to execute */
static unsigned int max_insts;

static unsigned int trigger_inst;

/* register simulator-specific options */
void sim_reg_options(struct opt_odb_t *odb)
{
  opt_reg_header(odb,
                 "sim-safe: This simulator implements a functional simulator.  This\n"
                 "functional simulator is the simplest, most user-friendly simulator in the\n"
                 "simplescalar tool set.  Unlike sim-fast, this functional simulator checks\n"
                 "for all instruction errors, and the implementation is crafted for clarity\n"
                 "rather than speed.\n");

  /* instruction limit */
  opt_reg_uint(odb, "-max:inst", "maximum number of inst's to execute",
               &max_insts, /* default */ 0,
               /* print */ TRUE, /* format */ NULL);

  opt_reg_uint(odb, "-trigger:inst", "trigger instruction",
               &trigger_inst, /* default */ 0,
               /* print */ TRUE, /* format */ NULL);
}

/* check simulator-specific option values */
void sim_check_options(struct opt_odb_t *odb, int argc, char **argv)
{
  /* nada */
}

/* register simulator-specific statistics */
void sim_reg_stats(struct stat_sdb_t *sdb)
{
  stat_reg_counter(sdb, "sim_num_insn",
                   "total number of instructions executed",
                   &sim_num_insn, sim_num_insn, NULL);
  stat_reg_counter(sdb, "sim_num_uops",
                   "total number of UOPs executed",
                   &sim_num_uops, sim_num_uops, NULL);
  stat_reg_formula(sdb, "sim_avg_flowlen",
                   "uops per instruction",
                   "sim_num_uops / sim_num_insn", NULL);
  stat_reg_counter(sdb, "sim_num_refs",
                   "total number of loads and stores executed",
                   &sim_num_refs, 0, NULL);
  stat_reg_int(sdb, "sim_elapsed_time",
               "total simulation time in seconds",
               &sim_elapsed_time, 0, NULL);
  stat_reg_formula(sdb, "sim_inst_rate",
                   "simulation speed (in insts/sec)",
                   "sim_num_insn / sim_elapsed_time", NULL);
  ld_reg_stats(sdb);
  mem_reg_stats(mem, sdb);
}

/* initialize the simulator */
void sim_init(void)
{
  sim_num_refs = 0;

  /* allocate and initialize register file */
  regs_init(&regs);

  /* allocate and initialize memory space */
  mem = mem_create("mem");
  mem_init(mem);
}

/* load program into simulated state */
void sim_load_prog(char *fname,           /* program to load */
                   int argc, char **argv, /* program arguments */
                   char **envp)           /* program environment */
{
  /* load program text and data, set up environment, memory, and regs */
  ld_load_prog(fname, argc, argv, envp, &regs, mem, TRUE);

  /* initialize the DLite debugger */
  dlite_init(md_reg_obj, dlite_mem_obj, dlite_mstate_obj);
}

/* print simulator-specific configuration information */
void sim_aux_config(FILE *stream) /* output stream */
{
  /* nothing currently */
}

/* dump simulator-specific auxiliary simulator statistics */
void sim_aux_stats(FILE *stream) /* output stream */
{
  /* nada */
}

/* un-initialize simulator-specific state */
void sim_uninit(void)
{
  /* nada */
}

/*
 * configure the execution engine
 */

/* dependence mask definition:

   0..15	- gpr's
   16..23	- fpr's
   24		- cpsr
   25		- fpsr
   26		- pc
*/
#define DEP(MSK, IDX) ((MSK) |= (IDX))
#define DEP_GPR(N) (assert((N) < 18), (1 << (N)))
#define DEP_FPR(N) (assert((N) < 8), (1 << ((N) + 18)))
#define DEP_ADDR (1 << 26)
#define DEP_PSR (1 << 27)
#define DEP_FPSR (1 << 28)
typedef word_t depmask_t;

void dep_print(FILE *stream, depmask_t dep)
{
  int i;

  for (i = 0; i < 16; i++)
    if (dep & (1 << i))
      fprintf(stream, "r%d ", i);

  for (i = 16; i < 18; i++)
    if (dep & (1 << i))
      fprintf(stream, "tmp%d ", i - 16);

  for (i = 18; i < 26; i++)
    if (dep & (1 << i))
      fprintf(stream, "f%d ", i - 18);

  if (dep & (1 << 26))
    fprintf(stream, "addr ");
  if (dep & (1 << 27))
    fprintf(stream, "psr ");
  if (dep & (1 << 28))
    fprintf(stream, "fpsr ");
}

#define RDEP_RD(IDX) DEP(ref_rdmask, (IDX))
#define RDEP_WR(IDX) DEP(ref_wrmask, (IDX))
static depmask_t ref_rdmask;
static depmask_t ref_wrmask;

static depmask_t test_rdmask;
static depmask_t test_wrmask;

/* register dependence decoders */
#define DNA ((void)0)
#define DGPR(N) DEP(mask, DEP_GPR(N))
#define DFPR(N) DEP(mask, DEP_FPR(N))
#define DADDR DEP(mask, DEP_ADDR)
#define DPSR DEP(mask, DEP_PSR)
#define DFPSR DEP(mask, DEP_FPSR)

/*
 * precise architected register accessors
 */

/* next program counter */
#define SET_NPC(EXPR) (RDEP_WR(DEP_GPR(15)), regs.regs_NPC = (EXPR))

/* current program counter */
#define CPC (RDEP_RD(DEP_GPR(15)), regs.regs_PC)

/* general purpose registers */
#define GPR(N) (RDEP_RD(DEP_GPR(N)), regs.regs_R[N])
#define SET_GPR(N, EXPR)                                   \
  ((void)(((N) == 15) ? setPC++ : 0), RDEP_WR(DEP_GPR(N)), \
   regs.regs_R[N] = (EXPR))

#if defined(TARGET_PISA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_L(N) (regs.regs_F.l[(N)])
#define SET_FPR_L(N, EXPR) (regs.regs_F.l[(N)] = (EXPR))
#define FPR_F(N) (regs.regs_F.f[(N)])
#define SET_FPR_F(N, EXPR) (regs.regs_F.f[(N)] = (EXPR))
#define FPR_D(N) (regs.regs_F.d[(N) >> 1])
#define SET_FPR_D(N, EXPR) (regs.regs_F.d[(N) >> 1] = (EXPR))

/* miscellaneous register accessors */
#define SET_HI(EXPR) (regs.regs_C.hi = (EXPR))
#define HI (regs.regs_C.hi)
#define SET_LO(EXPR) (regs.regs_C.lo = (EXPR))
#define LO (regs.regs_C.lo)
#define FCC (regs.regs_C.fcc)
#define SET_FCC(EXPR) (regs.regs_C.fcc = (EXPR))

#elif defined(TARGET_ALPHA)

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_Q(N) (regs.regs_F.q[N])
#define SET_FPR_Q(N, EXPR) (regs.regs_F.q[N] = (EXPR))
#define FPR(N) (regs.regs_F.d[(N)])
#define SET_FPR(N, EXPR) (regs.regs_F.d[(N)] = (EXPR))

/* miscellaneous register accessors */
#define FPCR (regs.regs_C.fpcr)
#define SET_FPCR(EXPR) (regs.regs_C.fpcr = (EXPR))
#define UNIQ (regs.regs_C.uniq)
#define SET_UNIQ(EXPR) (regs.regs_C.uniq = (EXPR))

#elif defined(TARGET_ARM)

#define ADDR (RDEP_RD(DEP_ADDR), regs.regs_C.addr)
#define SET_ADDR(EXPR) (RDEP_WR(DEP_ADDR), regs.regs_C.addr = (EXPR))

/* processor status register */
#define PSR (RDEP_RD(DEP_PSR), regs.regs_C.cpsr)
#define SET_PSR(EXPR) (RDEP_WR(DEP_PSR), regs.regs_C.cpsr = (EXPR))

#define PSR_N (RDEP_RD(DEP_PSR), _PSR_N(regs.regs_C.cpsr))
#define SET_PSR_N(EXPR) \
  (RDEP_WR(DEP_PSR), _SET_PSR_N(regs.regs_C.cpsr, (EXPR)))
#define PSR_C (RDEP_RD(DEP_PSR), _PSR_C(regs.regs_C.cpsr))
#define SET_PSR_C(EXPR) \
  (RDEP_WR(DEP_PSR), _SET_PSR_C(regs.regs_C.cpsr, (EXPR)))
#define PSR_Z (RDEP_RD(DEP_PSR), _PSR_Z(regs.regs_C.cpsr))
#define SET_PSR_Z(EXPR) \
  (RDEP_WR(DEP_PSR), _SET_PSR_Z(regs.regs_C.cpsr, (EXPR)))
#define PSR_V (RDEP_RD(DEP_PSR), _PSR_V(regs.regs_C.cpsr))
#define SET_PSR_V(EXPR) \
  (RDEP_WR(DEP_PSR), _SET_PSR_V(regs.regs_C.cpsr, (EXPR)))

/* floating point conversions */
union x
{
  float f;
  word_t w;
};
#define DTOW(D) ({ union x fw; fw.f = (float)(D); fw.w; })
#define WTOD(W) ({ union x fw; fw.w = (W); (double)fw.f; })
#define QSWP(Q) \
  ((((Q) << 32) & ULL(0xffffffff00000000)) | (((Q) >> 32) & ULL(0xffffffff)))

/* floating point registers, L->word, F->single-prec, D->double-prec */
#define FPR_Q(N) (RDEP_RD(DEP_FPR(N)), QSWP(regs.regs_F.q[N]))
#define SET_FPR_Q(N, EXPR) \
  (RDEP_WR(DEP_FPR(N)), regs.regs_F.q[N] = QSWP((EXPR)))
#define FPR_W(N) (RDEP_RD(DEP_FPR(N)), DTOW(regs.regs_F.d[N]))
#define SET_FPR_W(N, EXPR) \
  (RDEP_WR(DEP_FPR(N)), regs.regs_F.d[N] = (WTOD(EXPR)))
#define FPR(N) (RDEP_RD(DEP_FPR(N)), regs.regs_F.d[(N)])
#define SET_FPR(N, EXPR) \
  (RDEP_WR(DEP_FPR(N)), regs.regs_F.d[(N)] = (EXPR))

/* miscellaneous register accessors */
#define FPSR (RDEP_RD(DEP_FPSR), regs.regs_C.fpsr)
#define SET_FPSR(EXPR) (RDEP_WR(DEP_FPSR), regs.regs_C.fpsr = (EXPR))

#else
#error No ISA target defined...
#endif

/* precise architected memory state accessor macros */
#define READ_BYTE(SRC, FAULT) \
  ((FAULT) = md_fault_none, MEM_READ_BYTE(mem, addr = (SRC)))
#define READ_HALF(SRC, FAULT) \
  ((FAULT) = md_fault_none, MEM_READ_HALF(mem, addr = (SRC)))
#define READ_WORD(SRC, FAULT) \
  ((FAULT) = md_fault_none, MEM_READ_WORD(mem, addr = (SRC)))
#ifdef HOST_HAS_QWORD
#define READ_QWORD(SRC, FAULT) \
  ((FAULT) = md_fault_none, MEM_READ_QWORD(mem, addr = (SRC)))
#endif /* HOST_HAS_QWORD */

#define WRITE_BYTE(SRC, DST, FAULT) \
  ((FAULT) = md_fault_none, MEM_WRITE_BYTE(mem, addr = (DST), (SRC)))
#define WRITE_HALF(SRC, DST, FAULT) \
  ((FAULT) = md_fault_none, MEM_WRITE_HALF(mem, addr = (DST), (SRC)))
#define WRITE_WORD(SRC, DST, FAULT) \
  ((FAULT) = md_fault_none, MEM_WRITE_WORD(mem, addr = (DST), (SRC)))
#ifdef HOST_HAS_QWORD
#define WRITE_QWORD(SRC, DST, FAULT) \
  ((FAULT) = md_fault_none, MEM_WRITE_QWORD(mem, addr = (DST), (SRC)))
#endif /* HOST_HAS_QWORD */

/* system call handler macro */
#define SYSCALL(INST) sys_syscall(&regs, mem_access, mem, INST, TRUE)

word_t
GetWord(void *p, md_addr_t addr)
{
  if (addr < 0x2000000)
    myfprintf(stderr, "--rd @ %p(%n): addr:%x, val:%x....\n",
              regs.regs_R[15], sim_num_insn, addr, MEM_READ_WORD(mem, (addr & ~3)));

  return MEM_READ_WORD(mem, (addr & ~3));
}

void PutWord(void *p, md_addr_t addr, word_t data)
{
  if (addr < 0x2000000)
    myfprintf(stderr, "--wr @ %p(%n): addr:%x, val:%x....\n",
              regs.regs_R[15], sim_num_insn, addr, data);

  MEM_WRITE_WORD(mem, (addr & ~3), data);
}

/* start simulation, program loaded, processor precise state initialized */
void sim_main(void)
{
  md_inst_t inst;
  register md_addr_t addr;
  enum md_opcode op;
  register int is_write;
  enum md_fault_type fault;
  int next_inst;
  int setPC;
  int in_flow = FALSE, flow_index, nflow;
  struct md_uop_t flowtab[MD_MAX_FLOWLEN];
  depmask_t mask;

  fprintf(stderr, "sim: ** starting functional simulation **\n");

  /* set up initial default next PC */
  regs.regs_NPC = regs.regs_PC + sizeof(md_inst_t);

  /* synchronize register files... */
  /* pc = regs.regs_PC; */
  regs.regs_R[MD_REG_PC] = regs.regs_PC;
  next_inst = RESUME;

  /* check for DLite debugger entry condition */
  if (dlite_check_break(regs.regs_PC, /* !access */ 0, /* addr */ 0, 0, 0))
    dlite_main(regs.regs_PC - sizeof(md_inst_t),
               regs.regs_PC, sim_num_insn, &regs, mem);

  while (TRUE)
  {
    /* maintain $r0 semantics */
#ifdef TARGET_ARM
    /* regs.regs_R[MD_REG_PC] = regs.regs_PC; */
#endif
#ifndef TARGET_ARM
    regs.regs_R[MD_REG_ZERO] = 0;
#endif
#ifdef TARGET_ALPHA
    regs.regs_F.d[MD_REG_ZERO] = 0.0;
#endif /* TARGET_ALPHA */

    /* set default reference address and access mode */
    addr = 0;
    is_write = FALSE;

    /* set default fault - none */
    fault = md_fault_none;

    if (!in_flow)
    {
      /* keep an instruction count */
      sim_num_insn++;

      /* get the next instruction to execute */
      MD_FETCH_INST(inst, mem, regs.regs_PC);

      /* decode the instruction */
      MD_SET_OPCODE(op, inst);

      if (MD_OP_FLAGS(op) & F_CISC)
      {
        /* get instruction flow */
        nflow = md_get_flow(op, inst, flowtab);
        if (nflow > 0)
        {
          in_flow = TRUE;
          flow_index = 0;
        }
        else
          fatal("could not locate CISC flow");
        sim_num_uops += nflow;
        if (nflow < 2)
          panic("bogus flow length");
      }
      else
        sim_num_uops++;
    }
    if (in_flow)
    {
      op = flowtab[flow_index].op;
      inst = flowtab[flow_index++].inst;
      if (flow_index == nflow)
        in_flow = FALSE;
    }

    if (op == NA)
      fatal("bogus opcode detected @ 0x%08p", regs.regs_PC);

    setPC = 0;
    regs.regs_R[MD_REG_PC] = regs.regs_PC;

    /* record dependencies */
    test_rdmask = test_wrmask = 0;
    switch (op)
    {
#define DEFINST(OP, MSK, NAME, OPFORM, RES, CLASS, O1, O2, O3, I1, I2, I3, I4) \
  case OP:                                                                     \
    /* compute output/input dependencies to out1-2 and in1-3 */                \
    mask = 0;                                                                  \
    O1;                                                                        \
    O2;                                                                        \
    O3;                                                                        \
    test_wrmask = mask;                                                        \
    mask = 0;                                                                  \
    I1;                                                                        \
    I2;                                                                        \
    I3;                                                                        \
    I4;                                                                        \
    test_rdmask = mask;                                                        \
    break;
#define DEFUOP(OP, NAME, OPFORM, RES, CLASS, O1, O2, O3, I1, I2, I3, I4) \
  case OP:                                                               \
    /* compute output/input dependencies to out1-2 and in1-3 */          \
    mask = 0;                                                            \
    O1;                                                                  \
    O2;                                                                  \
    O3;                                                                  \
    test_wrmask = mask;                                                  \
    mask = 0;                                                            \
    I1;                                                                  \
    I2;                                                                  \
    I3;                                                                  \
    I4;                                                                  \
    test_rdmask = mask;                                                  \
    break;
#define DEFLINK(OP, MSK, NAME, MASK, SHIFT) /* nada... */
#define CONNECT(OP)                         /* nada... */
#include "machine.def"
    default:
      fatal("unexpected opcode: `%d'", (int)op);
    }
    if (!COND_VALID(PSR))
    {
      test_rdmask &= DEP_PSR;
      test_wrmask = 0;
    }

    /* execute the instruction */
    ref_rdmask = ref_wrmask = 0;
    switch (op)
    {
#define DEFINST(OP, MSK, NAME, OPFORM, RES, FLAGS, O1, O2, O3, I1, I2, I3, I4) \
  case OP:                                                                     \
    SYMCAT(OP, _IMPL);                                                         \
    break;
#define DEFUOP(OP, NAME, OPFORM, RES, FLAGS, O1, O2, O3, I1, I2, I3, I4) \
  case OP:                                                               \
    SYMCAT(OP, _IMPL);                                                   \
    break;
#define DEFLINK(OP, MSK, NAME, MASK, SHIFT) \
  case OP:                                  \
    panic("attempted to execute a linking opcode");
#define CONNECT(OP)
#define DECLARE_FAULT(FAULT) \
  {                          \
    fault = (FAULT);         \
    break;                   \
  }
#include "machine.def"
    default:
      panic("attempted to execute a bogus opcode");
    }

    if (fault != md_fault_none)
      fatal("fault (%d) detected @ 0x%08p", fault, regs.regs_PC);

    if (setPC != 0 /* regs.regs_R[MD_REG_PC] != regs.regs_PC */)
      regs.regs_NPC = regs.regs_R[MD_REG_PC];

#define XDEP(X) ((X) & ~(1 << 15))

    if (/* verbose && sim_num_insn >= trigger_inst && */
        (XDEP(ref_rdmask) != XDEP(test_rdmask) || XDEP(ref_wrmask) != XDEP(test_wrmask)))
    {
      myfprintf(stderr, "%10n [xor: 0x%08x] @ 0x%08p: ",
                sim_num_insn, md_xor_regs(&regs), regs.regs_PC);
      md_print_insn(inst, regs.regs_PC, stderr);
      fprintf(stderr, "\n");

      fprintf(stderr, "[ref rd]  -> ");
      dep_print(stderr, ref_rdmask);
      fprintf(stderr, "\n");
      fprintf(stderr, "[ref wr]  -> ");
      dep_print(stderr, ref_wrmask);
      fprintf(stderr, "\n");

      fprintf(stderr, "[test rd] -> ");
      dep_print(stderr, test_rdmask);
      fprintf(stderr, "\n");
      fprintf(stderr, "[test wr] -> ");
      dep_print(stderr, test_wrmask);
      fprintf(stderr, "\n");

      /* md_print_iregs(regs.regs_R, stderr); */
      /* md_print_fpregs(regs.regs_F, stderr); */
      /* md_print_cregs(regs.regs_C, stderr); */
      /* fflush(stderr); */
    }

    if (MD_OP_FLAGS(op) & F_MEM)
    {
      sim_num_refs++;
      if (MD_OP_FLAGS(op) & F_STORE)
        is_write = TRUE;
    }

    /* check for DLite debugger entry condition */
    if (dlite_check_break(regs.regs_NPC,
                          is_write ? ACCESS_WRITE : ACCESS_READ,
                          addr, sim_num_insn, sim_num_insn))
      dlite_main(regs.regs_PC, regs.regs_NPC, sim_num_insn, &regs, mem);

    /* go to the next instruction */
    if (!in_flow)
    {
      regs.regs_PC = regs.regs_NPC;
      regs.regs_NPC += sizeof(md_inst_t);
    }

    /* finish early? */
    if (max_insts && sim_num_insn >= max_insts)
      return;
  }
}
