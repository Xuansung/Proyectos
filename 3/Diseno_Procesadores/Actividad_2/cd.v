module cd(input wire clk, reset, r_inm, s_inc, we3, wef, push, pop, m_wr, input wire [1:0] s_wd, input wire [3:0] op_alu, output wire z, c, n, output wire [7:0] opcode, output wire [9:0] pc_salida);
//Camino de datos de instrucciones de un solo ciclo
    wire [31:0] instruccion;
    wire [9:0] pc, salida_sum, dir_salto, pc_siguiente, pc_final, pc_nuevo, pc_nuevo_inc, salida_pila;
    wire [3:0] ra1, ra2, wa3;
    wire [15:0] rd1, rd2, wd3, inmediato, salida_alu, salida_memoria, salida_muxinm;
    wire zalu, zero, zneg, zcarry;

    assign opcode = instruccion[31:24];
    assign ra1 = r_inm ? instruccion[19:16] : instruccion[15:12];
    assign ra2 = r_inm ? instruccion[23:20] : instruccion[19:16];
    assign wa3 = instruccion[23:20];
    assign dir_salto = instruccion[15:6];
    assign inmediato = instruccion[15:0];

    registro PCR(clk, reset, pc_nuevo, pc);
    regfile regs(clk, we3, ra1, ra2, wa3, wd3, rd1, rd2);
    memprog memoriaprograma(clk, pc, instruccion);

    // flags
    ffd zer(clk, reset, zalu, wef, z);
    ffd neg(clk, reset, zneg, wef, n);
    ffd carry(clk, reset, zcarry, wef, c);

    alu mialu(rd1, salida_muxinm, op_alu, salida_alu, zalu, zneg, zcarry);
    mux2 #(10) muxpc(dir_salto, salida_sum, s_inc, pc_nuevo_inc);
    sum suma_inc(10'b0000000001, pc, salida_sum);

    // ampliación
    mux4 #(16) muxescritura(16'b0, salida_alu, salida_memoria, inmediato, s_wd, wd3);
    stack pila(clk, reset, salida_sum, push, pop, salida_pila );
    memdat memoriadatos(clk, m_wr, salida_alu, rd2, salida_memoria);
    mux2 #(10) muxpila(pc_nuevo_inc, salida_pila, pop, pc_nuevo);
    mux2 #(16) muxr1(rd2, inmediato, r_inm, salida_muxinm);

    assign pc_salida = pc;

endmodule
