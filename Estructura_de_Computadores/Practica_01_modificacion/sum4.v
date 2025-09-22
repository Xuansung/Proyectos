//Sumador completo de tipo estructural
module sum4(output wire[3:0] S, output wire c_out, input wire[3:0] A, input wire[3:0] B, input wire c_in);
// Declaración de los wire
wire C1,C2,C3;
// Sumador completo del primer bit
fa fa1(C1,S[0],A[0],B[0],c_in);
// Sumador completo del segundo bit
fa fa2(C2,S[1],A[1],B[1],C1);
// Sumador completo del tercer bit
fa fa3(C3,S[2],A[2],B[2],C2);
// Sumador completo del cuarto bit
fa fa4(c_out,S[3],A[3],B[3],C3);

endmodule