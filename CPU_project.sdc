#This file must have the same name as your project

#Make input CLK a clock and set frequency to 250MHz (4ns period)
create_clock -name {CLK} -period 14.706 [get_ports {CLK}]
