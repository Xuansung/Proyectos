module cd(input wire clk, reset, s_inc, s_inm, we3, wez, input wire [2:0] op_alu, output wire z, output wire [5:0] opcode);
//Camino de datos de instrucciones de un solo ciclo
    wire [15:0] instruccion;
    wire [9:0] pc, pc_nuevo, dir_salto, salida_sum;
    wire [3:0] ra1, ra2, wa3;
    wire [7:0] rd1, rd2, wd3, salida_alu, inmediato;
    wire zalu, zero;

    assign opcode = instruccion[15:10];
    assign ra1 = instruccion[11:8];
    assign ra2 = instruccion[7:4];
    assign wa3 = instruccion[3:0];
    assign dir_salto = instruccion[9:0];
    assign inmediato = instruccion[11:4];
    assign z = zero;

    registro PCR(clk, reset, pc_nuevo, pc);
    regfile regs(clk, we3, ra1, ra2, wa3, wd3, rd1, rd2);
    memprog memoria(clk, pc, instruccion);
    ffd zer(clk, reset, zalu, wez, zero);
    alu mialu(rd1, rd2, op_alu, salida_alu, zalu);
    mux2 #(10) muxpc(dir_salto, salida_sum, s_inc, pc_nuevo);
    mux2 #(8) muxinm(salida_alu, inmediato, s_inm, wd3);
    sum suma_inc(10'b0000000001, pc, salida_sum);

endmodule
