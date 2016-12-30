/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - new_dynarec.h                                           *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef M64P_R4300_NEW_DYNAREC_H
#define M64P_R4300_NEW_DYNAREC_H

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define NEW_DYNAREC_X86 1
#define NEW_DYNAREC_AMD64 2
#define NEW_DYNAREC_ARM 3

#ifdef __cplusplus
extern "C" {
#endif
extern int pcaddr;
extern int pending_exception;
#ifdef __cplusplus
}
#endif

void invalidate_all_pages(void);
void invalidate_block(unsigned int block);
void invalidate_cached_code_new_dynarec(uint32_t address, size_t size);
void new_dynarec_init(void);
void new_dyna_start(void);
void new_dynarec_cleanup(void);

void verify_code();
void verify_code_vm();
void verify_code_ds();

#define NEW_DYNAREC NEW_DYNAREC_X86
#if NEW_DYNAREC == NEW_DYNAREC_X86
	#include "x86/assem_x86.h"
#elif NEW_DYNAREC == NEW_DYNAREC_ARM
	#include "arm/arm_cpu_features.h"
	#include "arm/assem_arm.h"
#else
	#error Unsupported dynarec architecture
#endif

struct regstat {
	signed char regmap_entry[HOST_REGS];
	signed char regmap[HOST_REGS];
	uint64_t was32;
	uint64_t is32;
	uint64_t wasdirty;
	uint64_t dirty;
	uint64_t u;
	uint64_t uu;
	u_int wasconst;
	u_int isconst;
	uint64_t constmap[HOST_REGS];
};

struct ll_entry {
	u_int vaddr;
	u_int reg32;
	void *addr;
	struct ll_entry *next;
};

#define MAXBLOCK 4096
#define MAX_OUTPUT_BLOCK_SIZE 262144
#define CLOCK_DIVIDER count_per_op

static u_int start;
static u_int *source;
static u_int pagelimit;
static char insn[MAXBLOCK][10];
static u_char itype[MAXBLOCK];
static u_char opcode[MAXBLOCK];
static u_char opcode2[MAXBLOCK];
static u_char bt[MAXBLOCK];
static u_char rs1[MAXBLOCK];
static u_char rs2[MAXBLOCK];
static u_char rt1[MAXBLOCK];
static u_char rt2[MAXBLOCK];
static u_char us1[MAXBLOCK];
static u_char us2[MAXBLOCK];
static u_char dep1[MAXBLOCK];
static u_char dep2[MAXBLOCK];
static u_char lt1[MAXBLOCK];
static int imm[MAXBLOCK];
static u_int ba[MAXBLOCK];
static char likely[MAXBLOCK];
static char is_ds[MAXBLOCK];
static char ooo[MAXBLOCK];
static uint64_t unneeded_reg[MAXBLOCK];
static uint64_t unneeded_reg_upper[MAXBLOCK];
static uint64_t branch_unneeded_reg[MAXBLOCK];
static uint64_t branch_unneeded_reg_upper[MAXBLOCK];
static uint64_t p32[MAXBLOCK];
static uint64_t pr32[MAXBLOCK];
static signed char regmap_pre[MAXBLOCK][HOST_REGS];
#ifdef ASSEM_DEBUG
static signed char regmap[MAXBLOCK][HOST_REGS];
static signed char regmap_entry[MAXBLOCK][HOST_REGS];
#endif
static uint64_t constmap[MAXBLOCK][HOST_REGS];
static struct regstat regs[MAXBLOCK];
static struct regstat branch_regs[MAXBLOCK];
static signed char minimum_free_regs[MAXBLOCK];
static u_int needed_reg[MAXBLOCK];
static uint64_t requires_32bit[MAXBLOCK];
static u_int wont_dirty[MAXBLOCK];
static u_int will_dirty[MAXBLOCK];
static int ccadj[MAXBLOCK];
static int slen;
static u_int instr_addr[MAXBLOCK];
static u_int link_addr[MAXBLOCK][3];
static int linkcount;
static u_int stubs[MAXBLOCK * 3][8];
static int stubcount;
static int literalcount;
static int is_delayslot;
static int cop1_usable;
u_char *out;
struct ll_entry *jump_in[4096];
static struct ll_entry *jump_out[4096];
struct ll_entry *jump_dirty[4096];

// Todo: Fix Align
//
//ALIGN(16, u_int hash_table[65536][4]);
//ALIGN(16, static char shadow[2097152]);
u_int hash_table[65536][4];
static char shadow[2097152];

static char *copy;
static int expirep;
u_int using_tlb;
static u_int stop_after_jal;

/* registers that may be allocated */
/* 1-31 gpr */
#define HIREG 32 // hi
#define LOREG 33 // lo
#define FSREG 34 // FPU status (FCSR)
#define CSREG 35 // Coprocessor status
#define CCREG 36 // Cycle count
#define INVCP 37 // Pointer to invalid_code
#define MMREG 38 // Pointer to memory_map
#define ROREG 39 // ram offset (if rdram!=0x80000000)
#define TEMPREG 40
#define FTEMP 40 // FPU temporary register
#define PTEMP 41 // Prefetch temporary register
#define TLREG 42 // TLB mapping offset
#define RHASH 43 // Return address hash
#define RHTBL 44 // Return address hash table address
#define RTEMP 45 // JR/JALR address register
#define MAXREG 45
#define AGEN1 46 // Address generation temporary register
#define AGEN2 47 // Address generation temporary register
#define MGEN1 48 // Maptable address generation temporary register
#define MGEN2 49 // Maptable address generation temporary register
#define BTREG 50 // Branch target temporary register
//*/

// TODO: Uncomment instruction types
/* instruction types */
#define NOP 0 // No operation
#define LOAD 1 // Load
#define STORE 2 // Store
#define LOADLR 3 // Unaligned load
#define STORELR 4 // Unaligned store
#define MOV 5 // Move 
#define ALU 6 // Arithmetic/logic
#define MULTDIV 7 // Multiply/divide
#define SHIFT 8 // Shift by register
#define SHIFTIMM 9 // Shift by immediate 
#define IMM16 10 // 16-bit immediate
#define RJUMP 11 // Unconditional jump to register
#define UJUMP 12 // Unconditional jump
#define CJUMP 13 // Conditional branch (BEQ/BNE/BGTZ/BLEZ)
#define SJUMP 14 // Conditional branch (regimm format)
#define COP0 15 // Coprocessor 0
#define COP1 16 // Coprocessor 1
#define C1LS 17 // Coprocessor 1 load/store
#define FJUMP 18 // Conditional branch (floating point)
#define FLOAT 19 // Floating point unit
#define FCONV 20 // Convert integer to float
#define FCOMP 21 // Floating point compare (sets FSREG)
#define SYSCALL 22 // SYSCALL
#define OTHER 23 // Other
#define SPAN 24 // Branch/delay slot spans 2 pages
#define NI 25 // Not implemented
//*/

/* stubs */
#define CC_STUB 1
#define FP_STUB 2
#define LOADB_STUB 3
#define LOADH_STUB 4
#define LOADW_STUB 5
#define LOADD_STUB 6
#define LOADBU_STUB 7
#define LOADHU_STUB 8
#define STOREB_STUB 9
#define STOREH_STUB 10
#define STOREW_STUB 11
#define STORED_STUB 12
#define STORELR_STUB 13
#define INVCODE_STUB 14
//*/

/* branch codes */
#define TAKEN 1
#define NOTTAKEN 2
#define NULLDS 3 
//*/

/* bug-fix to implement __clear_cache (missing in Android; http://code.google.com/p/android/issues/detail?id=1803) */
void __clear_cache_bugfix(char* begin, char *end);
#ifdef ANDROID
#define __clear_cache __clear_cache_bugfix
#endif

// asm linkage
#ifdef __cplusplus
extern "C" {
#endif
	int new_recompile_block(int addr);
	void *get_addr_ht(u_int vaddr);
	void *get_addr(u_int vaddr);
	void *get_addr_32(u_int vaddr, u_int flags);
	void add_link(u_int vaddr, void *src);
	void clean_blocks(u_int page);
	void dyna_linker();
	void dyna_linker_ds();
	void verify_code();
	void verify_code_vm();
	void verify_code_ds();
	void cc_interrupt();
	void fp_exception();
	void fp_exception_ds();
	void jump_syscall();
	void jump_eret();
#ifdef __cplusplus
}
#endif
static void remove_hash(int vaddr);
#if NEW_DYNAREC == NEW_DYNAREC_ARM
static void invalidate_addr(u_int addr);
#endif

// TLB
void TLBWI_new();
void TLBWR_new();
#ifdef __cplusplus
extern "C" {
#endif
	void read_nomem_new(void);
	void read_nomemb_new(void);
	void read_nomemh_new(void);
	void read_nomemd_new(void);
	void write_nomem_new(void);
	void write_nomemb_new(void);
	void write_nomemh_new(void);
	void write_nomemd_new(void);
	void write_rdram_new(void);
	void write_rdramb_new(void);
	void write_rdramh_new(void);
	void write_rdramd_new(void);
	extern u_int memory_map[1048576];
#ifdef __cplusplus
}
#endif

// Needed by assembler
static void wb_register(signed char r, signed char regmap[], uint64_t dirty, uint64_t is32);
static void wb_dirtys(signed char i_regmap[], uint64_t i_is32, uint64_t i_dirty);
static void wb_needed_dirtys(signed char i_regmap[], uint64_t i_is32, uint64_t i_dirty, int addr);
static void load_all_regs(signed char i_regmap[]);
static void load_needed_regs(signed char i_regmap[], signed char next_regmap[]);
static void load_regs_entry(int t);
static void load_all_consts(signed char regmap[], int is32, u_int dirty, int i);

static void add_stub(int type, int addr, int retaddr, int a, int b, int c, int d, int e);
static void add_to_linker(int addr, int target, int ext);
static int verify_dirty(void *addr);

//static int tracedebug=0;

//#define DEBUG_CYCLE_COUNT 1

// Uncomment these two lines to generate debug output:
//#define ASSEM_DEBUG 1
//#define INV_DEBUG 1

// Uncomment this line to output the number of NOTCOMPILED blocks as they occur:
//#define COUNT_NOTCOMPILEDS 1

#if defined (COUNT_NOTCOMPILEDS )
int notcompiledCount = 0;
#endif

#ifdef __cplusplus
static void nullf(...) {}
#else
static void nullf() {}
#endif

#if defined( ASSEM_DEBUG )
#define assem_debug(...) DebugMessage(M64MSG_VERBOSE, __VA_ARGS__)
#else
#define assem_debug nullf
#endif
#if defined( INV_DEBUG )
#define inv_debug(...) DebugMessage(M64MSG_VERBOSE, __VA_ARGS__)
#else
#define inv_debug nullf
#endif

#define log_message(...) DebugMessage(M64MSG_VERBOSE, __VA_ARGS__)


#endif /* M64P_R4300_NEW_DYNAREC_H */
