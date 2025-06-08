

#include <systemc>
#include "clock_div.cpp"

using namespace sc_core;


int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS);  // 10ns period = 100MHz
    sc_signal<bool> divided_clk;

    ClockDivider divider("Divider");
    divider.clk_in(clk);
    divider.clk_out(divided_clk);

    // Trace file to visualize output
    sc_trace_file* tf = sc_create_vcd_trace_file("clock_divider_trace");
    sc_trace(tf, clk, "clk");
    sc_trace(tf, divided_clk, "divided_clk");

    sc_start(100, SC_NS);  // Run simulation for 100ns
    sc_close_vcd_trace_file(tf);

    return 0;
}

