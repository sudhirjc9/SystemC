#include <systemc>
#include "cpu.h"
#include "memory.h"
#include "uart.h"
#include "bus.h"

using namespace sc_core;

int sc_main(int argc, char* argv[]) {
    CPU cpu("cpu");
    Memory mem("memory");
    UART uart("uart");
    Bus bus("bus");

    // CPU to Bus
    cpu.socket.bind(bus.cpu_socket);

    // Add memory and uart as targets
bus.bind_target<Memory>(0, mem.socket);
bus.bind_target<UART>(1, uart.socket);

    // Bind sockets
    bus.bind_target(0, mem.socket);
    bus.bind_target(1, uart.socket);

    sc_start();
    return 0;
}
