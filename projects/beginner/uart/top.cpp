// top
#include <systemc>
#include <deque>
#include "uart.h"

using namespace sc_core;
using namespace sc_dt;


SC_MODULE(Testbench) {
    sc_signal<bool> clk{"clk"};
    sc_signal<bool> rst{"rst"};
    sc_signal<bool> tx_start;
    sc_signal<sc_uint<8>> tx_data;
    sc_signal<bool> tx_busy;
    sc_signal<sc_uint<8>> rx_data;
    sc_signal<bool> rx_valid;

    UART uart{"UART"};

    void clock_gen() {
        while (true) {
            clk.write(false); wait(5, SC_NS);
            clk.write(true);  wait(5, SC_NS);
        }
    }

    void stimulus() {
        rst.write(true); wait(10, SC_NS);
        rst.write(false); wait(10, SC_NS);

        wait(10, SC_NS);
        tx_data.write(0x55);
        tx_start.write(true); wait(10, SC_NS);
        tx_start.write(false);

        wait(100, SC_MS);
        sc_stop();
    }

    SC_CTOR(Testbench) {
        uart.clk(clk);
        uart.rst(rst);
        uart.tx_data(tx_data);
        uart.tx_start(tx_start);
        uart.tx_busy(tx_busy);
        uart.rx_data(rx_data);
        uart.rx_valid(rx_valid);

        SC_THREAD(clock_gen);
        SC_THREAD(stimulus);
    }
};


int sc_main(int argc, char* argv[]) {
    Testbench tb("tb");

    // Create VCD trace file
    sc_trace_file* tf = sc_create_vcd_trace_file("uart_trace");
    tf->set_time_unit(1, SC_NS); // Optional: set time unit

    // Trace top-level signals
    sc_trace(tf, tb.clk, "clk");
    sc_trace(tf, tb.rst, "rst");
    sc_trace(tf, tb.tx_start, "tx_start");
    sc_trace(tf, tb.tx_data, "tx_data");
    sc_trace(tf, tb.tx_busy, "tx_busy");
    sc_trace(tf, tb.rx_data, "rx_data");
    sc_trace(tf, tb.rx_valid, "rx_valid");

    sc_start();

    sc_close_vcd_trace_file(tf);
    return 0;
}
