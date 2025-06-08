//timer.h

#include <systemc>
using namespace sc_core;

struct Timer : sc_module {
    sc_out<bool> irq;
    sc_time interval;

    SC_CTOR(Timer) : interval(sc_time(10, SC_MS)) {
        SC_THREAD(timer_thread);
    }

    void timer_thread() {
        while (true) {
            wait(interval);
            irq.write(true);
            wait(SC_ZERO_TIME);  // allow CPU to catch
            irq.write(false);
        }
    }
};
