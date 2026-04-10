; Solución al problema de la mochila (Knapsack) - Programación dinámica bottom-up
; Registros usados:
;   R0  - i (fila actual)
;   R1  - s (columna actual / capacidad)
;   R2  - n (número de items)
;   R3  - S (capacidad máxima)
;   R4  - valor temporal / resultado
;   R5  - peso del item actual (weights[i-1])
;   R6  - vals[i-1]
;   R7  - dirección base de memo[][]
;   R8  - dirección base de vals[]
;   R9  - dirección base de weights[]
;
; Layout de memoria de datos:
;   0x0000 - 0x00FF : memo[n+1][S+1], asumiendo n<=10, S<=15 (máx 176 palabras)
;   0x0100          : n
;   0x0101 - 0x010A : vals[0..9]
;   0x010B - 0x0114 : weights[0..9]
;   0x0115          : S

; -------------------------------------------------------
; Inicio: leer n
; -------------------------------------------------------
start:
    lw   R2, R0, 0x100      ; R2 = n  (mem[0x100])
    lw   R3, R0, 0x115      ; R3 = S  (mem[0x115])
    li   R7, 0x000          ; R7 = base de memo[][]

; -------------------------------------------------------
; for (int i = 0; i <= S; ++i) memo[0][i] = 0;
; memo[0][i] está en dirección base + 0*(S+1) + i = base + i
; -------------------------------------------------------
    li   R1, 0              ; i = 0
init_loop:
    eq   R4, R1, R3         ; R4 = (i == S)
    sw   R4, R7, 0          ; memo[0][i] = 0  (R4 aún es 0 si no eran iguales... ver nota)
    ; Nota: usamos R0=0 para escribir cero directamente
    sw   R0, R7, 0          ; memo[0][i] = 0  (R0 siempre 0)
    addi R7, R7, 1          ; avanzar puntero memo
    addi R1, R1, 1          ; i++
    ; if i > S salir: chequeamos con substracción
    sub  R4, R1, R3         ; R4 = i - S
    jnn  end_init           ; si R4 >= 0 (i > S), salir
    jmp  init_loop
end_init:

; -------------------------------------------------------
; for (int i = 1; i <= n; i++)
; -------------------------------------------------------
    li   R0, 0              ; R0 = 0 constante
    li   R1, 1              ; i = 1  (fila)
outer_loop:
    sub  R4, R1, R2         ; R4 = i - n
    jn   do_outer           ; si i < n, seguir
    eq   R4, R1, R2         ; chequear i == n
    jz   end_outer          ; si i > n, salir
do_outer:

    ; Calcular dirección base de memo[i-1] y memo[i]
    ; memo[row][col] = base_memo + row*(S+1) + col
    ; Usamos addi sucesivos (limitación del ISA sin multiplicación)
    ; Asumimos S+1 <= 16 y precalculamos con bucle si fuera necesario
    ; Para simplicidad aquí asumimos S+1 conocido en tiempo de ensamblado (ej: S=7, S+1=8)

    ; --- for (int s = 0; s <= S; s++) ---
    li   R6, 0              ; s = 0
inner_loop:
    sub  R4, R6, R3         ; R4 = s - S
    jn   do_inner           ; s < S, continuar
    eq   R4, R6, R3
    jz   next_outer         ; s > S, siguiente i
do_inner:

    ; Cargar weights[i-1]: dirección = 0x10B + (i-1)
    addi R5, R1, 0x10A      ; R5 = 0x10A + i = dirección de weights[i-1]
    lw   R5, R5, 0          ; R5 = weights[i-1]

    ; if (weights[i-1] > s) => memo[i][s] = memo[i-1][s]
    sub  R4, R5, R6         ; R4 = weights[i-1] - s
    jn   else_branch        ; si weights[i-1] < s, ir a else
    jz   else_branch        ; si weights[i-1] == s, ir a else

    ; weights[i-1] > s: memo[i][s] = memo[i-1][s]
    ; dirección memo[i-1][s]: necesitamos (i-1)*(S+1)+s
    ; Simplificado: usamos R7 como puntero actualizado en cada iteración
    lw   R4, R7, 0          ; R4 = memo[i-1][s]
    sw   R4, R7, 8          ; memo[i][s] = memo[i-1][s]  (offset S+1=8 asumido)
    jmp  end_if

else_branch:
    ; memo[i][s] = max(memo[i-1][s], memo[i-1][s-weights[i-1]] + vals[i-1])
    lw   R4, R7, 0          ; R4 = memo[i-1][s]

    ; s - weights[i-1]
    sub  R8, R6, R5         ; R8 = s - weights[i-1]
    ; dirección memo[i-1][s-weights[i-1]] = base + (i-1)*(S+1) + (s-w)
    ; usamos offset desde R7: R7 apunta a memo[i-1][s], retrocedemos weights[i-1]
    neg  R9, R5             ; R9 = -weights[i-1]
    add  R9, R7, R9         ; R9 = dirección de memo[i-1][s-weights[i-1]]
    lw   R9, R9, 0          ; R9 = memo[i-1][s-w]

    ; vals[i-1]: dirección = 0x101 + (i-1)
    addi R8, R1, 0x100      ; R8 = 0x100 + i = dirección vals[i-1]
    lw   R8, R8, 0          ; R8 = vals[i-1]
    add  R9, R9, R8         ; R9 = memo[i-1][s-w] + vals[i-1]

    ; max(R4, R9)
    sub  R8, R4, R9         ; R8 = memo[i-1][s] - candidato
    jnn  store_old          ; si >= 0, memo[i-1][s] es mayor
    sw   R9, R7, 8          ; memo[i][s] = R9
    jmp  end_if
store_old:
    sw   R4, R7, 8          ; memo[i][s] = R4

end_if:
    addi R7, R7, 1          ; avanzar columna en memo
    addi R6, R6, 1          ; s++
    jmp  inner_loop

next_outer:
    addi R1, R1, 1          ; i++
    jmp  outer_loop

end_outer:
    ; Resultado en memo[n][S]
    ; dirección = base + n*(S+1) + S
    ; resultado ya calculado, cargarlo
    lw   R4, R7, 0          ; R4 = memo[n][S] (resultado final)
    halt
