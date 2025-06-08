#pragma once
#include <systemc>

// Simplified AXI-Lite interface (read path only)
struct AXILiteIf : public sc_core::sc_module {
    sc_core::sc_in<bool> clk, rst;

    sc_core::sc_in<bool> awvalid, wvalid, arvalid;
    sc_core::sc_in<sc_dt::sc_uint<32>> awaddr, araddr, wdata;
    sc_core::sc_out<bool> awready, wready, arready;
    sc_core::sc_out<sc_dt::sc_uint<32>> rdata;

    SC_CTOR(AXILiteIf) {}
};

// Router module to decode addresses and forward to one of multiple targets
struct AXILiteRouter : public sc_core::sc_module {
    sc_core::sc_vector<sc_core::sc_in<sc_dt::sc_uint<32>>> rdata_inputs;
    sc_core::sc_in<sc_dt::sc_uint<32>> addr;
    sc_core::sc_out<sc_dt::sc_uint<32>> rdata_output;

    SC_CTOR(AXILiteRouter)
        : rdata_inputs("rdata_inputs", 2)
    {
        SC_METHOD(route);
        sensitive << addr;
        for (auto& in : rdata_inputs) sensitive << in;
    }

    void route() {
        // Simple address decoding for two targets
        if (addr.read() >= 0x00000000 && addr.read() < 0x00001000) {
            rdata_output.write(rdata_inputs[0].read());
        } else if (addr.read() >= 0x00001000 && addr.read() < 0x00002000) {
            rdata_output.write(rdata_inputs[1].read());
        } else {
            rdata_output.write(0xDEADBEEF); // Invalid address pattern
        }
    }
};
