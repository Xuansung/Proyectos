module multiplicador(input wire [2:0] multiplicando, input wire [2:0] multiplicador, input wire clk, start, output wire [5:0] resultado, output wire fin);
 
// Cables
wire Desplaza, CargaQ, CargaM, CargaA, Resta, Suma, ResetA, ResetQ_1;
wire [3:0] A_out, M_out, suma_out;
wire [2:0] Q_out;
wire Q_1_out;
wire c_out;

wire [3:0] mult_ext;
assign mult_ext = {multiplicando[2], multiplicando};

// Unidad de control
uc unidadcontrol(Q_out[0], Q_1_out, start, clk, M_out, A_out, Q_out, CargaQ, CargaM, ResetA, CargaA, ResetQ_1, Desplaza, Suma, Resta, fin);

// Registro A (Acumulador) 
registro4 A_(suma_out, A_out[3], CargaA, Desplaza, clk, ResetA, A_out);
// Registro M (Multiplicando)
registro4 M_(mult_ext, 1'b0, CargaM, 1'b0, clk, 1'b0, M_out);
// Registro Q (Multiplicador) 
registro3 Q_(multiplicador, A_out[0], CargaQ, Desplaza, clk, 1'b0, Q_out);
// Flip-Flop Q-1
ffdc q1(clk, ResetQ_1, Desplaza, Q_out[0], Q_1_out);
// Sumador/Restador
sum_resta4 suma(suma_out, c_out, A_out, M_out, Resta);

assign resultado = {A_out[2:0], Q_out};

endmodule