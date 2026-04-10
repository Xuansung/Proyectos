module alu(input wire [15:0] a, b,
           input wire [3:0] op_alu,
           output wire [15:0] y,
           output wire zero, zneg, zcarry);

reg [16:0] s;		   
		   
always @(*)
begin
  case (op_alu) // extendemos el signo a 0 para operaciones lógicas y a 16 bits para operaciones aritméticas, de esta forma el bit 17 de s se usará como carry en suma y resta
	4'b0000: s = {1'b0, b};              // EQ
	4'b0001: s = {1'b0, ~b};             // NOT
    // Concatenamos un 0 al inicio para forzar aritmética de 17 bits sin signo
  4'b0010: s = {1'b0, a} + {1'b0, b};  // ADD
  4'b0011: s = {1'b0, a} - {1'b0, b};  // SUB (s[16] será 1 si b > a)
  4'b0100: s = {1'b0, a & b};          // AND
  4'b0101: s = {1'b0, a | b};          // OR
  4'b0110: s = 17'd0 - {1'b0, b};         // NEG
    default: s = 17'b0;
  endcase
end

assign y = s[15:0];

//Calculo del flag de cero
assign zero = (y == 16'b0); 
assign zneg = y[15]; //El bit más significativo si es negativo.
assign zcarry = s[16]; //El bit 17 de carry. 
endmodule
