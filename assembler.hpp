#ifndef assembler_hpp
#define assembler_hpp

#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

class MU0 {
private:
    map<int, pair<string, int>> m_instructions; // instruction ram, int to indicate order
    int accumulator;
    map<int, int> ram;
    int pc;
    int ir;
public:
    MU0() {
        accumulator = 0;
        pc = 0;
        ir = 0;
        print();
    }
    void reset() {
        accumulator = 0;
        pc = 0;
        ir = 0;
        map<int, int> temp;
        ram = temp;
    }
    void print() {
        cout << "PC: " << pc;
        cout << "   Acc: " << accumulator;
        if(ir != 0) {
            cout << "   IR: " << ir << endl;
        } else {
            cout << '\n';
        }
        if(ram.empty()) {
            cout << "Uninitialized memory is zeroed" << endl;
        } else {
            for(map<int, int>::iterator it = ram.begin(); it != ram.end(); ++it) {
                cout << "Location: " << it->first;
                cout << " Data: " << it->second << endl;
            }
        }
    }
    void STA(int location) {
        ram.insert(pair<int, int>(location, accumulator));
        pc++;
        string instr = "1";
        instr += to_string(location);
        ir = stoi(instr);
        print();
    }
    void LDI(int immediate) {
        accumulator = immediate;
        pc++;
        string instr = "8";
        string immediate_string;
        if(immediate == 0) {
            immediate_string = "000";
        } else if(immediate < 10) {
            immediate_string = "00" + to_string(immediate);
        } else if(immediate < 100) {
            immediate_string = "0" + to_string(immediate);
        }
        ir = stoi(instr + immediate_string);
        print();
    }
    void LDA(int location) {
        int value = 0;
        map<int, int>::iterator it = ram.find(location);
        if(it != ram.end()) {
            value = it->second;
        }
        accumulator = value;
        pc++;
        print();
    }
    void read() {
        reset();
        string instruction;
        int immediate;
        int order = 0;
        while(1) {
            cin >> instruction;
            if(cin.fail()) {
                break;
            }
            //instruction = tolower(instruction);
            if(instruction == "ldi" || instruction == "sta") {
                cin >> immediate;
                m_instructions.insert(pair<int,pair<string,int>>(order,make_pair(instruction, immediate)));
            } else {
                m_instructions.insert(pair<int,pair<string,int>>(order,make_pair(instruction,-1)));
            }
            order++;
        }
    }
    /*
    istream &operator >> (istream &src, MU0 &c) {
        c.reset();
        string instruction;
        int immediate;
        while(!src.fail()) {
            src >> instruction;
            instruction.tolower();
            if(instruction == "ldi" || instruction == "sta") {
                src >> immediate;
                m_instructions.push_back(instruction + to_string(immediate))
            } else {
                m_instructions.push_back(instruction);
            }
        }
    }
    */

    // used to check if read() works correctly
    void check_instructions() {
        if(m_instructions.empty()) {
            cout << "no instructions";
        } else {
            for(map<int,pair<string,int>>::iterator it = m_instructions.begin(); it != m_instructions.end(); ++it) {
                cout << "PC:" << it->first << " " << it->second.first << " " << it->second.second << endl;
            }
        }
    }

    void execute() {
        map<int,pair<string,int>>::iterator it = m_instructions.find(pc);
        if(it != m_instructions.end()) { // found instruction for corresponding pc
            string current_instruction = it->second.first;
            int immediate = it->second.second;
            if(current_instruction == "ldi") {
                LDI(immediate);
            } else if(current_instruction == "sta") {
                STA(immediate);
            }
        } else { // didn't find instruction
            
        }
    }
    void run() {
        for(int i = 0; i < m_instructions.size(); i++) {
            execute();
        }
    }
};

#endif