// Declaración del módulo de la Celda lógica
module cl(output wire out, input wire a, b, input wire [1:0] S);
// Declaramos los wire para que los operadores lógicos tengan una salida
wire wire1,wire2,wire3,wire4;
// Creación de operador lógico and
and and1(wire1 ,a, b);
// Creación de operador lógico or
or or1(wire2, a, b);
// Creación de operador lógico xor
xor xor1(wire3, a, b);
// Creación de operador lógico not
not not1(wire4,b);
// Implementamos el multiplexor 4 a 1 con los wire
mux4_1 mux4_1_1(out, wire1, wire2, wire3, wire4,S);

endmodule