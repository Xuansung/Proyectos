create_clock -name {CLOCK_50} -period 20.000 [get_ports {CLOCK_50}]

derive_pll_clocks

set_false_path -from [get_ports {SW[*]}] -to [all_registers]
set_false_path -from [get_ports {KEY[*]}] -to [all_registers]