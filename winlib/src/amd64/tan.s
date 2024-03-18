.section .note.GNU-stack,"",@progbits

.section .text
# float wl_tanf(float a)
.globl wl_tanf
.type wl_tanf,@function
wl_tanf:
    subq $4, %rsp
    movd %xmm0, (%rsp)
    flds (%rsp)
    fptan
    fnstsw %ax
    test $4, %ah # Check C2 flag
    je 2f        # if abs(a) < 2^63

    # st(0) — a, st(1) — 2*pi
    fldpi 
    fadd %st(0), %st(0)
    fxch

1:
    # st(0) %= st(1)
    fprem1
    fnstsw %ax
    test $4, %ah
    jne 1b

    ffree %st(1)
    fptan
2:
    fxch
    ffree %st(1)
    fstps (%rsp)
    movd (%rsp), %xmm0
    addq $4, %rsp
    ret
.size wl_tanf,.-wl_tanf
