// Sumador completo versión 2
module sum4_v2(output wire[3:0] S, output wire c_out, input wire[3:0] A, input wire[3:0] B, input wire c_in);
// Declaración de los wire
wire C1,C2,C3;
//Asignación continua del primer bit, donde el C1 es el carry, S el resultado.
assign {C1,S[0]} = A[0] + B[0] + c_in;
//Asignación continua del segundo bit, donde el C2 es el carry, S el resultado.
assign {C2,S[1]} = A[1] + B[1] + C1;
//Asignación continua del tercer bit, donde el C3 es el carry, S el resultado.
assign {C3,S[2]} = A[2] + B[2] + C2;
//Asignación continua del cuarto bit, donde el c_out es el carry, S el resultado.
assign {c_out,S[3]} = A[3] + B[3] + C3;

endmodule