module multiplicador(input wire [2:0] multiplicando, input wire [2:0] multiplicador, input wire clk, start, reset, output wire [5:0] resultado, output wire fin);
    reg q0, q1;
    reg [3:0] A;
    reg [2:0] Q;
    
    wire Desplaza, CargaQ, CargaM, CargaA, Resta, Suma, ResetA, ResetQ_1;
    uc unidadcontrol(q0, qu, start, clk, multiplicando, multiplicador, P, CargaQ, CargaM, ResetA, CargaA, ResetQ_1, );

    registro4 A_(4'b0000, A[3], CargaA, Desplaza, clk, ResetA, A);
    registro3 Q_(3'b000, A[0], CargaQ, Desplaza, clk, 1'b0, Q);
    registr

endmodule
