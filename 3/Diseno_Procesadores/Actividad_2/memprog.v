//Memoria de programa, se inicializa y no se modifica

module memprog(input  wire        clk,
               input  wire [9:0]  a,
               output wire [31:0] rd);

  reg [31:0] mem[0:1023]; //memoria de 1024 palabras de 16 bits de ancho

  initial
  begin
    $readmemb("C:/Users/estudiante/Downloads/proyect1_act1/progfile.dat",mem); // inicializa la memoria del fichero en texto binario
  end
  
  assign rd = mem[a];

endmodule

module memdat(input wire clk, input wire m_wr, input wire [15:0] a, input wire [15:0] wd, output wire [15:0] rd); // Modulo para la memoria de datos
  reg [15:0] mem[0:1023];


  always @(posedge clk) begin
    if (m_wr) mem[a] <= wd;
  end

  assign rd = mem[a];
endmodule

module stack(input wire clk, input wire [9:0] wd, input wire push, input wire pop, output wire [9:0] rd, reset); // pila para los saltos de subrutina
  reg [9:0] stackmem[0:15];
  reg [3:0] sp;
  always @(posedge clk) begin
    if (reset) sp <= 4'b0;
    else if (push) begin
      stackmem[sp] <= wd;
      sp <= sp + 1;
    end
    if (pop) begin
      sp = sp - 1;
    end

    assign rd = stackmem[sp];
  end
endmodule

