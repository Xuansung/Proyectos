//Sumador completo (full-adder) de tres entradas de 1 bit realizado a partir de puertas l�gicas 
module far_vr(output wire sum, output wire c_out, input wire a, input wire b, input wire c_in);
//Declaración de las variables
wire wire1,wire2,wire3;
ha_vr har1(wire1,wire2,a,b);
ha_vr har2(sum,wire3,c_in,wire1);
or #1 or1(c_out,wire2,wire3);

endmodule
