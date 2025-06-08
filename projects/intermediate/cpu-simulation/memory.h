#ifndef MEMORY_H
#define MEMORY_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>

SC_MODULE(Memory) {
    tlm_utils::simple_target_socket<Memory> socket;

    SC_CTOR(Memory) : socket("socket") {
        socket.register_b_transport(this, &Memory::b_transport);
    }

    enum { SIZE = 1024 };
    int mem[SIZE];

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay);
};

#endif
