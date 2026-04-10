inicio:
    li   r0, 0          // r0 = 0 (Se usa r0 como un registro que siempre vale 0)
    lw   r1, r0, 0      // r1 = n (Carga la cantidad de elementos desde la dirección de memoria 0)
    lw   r2, r0, 1      // r2 = W (Carga la capacidad de la mochila desde la dirección de memoria 1)
    addi r3, r2, 1      // r3 = W + 1 (Calcula el tamaño del vector dp, aunque no se usa más adelante)
    addi r12, r0, 2     // r12 = 2. Fija la dirección base del array de 'valores' en la posición 2.
    addi r13, r1, 2     // r13 = n + 2. Calcula la dirección base del array de 'pesos'.
    add  r11, r1, r1    // r11 = 2 * n
    addi r11, r11, 2    // r11 = 2*n + 2. Calcula la dirección base del array 'dp'.
    li   r4, 0          // r4 = 0. Inicializa r4, que usaremos como contador.

init_loop:              // BUCLE PARA PONER LA MEMORIA A CERO
    sub r6, r4, r2      // r6 = r2 - r4 = (W - contador). Recuerda: sub calcula el 2º operando menos el 1º.
    jn init_fin         // Si W < contador (da negativo), hemos terminado de limpiar la memoria, saltar a fin.
    add r7, r11, r4     // r7 = Dirección de dp[contador] (dirección base r11 + desplazamiento r4).
    sw r0, r7, 0        // Guarda 0 (el valor de r0) en la dirección calculada de dp[contador].
    addi r4, r4, 1      // Incrementa el contador en 1.
    jmp init_loop       // Vuelve a comprobar el siguiente elemento para ponerlo a 0.
    
init_fin:               // FIN DE LA INICIALIZACIÓN
    li r4, 0            // r4 = 0. Reinicia r4. A partir de aquí será 'i' (el índice de los objetos).

for1:                   // BUCLE EXTERIOR (Recorre los objetos)
    sub r6, r4, r1      // r6 = r1 - r4 = (n - i). Comprueba si ya procesamos todos los objetos.
    jz for1_fin         // Si n - i es 0, i == n. El algoritmo ha terminado.
    add r5, r0, r2      // r5 = W. Inicializa 'w' (peso actual iterado) con la capacidad máxima W.

for2:                   // BUCLE INTERIOR (Recorre los pesos desde W hacia abajo)
    add r7, r13, r4     // r7 = Dirección de peso[i] (base r13 + índice r4).
    lw r8, r7, 0        // r8 = peso[i]. Carga el peso del objeto actual.
    
    sub r9, r8, r5      // r9 = r5 - r8 = (w - peso[i]). Comprueba cuánto espacio sobra si metemos el objeto.
    jn next_i           // Si es negativo (el objeto pesa más que w), no cabe. Salta al siguiente objeto.
    
    add r7, r11, r9     // r7 = Dirección de dp[w - peso[i]] (base r11 + espacio sobrante r9).
    lw r14, r7, 0       // r14 = dp[w - peso[i]]. Carga el valor máximo de lo que cabe en el espacio sobrante.
    
    add r7, r12, r4     // r7 = Dirección de valor[i] (base r12 + índice r4).
    lw r15, r7, 0       // r15 = valor[i]. Carga el valor del objeto actual.
    
    add r14, r14, r15   // r14 = dp[w - peso[i]] + valor[i]. Este es el VALOR CANDIDATO (NUEVO) para la mochila.
    
    add r7, r11, r5     // r7 = Dirección de dp[w] (base r11 + peso actual r5).
    lw r15, r7, 0       // r15 = dp[w]. Carga el valor actual de la mochila sin este objeto (VALOR VIEJO).
    
    sub r9, r15, r14    // r9 = r14 - r15 = (VALOR NUEVO - VALOR VIEJO).
    jn for2_fin         // Si NUEVO < VIEJO (da negativo), el candidato es peor. Salta y no guarda nada.
    
    sw r14, r7, 0       // Si llegó hasta aquí, el NUEVO es mejor o igual. Guarda el nuevo máximo en dp[w].

for2_fin:               // CONTINUACIÓN BUCLE INTERIOR
    subi r5, r5, 1      // Decrementa el peso que estamos evaluando en 1 (w = w - 1).
    jmp for2            // Vuelve a for2 para evaluar el siguiente peso con este mismo objeto.

next_i:                 // SALTO PARA EL SIGUIENTE OBJETO
    addi r4, r4, 1      // Incrementa el índice de objetos en 1 (i = i + 1).
    jmp for1            // Vuelve al bucle exterior para cargar el siguiente objeto.

for1_fin:               // FIN DEL ALGORITMO
    add r7, r11, r2     // r7 = Dirección de dp[W] (base r11 + capacidad máxima W).
    lw r10, r7, 0       // r10 = dp[W]. Carga la solución final (valor máximo absoluto) en el registro 10.
    halt                // Detiene por completo la ejecución de la CPU.
