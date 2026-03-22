module cpu(input wire clk, reset, output wire [9:0] pc_salida);
    wire [7:0] opcode;
    wire zero, c, n, s_inc, wd, we3, wf, m_wr, r_inm, push, pop, halt;
    wire [3:0] op_alu;
    cd caminodatos(clk, reset, r_inm, s_inc, we3, wf, push, pop, halt, m_wr, wd, op_alu, zero, c, n, opcode, pc_salida);
    uc unidadcontrol(opcode, zero, c, n, s_inc, wd, we3, wf, m_wr, r_inm, push, pop, halt, op_alu);
endmodule
