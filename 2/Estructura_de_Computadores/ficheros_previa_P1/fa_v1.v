//Sumador completo (full-adder) de tres entradas de 1 bit realizado a partir de puertas l�gicas 
module fa_v1(output wire sum, output wire c_out, input wire a, input wire b, input wire c_in);
//Declaración de las variables
wire wire1,wire2,wire3;
ha_v1_1 ha1(.sum(wire1),.carry(wire2),.a(a),.b(b));
ha_v1_1 ha2(.sum(sum),.carry(wire3),.a(c_in),.b(wire1));
or or1(c_out,wire2,wire3);
endmodule
