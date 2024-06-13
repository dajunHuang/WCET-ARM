/************************************************************************************
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
 * $Id: cache.h,v 1.2 2006/06/24 08:54:56 lixianfe Exp $
 *
 ***********************************************************************************/

#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "tcfg.h"
#include "bpred.h"
#include "loops.h"
#include "misc.h"

#define MAX_CACHE_SETS 1024

// 14-bit tag is selected for the following reason:
// - it is enough: can analyze program up to 16MB for a 1-KB cache
// - in cache.c, a valid bit is needed with tag in some cases, thus the valid
//   bit and the tag can be accomodated in a short int
#define MAX_TAG_BITS 14

#define CACHE_LINE(addr) ((addr) & cache.l_msk)
#define SET(addr) (((addr) & cache.s_msk) >> cache.lsb)
#define TAG(addr) (((addr) & cache.t_msk) >> cache.s_lb)
#define TAGSET(addr) (((addr) & cache.t_s_msk) >> cache.lsb)
#define TAGSET2(tag, set) (((tag) << cache.nsb) | (set))
#define LSB_OFF(addr) ((addr) >> cache.lsb)
#define LSB_ON(addr) ((addr) << cache.lsb)
// clear the LSB bits (all LSB bits are set to 0 whatever they are)
#define CLEAR_LSB(addr) (((addr) >> cache.lsb) << cache.lsb)
#define MBLK_ID(sa, addr) (TAGSET(addr) - TAGSET(sa))

#define IC_HIT 0
#define IC_MISS 1
#define IC_UNCLEAR 2
#define MC_INC_SIZE 8

enum
{
    IC_ILP,
    IC_CATEGORIZE
};

// cache configuration
typedef struct
{
    // decided by cache config parameters
    int ns;  // #sets
    int ls;  // cache line size
    int nsb; // nsb = log(ns)
    int lsb; // lsb = log(ls)
    int cmp; // cache miss penalty
    int na;

    // decided by cache config + program size
    int nt;  // #tags
    int ntb; // tag bits = log(nt)

    // combined bits
    int s_lb; // set + line bits
    int t_sb; // tag + set bits
    int nt_s; // # of tag + set

    // masks for set, tag, etc
    unsigned l_msk;   // block mask
    unsigned s_msk;   // set mask
    unsigned t_msk;   // tag mask
    unsigned t_s_msk; // set+tag mask
} cache_t;

// memory block data structure (complete memory block info)
typedef struct
{
    unsigned short set; // cache line
    unsigned short tag; // valid tag
} mem_blk_t;

typedef struct tag_link_t tag_link_t;
struct tag_link_t
{
    unsigned short tag;
    tag_link_t *next;
};

// memory block tag data structure (or partial memory block info w/o set) this
// implementation supports up to 2^16 = 64K memory blocks mapped to each cache
// line, thus for a 1KB cache, the maximum program size supported would be 64M
typedef struct
{
    unsigned short valid; // valid == 0 means empty mempry block
    unsigned short tag;   // memory block tag
} mblk_tag_t;

void set_cache_basic(int nsets, int assoc, int bsize, int miss_penalty);

void set_loop_flags(int flag);
void set_cache();
static void tcfg_node_gen(tcfg_node_t *bbi);
static void tcfg_edge_mp_gen(tcfg_edge_t *e);
static void calc_gen();
static void calc_num_mblks();
void get_mblks();
static tag_link_t *search_tag(int lp_id, unsigned short set, unsigned short tag);
static void add_tag(int lp_id, unsigned short set, unsigned short tag);
static void get_loop_tags();
static int get_mp_conflicts(tcfg_node_t *bbi);
static int handle_first_mblk(int bbi_id);
static void handle_other_mblk(int bbi_id, int start_mb);
static void find_hitloop();
static void bbi_categorize(tcfg_node_t *bbi, loop_t **bbi_hit_loops, int len);
static void categorize();
void cache_analysis();
int get_mblk_hitmiss(tcfg_node_t *bbi, int mblk_id, loop_t *lp);
void dump_gen();
void dump_loop_tags();
void dump_mblk_hitloop();
void dump_hm_list();
void dump_cache_conflicts();
#endif
