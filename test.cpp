#include "simulator.hpp"

int main() {
    CPU a;
    a.update();
    //a.read();
    //a.run();
    a.show_content();
    string immediate = "00010001000";
    a.ADM_test();
    //a.ADR(immediate);
    a.ADM(immediate);
    a.show_content();
}