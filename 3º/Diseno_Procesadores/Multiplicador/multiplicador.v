module multiplicador(input wire [3:0] multiplicando, input wire [2:0] multiplicador, input wire clk, start, output wire [5:0] resultado, output wire fin);

wire Desplaza, CargaQ, CargaM, CargaA, Resta, Suma, ResetA, ResetQ_1;
wire [3:0] A_in, A_out, M_out, suma_out;
wire [2:0] Q_out;
wire Q_1_out;
wire c_out;
uc unidadcontrol(q0, qu, start, clk, multiplicando, A_out, multiplicador, CargaQ, CargaM, ResetA, CargaA, ResetQ_1, Desplaza, Suma, Resta, fin);

registro4 A_(suma_out, A_out[3], CargaA, Desplaza, clk, ResetA, A_out);
registro4 M_(multiplicando, 1'b0, CargaM, 1'b0, clk, 1'b0, M_out);
registro3 Q_(multiplicador, A_out[0], CargaQ, Desplaza, clk, 1'b0, Q_out);

ffdc q1(clk, ResetQ_1, Desplaza, Q_out[0], Q_1_out);

sum_resta4 suma(suma_out,c_out, A_out, M_out, Resta);

// El resultado se obtiene concatenando el registro A con el registro Q
assign resultado = {A_out[2:0], Q_out};

endmodule