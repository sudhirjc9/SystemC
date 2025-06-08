#include "cpu.h"
#include <iostream>

using namespace sc_core;

void CPU::run() {
    sc_dt::uint64 addr = 0x00000000;
    int data = 42;

    tlm::tlm_generic_payload trans;
    sc_time delay = SC_ZERO_TIME;

    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_address(addr);
    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
    trans.set_data_length(4);
    trans.set_streaming_width(4);
    trans.set_byte_enable_ptr(0);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    std::cout << "[CPU] Writing value " << data << " to address 0x" << std::hex << addr << "\n";
    socket->b_transport(trans, delay);

    if (trans.is_response_error())
        SC_REPORT_ERROR("TLM-2", "Response error from b_transport");

    wait(10, SC_NS);

    trans.set_command(tlm::TLM_READ_COMMAND);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    socket->b_transport(trans, delay);
    int read_data = *reinterpret_cast<int*>(trans.get_data_ptr());

    std::cout << "[CPU] Read back value: " << read_data << "\n";
}
