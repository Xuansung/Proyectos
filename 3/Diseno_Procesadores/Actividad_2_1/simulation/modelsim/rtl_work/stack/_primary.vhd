library verilog;
use verilog.vl_types.all;
entity stack is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        wd              : in     vl_logic_vector(9 downto 0);
        push            : in     vl_logic;
        pop             : in     vl_logic;
        rd              : out    vl_logic_vector(9 downto 0)
    );
end stack;
