/* How to use the simulator:
    1. Create an input text file e.g. test_script.txt
    2. Write down instructions inside the text file e.g.
        LDI 0000000001000
        LDI 0100000000010
        LDI 1100000000110
        JMR 10000010011
        LDI 1000000000011
        LDI 1100000000100
        STP 00000000000
    3. Compile test.cpp e.g. g++ test.cpp -o test
    4. Run the compiled file with the text file e.g. ./test < test_script.txt
*/

#include "simulator.hpp"

int main() {
    CPU a;
    a.read();
    a.run();
}