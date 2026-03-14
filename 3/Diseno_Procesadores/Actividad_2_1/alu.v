module alu(input wire [15:0] a, b,
           input wire [3:0] op_alu,
           output wire [15:0] y,
           output wire zero, zneg, zcarry);

reg [16:0] s;		   
		   
always @(a, b, op_alu)
begin
  case (op_alu)              
    4'b0000: s = a;
    4'b0001: s = ~a;
    4'b0010: s = a + b;
    4'b0011: s = a - b;
    4'b0100: s = a & b;
    4'b0101: s = a | b;
    4'b0110: s = -a;
    4'b0111: s = -b;
    4'b1000: s = b;
    default: s = 17'b0;
  endcase
end

assign y = s[15:0];

//Calculo del flag de cero
assign zero = ~(|y);   //operador de reducción |y hace la or de los bits del vector 'y' y devuelve 1 bit resultado
assign zneg = y[15]; //El bit más significativo si es negativo.
assign zcarry = s[16]; //El bit 17 de carry. 
endmodule