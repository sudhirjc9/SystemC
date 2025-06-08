#ifndef BUS_H
#define BUS_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <vector>
#include <tuple>

struct Bus : sc_core::sc_module {
    tlm_utils::simple_target_socket<Bus> cpu_socket;

    std::vector<tlm_utils::simple_initiator_socket<Bus>*> initiator_sockets;
    std::vector<std::tuple<uint64_t, uint64_t>> address_map;

    SC_CTOR(Bus) : cpu_socket("cpu_socket") {
        cpu_socket.register_b_transport(this, &Bus::b_transport);
    }

    // Add peripheral during elaboration
    void add_target(std::string name, sc_module& target_module, uint64_t start, uint64_t end) {
        auto* socket = new tlm_utils::simple_initiator_socket<Bus>(name.c_str());
        initiator_sockets.push_back(socket);
        address_map.emplace_back(start, end);
    }

    template<typename MODULE>
	void bind_target(unsigned index, tlm_utils::simple_target_socket<MODULE>& target_socket) {
    (*initiator_sockets[index]).bind(target_socket);
	}

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        uint64_t addr = trans.get_address();

        for (size_t i = 0; i < initiator_sockets.size(); ++i) {
            auto [start, end] = address_map[i];
            if (addr >= start && addr <= end) {
                trans.set_address(addr - start);
                (*initiator_sockets[i])->b_transport(trans, delay);
                return;
            }
        }

        trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    }
};

#endif
