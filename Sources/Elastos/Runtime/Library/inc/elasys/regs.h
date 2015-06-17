//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#ifndef __REGS_H__
#define __REGS_H__

#include <kxmips.h>

#define PAGE_SIZE_ASM   0x1000

#define MIPS_ENABLE_CACHE 1
#define MIPS_TLBRREFILL_INDEX 0 // refille tlb by index not randomly
#define MAX_TLB_ENTRY  48

// MIPS pt_regs offsets.
#define     PT_R0           24          // zero
#define     PT_R1           28          // at
#define     PT_R2           32          // v0
#define     PT_R3           36          // v1
#define     PT_R4           40          // a0
#define     PT_R5           44          // a1
#define     PT_R6           48          // a2
#define     PT_R7           52          // a3
#define     PT_R8           56          // t0
#define     PT_R9           60          // t1
#define     PT_R10          64          // t2
#define     PT_R11          68          // t3
#define     PT_R12          72          // t4
#define     PT_R13          76          // t5
#define     PT_R14          80          // t6
#define     PT_R15          84          // t7
#define     PT_R16          88          // s0
#define     PT_R17          92          // s1
#define     PT_R18          96          // s2
#define     PT_R19          100         // s3
#define     PT_R20          104         // s4
#define     PT_R21          108         // s5
#define     PT_R22          112         // s6
#define     PT_R23          116         // s7
#define     PT_R24          120         // t8
#define     PT_R25          124         // t9
#define     PT_R26          128         // k0
#define     PT_R27          132         // k1
#define     PT_R28          136         // gp
#define     PT_R29          140         // sp
#define     PT_R30          144         // fp
#define     PT_R31          148         // ra
#define     PT_LO           152         // taglo
#define     PT_HI           156         // taghi
#define     PT_EPC          160         // epc
#define     PT_BVADDR       164         // bad address
#define     PT_STATUS       168         // status
#define     PT_CAUSE        172         // cause
#define     PT_SIZE         176

//
// Bitfields and bit numbers in the coprocessor 0 cause register.
//
// Refer to to your MIPS R4xx0 manual, chapter 5 for explanation.
//
#define     CAUSEB_EXCCODE  2
#define     CAUSEF_EXCCODE  (31  <<  2)
#define     CAUSEB_IP       8
#define     CAUSEF_IP       (255 <<  8)
#define     CAUSEB_IP0      8
#define     CAUSEF_IP0      (1   <<  8)
#define     CAUSEB_IP1      9
#define     CAUSEF_IP1      (1   <<  9)
#define     CAUSEB_IP2      10
#define     CAUSEF_IP2      (1   << 10)
#define     CAUSEB_IP3      11
#define     CAUSEF_IP3      (1   << 11)
#define     CAUSEB_IP4      12
#define     CAUSEF_IP4      (1   << 12)
#define     CAUSEB_IP5      13
#define     CAUSEF_IP5      (1   << 13)
#define     CAUSEB_IP6      14
#define     CAUSEF_IP6      (1   << 14)
#define     CAUSEB_IP7      15
#define     CAUSEF_IP7      (1   << 15)
#define     CAUSEB_IV       23
#define     CAUSEF_IV       (1   << 23)
#define     CAUSEB_CE       28
#define     CAUSEF_CE       (3   << 28)
#define     CAUSEB_BD       31
#define     CAUSEF_BD       (1   << 31)

//
// Status register bits available in all MIPS CPUs.
//
#define     STATUSB_IP0     8
#define     STATUSF_IP0     (1   <<  8)
#define     STATUSB_IP1     9
#define     STATUSF_IP1     (1   <<  9)
#define     STATUSB_IP2     10
#define     STATUSF_IP2     (1   << 10)
#define     STATUSB_IP3     11
#define     STATUSF_IP3     (1   << 11)
#define     STATUSB_IP4     12
#define     STATUSF_IP4     (1   << 12)
#define     STATUSB_IP5     13
#define     STATUSF_IP5     (1   << 13)
#define     STATUSB_IP6     14
#define     STATUSF_IP6     (1   << 14)
#define     STATUSB_IP7     15
#define     STATUSF_IP7     (1   << 15)
#define     STATUSB_IP8     0
#define     STATUSF_IP8     (1   << 0)
#define     STATUSB_IP9     1
#define     STATUSF_IP9     (1   << 1)
#define     STATUSB_IP10    2
#define     STATUSF_IP10    (1   << 2)
#define     STATUSB_IP11    3
#define     STATUSF_IP11    (1   << 3)
#define     STATUSB_IP12    4
#define     STATUSF_IP12    (1   << 4)
#define     STATUSB_IP13    5
#define     STATUSF_IP13    (1   << 5)
#define     STATUSB_IP14    6
#define     STATUSF_IP14    (1   << 6)
#define     STATUSB_IP15    7
#define     STATUSF_IP15    (1   << 7)
#define     ST0_IM          0x0000ff00
#define     ST0_CH          0x00040000
#define     ST0_SR          0x00100000
#define     ST0_BEV         0x00400000
#define     ST0_RE          0x02000000
#define     ST0_FR          0x04000000
#define     ST0_CU          0xf0000000
#define     ST0_CU0         0x10000000
#define     ST0_CU1         0x20000000
#define     ST0_CU2         0x40000000
#define     ST0_CU3         0x80000000

// this marco is only for cpp souce code, never use it for .asm file
#define     $0
#define     $1
#define     $2
#define     $3
#define     $4
#define     $5
#define     $6
#define     $7
#define     $8
#define     $9
#define     $10
#define     $11
#define     $12
#define     $13
#define     $14
#define     $15
#define     $16
#define     $17
#define     $18
#define     $19
#define     $20
#define     $21
#define     $22
#define     $23
#define     $24
#define     $25
#define     $26
#define     $27
#define     $28
#define     $29
#define     $30
#define     $31

#if defined(_GNUC)

#define read_cp0_register(__source,__val)       \
        ASM(                                    \
            "mfc0 %0," #__source ";"            \
            : "=&r"(__val));

#define write_cp0_register(__source,__res)      \
        ASM(                                    \
            "mtc0 %0," #__source ";"            \
            ::"r"(__res));

#define read_common_register(__source, __res)   \
        ASM(                                    \
            "move %0, " #__source";"            \
            :"=r&"(__res)::#__source);
#elif defined(_EVC)
#define read_cp0_register(__source,__val)       \
        ASM(                                    \
            "mfc0   t0," #__source ";"          \
            "sw     t0, 0(%0);", &__val);

#define write_cp0_register(__source,__val)      \
        ASM(                                    \
            "mtc0 %0," #__source ";", __val);

#define read_common_register(__source, __val)   \
        ASM(                                    \
            "sw"  #__source",0(%0);", &__val);
#else
#error unknown compiler
#endif

#define     DEBUG_LEVEL     0
#define     CURRENT_DEBUG_LEVEL 5
#define     NOT_NEED_DEBUG  5

#ifdef _DEBUG
#define DebugPrint(lev, func)   { if(lev > DEBUG_LEVEL) {func;} }
#else
#define DebugPrint(lev, func)
#endif

#endif  /* __REGS_H__  */
