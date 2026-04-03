module cd(input wire interrupcion, clk, reset, r_inm, s_inc, we3, wef, push, pop, halt, m_wr, s_wd, ret, call,input wire [3:0] op_alu, output wire zero, carry, negativo, output wire [7:0] opcode, output wire [9:0] pc_salida);
//Camino de datos de instrucciones de un solo ciclo
    wire [31:0] instruccion;
    wire [9:0] pc, salida_sum, dir_salto, pc_siguiente, pc_final, pc_nuevo, pc_nuevo_inc, salida_pila;
    wire [3:0] ra1, ra2, wa3;
    wire [15:0] rd1, rd2, wd3, inmediato, salida_alu, salida_memoria, salida_muxinm;
    wire zzero, zneg, zcarry, push_real, interrupcion_real;

    assign opcode = instruccion[31:24];
    mux2 #(4) mux_ra1(instruccion[15:12], instruccion[19:16], r_inm, ra1);
    mux2 #(4) mux_ra2(instruccion[19:16], instruccion[23:20], r_inm, ra2);
    assign wa3 = instruccion[23:20];
    assign dir_salto = instruccion[15:6];
    assign inmediato = instruccion[15:0];

    assign interrupcion_real = interrupcion & ~ret & ~call; // Solo generar interrupcion si no es un retorno de subrutina
    wire [9:0] pc_int;
    assign pc_int = interrupcion_real ? 10'd30 : pc_nuevo;
    registro PCR(clk, reset, halt, pc_int, pc);
    regfile regs(clk, we3, ra1, ra2, wa3, wd3, rd1, rd2);
    memprog memoriaprograma(clk, pc, instruccion);


    // flags
    ffd zerff(clk, reset, zzero, wef, zero);
    ffd negff(clk, reset, zneg, wef, negativo);
    ffd carryff(clk, reset, zcarry, wef, carry);
    

    alu mialu(rd1, salida_muxinm, op_alu, salida_alu, zzero, zneg, zcarry);
    mux2 #(10) muxpc(dir_salto, salida_sum, s_inc, pc_nuevo_inc);
    sum #(10) suma_inc(10'b0000000001, pc, salida_sum);

    // parte de ampliacion
    assign push_real = (push | interrupcion_real); // Hacer push tambien en interrupciones
    mux2 #(16) muxescritura(salida_alu, salida_memoria, s_wd, wd3);
    stack pila(clk, reset, salida_sum, push_real, pop, salida_pila );
    memdat memoriadatos(clk, m_wr, salida_alu, rd2, salida_memoria);
    mux2 #(10) muxpila(pc_nuevo_inc, salida_pila, pop, pc_nuevo);
    mux2 #(16) muxr1(rd2, inmediato, r_inm, salida_muxinm);

    assign pc_salida = pc;

endmodule