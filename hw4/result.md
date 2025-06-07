# 執行結果

``` 

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw4> gcc -std=c99 mul3.c mul3.s -o mul3
>>

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw4> ./mul3
>>
mul3(3,2,5)=30

``` 

# 執行結果(64位元版本)

想測試更大數字乘法但是發現好像不行結果會變成負數  
去問了一下ChatGPT發現是因為一開始用的都是 32 位的整數運算（movl+imull）  
超出了32位元的運算造成運算的溢出所以結果變成了負數  
所以要改用 64 位元整數的運算  
在C層把函式與變數都宣告為 long long (或 int64_t)  
然後在匯編層使用 64 位元指令 movq、imulq，並操作 64 位元寄存器 (rax, rcx, rdx, r8)  

``` 

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw4> gcc -std=c99 mul3-64bit.c mul3-64bit.s -o mul3-64bit
>>
mul3-64bit.s: Assembler messages:
mul3-64bit.s: Warning: end of file in comment; newline inserted

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw4> ./mul3-64bit
>>
mul3(55555,9487666,5201314)=2741546472768003820

``` 