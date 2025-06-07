# 執行結果

``` 

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw5> gcc factorial.c -o factorial
>>

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw5> ./factorial
>>
factorial(5) = 120

``` 

# 執行結果(64位元版本)

跟上個一樣因為32位元在超過12階層時會顯示異常  
所以改用 64 位元整數的運算   
在C層把函式與變數都宣告為 long long (或 int64_t)   
然後在匯編層使用 64 位元指令 movq、imulq，並操作 64 位元寄存器 (rax, rcx, rdx, r8)   

``` 

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw5> gcc -std=c99 factorial-64bit.c -o factorial-64bit
>>

PS C:\Users\rafut\OneDrive\桌面\網頁\_sp\_sp\hw5> ./factorial-64bit
>>
factorial(20) = 2432902008176640000

``` 