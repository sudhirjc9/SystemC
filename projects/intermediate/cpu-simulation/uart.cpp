#include "uart.h"
#include <iostream>

void UART::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    sc_dt::uint64 addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();
    auto cmd = trans.get_command();

    switch (addr) {
        case 0x00: // TX
            if (cmd == tlm::TLM_WRITE_COMMAND) {
                tx_reg = *ptr;
                std::cout << "[UART] Transmit: " << tx_reg << " ('" << (char)tx_reg << "')\n";
            } else {
                trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            }
            break;

        case 0x04: // RX
            if (cmd == tlm::TLM_READ_COMMAND) {
                *ptr = rx_reg;
                std::cout << "[UART] Read RX: " << (int)rx_reg << "\n";
            } else {
                trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            }
            break;

        case 0x08: // STATUS
            if (cmd == tlm::TLM_READ_COMMAND) {
                *ptr = status_reg;
                std::cout << "[UART] Read STATUS: " << (int)status_reg << "\n";
            } else {
                trans.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            }
            break;

        default:
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
