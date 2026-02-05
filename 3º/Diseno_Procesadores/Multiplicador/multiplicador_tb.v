`timescale 1 ns / 10 ps
module multiplicador_tb;
//Recordar que el caso de -4 en tres bits en el multiplicando dará error en Booth si no hacemos que los registros A y M tengan un bit más (hay overflow) (depende de la implementación del sumador/restador)
parameter NUM_BITS = 3;
parameter NUM_TESTS = (2**(2*NUM_BITS));
parameter MAX_RANGE = (2**(NUM_BITS-1))-1;
parameter MIN_RANGE = - (2**(NUM_BITS-1));
integer contador;
reg clk, start;
wire Fin;
reg signed [NUM_BITS-1:0] multiplicador;
reg signed [NUM_BITS-1:0] multiplicando;
wire [(2*NUM_BITS)-1:0] resultado;
reg signed [(2*NUM_BITS)-1:0] resultado_esperado;

// generación de reloj clk
always //siempre activo, no hay condición de activación
begin
  clk = 1'b0;
  #5;
  clk = 1'b1;
  #5;
end

// Módulo a probar
multiplicador mult(multiplicando, multiplicador, clk, start, resultado, Fin);

initial
begin
  $dumpfile("multiplicador_tb.vcd");
  $dumpvars;
  #10000;   //Evita ejecución infinita
  $finish;
end

//Preparación caso inicial
initial
begin
  multiplicando = MIN_RANGE;
  multiplicador = MIN_RANGE;
  contador = 1;
end 

initial
begin
  start = 1'b1;
  #1;
  start = 1'b0;
end

always @(posedge Fin)
begin
  resultado_esperado = multiplicador * multiplicando;
  //Comprobación de resultados
  $write("%d x %d = %d", multiplicando, multiplicador, resultado_esperado);
  if (resultado !== resultado_esperado)
    $display("\tERROR    Obtenido=%b", resultado);
  else
    $display("\tCORRECTO Obtenido=%b", resultado);
  
  #20  //tiempo de espera entre una multiplicación y la siguiente
  if (contador == NUM_TESTS)
    $finish;
  else //Siguiente caso
  begin
    contador = contador + 1;
    if (multiplicador == MAX_RANGE)
    begin
      multiplicador = MIN_RANGE;
      multiplicando = multiplicando + 1;
    end
    else
    begin
      multiplicador = multiplicador + 1;
    end
    start = 1'b1;
    #1;
    start = 1'b0;           
  end
end

endmodule