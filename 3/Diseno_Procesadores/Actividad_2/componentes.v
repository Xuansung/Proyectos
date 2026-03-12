//Componentes varios

//Banco de registros de dos salidas y una entrada
module regfile(input  wire        clk, 
               input  wire        we3,           //se�al de habilitaci�n de escritura
               input  wire [3:0]  ra1, ra2, wa3, //direcciones de regs leidos y reg a escribir
               input  wire [15:0]  wd3, 			 //dato a escribir
               output wire [15:0]  rd1, rd2);     //datos leidos

  reg [15:0] regb[0:15]; //memoria de 16 registros de 16 bits de ancho

  initial
  begin
    $readmemb("C:/Users/estudiante/Downloads/proyect1_act1/regfile.dat",regb); // inicializa los registros a valores conocidos
  end  
  
  // El registro 0 siempre es cero
  // se leen dos reg combinacionalmente
  // y la escritura del tercero ocurre en flanco de subida del reloj
  
  always @(posedge clk)
    if (we3) regb[wa3] <= wd3;	
  
  assign rd1 = (ra1 != 0) ? regb[ra1] : 16'b0;
  assign rd2 = (ra2 != 0) ? regb[ra2] : 16'b0;

endmodule

//modulo sumador  
module sum(input  wire [15:0] a, b,
             output wire [15:0] y);

  assign y = a + b;

endmodule

//modulo registro para modelar el PC, cambia en cada flanco de subida de reloj o de reset
module registro #(parameter WIDTH = 10)
              (input wire             clk, reset,
               input wire [WIDTH-1:0] d, 
               output reg [WIDTH-1:0] q);

  always @(posedge clk, posedge reset)
    if (reset) q <= {WIDTH{1'b0}};
    else       q <= d;

endmodule

//modulo multiplexor, si s=1 sale d1, s=0 sale d0
module mux2 #(parameter WIDTH = 8)
             (input  wire [WIDTH-1:0] d0, d1, 
              input  wire             s, 
              output wire [WIDTH-1:0] y);

  assign y = s ? d1 : d0; 

endmodule

module mux4 #(parameter WIDTH = 8) // modulo multiplexor 4 a 1
             (input  wire [WIDTH-1:0] d0, d1, d2, d3
              input  wire [2:0]       s, 
              output wire [WIDTH-1:0] y);
  always @(*) begin
    case(s)
      2'b00: y = d0;
      2'b01: y = d1;
      2'b10: y = d2;
      2'b11: y = d3;
    endcase
  end

endmodule

//Biestable para los flags de la ALU
//Biestable tipo D síncrono con reset asíncrono por flanco y entrada de habilitación de carga
module ffd(input wire clk, reset, d, carga, output reg q);

  always @(posedge clk, posedge reset)
    if (reset)
	    q <= 1'b0;
	  else
	    if (carga)
	      q <= d;

endmodule 

