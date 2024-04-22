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
 * $Id: isa.c,v 1.2 2006/06/24 08:54:56 lixianfe Exp $
 *
 ******************************************************************************/

#include <stdio.h>
#include "isa.h"

int ss_max_inst_lat(de_inst_t *inst);
void init_isa_ss();

isa_t *isa;  // info of the instruction types of the ISA
int num_isa; // number of instruction types of the ISA

// initiate ISA info
void init_isa()
{
    // if ARM is used, call this to init ARM ISA info
    init_isa_ss();
    // show registered ARM instructions
    // dump_isa();
}

// return (decoded) instruction type
inline int
inst_type(de_inst_t *inst)
{
    return isa[inst->op_enum].type;
}

int max_inst_lat(de_inst_t *inst)
{
    int fu;

    return ss_max_inst_lat(inst);
}

// dump functions for debug usage
//==============================================================================

void dump_isa()
{
    int i;

    for (i = 1; i <= num_isa; i++) {
        printf("instruction %3d: %-10s type: ", i, isa[i].name);
        if(isa[i].type == 1)
            printf("INST_ICOMP\n");
        else if(isa[i].type == 2)
            printf("INST_FCOMP\n");
        else if(isa[i].type == 3)
            printf("INST_LOAD\n");
        else if(isa[i].type == 4)
            printf("INST_STORE\n");
        else if(isa[i].type == 5)
            printf("INST_COND\n");
        else if(isa[i].type == 6)
            printf("INST_UNCOND\n");
        else if(isa[i].type == 7)
            printf("INST_CALL\n");
        else if(isa[i].type == 8)
            printf("INST_RET\n");
        else if(isa[i].type == 9)
            printf("INST_TRAP\n");
        else
            printf("INST_NOP\n");
    }
}
