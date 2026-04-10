inicio:
    li   r0, 0
    lw   r1, r0, 0
    lw   r2, r0, 1
    addi r3, r2, 1
    addi r12, r0, 2
    addi r13, r1, 2
    add  r11, r1, r1
    addi r11, r11, 2
    li   r4, 0
init_loop:
    sub r6, r4, r2
    jn init_fin
    add r7, r11, r4
    sw r0, r7, 0
    addi r4, r4, 1
    jmp init_loop
init_fin:
    li r4, 0
for1:
    sub r6, r4, r1
    jz for1_fin
    add r5, r0, r2

for2: 
    add r7, r13, r4
    lw r8, r7, 0
    
    sub r9, r8, r5
    jn next_i
    
    add r7, r11, r9
    lw r14, r7, 0
    
    add r7, r12, r4
    lw r15, r7, 0
    
    add r14, r14, r15
    
    add r7, r11, r5
    lw r15, r7, 0
    
    sub r9, r15, r14
    jn for2_fin
    
    sw r14, r7, 0

for2_fin:
    subi r5, r5, 1
    jmp for2

next_i:
    addi r4, r4, 1
    jmp for1

for1_fin:
    add r7, r11, r2
    lw r10, r7, 0
    halt