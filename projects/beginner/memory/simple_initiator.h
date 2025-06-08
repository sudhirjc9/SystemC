#ifndef SIMPLE_INITIATOR_H
#define SIMPLE_INITIATOR_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

SC_MODULE(SimpleInitiator) {
    tlm_utils::simple_initiator_socket<SimpleInitiator> socket;

    sc_signal<uint64_t> addr_sig;
    sc_signal<bool> rw_sig; // 0 = Read, 1 = Write

    void process() {
        tlm::tlm_generic_payload trans;
        sc_time delay = SC_ZERO_TIME;
        unsigned char data[4];

        // ---- Write ----
        data[0] = 0xDE; data[1] = 0xAD;
        data[2] = 0xBE; data[3] = 0xEF;

        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(0x10);
        trans.set_data_ptr(data);
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(0);
        trans.set_dmi_allowed(false);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        // Update traceable signals
        addr_sig.write(0x10);
        rw_sig.write(1);  // Write

        socket->b_transport(trans, delay);
        std::cout << "Write Done @ " << sc_time_stamp() << std::endl;

        // ---- Read ----
        unsigned char rdata[4];
        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_address(0x10);
        trans.set_data_ptr(rdata);
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        addr_sig.write(0x10);
        rw_sig.write(0);  // Read

        delay = SC_ZERO_TIME;
        socket->b_transport(trans, delay);

        std::cout << "Read Data @ " << sc_time_stamp() << " = "
                  << std::hex << (int)rdata[0] << " "
                  << (int)rdata[1] << " "
                  << (int)rdata[2] << " "
                  << (int)rdata[3] << std::dec << std::endl;
    }

    SC_CTOR(SimpleInitiator) {
        SC_THREAD(process);
    }
};

#endif
