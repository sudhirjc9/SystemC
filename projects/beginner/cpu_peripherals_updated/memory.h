// memory.h
#include <systemc>
#include <tlm_utils/simple_target_socket.h>

struct Memory : sc_module {
    tlm_utils::simple_target_socket<Memory> socket;
    std::vector<uint8_t> mem;

    SC_CTOR(Memory) : mem(1024, 0), socket("socket") {
        socket.register_b_transport(this, &Memory::b_transport);
    }

    void b_transport(tlm_generic_payload& trans, sc_time& delay) {
        auto addr = trans.get_address();
        auto* ptr = trans.get_data_ptr();
        auto len = trans.get_data_length();

        if (addr + len > mem.size()) {
            trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }

        if (trans.is_read())
            std::copy(mem.begin() + addr, mem.begin() + addr + len, ptr);
        else if (trans.is_write())
            std::copy(ptr, ptr + len, mem.begin() + addr);

        trans.set_response_status(TLM_OK_RESPONSE);
    }
};



