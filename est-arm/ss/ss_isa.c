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
 * $Id: ss_isa.c,v 1.2 2006/06/24 08:55:05 lixianfe Exp $
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common.h"
#include "../isa.h"
#include "ss_isa.h"
#include "machine.h"

void decode_inst(de_inst_t *de_inst, md_inst_t inst);

extern isa_t *isa;	// �����ص�ָ���Щָ�������������֡�
extern int num_isa; // ���ָ�������

// BEGIN: from machine.h in SimpleScalar 3.0
// -----------------------------------------------------------------------------

#if 0

char *md_op2name[OP_MAX] = {
  NULL, /* NA */
#define DEFINST(OP, MSK, NAME, OPFORM, RES, FLAGS, O1, O2, I1, I2, I3) NAME,
#define DEFLINK(OP, MSK, NAME, MASK, SHIFT) NAME,
#define CONNECT(OP)
#include "machine.def"
};



/* enum md_opcode -> opcode flags, used by simulators */
unsigned int md_op2flags[OP_MAX] = {
  NA, /* NA */
#define DEFINST(OP, MSK, NAME, OPFORM, RES, FLAGS, O1, O2, I1, I2, I3) FLAGS,
#define DEFLINK(OP, MSK, NAME, MASK, SHIFT) NA,
#define CONNECT(OP)
#include "machine.def"
};

#endif

extern char *md_op2name[];
extern unsigned int md_op2flags[];

// -----------------------------------------------------------------------------
// END: from machine.h in SimpleScalar 3.0

// initiate SimpleScalar ISA info by reading its machine.def file
// �����е�ָ���ʼ���ŵ�ȫ�ֱ��ￄ1�7? isa �У����а���ָ��ￄ1�7? opcode��name��type;
void init_isa_ss()
{
	int i, len;

	num_isa = OP_MAX;

	isa = (isa_t *)calloc(OP_MAX - 1, sizeof(isa_t));
	CHECK_MEM(isa);
	for (i = 1; i < OP_MAX - 1; i++)
	{
		isa[i].opcode = i;
		if (md_op2name[i] != NULL)
		{
			len = strlen(md_op2name[i]) + 1;
			isa[i].name = (char *)malloc(len);
			CHECK_MEM(isa[i].name);
			strcpy(isa[i].name, md_op2name[i]);
		}
		if (md_op2flags[i] == F_LINK)
			isa[i].type = INST_NOP;
		else if ((md_op2flags[i] & (F_CTRL | F_DIRJMP)) == (F_CTRL | F_DIRJMP))
			isa[i].type = INST_TRAP;
		else if (md_op2flags[i] & F_ICOMP)
			isa[i].type = INST_ICOMP;
		else if (md_op2flags[i] & F_FCOMP)
			isa[i].type = INST_FCOMP;
		else if (md_op2flags[i] & F_LOAD)
			isa[i].type = INST_LOAD;
		else if (md_op2flags[i] & F_STORE)
			isa[i].type = INST_STORE;
		else if (md_op2flags[i] & F_COND)
			isa[i].type = INST_COND;
		else if (md_op2flags[i] & F_CALL)
			isa[i].type = INST_CALL;
		else if ((md_op2flags[i] & (F_CTRL | F_UNCOND | F_DIRJMP)) == (F_CTRL | F_UNCOND | F_DIRJMP))
			isa[i].type = INST_UNCOND;
		else if ((md_op2flags[i] & (F_CTRL | F_UNCOND | F_INDIRJMP)) == (F_CTRL | F_UNCOND | F_INDIRJMP))
			isa[i].type = INST_RET;
		else if (md_op2flags[i] & F_TRAP)
			isa[i].type = INST_TRAP;
		else
		{
			fprintf(stderr, "%s  %d: unidentified instruction type!  flag :%d \n", isa[i].name, i, md_op2flags[i]);
			exit(1);
		}
		// this code for debug;
		// printf("opcod:%d  name:%s  type:%d\n",isa[i].opcode,isa[i].name,isa[i].type);
	}
}

#if 0
//decode the raw instr into decoded instr
//��һ��ָ����н����������ￄ1�7? addr��opcode���������ȣ���������ￄ1�7? de_inst��
void
decode_inst(de_inst_t *de_inst, md_inst_t inst)
{
    int	    op, type, offset, i;
    int	    in[4], out[3];
    int	    num_in = 0, num_out = 0;

    // get inst opcode and from the opcode get its type info
    MD_SET_OPCODE(op, inst);
    op = MD_OP_ENUM(op);
    de_inst->op_enum = op;
    de_inst->size = sizeof(md_inst_t);
    type = isa[op].type;
    // if inst is a ctr transfer, compute the target addr
    if (type == INST_COND) {
	offset = ((int)((short)(inst.b & 0xffff))) << 2;
	de_inst->target = de_inst->addr + sizeof(md_inst_t) + offset;
    } else if ((type == INST_UNCOND) || (type == INST_CALL)) {
	offset = (inst.b & 0x3ffffff) << 2;
	de_inst->target = (de_inst->addr & 0xf0000000) | offset;
    }
	
    // decode the input/output info
    switch(op) {
#define DEFINST(OP, MSK, NAME, OPFORM, RES, FLAGS, O1, O2, O3, I1, I2, I3, I4) \
	case OP:                                                                   \
		in[0] = I1;                                                            \
		in[1] = I2;                                                            \
		in[2] = I3;                                                            \
		in[3] = I4                                                             \
			out[0] = O1;                                                       \
		out[1] = O2;                                                           \
		out[2] = O3 break;
#include "machine.def"
#undef DEFINST
    default:
	     in[0] = in[1] = in[2] = in[3] = NA;
	     out[0] = out[1] = out[2] = NA;
    }
    
    for (i = 0; i < 4; i++) {
	if (in[i] != NA)
	    num_in++;
    }
    for (i = 0; i < 3; i++) {
	if (out[i] != NA)
	    num_out++;
    }
    de_inst->in = (int *) calloc(num_in, sizeof(int));
    CHECK_MEM(de_inst->in);
    de_inst->num_in = 0;
    for (i = 0; i < 4; i++) {
	if (in[i] != NA)
	    de_inst->in[de_inst->num_in++] = in[i];
    }
    de_inst->out = (int *) calloc(num_out, sizeof(int));
    CHECK_MEM(de_inst->out);
    de_inst->num_out = 0;
    for (i = 0; i < 3; i++) {
	if (out[i] != NA)
	    de_inst->out[de_inst->num_out++] = out[i];
    }
}
#endif

#define DFPR(N) (N)
// decode the raw instr into decoded instr
void decode_inst(de_inst_t *de_inst, md_inst_t inst)
{
	int op, type, offset, i;
	int in[4], out[3];
	int num_in = 0, num_out = 0;

	// get inst opcode and from the opcode get its type info
	MD_SET_OPCODE(op, inst);
	op = MD_OP_ENUM(op);
	de_inst->op_enum = op;
	de_inst->size = sizeof(md_inst_t);
	type = isa[op].type;
	// if inst is a ctr transfer, compute the target addr
	if (type == INST_COND || type == INST_UNCOND || type == INST_CALL)
	{
		// offset = ((int)((short)(inst & 0xffff))) << 2;
		// 取得bit23的��，填充时要填充高位
		int flag = 0x00800000 & inst;
		// 高位丄1�71
		if (flag)
		{
			offset = ((inst & 0xffffff) << 2) | 0xfa000000;
		}
		else
		{
			offset = ((inst & 0xffffff) << 2) & 0x03ffffff;
		}
		// pc+8?
		de_inst->target = de_inst->addr + 2 * sizeof(md_inst_t) + offset;
	}
	// else if ((type == INST_UNCOND) || (type == INST_CALL))
	// {
	// 	offset = (inst & 0x3ffffff) << 2;
	// 	de_inst->target = (de_inst->addr & 0xf0000000) | offset;
	// }

	// decode the input/output info
	switch (op)
	{
#define DEFINST(OP, MSK, NAME, FMT, FU, CLASS, O1, O2, O3, IN1, IN2, IN3, IN4) \
	case OP:                                                                   \
	{                                                                          \
		in[0] = IN1;                                                           \
		in[1] = IN2;                                                           \
		in[2] = IN3;                                                           \
		in[3] = IN4;                                                           \
		out[0] = O1;                                                           \
		out[1] = O2;                                                           \
		out[2] = O3;                                                           \
		break;                                                                 \
	}
#define DEFLINK(OP, MSK, NAME, MASK, SHIFT)
#define CONNECT(OP)
#include "machine.def"
#undef DEFINST
#undef DEFLINK
#undef CONNECT
	default:
		in[0] = in[1] = in[2] = in[3] = NA;
		out[0] = out[1] = out[2] = NA;
	}

	for (i = 0; i < 4; i++)
	{
		if (in[i] != NA)
			num_in++;
	}
	for (i = 0; i < 3; i++)
	{
		if (out[i] != NA)
			num_out++;
	}
	de_inst->in = (int *)calloc(num_in, sizeof(int));
	CHECK_MEM(de_inst->in);
	de_inst->num_in = 0;
	for (i = 0; i < 4; i++)
	{
		if (in[i] != NA)
			de_inst->in[de_inst->num_in++] = in[i];
	}
	de_inst->out = (int *)calloc(num_out, sizeof(int));
	CHECK_MEM(de_inst->out);
	de_inst->num_out = 0;
	for (i = 0; i < 3; i++)
	{
		if (out[i] != NA)
			de_inst->out[de_inst->num_out++] = out[i];
	}
}

int ss_inst_fu(de_inst_t *inst)
{
	return MD_OP_FUCLASS(inst->op_enum);
}

extern range_t fu_lat[];
int ss_max_inst_lat(de_inst_t *inst)
{
	int fu;

	fu = ss_inst_fu(inst);
	return fu_lat[fu].hi;
}
