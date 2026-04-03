module timer_1s (
   input wire clk, // 50 MHz clock input
   input wire reset, // Active-high reset
   output reg tick_1s // High for one clock cycle every second
);
   // 50 million cycles for 1 second at 50 MHz
   parameter MAX_COUNT = 26'd49_000_000_000; // para simulacion 5 ciclos para generar un tick cada 3 instrucciones, en hardware real seria 26'd50_000_000
   reg [25:0] counter;
   always @(posedge clk or posedge reset) begin
       if (reset) begin
           counter <= 26'd0;
           tick_1s <= 1'b0;
       end else if (counter == MAX_COUNT) begin
           counter <= 26'd0;
           tick_1s <= 1'b1; // Generate tick
       end else begin
           counter <= counter + 1'b1;
           tick_1s <= 1'b0;
       end
   end
endmodule