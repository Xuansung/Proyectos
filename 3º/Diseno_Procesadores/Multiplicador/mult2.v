`timescale 1 ns / 10 ps
`default_nettype none

module multiplicador(
  input  wire signed [2:0] multiplicando,
  input  wire signed [2:0] multiplicador,
  input  wire              clk,
  input  wire              start,
  output wire signed [5:0] resultado,
  output wire              Fin
);

  wire Desplaza, CargaQ, CargaM, CargaA, Resta, Suma, ResetA, ResetQu;
  wire [3:0] A_out, M_out, suma_out;
  wire [2:0] Q_out;
  wire       Q_1_out;
  wire       c_out;

  // Extensión de signo del multiplicando (3 -> 4 bits) para evitar overflow con -4
  wire [3:0] M_ext = {multiplicando[2], multiplicando};

  // Unidad de control (q0 = Q[0], qu = Q-1)
  uc unidadcontrol(
    .q0(Q_out[0]),
    .qu(Q_1_out),
    .start(start),
    .clk(clk),
    .M(M_out),
    .A(A_out),
    .Q(Q_out),
    .CargaQ(CargaQ),
    .CargaM(CargaM),
    .ResetA(ResetA),
    .CargaA(CargaA),
    .ResetQu(ResetQu),
    .DesplazaA(Desplaza),
    .Suma(Suma),
    .Resta(Resta),
    .Fin(Fin)
  );

  // ALU: A +/- M (seleccionado por Resta)
  sum_resta4 alu(
    .S(suma_out),
    .c_out(c_out),
    .A(A_out),
    .B(M_out),
    .resta(Resta)
  );

  // Registro A (4 bits). Shift aritmético: entra el bit de signo A[3]
  registro4 A_(
    .entrada(suma_out),
    .bit_en_desp(A_out[3]),
    .Carga(CargaA),
    .Desplaza(Desplaza),
    .clk(clk),
    .reset(ResetA),
    .salida(A_out)
  );

  // Registro M (4 bits), solo carga en S0
  registro4 M_(
    .entrada(M_ext),
    .bit_en_desp(1'b0),
    .Carga(CargaM),
    .Desplaza(1'b0),
    .clk(clk),
    .reset(1'b0),
    .salida(M_out)
  );

  // Registro Q (3 bits). En el shift, entra A[0] por el MSB de Q
  registro3 Q_(
    .entrada(multiplicador),
    .bit_en_desp(A_out[0]),
    .Carga(CargaQ),
    .Desplaza(Desplaza),
    .clk(clk),
    .reset(1'b0),
    .salida(Q_out)
  );

  // Q-1: se actualiza durante el desplazamiento
  ffdc Qm1(
    .clk(clk),
    .reset(ResetQu),
    .carga(Desplaza),
    .d(Q_out[0]),
    .q(Q_1_out)
  );

  // Producto final: se descarta el bit extra de A
  assign resultado = {A_out[2:0], Q_out};

endmodule

`default_nettype wire
