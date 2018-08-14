    rrmovq  %rax, %rbx          # 2003
    rmmovq  %rax, $0x00000004(%r10) # 400a04000000
    mrmovq  $0x00000004(%r10), %rax # 500a04000000
    pushq   %rax                # A00F
    popq    %rax                # B00F
    nop                         # 10
    ret                         # 90
    subq    %rcx, %rbx          # 6113
    cmovle  %rax, %r10          # 210a
    jl      0x00000000             # 7200000000
