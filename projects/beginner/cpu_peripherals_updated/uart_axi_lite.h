//uart_axi_lite.h

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
using namespace sc_core;
using namespace tlm;

struct UART : sc_module {
    tlm_utils::simple_target_socket<UART> socket;
    sc_event tx_event;

    uint8_t tx_data;

    SC_CTOR(UART) : socket("socket"), tx_data(0) {
        socket.register_b_transport(this, &UART::b_transport);
        SC_THREAD(tx_thread);
    }

    void b_transport(tlm_generic_payload& trans, sc_time& delay) {
        auto* data = reinterpret_cast<uint8_t*>(trans.get_data_ptr());
        if (trans.is_write()) {
            tx_data = data[0];
            tx_event.notify(SC_ZERO_TIME);
        }
        trans.set_response_status(TLM_OK_RESPONSE);
    }

    void tx_thread() {
        while (true) {
            wait(tx_event);
            std::cout << "[UART] Sent: " << tx_data << " at " << sc_time_stamp() << "\n";
        }
    }
};
