    .globl mul3      # 將 mul3 設為全域符號
    .text            # 下面是程式碼段
    .align 16        # 16 位元對齊（效能最佳化）

mul3:
    # Windows x64 呼叫約定：a→rcx, b→rdx, c→r8
    movq    %rcx, %rax    # rax = a
    imulq   %rdx, %rax    # rax = rax * b
    imulq   %r8,  %rax    # rax = rax * c
    ret                   # 返回，結果放在 rax