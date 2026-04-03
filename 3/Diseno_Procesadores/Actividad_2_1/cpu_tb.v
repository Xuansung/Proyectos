`timescale 1 ns / 10 ps
module cpu_tb;
reg clk, reset;

// generación de reloj clk
initial clk = 0;  
always
begin
  #10 clk = ~clk;
end

// instanciación del procesador
cpu micpu(clk, reset);

initial
begin
  $dumpfile("C:/Users/estudiante/Downloads/proyect1_act1/cpu_tb.vcd");
  $dumpvars;
  reset = 1;
  #25;          // esperar 1 flanco de subida completo antes de bajar reset
  reset = 0;
end

initial
begin
  #(20000*20); 
  $finish;
end
endmodule