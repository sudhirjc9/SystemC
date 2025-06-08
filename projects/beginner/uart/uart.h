#include <systemc>
#include <deque>

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(UART) {
    sc_in<bool> clk;
    sc_in<bool> rst;

    sc_in<bool> tx_start;
    sc_in<sc_uint<8>> tx_data;
    sc_out<bool> tx_busy;

    sc_out<sc_uint<8>> rx_data;
    sc_out<bool> rx_valid;

    sc_time baud_delay;

    std::deque<sc_uint<8>> tx_fifo;
    std::deque<sc_uint<8>> rx_fifo;

    // Transmit process
    void tx_process() {
        tx_busy.write(false);
        while (true) {
            wait(); // wait for clk.posedge
            if (!tx_fifo.empty()) {
                tx_busy.write(true);
                sc_uint<8> data = tx_fifo.front();
                tx_fifo.pop_front();

                // Start bit
                std::cout << "[TX] Start bit\n";
                wait(baud_delay);

                // Data bits
                for (int i = 0; i < 8; ++i) {
                    std::cout << "[TX] Sending bit: " << data[i] << "\n";
                    wait(baud_delay);
                }

                // Stop bit
                std::cout << "[TX] Stop bit\n";
                wait(baud_delay);

                tx_busy.write(false);
            }
        }
    }

    // RX emulation (stub - in practice you'd get bits from a serial line)
    void rx_process() {
        rx_valid.write(false);
        while (true) {
            wait(20 * baud_delay); // simulate receiving data occasionally
            sc_uint<8> dummy = 0xA5; // just a dummy value for example
            rx_data.write(dummy);
            rx_valid.write(true);
            std::cout << "[RX] Received byte: " << std::hex << dummy << "\n";
            wait(baud_delay);
            rx_valid.write(false);
        }
    }

    // Capture TX request
    void write_handler() {
        while (true) {
            wait(); // wait for clk.posedge
            if (tx_start.read()) {
                tx_fifo.push_back(tx_data.read());
                std::cout << "[UART] Queued byte: " << std::hex << tx_data.read() << "\n";
            }
        }
    }

    SC_CTOR(UART) : baud_delay(sc_time(1, SC_MS)) {
        SC_THREAD(tx_process);
        sensitive << clk.pos();
        async_reset_signal_is(rst, true);

        SC_THREAD(rx_process);
        sensitive << clk.pos();
        async_reset_signal_is(rst, true);

        SC_THREAD(write_handler);
        sensitive << clk.pos();
        async_reset_signal_is(rst, true);
    }
};
