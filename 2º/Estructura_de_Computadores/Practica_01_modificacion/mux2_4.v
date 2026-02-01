//Multiplexor 2 a 1.
module mux2_4(output wire [3:0] Out, input wire [3:0] A, input wire [3:0] B, input wire s);
//Asignación a Out dependiendo del valor de s para obtener B o A.
assign Out = (s)?B:A;
endmodule