module cpu(input wire clk, reset, timer, output wire [9:0] pc_salida);
    wire [7:0] opcode;
    wire zero, c, n, s_inc, wd, we3, wf, m_wr, r_inm, push, pop, halt;
    wire [3:0] op_alu;
    wire tick;
    reg interrupcion;

    timer_1s tempo(clk, reset, tick);

    always @(posedge clk or posedge reset) begin
        if (reset)
            interrupcion <= 1'b0;
        else if (tick)
            interrupcion <= 1'b1;
        else
            interrupcion <= 1'b0;
    end

    cd caminodatos(interrupcion, clk, reset, r_inm, s_inc, we3, wf, push, pop, halt, m_wr, wd, ret, call, op_alu, zero, c, n, opcode, pc_salida);
    uc unidadcontrol(opcode, zero, c, n, s_inc, wd, we3, wf, m_wr, r_inm, push, pop, halt, ret, call, op_alu);
endmodule
