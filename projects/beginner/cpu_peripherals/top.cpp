// top.h
#include "cpu.h"
#include "uart_axi_lite.h"
#include "timer.h"
//#include "memory.h"

SC_MODULE(Top) {
    CPU cpu{"cpu"};
    UART uart{"uart"};
    Timer timer{"timer"};
    //Memory mem{"mem"};

    sc_signal<bool> irq_sig{"irq_sig"};

    SC_CTOR(Top) {
        cpu.socket.bind(uart.socket);   // CPU writes to UART
        cpu.irq(irq_sig);
        timer.irq(irq_sig);
    }
};

int sc_main(int argc, char* argv[]) {
    Top top{"top"};
    sc_start(50, SC_MS);  // run simulation
    return 0;
}
