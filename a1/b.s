    .pos 8
    call    0x00000077              # 8077000000
    rrmovq  %rax, %rax          # 2000
    rrmovq  %rax, %rbx          # 2003
    rrmovq  %rax, %rsp          # 2004
    rrmovq  %rdx, %rsi          # 2026
    rrmovq  %rdi, %rsi          # 2076
    irmovq  $0xdeadbeef, %rax       # 30f0efbeadde
    irmovq  $0xdeadbeef, %rcx       # 30f1efbeadde
    irmovq  $0xdeadbeef, %rdx       # 30f2efbeadde
    irmovq  $0xdeadbeef, %rbx       # 30f3efbeadde
    irmovq  $0xdeadbeef, %rsi       # 30f6efbeadde
    irmovq  $0xdeadbeef, %rdi       # 30f7efbeadde
    irmovq  $0xdeadbeef, %rsp       # 30f4efbeadde
    irmovq  $0xdeadbeef, %rbp       # 30f5efbeadde
    rmmovq  %rax, $0xdeadfeed(%rax) # 4000edfeadde
    byte.    14x
    popq    %rax                # B00F
    rmmovq  %rax, $0xbeefdead(%rcx) # 4001addeefbe
    rmmovq  %rax, $0xbeefdead(%rdx) # 4002addeefbe
    rmmovq  %rax, $0xdeadfeed(%rbx) # 4003edfeadde
    byte.    14x
    popq    %rax                # B00F
    rmmovq  %rax, $0xbeefdead(%rsi) # 4006addeefbe
    rmmovq  %rax, $0xbeefdead(%rdi) # 4007addeefbe
    rmmovq  %rax, $0xdeadfeed(%rsp) # 4004edfeadde
    byte.    14x
    popq    %rax                # B00F
    rmmovq  %rax, $0xfdeadfed(%rbp) # 4005eddfeafd
    byte.    14x
    rmmovq  %rax, $0xbeefdead(%rax) # 4000addeefbe
    rmmovq  %rcx, $0xbeefdead(%rax) # 4010addeefbe
    rmmovq  %rdx, $0xbeefdead(%rax) # 4020addeefbe
    rmmovq  %rbx, $0xbeefdead(%rax) # 4030addeefbe
    rmmovq  %rsi, $0x77778889(%rax) # 406089887777
    pushq   %r13                # A0dF
    quad.    14x
    popq    %rsp                # B04F
    jmp     0xbeefdead             # 70addeefbe
    rmmovq  %rsp, $0xbeefdead(%rax) # 4040addeefbe
    rmmovq  %rbp, $0xbeefdead(%rax) # 4050addeefbe
    mrmovq  $0xbeefdead(%rax), %rax # 5000addeefbe
    mrmovq  $0xbeefdead(%rcx), %rax # 5001addeefbe
    mrmovq  $0xbeefdead(%rdx), %rax # 5002addeefbe
    mrmovq  $0xbeefdead(%rbx), %rax # 5003addeefbe
    mrmovq  $0xbeefdead(%rsi), %rax # 5006addeefbe
    mrmovq  $0xbeefdead(%rdi), %rax # 5007addeefbe
    mrmovq  $0xbeefdead(%rsp), %rax # 5004addeefbe
    mrmovq  $0xbeefdead(%rbp), %rax # 5005addeefbe
    mrmovq  $0xbeefdead(%rax), %rax # 5000addeefbe
    mrmovq  $0xbeefdead(%rax), %rcx # 5010addeefbe
    mrmovq  $0xbeefdead(%rax), %rdx # 5020addeefbe
    mrmovq  $0xbeefdead(%rax), %rbx # 5030addeefbe
    mrmovq  $0xbeefdead(%rax), %rsi # 5060addeefbe
    mrmovq  $0xbeefdead(%rax), %rdi # 5070addeefbe
    mrmovq  $0xbeefdead(%rax), %rsp # 5040addeefbe
    mrmovq  $0xbeefdead(%rax), %rbp # 5050addeefbe
    addq    %rax, %rax          # 6000
    addq    %rax, %rcx          # 6001
    addq    %rax, %rdx          # 6002
    addq    %rax, %rbx          # 6003
    addq    %rax, %rsi          # 6006
    addq    %rax, %rdi          # 6007
    addq    %rax, %rsp          # 6004
    addq    %rax, %rbp          # 6005
    addq    %rcx, %rsi          # 6016
    addq    %rdx, %rsi          # 6026
    addq    %rbx, %rsi          # 6036
    addq    %rsi, %rsi          # 6066
    addq    %rdi, %rsi          # 6076
    addq    %rsp, %rsi          # 6046
    addq    %rbp, %rsi          # 6056
    addq    %rcx, %rax          # 6010
    addq    %rcx, %r8          # 6018
    addq    %rdx, %r9          # 6029
    addq    %rbx, %r10          # 603a
    addq    %rsi, %r11          # 606b
    addq    %rdi, %r12          # 607c
    addq    %rsp, %r13          # 604d
    addq    %rbp, %r14          # 605e
    addq    %r14, %rbp          # 60e5
    addq    %r12, %r8          # 60c8
    addq    %r12, %r9          # 60c9
    addq    %r11, %r10          # 60ba
    addq    %r10, %r11          # 60ab
    addq    %r9, %r12          # 609c
    addq    %r8, %r13          # 608d
    addq    %rsp, %r14          # 604e
    subq    %rcx, %rsi          # 6116
    andq    %rdx, %rsi          # 6226
    xorq    %rbx, %rsi          # 6336
    jmp     0x00daddad             # 70adddda00
    jle     0x00daddad             # 71adddda00
    jl      0x00daddda             # 72daddda00
    je      0x00daddad             # 73adddda00
    jne     0x00daddad             # 74adddda00
    jge     0x00daddad             # 75adddda00
    jg      0x00daddad             # 76adddda00
    cmovl   %rax, %rax          # 2200
    cmovl   %rax, %rbx          # 2203
    cmovl   %rax, %rsp          # 2204
    nop                         # 10
    cmovl   %rdx, %rsi          # 2226
    cmovl   %rdi, %rsi          # 2276
    call    0x90123456              # 8056341290
    byte.    7x
    nop                         # 10
    cmovl   %rdx, %rsi          # 2226
    cmovg   %rdi, %rsi          # 2676
    pushq   %rdx                # A02F
    pushq   %rbx                # A03F
    pushq   %rsi                # A06F
    pushq   %rdi                # A07F
    pushq   %rsp                # A04F
    pushq   %rbp                # A05F
    popq    %rax                # B00F
    popq    %rcx                # B01F
    popq    %rdx                # B02F
    popq    %rbx                # B03F
    popq    %rsi                # B06F
    popq    %rdi                # B07F
    popq    %rsp                # B04F
    popq    %rbp                # B05F
