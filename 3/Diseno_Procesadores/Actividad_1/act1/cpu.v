module cpu(input wire clk, reset);
//Procesador sin memoria de datos de un solo ciclo

    wire [5:0] opcode;
    wire z, s_inc, s_inm, we3, wez;
    wire [2:0] op_alu;

    cd caminodatos(clk, reset, s_inc, s_inm, we3, wez, op_alu, z, opcode);
    uc unidadcontrol(opcode, z, s_inc, s_inm, we3, wez, op_alu);

endmodule
