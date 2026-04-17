



.global saveStateAsm

.global getStateAsm

saveStateAsm:
    mov (%rsp), %rax
    mov %rax, 0(%rdi)

    lea 8(%rsp), %rax
    mov %rax, 8(%rdi)

    mov %rbp, 16(%rdi)
    mov %rbx, 24(%rdi)
    mov %r12, 32(%rdi)
    mov %r13, 40(%rdi)
    mov %r14, 48(%rdi)
    mov %r15, 56(%rdi)

    xor %rax, %rax
    ret

getStateAsm:
    mov 8(%rdi), %rsp
    mov 16(%rdi), %rbp
    mov 24(%rdi), %rbx
    mov 32(%rdi), %r12
    mov 40(%rdi), %r13
    mov 48(%rdi), %r14
    mov 56(%rdi), %r15

    mov (%rdi), %rax
    push %rax
    mov $1, %rax
    ret



























