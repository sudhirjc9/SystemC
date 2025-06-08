#ifndef UART_H
#define UART_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

SC_MODULE(UART) {
    tlm_utils::simple_target_socket<UART> socket;

    // Registers
    uint8_t tx_reg = 0;
    uint8_t rx_reg = 0x55; // Simulate a static incoming byte (e.g., 'U')
    uint8_t status_reg = 0x01; // RX ready

    SC_CTOR(UART) : socket("socket") {
        socket.register_b_transport(this, &UART::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
