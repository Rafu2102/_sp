// ucalc_vm_ext.c
// 編譯：gcc -O2 -std=c99 -lm ucalc_vm_ext.c -o ucalc-vm

#ifdef _WIN32
#include <windows.h>    // Windows console UTF-8 支援
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define STACK_SIZE   512
#define CODE_SIZE    8192
#define VAR_COUNT    256
#define CALL_STACK   256

// 指令集編號
enum {
    OP_PUSH, OP_ADD, OP_SUB, OP_MUL, OP_DIV,
    OP_DUP, OP_SWAP,
    OP_STORE, OP_LOAD,
    OP_CMP,
    OP_JMP, OP_JZ, OP_JNZ, OP_JE, OP_JNE, OP_JG, OP_JL,
    OP_CALL, OP_RET,
    OP_SIN, OP_COS, OP_POW,
    OP_PRINT, OP_HALT
};

// 虛擬機結構
typedef struct {
    double stack[STACK_SIZE];
    int sp;                  // 棧頂
    uint8_t code[CODE_SIZE];
    int ip;                  // 指令指標
    double constants[STACK_SIZE];
    int const_count;
    double vars[VAR_COUNT];
    int8_t flags;            // 比較旗標
    int ret_addr[CALL_STACK];
    int csp;                 // call stack pointer
} VM;

// 前置宣告
static void vm_init(VM* vm);
static void vm_load(VM* vm, uint8_t* bc, int len);
static void vm_run(VM* vm);
static int compile_file(const char* src, VM* vm, uint8_t* out);

// VM 初始化
static void vm_init(VM* vm){
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    vm->sp = vm->ip = vm->const_count = vm->flags = vm->csp = 0;
    memset(vm->vars, 0, sizeof vm->vars);
}

// 載入 bytecode
static void vm_load(VM* vm, uint8_t* bc, int len){
    memcpy(vm->code, bc, len);
}

// VM 執行核心（computed goto ）
static void vm_run(VM* vm){
    static void* disp[] = {
        &&L_PUSH,&&L_ADD,&&L_SUB,&&L_MUL,&&L_DIV,
        &&L_DUP,&&L_SWAP,
        &&L_STORE,&&L_LOAD,
        &&L_CMP,
        &&L_JMP,&&L_JZ,&&L_JNZ,&&L_JE,&&L_JNE,&&L_JG,&&L_JL,
        &&L_CALL,&&L_RET,
        &&L_SIN,&&L_COS,&&L_POW,
        &&L_PRINT,&&L_HALT
    };
    #define DISPATCH() goto *disp[ vm->code[ vm->ip++ ] ]
    DISPATCH();

    L_PUSH: {
        int idx = vm->code[vm->ip++];
        vm->stack[vm->sp++] = vm->constants[idx];
        DISPATCH();
    }
    L_ADD: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=a+b; DISPATCH();
    }
    L_SUB: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=a-b; DISPATCH();
    }
    L_MUL: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=a*b; DISPATCH();
    }
    L_DIV: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=a/b; DISPATCH();
    }
    L_DUP: {
        vm->stack[vm->sp]=vm->stack[vm->sp-1]; vm->sp++; DISPATCH();
    }
    L_SWAP: {
        double a=vm->stack[vm->sp-1], b=vm->stack[vm->sp-2];
        vm->stack[vm->sp-1]=b; vm->stack[vm->sp-2]=a; DISPATCH();
    }
    L_STORE: {
        int vi=vm->code[vm->ip++];
        vm->vars[vi]=vm->stack[--vm->sp]; DISPATCH();
    }
    L_LOAD: {
        int vi=vm->code[vm->ip++];
        vm->stack[vm->sp++]=vm->vars[vi]; DISPATCH();
    }
    L_CMP: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->flags = (a==b?0:(a>b?1:-1)); DISPATCH();
    }
    L_JMP: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        vm->ip+=off; DISPATCH();
    }
    L_JZ: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags==0) vm->ip+=off; DISPATCH();
    }
    L_JNZ: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags!=0) vm->ip+=off; DISPATCH();
    }
    L_JE: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags==0) vm->ip+=off; DISPATCH();
    }
    L_JNE: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags!=0) vm->ip+=off; DISPATCH();
    }
    L_JG: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags>0) vm->ip+=off; DISPATCH();
    }
    L_JL: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        if(vm->flags<0) vm->ip+=off; DISPATCH();
    }
    L_CALL: {
        int8_t off=(int8_t)vm->code[vm->ip++];
        vm->ret_addr[vm->csp++]=vm->ip;
        vm->ip+=off; DISPATCH();
    }
    L_RET: {
        vm->ip=vm->ret_addr[--vm->csp]; DISPATCH();
    }
    L_SIN: {
        double a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=sin(a); DISPATCH();
    }
    L_COS: {
        double a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=cos(a); DISPATCH();
    }
    L_POW: {
        double b=vm->stack[--vm->sp], a=vm->stack[--vm->sp];
        vm->stack[vm->sp++]=pow(a,b); DISPATCH();
    }
    L_PRINT: {
        printf("-> %.6g\n", vm->stack[vm->sp-1]); DISPATCH();
    }
    L_HALT:
        return;
}

// 將外部 .vm 檔編譯為 bytecode
static int compile_file(const char* src, VM* vm, uint8_t* out){
    FILE* f=fopen(src,"r");
    if(!f){ perror("fopen"); return 0; }
    char op[16];
    double val;
    int vi, ip=0;
    while(fscanf(f,"%15s",op)==1){
        if(!strcmp(op,"PUSH") && fscanf(f,"%lf",&val)==1){
            int idx=vm->const_count++;
            vm->constants[idx]=val;
            out[ip++]=OP_PUSH; out[ip++]=idx;
        } else if(!strcmp(op,"ADD"))    out[ip++]=OP_ADD;
        else if(!strcmp(op,"SUB"))       out[ip++]=OP_SUB;
        else if(!strcmp(op,"MUL"))       out[ip++]=OP_MUL;
        else if(!strcmp(op,"DIV"))       out[ip++]=OP_DIV;
        else if(!strcmp(op,"DUP"))       out[ip++]=OP_DUP;
        else if(!strcmp(op,"SWAP"))      out[ip++]=OP_SWAP;
        else if(!strcmp(op,"STORE") && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_STORE; out[ip++]=vi;
        } else if(!strcmp(op,"LOAD") && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_LOAD; out[ip++]=vi;
        } else if(!strcmp(op,"CMP"))     out[ip++]=OP_CMP;
        else if(!strcmp(op,"JMP")  && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JMP; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JZ")   && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JZ; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JNZ")  && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JNZ; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JE")   && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JE; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JNE")  && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JNE; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JG")   && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JG; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"JL")   && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_JL; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"CALL") && fscanf(f,"%d",&vi)==1){
            out[ip++]=OP_CALL; out[ip++]=(int8_t)vi;
        } else if(!strcmp(op,"RET"))   out[ip++]=OP_RET;
        else if(!strcmp(op,"SIN"))     out[ip++]=OP_SIN;
        else if(!strcmp(op,"COS"))     out[ip++]=OP_COS;
        else if(!strcmp(op,"POW"))     out[ip++]=OP_POW;
        else if(!strcmp(op,"PRINT"))   out[ip++]=OP_PRINT;
        else if(!strcmp(op,"HALT"))    out[ip++]=OP_HALT;
        // 其餘忽略
    }
    fclose(f);
    return ip;
}

// 主程式
int main(int argc,char**argv){
    if(argc!=2){
        fprintf(stderr,"用法：%s <程式.vm>\n",argv[0]);
        return 1;
    }
    VM vm; vm_init(&vm);
    static uint8_t bc[CODE_SIZE];
    int len = compile_file(argv[1], &vm, bc);
    vm_load(&vm, bc, len);
    vm_run(&vm);
    return 0;
}
