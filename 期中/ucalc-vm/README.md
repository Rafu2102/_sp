# 算域虛擬機

**類別**：棧式虛擬機 (Emulator) + 直譯器 (Interpreter)

## 一、期中作業介紹

這個算域虛擬機是一個高效能的棧式虛擬機（VM）與直譯器組合，可以做到：

- **動態執行** 透過 `.vm` 腳本檔指令（包括算術運算、條件跳躍、呼叫堆疊、三角與指數函式等）
- **可擴充指令集**，使用 computed goto 提升 dispatch 效率
- **獨立執行** 腳本，無須重新編譯 C 程式，即可微調與測試新邏輯

涵蓋:

1. **解譯器 (Interpreter)**：將高階指令轉為 bytecode  
2. **虛擬機 (VM/Emulator)**：實作 fetch-decode-execute  
3. **效能優化**：利用 GCC `-O2` 以及 computed goto 技術  

## 二、核心檔案說明

### 1. `ucalc_vm_ext.c`
- **VM 結構 (VM)**：包含棧、常數表、變數表、呼叫堆疊、指令緩衝區等，所有狀態皆保存在記憶體中。
- **指令集 (Opcode)**：超過 25 種指令，包括：
  - 算術：`ADD`, `SUB`, `MUL`, `DIV`
  - 棧操作：`PUSH`, `DUP`, `SWAP`
  - 變數存取：`STORE`, `LOAD`
  - 比較與分支：`CMP`, `JMP`, `JZ`, `JNZ`, `JE`, `JNE`, `JG`, `JL`
  - 呼叫堆疊：`CALL`, `RET`
  - 三角函式：`SIN`, `COS`
  - 指數函式：`POW`
  - 輸出：`PRINT`；程式結束：`HALT`
- **關鍵技術**：
  1. **Computed Goto**：透過 GCC 擴充的 `&&label` 及 `goto *`，將指令 dispatch 從 O(N) `switch` 降至 O(1)，大幅提升解譯效能。
  2. **最佳化編譯**：編譯時使用 `-O2`，並可搭配 `-march=native` 或 `-funroll-loops` 等參數進一步優化。

### 2. `prog_opt.vm`
- **檔案結構**：
  - 每行一條指令，採用空白分隔 opcode 與參數。
  - 支援註解（`//` 開頭）及空行，自動忽略。
- **範例內容**：
  1. `fact(5)`：遞迴計算 5 的階乘，使用 `CALL`/`RET` 進行流程控制。
  2. `fib(7)`：遞迴計算 Fibonacci，示範多重 `CALL` 深度。
  3. `gcd(48,18)`：歐幾里得算法，使用條件跳躍實作。
  4. `sin`, `cos`, `pow` 測試：展示數學函式指令應用。

## 三、執行結果

``` 

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\期中\ucalc-vm> gcc -O2 -std=c99 -lm ucalc_vm_ext.c -o ucalc-vm
>>

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\期中\ucalc-vm> ./ucalc-vm prog_opt.vm
>>
-> 120      # fact(5)
-> 13       # fib(7)
-> 6        # gcd(48,18)
-> 0        # sin(0)
-> 1        # sin(pi/2)
-> -1       # cos(pi)
-> 256      # pow(2,8)

``` 

## 四、實作

由ChatGPT提供主題構思
參考老師上課內容先做出雛形
之後由ChatGPT處理主要程式結構規劃與排版優化