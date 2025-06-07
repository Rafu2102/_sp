    .globl mul3     # 將符號 mul3 設為全域，讓連結器可以找到
    .text           # 下面都是程式碼段
    .align 16       # 16 位元對齊，符合效能最佳化需求

mul3:
    # Windows x64 呼叫約定（前四個整數／指標參數依序放到 rcx, rdx, r8, r9）
    # 本函式有三個整數參數：
    #   a -> ecx
    #   b -> edx
    #   c -> r8d

    mov     %ecx, %eax    # eax = a        ; 把第一個參數搬到 eax，準備累乘
    imul    %edx, %eax    # eax = eax * b  ; 用帶符號乘法，把第二個參數乘上
    imul    %r8d, %eax    # eax = eax * c  ; 再把第三個參數乘上
    ret                   # 回傳，結果已經在 eax 裡
