#ifndef CPU_H
#define CPU_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

SC_MODULE(CPU) {
    tlm_utils::simple_initiator_socket<CPU> socket;

    SC_CTOR(CPU) : socket("socket") {
        SC_THREAD(run);
    }

    void run();
};

#endif
