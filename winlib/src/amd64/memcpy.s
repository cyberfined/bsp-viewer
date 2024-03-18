.section .note.GNU-stack,"",@progbits

.section .text
.globl wl_memcpy
.type wl_memcpy,@function
# void wl_memcpy(void *dst, void *src, uintptr_t size)
wl_memcpy:
    movq %rdx, %rcx
    shrq $3, %rcx
    je 1f
    rep movsq

1:
    movq %rdx, %rcx
    andq $7, %rcx
    rep movsb
    ret
.size wl_memcpy,.-wl_memcpy
