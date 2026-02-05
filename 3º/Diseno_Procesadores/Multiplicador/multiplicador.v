module multiplicador(input wire [2:0] multiplicando, input wire [2:0] multiplicador, input wire clk, start, reset, output wire [5:0] resultado, output wire fin);

wire Desplaza, CargaQ, CargaM, CargaA, Resta, Suma, ResetA, ResetQ_1;
wire [3:0] A_in, A_out, M_out, suma_out;
wire [2:0] Q_out;
wire Q_1_out;
uc unidadcontrol(q0, qu, start, clk, multiplicando, multiplicador, P, CargaQ, CargaM, ResetA, CargaA, ResetQ_1, fin);

registro4 A_(4'b0000, A_in[3], CargaA, Desplaza, clk, ResetA, A_out);
registro4 M_(multiplicando, 1'b0, CargaM, 1'b0, clk, reset, M_out);
registro3 Q_(3'b000, A_in[0], CargaQ, Desplaza, clk, 1'b0, Q);

ffdc q1(clk, ResetQ_1, Desplaza, Q_out[0], Q_1_out);
sum_resta4 suma(suma_out, ,A_out, M_out, Resta);
assign resultado = {A_out[2:0], Q_out};

endmodule