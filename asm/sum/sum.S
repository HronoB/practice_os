.global sum
.global sum_struct
.global sum_array
.global sum_args

# rax <- sum(rdi, rsi)
sum:
    mov %rdi, %rax
    add %rsi, %rax
    ret



# (8,8,8) -> (_,_,24)
sum_struct:
    mov (%rdi), %rax
    add 8(%rdi), %rax
    mov %rax, 16(%rdi)
    ret




# sum(array(%rsi)=$rdi)
sum_array:
    mov $0, %rax

    test %rsi, %rsi
    jnz correct
    ret
    correct:

    arcycle:
    add (%rdi), %rax
    add $8, %rdi
    add $-1, %rsi

    jnz arcycle

    ret



//sumfromstack(n) = sum(stack-n(=%rdi) ... stack (=%rsp))
sum_args:
    mov $0, %rax


    test %rdi, %rdi
    jnz corr
    ret
    corr:

    mov %rsp, %rsi
    add $8, %rsi


    args_repeat:
    add (%rsi), %rax
    add $8, %rsi
    add $-1, %rdi
    jnz args_repeat

    ret



# int sum_args(int n, ...)
# Sums n integers passed on the stack
# First argument n is in %rdi
# Subsequent arguments are on stack after return address
suum_args:
    push %rbp                # save base pointer
    mov %rsp, %rbp           # set up frame pointer
    push %rbx                # save callee-saved register

    xor %rax, %rax           # sum = 0
    test %rdi, %rdi          # if n <= 0
    jle .Lsum_args_done      #   return 0

    mov %rdi, %rcx           # counter = n
    lea 16(%rbp), %rbx       # point to first argument on stack
                             # 8(%rbp) is return address
                             # 16(%rbp) is first argument

.Lsum_args_loop:
    add (%rbx), %rax         # add argument to sum
    add $8, %rbx             # move to next argument
    loop .Lsum_args_loop     # decrement counter and continue

.Lsum_args_done:
    pop %rbx                 # restore callee-saved register
    pop %rbp                 # restore base pointer
    ret