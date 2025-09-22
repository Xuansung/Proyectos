module control(output wire add1, op1_A, op2_B,cpl, input wire[2:0]Op);
//Función lógica de add1
assign add1 = ((~Op[2] & Op[1]) | (~Op[0]));
//Función lógica de op1_A
assign op1_A = (Op[1] & ~Op[0]) | (Op[2] & ~Op[1]);
//Función lógica de op2_B
assign op2_B = (Op[1] & ~Op[0]) | (Op[2] & ~Op[1]);
//Función lógica de cpl
assign cpl = (~Op[2] & ~Op[1] & ~Op[0]);

endmodule