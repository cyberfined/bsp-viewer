.section .note.GNU-stack,"",@progbits

.section .text
# float wl_sinf(float a)
.globl wl_sinf
.type wl_sinf,@function
wl_sinf:
    subq $4, %rsp
    movd %xmm0, (%rsp)
    flds (%rsp)
    fsin
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
    fsin
2:
    fstps (%rsp)
    movd (%rsp), %xmm0
    addq $4, %rsp
    ret
.size wl_sinf,.-wl_sinf
