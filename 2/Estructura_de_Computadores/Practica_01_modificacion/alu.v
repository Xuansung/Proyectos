// Declaración del módulo alu
module alu(output wire [3:0] R, output wire zero, carry, sign, input wire [3:0] A, B, input wire c_in,
input wire [2:0] Op);
// Declaración los wire de 4 bits.
wire[3:0] OP1,OP2,sum4_wire,ul4_wire;
//Llamada al módulo preprocess.
preprocess preprocess_1(OP1,OP2,A,B,Op);
//Llamada al módulo sum4(sumador de 4 bits).
sum4 sum4_1(sum4_wire,carry,OP1,OP2,c_in);
//Llamada al módulo ul(unidad lógica).
ul4  ul4_1(ul4_wire,OP1,OP2,Op[1:0]);
//Llamada al módulo mux2_4(Multiplexor 2 a 1 de 4 bits).
mux2_4 mux2_4_1(R,sum4_wire,ul4_wire,Op[2]);
//Asignación zero a 1 si R == 0.
assign zero = (R == 4'b0000);
//Asignación sign al primer bit de R.
assign sign = (R[3]);

endmodule