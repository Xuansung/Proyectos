module cpu(input wire clk, reset, output wire [9:0] pc_salida);
//Procesador sin memoria de datos de un solo ciclo

    wire [7:0] opcode;
    wire z, c, n, s_inc, we3, wf, m_wr, s_inm, push, pop;
    wire [1:0] wd;
    wire [3:0] op_alu;

    cd caminodatos(clk, reset, s_inc, s_inm, we3, wez, op_alu, z, opcode, pc_salida);
    uc unidadcontrol(opcode, z, c, n, s_inc, we3, wf, m_wr, s_inm, push, pop, wd, op_alu);

endmodule
