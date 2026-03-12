// Testbench ejemplo

`timescale 1 ns / 10 ps //Directiva que fija la unidad de tiempo de simulaci�n y el paso de simulaci�n

module cuenta1_tb; //Declaracion de modulo del testbench


//declaracion de se�ales

reg clk;
reg start;
reg q0;

wire CargaQ, DesplazaQ, ResetA, CargaA, Fin;

cuenta uut(
  .q0(q0),
  .start(start),
  .clk(clk),
  .CargaQ(CargaQ),
  .DesplazaQ(DesplazaQ),
  .ResetA(ResetA),
  .CargaA(CargaA),
  .Fin(Fin)
);

always begin
  clk = 1;
  #20;
  clk = 0;
  #60;
end

initial begin
  $dumpfile("ondas.vcd"); // Nombre del archivo de salida
  $dumpvars(0, cuenta1_tb); // Graba todas las variables del testbench
  // Inicialización
  start = 0;
  q0 = 0;
  // Pulso de start (debe ser breve)
  start = 1;    // Ponemos start a alta
  #30;          // Lo mantenemos un tiempo (menos que un ciclo de reloj para que sea "breve")
  start = 0;    // Lo bajamos

  // Simulamos que q0 cambia para probar la lógica de CargaA
  // En los estados S1, S3 o S5, CargaA debería activarse si q0 es 1
  #80 q0 = 1;
  #80 q0 = 0;
  #80 q0 = 1;

// Esperar a que llegue al estado final S6
  #500;      
  $display("Simulación finalizada. Fin = %b", Fin);
  $stop; // Detiene la simulación en el visualizador de ondas
end
endmodule
