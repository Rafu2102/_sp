# 習題5: 寫一個 C 語言的程式，然後用 gcc 與 objdump 做出下列結果

我不知道要做什麼所以去請教了ChatGPT  
最後決定做簡單的 C 程式──遞迴計算階乘（factorial）  
然後再依序生成組合語言、編譯目標檔，並觀察反組譯與段表資訊  

## 1. 撰寫 C 原始程式：factorial.c

``` 

// factorial.c
#include <stdio.h>

// 計算 n 的階乘
int factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

int main() {
    int n = 5;
    int result = factorial(n);
    printf("factorial(%d) = %d\n", n, result);
    return 0;
}

``` 

## 2. 產生組合語言：factorial.s

### 輸入:

``` 

gcc -O0 -std=c99 -S factorial.c -o factorial.s

``` 

### 產生組合語言:

``` 

	.file	"factorial.c"
	.text
	.globl	factorial
	.def	factorial;	.scl	2;	.type	32;	.endef
	.seh_proc	factorial
factorial:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movl	%ecx, 16(%rbp)
	cmpl	$1, 16(%rbp)
	jg	.L2
	movl	$1, %eax
	jmp	.L3
.L2:
	movl	16(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %ecx
	call	factorial
	imull	16(%rbp), %eax
.L3:
	addq	$32, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "factorial(%d) = %d\12\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	movl	$5, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	%eax, %ecx
	call	factorial
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %edx
	movl	-4(%rbp), %eax
	leaq	.LC0(%rip), %rcx
	movl	%edx, %r8d
	movl	%eax, %edx
	call	__mingw_printf
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev5, Built by MSYS2 project) 15.1.0"

``` 

## 3. 編譯成目標檔：factorial.o

### 輸入:

``` 

gcc -std=c99 -c factorial.c -o factorial.o

``` 

### 產生目標檔

##　4. 列印目標檔的反組譯：factorial_dis.txt

### 輸入:

``` 

objdump -d factorial.o > factorial_dis.txt

``` 

### 列印目標檔的反組譯:

``` 

factorial.o:     file format pe-x86-64

Disassembly of section .text:

0000000000000000 <factorial>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 20          	sub    $0x20,%rsp
   8:	89 4d 10             	mov    %ecx,0x10(%rbp)
   b:	83 7d 10 01          	cmpl   $0x1,0x10(%rbp)
   f:	7f 07                	jg     18 <factorial+0x18>
  11:	b8 01 00 00 00       	mov    $0x1,%eax
  16:	eb 11                	jmp    29 <factorial+0x29>
  18:	8b 45 10             	mov    0x10(%rbp),%eax
  1b:	83 e8 01             	sub    $0x1,%eax
  1e:	89 c1                	mov    %eax,%ecx
  20:	e8 db ff ff ff       	call   0 <factorial>
  25:	0f af 45 10          	imul   0x10(%rbp),%eax
  29:	48 83 c4 20          	add    $0x20,%rsp
  2d:	5d                   	pop    %rbp
  2e:	c3                   	ret

000000000000002f <main>:
  2f:	55                   	push   %rbp
  30:	48 89 e5             	mov    %rsp,%rbp
  33:	48 83 ec 30          	sub    $0x30,%rsp
  37:	e8 00 00 00 00       	call   3c <main+0xd>
  3c:	c7 45 fc 05 00 00 00 	movl   $0x5,-0x4(%rbp)
  43:	8b 45 fc             	mov    -0x4(%rbp),%eax
  46:	89 c1                	mov    %eax,%ecx
  48:	e8 b3 ff ff ff       	call   0 <factorial>
  4d:	89 45 f8             	mov    %eax,-0x8(%rbp)
  50:	8b 55 f8             	mov    -0x8(%rbp),%edx
  53:	8b 45 fc             	mov    -0x4(%rbp),%eax
  56:	48 8d 0d 00 00 00 00 	lea    0x0(%rip),%rcx        # 5d <main+0x2e>
  5d:	41 89 d0             	mov    %edx,%r8d
  60:	89 c2                	mov    %eax,%edx
  62:	e8 00 00 00 00       	call   67 <main+0x38>
  67:	b8 00 00 00 00       	mov    $0x0,%eax
  6c:	48 83 c4 30          	add    $0x30,%rsp
  70:	5d                   	pop    %rbp
  71:	c3                   	ret
  72:	90                   	nop
  73:	90                   	nop
  74:	90                   	nop
  75:	90                   	nop
  76:	90                   	nop
  77:	90                   	nop
  78:	90                   	nop
  79:	90                   	nop
  7a:	90                   	nop
  7b:	90                   	nop
  7c:	90                   	nop
  7d:	90                   	nop
  7e:	90                   	nop
  7f:	90                   	nop

``` 

## 5. 列印目標檔的標頭：factorial_hdr.txt

### 輸入:

``` 

objdump -h factorial.o > factorial_hdr.txt

``` 

### 列印目標檔的標頭：

``` 

factorial.o:     file format pe-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         00000080  0000000000000000  0000000000000000  0000012c  2**4
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  00000000  2**4
                  ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000000  2**4
                  ALLOC
  3 .xdata        00000018  0000000000000000  0000000000000000  000001ac  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .pdata        00000018  0000000000000000  0000000000000000  000001c4  2**2
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
  5 .rdata        00000020  0000000000000000  0000000000000000  000001dc  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .rdata$zzz    00000030  0000000000000000  0000000000000000  000001fc  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, DATA

``` 