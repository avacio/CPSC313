    irmovq  $0x00000001, %rax       # 30f001000000
    irmovq  $0x00000002, %rax       # 30f002000000
    rrmovq  %rax, %rcx          # 2001
    irmovq  $0x00000002, %rdx       # 30f202000000
    irmovq  $0x00000003, %rdx       # 30f203000000
    rrmovq  %rdx, %rbx          # 2023
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000000, %rbx       # 30f300000000
    irmovq  $0x00000001, %rax       # 30f001000000
    addq    %rax, %rbx          # 6003
    irmovq  $0x00000002, %rcx       # 30f102000000
    nop                         # 10
    addq    %rcx, %rbx          # 6013
    irmovq  $0x00000003, %rdx       # 30f203000000
    nop                         # 10
    nop                         # 10
    addq    %rdx, %rbx          # 6023
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000010, %rbx       # 30f310000000
    irmovq  $0x00000001, %rax       # 30f001000000
    addq    %rbx, %rax          # 6030
    irmovq  $0x00000002, %rcx       # 30f102000000
    nop                         # 10
    addq    %rbx, %rcx          # 6031
    irmovq  $0x00000003, %rdx       # 30f203000000
    nop                         # 10
    nop                         # 10
    addq    %rbx, %rdx          # 6032
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000000, %rbx       # 30f300000000
    irmovq  $0x00001000, %rdi       # 30f700100000
    mrmovq  $0x00000000(%rdi), %rax # 500700000000
    addq    %rax, %rbx          # 6003
    mrmovq  $0x00000000(%rdi), %rcx # 501700000000
    nop                         # 10
    addq    %rcx, %rbx          # 6013
    mrmovq  $0x00000000(%rdi), %rdx # 502700000000
    nop                         # 10
    nop                         # 10
    addq    %rdx, %rbx          # 6023
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000010, %rbx       # 30f310000000
    irmovq  $0x00001000, %rdi       # 30f700100000
    mrmovq  $0x00000000(%rdi), %rax # 500700000000
    addq    %rbx, %rax          # 6030
    mrmovq  $0x00000000(%rdi), %rcx # 501700000000
    nop                         # 10
    addq    %rbx, %rcx          # 6031
    mrmovq  $0x00000000(%rdi), %rdx # 502700000000
    nop                         # 10
    nop                         # 10
    addq    %rbx, %rdx          # 6032
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000000, %rbx       # 30f300000000
    irmovq  $0x00000000, %rsp       # 30f400000000
    andq    %rax, %rax          # 6200
    je      0x00000653             # 7353060000
    irmovq  $0x00000001, %rcx       # 30f101000000
    irmovq  $0x00000001, %rdx       # 30f201000000
    nop                         # 10
    irmovq  $0x00000001, %rbx       # 30f301000000
    irmovq  $0x00000001, %rsp       # 30f401000000
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    irmovq  $0x00000000, %rbx       # 30f300000000
    irmovq  $0x00000000, %rsp       # 30f400000000
    andq    %rax, %rax          # 6200
    jne     0x00000752             # 7452070000
    irmovq  $0x00000001, %rcx       # 30f101000000
    irmovq  $0x00000001, %rdx       # 30f201000000
    irmovq  $0x00000001, %rbx       # 30f301000000
    irmovq  $0x00000001, %rsp       # 30f401000000
    irmovq  $0x0000f028, %rsp       # 30f428f00000
    irmovq  $0x00000000, %rax       # 30f000000000
    irmovq  $0x00000000, %rcx       # 30f100000000
    irmovq  $0x00000000, %rdx       # 30f200000000
    call    0x0000083c              # 803c080000
    irmovq  $0x00000001, %rcx       # 30f101000000
    irmovq  $0x00000001, %rax       # 30f001000000
    ret                         # 90
    irmovq  $0x00000001, %rdx       # 30f201000000
    irmovq  $0x00000001, %rax       # 30f001000000
    irmovq  $0x00000002, %rbx       # 30f302000000
    xorq    %rcx, %rcx          # 6311
    cmovne  %rax, %rbx          # 2403
