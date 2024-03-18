.section .note.GNU-stack,"",@progbits

.section .text
# void wl_memset(void *dst, uint8_t c, size_t n)
.globl wl_memset
.type wl_memset,@function
wl_memset:
    movzbq %sil,%rax
    mov %rdx, %rcx
    shr $3, %rcx
    je 1f

    mov $0x0101010101010101, %r8
    imul %r8, %rax
    rep stosq

1:
    movq %rdx, %rcx
    and $7, %rcx
    rep stosb
    ret
.size wl_memset,.-wl_memset
