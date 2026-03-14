library verilog;
use verilog.vl_types.all;
entity cpu is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        pc_salida       : out    vl_logic_vector(9 downto 0)
    );
end cpu;
