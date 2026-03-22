module alu(input wire [15:0] a, b,
           input wire [3:0] op_alu,
           output wire [15:0] y,
           output wire zero, zneg, zcarry);

reg [16:0] s;		   
		   
always @(a, b, op_alu)
begin
  case (op_alu) // extendemos el signo a 0 para operaciones lógicas y a 16 bits para operaciones aritméticas, de esta forma el bit 17 de s se usará como carry en suma y resta
    4'b0000: s = {1'b0, a};              // EQ
    4'b0001: s = {1'b0, ~a};             // NOT
    4'b0010: s = a + b;                  // ADD (aquí el carry sale solo)
    4'b0011: s = a - b;                  // SUB
    4'b0100: s = {1'b0, a & b};          // AND
    4'b0101: s = {1'b0, a | b};          // OR
    4'b0110: s = {1'b0, -a[15:0]};       // NEG A (Forzamos 16 bits antes de expandir)
    4'b0111: s = {1'b0, -b[15:0]};       // NEG B
	4'b1000: s = {1'b0, b};            // B
    default: s = 17'b0;
  endcase
end

assign y = s[15:0];

//Calculo del flag de cero
assign zero = ~(|y[15:0]);   //operador de reducción |y hace la or de los bits del vector 'y' y devuelve 1 bit resultado
assign zneg = y[15]; //El bit más significativo si es negativo.
assign zcarry = s[16]; //El bit 17 de carry. 
endmodule
