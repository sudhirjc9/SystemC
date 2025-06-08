// 
#ifndef SIMPLE_MEMORY_H
#define SIMPLE_MEMORY_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

using namespace sc_core;

SC_MODULE(SimpleMemory) {
    tlm_utils::simple_target_socket<SimpleMemory> socket;

    enum { SIZE = 256 };
    uint8_t mem[SIZE];

    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay) {
        uint64_t addr = trans.get_address();
        unsigned char* ptr = trans.get_data_ptr();
        unsigned int len = trans.get_data_length();

        if (addr + len > SIZE) {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }

        if (trans.is_read()) {
            memcpy(ptr, &mem[addr], len);
        } else if (trans.is_write()) {
            memcpy(&mem[addr], ptr, len);
        }

        delay += sc_time(10, SC_NS);  // Add 10ns delay for memory access
        trans.set_response_status(tlm::TLM_OK_RESPONSE);
    }

    SC_CTOR(SimpleMemory) {
        socket.register_b_transport(this, &SimpleMemory::b_transport);
        memset(mem, 0, SIZE);  // Initialize memory to 0
    }
};

#endif
