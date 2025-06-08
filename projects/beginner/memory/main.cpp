#include <systemc>
#include "simple_memory.h"
#include "simple_initiator.h"

// Custom wrapper module to connect and expose traceable signals
SC_MODULE(Top) {
    SimpleInitiator* initiator;
    SimpleMemory* memory;

    sc_signal<uint64_t> last_access_addr;
    sc_signal<bool> rw_flag;  // 0 = Read, 1 = Write

    SC_CTOR(Top) {
        initiator = new SimpleInitiator("initiator");
        memory = new SimpleMemory("memory");

        // Connect sockets
        initiator->socket.bind(memory->socket);

        // Hook up tracing values inside initiator
        SC_METHOD(trace_helper);
        sensitive << initiator->addr_sig << initiator->rw_sig;
        dont_initialize();
    }

    void trace_helper() {
        last_access_addr.write(initiator->addr_sig.read());
        rw_flag.write(initiator->rw_sig.read());
    }
};

int sc_main(int argc, char* argv[]) {
    Top top("top");

    // Create trace file
    sc_trace_file* tf = sc_create_vcd_trace_file("tlm_trace");
    tf->set_time_unit(1, SC_NS);

    sc_trace(tf, top.last_access_addr, "last_access_addr");
    sc_trace(tf, top.rw_flag, "rw_flag");  // 1 = write, 0 = read

    sc_start(50, SC_NS);  // Simulate for 50 ns

    sc_close_vcd_trace_file(tf);
    return 0;
}
