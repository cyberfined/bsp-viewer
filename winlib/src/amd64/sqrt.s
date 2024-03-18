.section .note.GNU-stack,"",@progbits

.section .text
# float wl_sqrtf(float a)
.globl wl_sqrtf
.type wl_sqrtf,@function
wl_sqrtf:
    subq $4, %rsp
    movd %xmm0, (%rsp)
    flds (%rsp)
    fsqrt
    fstps (%rsp)
    movd (%rsp), %xmm0
    addq $4, %rsp
    ret
.size wl_sqrtf,.-wl_sqrtf
