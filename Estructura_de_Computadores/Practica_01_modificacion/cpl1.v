// Declaración del módulo de complemento a 1.
module cpl1(output wire [3:0] Out, input wire [3:0] Inp, input wire cpl);
/* Asignación continua de Out dependiendo si cpl es 1 o 0 para que se invierta o no 
los bits de Inp.*/
assign Out = (cpl == 1)? ~Inp : Inp;

endmodule