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
    FileOut << "ADDRESS_RADIX=UNS;\nDATA_RADIX=HEX;\n \n";
    FileOut << "CONTENT BEGIN" << endl;

    while (1)
    {
        string hex;
        cin >> hex;
        if (cin.fail())
        {
            break;
        }
        FileOut << "\t" + to_string(address) + "     :   " + hex + ";" << endl;
        address++;
    }

    FileOut << "\t[" + to_string(address) + "..2047]" + "     :   " + "0000;" << endl;
    FileOut << "END;" << endl;

    FileOut.close();
}
