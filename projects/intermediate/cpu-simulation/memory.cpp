#include "memory.h"
#include <iostream>

void Memory::b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
    sc_dt::uint64 addr = trans.get_address();
    unsigned char* ptr = trans.get_data_ptr();

    if (addr >= SIZE * sizeof(int)) {
        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        return;
    }

    int index = addr / sizeof(int);

    if (trans.get_command() == tlm::TLM_WRITE_COMMAND) {
        int val = *reinterpret_cast<int*>(ptr);
        mem[index] = val;
        std::cout << "[Memory] Wrote " << val << " at addr 0x" << std::hex << addr << "\n";
    } else if (trans.get_command() == tlm::TLM_READ_COMMAND) {
        *reinterpret_cast<int*>(ptr) = mem[index];
        std::cout << "[Memory] Read " << mem[index] << " from addr 0x" << std::hex << addr << "\n";
    }

    trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
