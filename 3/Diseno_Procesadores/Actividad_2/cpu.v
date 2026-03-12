module cpu(input wire clk, reset, output wire [9:0] pc_salida);
//Procesador sin memoria de datos de un solo ciclo

    wire [7:0] opcode;
    wire z, c, n, s_inc, we3, wf, m_wr, r_inm, push, pop;
    wire [1:0] wd;
    wire [3:0] op_alu;

    cd caminodatos(clk, reset, r_inm, s_inc, r_inm, we3, wef, push, pop, m_wr, wd, op_alu, z, opcode, pc_salida);
    uc unidadcontrol(opcode, z, c, n, s_inc, we3, wf, m_wr, r_inm, push, pop, wd, op_alu);

endmodule
