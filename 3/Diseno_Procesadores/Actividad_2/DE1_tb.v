`timescale 1 ns / 10 ps

module DE1_tb;


reg clk, reset;
wire [9:0] pc;


// generación de reloj clk
always //siempre activo, no hay condición de activación
begin
  clk = 1'b1;
  #10;
  clk = 1'b0;
  #10;
end

// instanciación del procesador
DE1_System sistema(clk, reset, pc);

initial
begin
  $dumpfile("C:/Users/estudiante/Downloads/proyect1_act1/DE1_tb.vcd");
  $dumpvars;
  reset = 1;  //a partir del flanco de subida del reset empieza el funcionamiento normal
  #5;
  reset = 0;  //bajamos el reset 
end

initial
begin
  #(20*20);  //Esperamos 14 ciclos o 14 instrucciones
  $finish;
end

endmodule