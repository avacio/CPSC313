    pushq   %rax                # A00F
    popq    %rax                # B00F
    nop                         # 10
    ret                         # 90
    halt                        # 00
    halt                        # 00
    halt                        # 00
    halt                        # 00
    halt                        # 00
    jl      0x00000000             # 7200000000
    halt                        # 00
    halt                        # 00
    halt                        # 00
