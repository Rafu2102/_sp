// c4.c - C in four functions

// char, int, and pointer types
// if, while, return, and expression statements
// just enough features to allow self-compilation and a bit more

// Written by Robert Swierczek
// 修改者: 陳鍾誠 (模組化並加上中文註解)
 
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>

#define int long long // 64-bit 電腦 (long long = 64 bit)

int *e;         // emitted code pointer (目前機器碼指標)
int debug = 1;  // debug 開關

// 虛擬機的操作碼 opcodes
enum {
    LLA, IMM, JMP, JSR, BZ, BNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PSH,
    OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
    OPEN, READ, CLOS, PRTF, MALC, FREE, MSET, MCMP, EXIT
};

// 虛擬機執行函式
int run(int *pc, int *bp, int *sp) {
    int a, cycle = 0;
    int i, *t;

    while (1) {
        i = *pc++;
        cycle++;

        if (debug) {
            printf("%d> %.4s", cycle,
                &"LLA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
                 "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                 "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[i * 5]);
            if (i <= ADJ) printf(" %lld\n", *pc);
            else printf("\n");
        }

        if      (i == LLA)  a = (int)(bp + *pc++);
        else if (i == IMM)  a = *pc++;
        else if (i == JMP)  pc = (int *)*pc;
        else if (i == JSR)  { *--sp = (int)(pc + 1); pc = (int *)*pc; }
        else if (i == BZ)   pc = a ? pc + 1 : (int *)*pc;
        else if (i == BNZ)  pc = a ? (int *)*pc : pc + 1;
        else if (i == ENT)  { *--sp = (int)bp; bp = sp; sp = sp - *pc++; }
        else if (i == ADJ)  sp = sp + *pc++;
        else if (i == LEV)  { sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
        else if (i == LI)   a = *(int *)a;
        else if (i == LC)   a = *(char *)a;
        else if (i == SI)   *(int *)*sp++ = a;
        else if (i == SC)   *(char *)*sp++ = a;
        else if (i == PSH)  *--sp = a;

        else if (i == OR)   a = *sp++ | a;
        else if (i == XOR)  a = *sp++ ^ a;
        else if (i == AND)  a = *sp++ & a;
        else if (i == EQ)   a = *sp++ == a;
        else if (i == NE)   a = *sp++ != a;
        else if (i == LT)   a = *sp++ < a;
        else if (i == GT)   a = *sp++ > a;
        else if (i == LE)   a = *sp++ <= a;
        else if (i == GE)   a = *sp++ >= a;
        else if (i == SHL)  a = *sp++ << a;
        else if (i == SHR)  a = *sp++ >> a;
        else if (i == ADD)  a = *sp++ + a;
        else if (i == SUB)  a = *sp++ - a;
        else if (i == MUL)  a = *sp++ * a;
        else if (i == DIV)  a = *sp++ / a;
        else if (i == MOD)  a = *sp++ % a;

        else if (i == OPEN) a = open((char *)sp[1], *sp);
        else if (i == READ) a = read(sp[2], (char *)sp[1], *sp);
        else if (i == CLOS) a = close(*sp);
        else if (i == PRTF) {
            t = sp + pc[1];
            a = printf((char *)t[-1], t[-2], t[-3], t[-4], t[-5], t[-6]);
        }
        else if (i == MALC) a = (int)malloc(*sp);
        else if (i == FREE) free((void *)*sp);
        else if (i == MSET) a = (int)memset((char *)sp[2], sp[1], *sp);
        else if (i == MCMP) a = memcmp((char *)sp[2], (char *)sp[1], *sp);
        else if (i == EXIT) {
            printf("exit(%lld) cycle = %d\n", *sp, cycle);
            return *sp;
        }
        else {
            printf("unknown instruction = %d! cycle = %d\n", i, cycle);
            return -1;
        }
    }
}

int main() {
    int *pc, *bp, *sp, poolsz, *t, *pow, *loc;

    poolsz = 256 * 1024;
    if (!(e = malloc(poolsz))) {
        printf("could not malloc(%d) text area\n", poolsz);
        return -1;
    }
    if (!(sp = malloc(poolsz))) {
        printf("could not malloc(%d) stack area\n", poolsz);
        return -1;
    }

    memset(e, 0, poolsz);

    // int f(int n) { if(n==0) return 1; else return f(n-1)*2; }
    pow = e;
    *e++ = ENT; *e++ = 0;
    *e++ = LLA; *e++ = 2; *e++ = LI; *e++ = PSH;
    *e++ = IMM; *e++ = 0; *e++ = EQ;
    *e++ = BZ; loc = e++;  // jump placeholder

    *e++ = IMM; *e++ = 1; *e++ = LEV;

    *loc = (int)e;  // fix jump target
    *e++ = LLA; *e++ = 2; *e++ = LI; *e++ = PSH;
    *e++ = IMM; *e++ = 1; *e++ = SUB; *e++ = PSH;
    *e++ = JSR; *e++ = (int)pow;
    *e++ = ADJ; *e++ = 1; *e++ = PSH;
    *e++ = IMM; *e++ = 2; *e++ = MUL;
    *e++ = LEV;

    // int main() { printf("power2(3)=%d\n", power2(3)); }
    pc = e;
    *e++ = ENT; *e++ = 0;
    *e++ = IMM; *e++ = (int)"power2(3)=%d\n"; *e++ = PSH;
    *e++ = IMM; *e++ = 3; *e++ = PSH;
    *e++ = JSR; *e++ = (int)pow;
    *e++ = ADJ; *e++ = 1; *e++ = PSH;
    *e++ = PRTF;
    *e++ = ADJ; *e++ = 2;
    *e++ = LEV;

    // setup stack
    bp = sp = (int *)((int)sp + poolsz);
    *--sp = EXIT;
    *--sp = PSH;
    t = sp;
    *--sp = (int)t;

    return run(pc, bp, sp);
}