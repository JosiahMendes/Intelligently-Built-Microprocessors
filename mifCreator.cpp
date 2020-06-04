/* How to use mif creator:
    1. compile this file into whatever. 
    2. run this file, it takes in one input the filename.
        e.g " ./mifCreator testing" generates a mif file named testing.mif
        if no argument is given, default filename is output.mif
    3. enter in the instructions in the form "Instruction" "Immediate"
        e.g LDI 0000000111111 (the space between instruction and immediate is very important.)
    4. if using command line, just use control/command-d to signal end of file.
    DONE!
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

using namespace std;

string lazyImplement()
{
    string immediate;
    cin >> immediate;

    if (stoi(immediate, nullptr, 2) < 0)
    {
        cout << "Error: invalid immediate value (not a binary value)" << endl;
        abort();
    }
    return immediate;
}

string transform_to_uppercase(string instr)
{
    string transformed = "";
    for (int i = 0; i < instr.length(); i++)
    {
        transformed += toupper(instr[i]);
    }
    return transformed;
}

int main(int argc, char *argv[])
{
    string filename = "output";
    if (argc > 1)
    {
        filename.assign(argv[1]);
    }
    ofstream FileOut(filename + ".mif");
    int address = 0;

    FileOut << "WIDTH = 16;\n";
    FileOut << "DEPTH = 2048;\n \n";
    FileOut << "ADDRESS_RADIX=UNS;\nDATA_RADIX=BIN;\n \n";
    FileOut << "CONTENT BEGIN" << endl;

    while (1)
    {
        string instruction;
        cin >> instruction;
        if (cin.fail())
        {
            break;
        }
        assert(instruction.length() == 3);
        instruction = transform_to_uppercase(instruction);
        if (instruction == "ADR" ||
            instruction == "ADM" ||
            instruction == "SBR" ||
            instruction == "SBM" ||
            instruction == "MLR" ||
            instruction == "BBO" ||
            instruction == "XSL" ||
            instruction == "XSR" ||
            instruction == "ADI" ||
            instruction == "SBI" ||
            instruction == "LDI" ||
            instruction == "LDR" ||
            instruction == "LDA" ||
            instruction == "STA" ||
            instruction == "STI" ||
            instruction == "STK" ||
            instruction == "JMR" ||
            instruction == "JMP" ||
            instruction == "JEQ" ||
            instruction == "JNQ" ||
            instruction == "STP")
        {

            string instrb;

            if (instruction == "ADR")
            {
                instrb = "00001";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "ADM")
            {
                instrb = "0001";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "ADI")
            {
                instrb = "00100";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "SBR")
            {
                instrb = "00101";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "SBM")
            {
                instrb = "0011";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "SBI")
            {
                instrb = "01000";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "MLR")
            {
                instrb = "01001";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "XSL")
            {
                instrb = "01010";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "XSR")
            {
                instrb = "01011";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "BBO")
            {
                instrb = "01100";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "STK")
            {
                instrb = "01101";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "LDR")
            {
                instrb = "01110";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "STI")
            {
                instrb = "01111";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "LDI")
            {
                instrb = "100";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "STA")
            {
                instrb = "101";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "LDA")
            {
                instrb = "110";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "JMR")
            {
                instrb = "11100";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "JMP")
            {
                instrb = "11101";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "JEQ")
            {
                instrb = "11110";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "JNQ")
            {
                instrb = "11111";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            else if (instruction == "STP")
            {
                instrb = "00000";
                FileOut << "\t" + to_string(address) + "     :   " + instrb + lazyImplement() + ";" << endl;
            }
            address++;
        }
        else
        {
            cerr << "Error: " << instruction << " is not a valid instruction" << endl;
            abort();
        }
    }

    FileOut << "\t[" + to_string(address) + "..2047]" + "     :   " + "0000000000000000;" << endl;
    FileOut << "END;" << endl;

    FileOut.close();
}
