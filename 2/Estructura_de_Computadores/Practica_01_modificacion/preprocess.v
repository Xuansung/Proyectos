module preprocess(output wire [3:0] AMod, output wire [3:0] BMod, input wire [3:0] A,
input wire [3:0] B, input wire [2:0] Op);
// Declaración de los wire de 1 bit
wire add1,op1_A,op2_B,cpl;
// Declaración de los wire de 4 bits
wire[3:0] wiremux1, wiremux2;
/*Llamada al multiplexor 2 a 1 de 4 bits
IN: 0 ,1; OUT: wiremux1; señal de control: add1 
*/
mux2_4 mux2_1(wiremux1,4'b0000,4'b0001,add1);
/*Llamada al multiplexor 2 a 1 de 4 bits
IN: A, B; OUT: wiremux2; señal de control: op2_B 
*/
mux2_4 mux2_2(wiremux2,A,B,op2_B);
/*Llamada al multiplexor 2 a 1 de 4 bits
IN: A , wiremux1; OUT: AMod; señal de control: op1_A 
*/
mux2_4 mux2_3(AMod,wiremux1,A,op1_A);
/*Llamada al complementario a 1.
IN: wiremux2; OUT: BMod; señal de control: cpl
*/
cpl1   cpl_1(BMod,wiremux2,cpl);
/*Llamada al modulo de control
IN: Op ; OUT: add1, op1_A, op2_B, cpl
*/
control control_1(add1, op1_A, op2_B, cpl, Op);

endmodule