#include<systemc>
using namespace sc_core;

SC_MODULE(ClockDivider){
        sc_in<bool> clk_in;
        sc_out<bool> clk_out;

        bool toggle;

        SC_CTOR(ClockDivider):SC_NAMED(clk_in),
                         SC_NAMED(clk_out),
                         toggle(false)
        {

                SC_METHOD(func);
                sensitive<<clk_in.pos();
                dont_initialize();

        }

        void func(){
                if(clk_in.posedge()){
                        toggle = !toggle;
                        clk_out.write(toggle);
                }
        }

};

