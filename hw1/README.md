# Compiler

## 語法

```
PROG = STMTS
BLOCK = { STMTS }
STMTS = STMT*
STMT = DOWHILE | WHILE | BLOCK | ASSIGN
WHILE = while (E) STMT
DOWHILE = do { STMTS } while (E); //加上do while 迴圈
ASSIGN = id '=' E;
E = F (op E)*
F = (E) | Number | Id
```

## 執行結果

```
C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw1> ./mycompiler test_do.c
$ make clean
rm -f *.o *.exe

C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw1> ./mycompiler test_do.c
$ make
gcc -std=c99 -O0 lexer.c compiler.c main.c -o compiler

C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw1> ./mycompiler test_do.c
$ ./compiler test_do.c

========== lex ==============
token=do
token={
token=x
token==
token=x
token=+
token=2
token=}
token=while
token=(
token=x
token=<
token=5
token=)
token=;
========== dump ==============
0:do
1:{
2:x
3:=
4:x
5:+
6:2
7:;
8:}
9:while
10:(
11:x
12:<
13:5
14:)
15:;
============ parse =============
(L0)
t0 = x
t1 = 2
t2 = t0 + t1
x = t2
t3 = x
t4 = 5
t5 = t3 < t4
if T5 goto L1
goto L0
(L1)
``` 