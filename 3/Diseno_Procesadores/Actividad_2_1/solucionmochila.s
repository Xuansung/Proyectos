inicio:
    li   r0, 0
    li   r11, 22           
    lw   r1, r0, 0         
    lw   r2, r0, 1        
    addi r3, r2, 1
    addi r13, r1, 2        
    li   r5, 0            
    add  r8, r11, r0        
init_loop:
    sw   r0, r8, 0         
    addi r8, r8, 1         
    addi r5, r5, 1         
    sub  r10, r3, r5      
    jn   init_loop         
    li   r4, 1            
outer_loop:
    sub  r10, r1, r4        
    jn   do_outer
    jz   do_outer
    jmp  end_outer         
do_outer:
    add  r8, r11, r0       
    li   r12, 1             
stride_loop:
    sub  r10, r4, r12     
    jz   stride_done       
    add  r8, r8, r3        
    addi r12, r12, 1       
    jmp  stride_loop
stride_done:
    add  r9, r8, r3        
    li   r5, 0            
inner_loop:
    sub  r10, r2, r5     
    jn   do_inner
    jz   do_inner
    jmp  next_outer     
do_inner:
    add  r6, r4, r13
    addi r6, r6, -1        
    lw   r6, r6, 0        
    sub  r10, r5, r6       
    jn   else_branch     
    jz   else_branch     
then_branch:
    lw   r10, r8, 0         
    sw   r10, r9, 0        
    jmp  end_if
else_branch:
    addi r7, r4, 1     
    lw   r7, r7, 0         
    lw   r10, r8, 0       
    add  r6, r4, r13
    addi r6, r6, -1
    lw   r6, r6, 0      
    neg  r6, r6           
    add  r6, r8, r6       
    lw   r6, r6, 0      
    add  r6, r6, r7        
    sub  r7, r6, r10     
    jnn  store_old      
    sw   r6, r9, 0         
    jmp  end_if
store_old:
    sw   r10, r9, 0       
end_if:
    addi r8, r8, 1       
    addi r9, r9, 1          
    addi r5, r5, 1         
    jmp  inner_loop
next_outer:
    addi r4, r4, 1       
    jmp  outer_loop
end_outer:
    add  r10, r8, r2    
    lw   r10, r10, 0     
    sw   r10, r0, 0       
    halt