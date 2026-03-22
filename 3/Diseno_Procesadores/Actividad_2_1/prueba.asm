inicio:
    li r1 , 5
    li r2 , 10
    add r3 , r1 , r2
    sub r4 , r2 , r1
    eq r5 , r1
    neg r6 , r2
    not r7 , r5
    sw r1 , r1 , 0
    lw r8 , r1 , 0
    nop
    call subrutina
    addi r3 , r3 , 1
    jz fin
    jnz part2
    addi r3 , r3 , 1
part2:
    subi r4 , r4 , 20
    jnn part3
    sw r3 , r1 , 2
    add r3 , r3 , r4
    lw r5 , r1 , 2
    subi r5 , r5 , 20
    jn part3
    addi r5 , r5 , 1
part3:
    halt
subrutina:
    li r1 , 1
    subi r1 , r1 , 1
    jnz fin
    ret
fin:
    halt
