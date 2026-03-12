//Memoria de programa, se inicializa y no se modifica

module memprog(input  wire        clk,
               input  wire [9:0]  a,
               output wire [15:0] rd);

  reg [15:0] mem[0:1023]; //memoria de 1024 palabras de 16 bits de ancho

  initial
  begin
    $readmemb("C:/Users/estudiante/Downloads/proyect1_act1/progfile.dat",mem); // inicializa la memoria del fichero en texto binario
  end
  
  assign rd = mem[a];

endmodule

module memdat(input wire clk, input wire [15:0] a, input wire [15:0] wd, input wire we3, output wire [15:0] s); // Modulo para la memoria de datos
  reg [15:0] mem[65535:0];

  mem[a] <= wd;
  initial
  begin
    if (~we3) $readmemb("memdat.dat", mem);
    else $writememb("memdat.dat", mem, a);
  end

  assign rd = mem[a];
endmodule

module stack(input wire clk, input wire [15:0] wd, input wire push, input wire pop, output wire [9:0] rd); // pila para los saltos de subrutina
  reg [9:0] stackmem[0:1023];
  reg [9:0] sp;


  always @(posedge clk) begin
    if (push) begin
      sp <= sp + 1;
      stackmem[sp] <= a;
    end
    if (pop) begin
      rd <= stackmem[sp];
      sp = sp - 1;
    end
  end

    initial
  begin
    if (push) $writememb("stack.dat", stackmem);
    else if (pop) $readmemb("stack.dat", stackmem);
  end
endmodule

