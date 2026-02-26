

.globl savectx
.globl jumpctx














savectx:
    #save rip
    mov (%rsp), %rax
    mov %rax, (%rdi)

    #save rsp
    mov %rsp, %rax
    add $8, %rax
    mov %rax, 8(%rdi)

    #rbp frame
    mov %rbp, 16(%rdi)

    mov $0, %rax
    ret




jumpctx:
    mov 8(%rdi), %rsp

    # stck.push(ctx.0)
    mov (%rdi), %rax
    push %rax

    mov 16(%rdi), %rbp

    mov $1, %rax
    ret











































































































































