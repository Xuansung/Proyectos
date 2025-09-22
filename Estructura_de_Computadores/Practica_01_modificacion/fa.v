//Declaración del módulo del Sumador completo
module fa(output wire c_out, sum, input wire a, b, c_in);
// Asignación continua de c_out y sum dado por la suma de a + b + c_in.
assign {c_out,sum} = a + b + c_in;

endmodule