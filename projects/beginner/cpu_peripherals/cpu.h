//cpu.h

#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
using namespace sc_core;
using namespace tlm;

struct CPU : sc_module {
    tlm_utils::simple_initiator_socket<CPU> socket;
    sc_in<bool> irq;

    SC_CTOR(CPU) {
        SC_THREAD(run);
        sensitive << irq.pos();
        dont_initialize();
    }

    void run() {
        while (true) {
            wait(irq.posedge_event()); // wait for interrupt
            std::cout << "[CPU] IRQ received at " << sc_time_stamp() << "\n";

            // simulate ISR: send a char to UART
            uint8_t c = 'A';
            tlm_generic_payload trans;
            sc_time delay = SC_ZERO_TIME;
            trans.set_command(TLM_WRITE_COMMAND);
            trans.set_address(0);  // assuming UART at address 0
            trans.set_data_ptr(&c);
            trans.set_data_length(1);
            socket->b_transport(trans, delay);
        }
    }
};
