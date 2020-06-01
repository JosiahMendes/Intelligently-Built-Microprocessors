#ifndef simulator_hpp
#define simulator_hpp

#include <iostream>
#include <string>
#include <assert.h>
#include <map>
#include <vector>
using namespace std;

class CPU {
private:
    bool stop;
    string reg0; // assuming reg 00
    string reg1; // assuming reg 01
    string reg2; // assuming reg 10
    string reg3; // assuming reg 11
    int pc;
    int m_carry;
    map<int,pair<string,string>> m_instructions;
    map<string,string> memory; // location and data written as string
    vector<string> stack; // stack with 16 bit data stored
public:
    CPU() {
        stop = false;
        reg0 = "0000000000000000";
        reg1 = "0000000000000000";
        reg2 = "0000000000000000";
        reg3 = "0000000000000000";
        pc = 0;
        m_carry = 0;
    }

    // read instructions and store them in m_instructions
    void read() {
        string instruction;
        string immediate;
        int order = 0;
        while(1) {
            cin >> instruction;
            if(cin.fail()) {
                break;
            }
            assert(instruction.length() == 3);
            instruction = transform_to_uppercase(instruction);
            if(instruction == "ADR" || 
                instruction == "ADM" ||
                instruction == "SBR" || 
                instruction == "SBM" ||
                instruction == "MLR" ||
                instruction == "MLM" ||
                instruction == "BBO" ||
                instruction == "BFE" ||
                instruction == "XSL" ||
                instruction == "XSR" ||
                instruction == "ADI" ||
                instruction == "SBI" ||
                instruction == "LDI" ||
                instruction == "LDR" ||
                instruction == "STA" ||
                instruction == "STI" ||
                instruction == "PSH" ||
                instruction == "POP" ||
                instruction == "JMR" ||
                instruction == "JMP" ||
                instruction == "STP") {
                cin >> immediate;
                if(stoi(immediate,nullptr,2)<0) { // immediate stored as binary
                    cout << "Error: invalid immediate value (not a binary value)" << endl;
                    return;
                }
                m_instructions.insert(pair<int,pair<string,string>>(order,make_pair(instruction,immediate)));
            }
            else {
                cout << "Error: " << instruction << " is not a valid instruction" << endl;
                return;
            }
            order++;
        }
    }

    // execute the corresponding instruction
    void execute() {
        map<int,pair<string,string>>::iterator it = m_instructions.find(pc);
        if(it != m_instructions.end()) { // found
            string current_instruction = it->second.first;
            string current_immediate = it->second.second;
            if(current_instruction == "ADR") {
                ADR(current_immediate);
            } else if(current_instruction == "ADM") {
                ADM(current_immediate);
            } else if(current_instruction == "SBR") {
                SBR(current_immediate);
            } else if(current_instruction == "SBM") {
                SBM(current_immediate);
            } else if(current_instruction == "MLR") {
                MLR(current_immediate);
            } else if(current_instruction == "MLM") {
                MLM(current_immediate);
            } else if(current_instruction == "BBO") {
                BBO(current_immediate);
            } else if(current_instruction == "BFE") {
                BFE(current_immediate);
            } else if(current_instruction == "XSL") {
                XSL(current_immediate);
            } else if(current_instruction == "XSR") {
                XSR(current_immediate);
            } else if(current_instruction == "ADI") {
                ADI(current_immediate);
            } else if(current_instruction == "SBI") {
                SBI(current_immediate);
            } else if(current_instruction == "LDI") {
                LDI(current_immediate);
            } else if(current_instruction == "LDR") {
                LDR(current_immediate);
            } else if(current_instruction == "STA") {
                STA(current_immediate);
            } else if(current_instruction == "STI") {
                STI(current_immediate);
            } else if(current_instruction == "PSH") {
                PSH(current_immediate);
            } else if(current_instruction == "POP") {
                POP(current_immediate);
            } else if(current_instruction == "JMR") {
                JMR(current_immediate);
            } else if(current_instruction == "JMP") {
                JMP(current_immediate);
            } else if(current_instruction == "STP") {
                STP(current_immediate);
            }
        } else { // didn't find instruction
            cout << "no instruction found for the current pc value: " << pc;
        }
        show_content();
    }

    // run the instructions stored in m_instructions
    void run() {
        for(int i = 0; i < m_instructions.size(); i++) {
            execute();
            if(stop) {
                return;
            }
        }
    }

    // test function to check if read() works
    void print_instructions() {
        if(m_instructions.empty()) {
            cout << "no instructions" << endl;
        } else {
            for(map<int,pair<string,string>>::iterator it = m_instructions.begin(); it != m_instructions.end(); ++it) {
                cout << "PC: " << it->first << " " << "Instruction: " << it->second.first << " " << "Immediate: " << it->second.second << endl;
            }
        }
    }

    void show_content() {
        cout << "reg0: " << reg0 << endl;
        cout << "reg1: " << reg1 << endl;
        cout << "reg2: " << reg2 << endl;
        cout << "reg3: " << reg3 << endl;
        cout << "stack: ";
        if(stack.empty()) {
            cout << "empty" << endl;
        } else {
            cout << stack.size() << endl;
            for(int i = stack.size()-1; i >= 0; i--) {
                cout << stack[i] << endl;
            }
        }
        cout << "PC: " << pc << endl;
        cout << "Carry: " << m_carry << endl;
        for(map<string,string>::iterator it = memory.begin(); it != memory.end(); ++it) {
            cout << "Location: " << it->first << " " << "Data: " << it->second << endl;
        }
        cout << "Uninitialized memory is zeroed" << endl;
        cout << "\n\n";
    }

    // Rn := Rn + Rm + Cin
    void ADR(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[10-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rx = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4])); // instr[5..4] gives register Rx
        assert(register_Rx.length() == 16);
        string register_Rn = get_register_value(string(1,immediate[10-3])+string(1,immediate[10-2])); // instr[3..2] gives register Rn
        assert(register_Rn.length() == 16);
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0])); // instr[1..0] gives register Rm
        assert(register_Rm.length() == 16);
        int carry_in = 0; // assuming instr[9..8] = 00
        string which_carry_in = string(1,immediate[10-9]) + string(1,immediate[10-8]);
        if(which_carry_in == "01") { // if instr[9..8] = 01, cin = 1
            carry_in = 1;
        } else if(which_carry_in == "10") { // if instr[9..8] = 10, cin = carry
            carry_in = m_carry;
        } else if(which_carry_in == "11") { // if instr[9..8] = 11, cin = MSB of Rm
            if(register_Rm[15-15] == '0') {
                carry_in = 0;
            } else if(register_Rm[15-15] == '1') {
                carry_in = 1;
            }
        }
        string addition_result;
        string immediate_offset = string(1,immediate[10-7]) + string(1,immediate[10-6]);
        pair<string,bool> result;
        if(immediate_offset == "00") { // no offset
            result = addition(register_Rn, register_Rm, carry_in);
        } else if(immediate_offset == "01") { // + Rx
            pair<string,bool> temp_result = addition(register_Rn, register_Rm, carry_in);
            result = addition(temp_result.first, register_Rx, 0);
            if(temp_result.second) { // if overflow happens in temp_result
                result.second = true;
            }
        } else if(immediate_offset == "10") { // Rm logical shift left by Rx
            string shifted_Rm = left_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = addition(register_Rn, shifted_Rm, carry_in);
        } else if(immediate_offset == "11") { // Rm logical shift right by Rx
            string shifted_Rm = right_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = addition(register_Rn, shifted_Rm, carry_in);
        }
        assert(result.first.length() == 16);
        if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "00") { // if Rn = reg0
            reg0 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "01") { // if Rn = reg1
            reg1 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "10") { // if Rn = reg2
            reg2 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "11") { // if Rn = reg3
            reg3 = result.first;
        }
        if(write_carry) {
            if(result.second) {
                m_carry = 1;
            } else {
                m_carry = 0;
            }
        }
        pc++;
    }

    // R0 := R0 + Mem[N], carry always written
    void ADM(string immediate) {
        assert(immediate.length() == 11);
        string immediate_16 = immediate;
        for(int i = 0; i < 5; i++) {
            immediate_16 = "0" + immediate_16;
        }
        assert(immediate_16.length() == 16);
        string register_R0 = reg0;
        map<string,string>::iterator it = memory.find(immediate_16);
        string data;
        if(it != memory.end()) { // found Mem[N]
            data = it->second;
        } else {
            data = "0000000000000000";
        }
        assert(data.length() == 16);
        pair<string,bool> result = addition(reg0, data, 0); // cin = 0
        reg0 = result.first;
        if(result.second) { // if cout = 1
            m_carry = 1;
        } else {
            m_carry = 0;
        }
        pc++;
    }

    // Rn := Rn - Rm - cin
    void SBR(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[10-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rx = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4]));
        assert(register_Rx.length() == 16);
        string register_Rn = get_register_value(string(1,immediate[10-3])+string(1,immediate[10-2]));
        assert(register_Rn.length() == 16);
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0]));
        assert(register_Rm.length() == 16);
        int carry_in = 0; // assuming instr[9..8] = 0
        string which_carry_in = string(1,immediate[10-9]) + string(1,immediate[10-8]);
        if(which_carry_in == "01") { // cin = 1
            carry_in = 1;
        } else if(which_carry_in == "10") { // cin = carry
            carry_in = m_carry;
        } else if(which_carry_in == "11") { // cin = MSB of Rm
            if(register_Rm[15-15] == '0') {
                carry_in = 0;
            } else if(register_Rm[15-15] == '1') {
                carry_in = 1;
            }
        }
        string immediate_offset = string(1,immediate[10-7]) + string(1,immediate[10-6]);
        pair<string,bool> result;
        if(immediate_offset == "00") { // no offset
            result = subtraction(register_Rn, register_Rm, carry_in);
        } else if(immediate_offset == "01") { // + Rx
            pair<string,bool> temp_result = addition(register_Rn, register_Rx, 0);
            result = subtraction(temp_result.first, register_Rm, carry_in);
            if(temp_result.second) {
                result.second = true;
            }
        } else if(immediate_offset == "10") { // Rm logical shift left by Rx
            string shifted_Rm = left_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = subtraction(register_Rn, shifted_Rm, carry_in);
        } else if(immediate_offset == "11") { // Rm logical shift right by Rx
            string shifted_Rm = right_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = subtraction(register_Rn, shifted_Rm, carry_in);
        }
        assert(result.first.length() == 16);
        if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "00") { // if Rn = reg0
            reg0 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "01") { // if Rn = reg1
            reg1 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "10") { // if Rn = reg2
            reg2 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "11") { // if Rn = reg3
            reg3 = result.first;
        }
        if(write_carry) {
            if(result.second) {
                m_carry = 1;
            } else {
                m_carry = 0;
            }
        }
        pc++;
    }

    // R0 := R0 - Mem[N], carry always written
    void SBM(string immediate) {
        assert(immediate.length() == 11);
        string immediate_16 = immediate;
        for(int i = 0; i < 5; i++) {
            immediate_16 = "0" + immediate_16;
        }
        assert(immediate_16.length() == 16);
        string register_R0 = reg0;
        map<string,string>::iterator it = memory.find(immediate_16);
        string data;
        if(it != memory.end()) { // found Mem[N]
            data = it->second;
        } else {
            data = "0000000000000000";
        }
        assert(data.length() == 16);
        pair<string,bool> result = subtraction(reg0, data, 0); // cin = 0
        reg0 = result.first;
        if(result.second) { // if cout = 1
            m_carry = 1;
        } else {
            m_carry = 0;
        }
        pc++;
    }

    // Rn := Rn * Rm + cin
    void MLR(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[10-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rx = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4])); // instr[5..4] gives register Rx
        assert(register_Rx.length() == 16);
        string register_Rn = get_register_value(string(1,immediate[10-3])+string(1,immediate[10-2])); // instr[3..2] gives register Rn
        assert(register_Rn.length() == 16);
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0])); // instr[1..0] gives register Rm
        assert(register_Rm.length() == 16);
        int carry_in = 0; // assuming instr[9..8] = 00
        string which_carry_in = string(1,immediate[10-9]) + string(1,immediate[10-8]);
        if(which_carry_in == "01") { // if instr[9..8] = 01, cin = 1
            carry_in = 1;
        } else if(which_carry_in == "10") { // if instr[9..8] = 10, cin = carry
            carry_in = m_carry;
        } else if(which_carry_in == "11") { // if instr[9..8] = 11, cin = MSB of Rm
            if(register_Rm[15-15] == '0') {
                carry_in = 0;
            } else if(register_Rm[15-15] == '1') {
                carry_in = 1;
            }
        }
        string immediate_offset = string(1,immediate[10-7]) + string(1,immediate[10-6]);
        pair<string,bool> result;
        if(immediate_offset == "00") { // no offset
            result = multiplication(register_Rn, register_Rm, carry_in);
        } else if(immediate_offset == "01") { // + Rx
            pair<string,bool> temp_result = multiplication(register_Rn, register_Rm, carry_in);
            result = addition(temp_result.first, register_Rx, 0);
            if(temp_result.second) { // if overflow happens in temp_result
                result.second = true;
            }
        } else if(immediate_offset == "10") { // Rm logical shift left by Rx
            string shifted_Rm = left_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = multiplication(register_Rn, shifted_Rm, carry_in);
        } else if(immediate_offset == "11") { // Rm logical shift right by Rx
            string shifted_Rm = right_shift(register_Rm, stoi(register_Rx,nullptr,2));
            result = multiplication(register_Rn, shifted_Rm, carry_in);
        }
        assert(result.first.length() == 16);
        if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "00") { // if Rn = reg0
            reg0 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "01") { // if Rn = reg1
            reg1 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "10") { // if Rn = reg2
            reg2 = result.first;
        } else if(string(1,immediate[10-3])+string(1,immediate[10-2]) == "11") { // if Rn = reg3
            reg3 = result.first;
        }
        if(write_carry) {
            if(result.second) {
                m_carry = 1;
            } else {
                m_carry = 0;
            }
        }
        pc++;
    }

    // R0 := R0 * Mem[N]
    void MLM(string immediate) {
        assert(immediate.length() == 11);
        string immediate_16 = immediate;
        for(int i = 0; i < 5; i++) {
            immediate_16 = "0" + immediate_16;
        }
        assert(immediate_16.length() == 16);
        string register_R0 = reg0;
        map<string,string>::iterator it = memory.find(immediate_16);
        string data;
        if(it != memory.end()) { // found Mem[N]
            data = it->second;
        } else {
            data = "0000000000000000";
        }
        assert(data.length() == 16);
        pair<string,bool> result = multiplication(reg0, data, 0); // cin = 0
        reg0 = result.first;
        if(result.second) { // if cout = 1
            m_carry = 1;
        } else {
            m_carry = 0;
        }
        pc++;
    }

    // Bitwise Boolean Operations, carry NOT written
    void BBO(string immediate) {
        assert(immediate.length() == 11);
        string which_boolean_operation = string(1,immediate[10-10])+string(1,immediate[10-9])+string(1,immediate[10-8]);
        string register_Rn = get_register_value(string(1,immediate[10-3])+string(1,immediate[10-2]));
        assert(register_Rn.length() == 16);
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0]));
        assert(register_Rm.length() == 16);
        string N_value = "";
        for(int i = 10-7; i <= 10-4; i++) {
            N_value = N_value + immediate[i];
        }
        assert(N_value.length() == 4);
        int n = stoi(N_value,nullptr,2);
        string result;
        string zero_register = "0000000000000000";
        if(which_boolean_operation == "000") { // Rn = N + NOT Rm
            string not_Rm = bitwise_not(register_Rm);
            assert(not_Rm.length() == 16);
            pair<string,bool> sum = addition(not_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "001") { // Rn = Rn AND Rm + N
            string Rn_and_Rm = bitwise_and(register_Rn,register_Rm);
            assert(Rn_and_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_and_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "010") { // Rn = Rn OR Rm + N
            string Rn_or_Rm = bitwise_or(register_Rn,register_Rm);
            assert(Rn_or_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_or_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "011") { // Rn = Rn XOR Rm + N
            string Rn_xor_Rm = bitwise_xor(register_Rn,register_Rm);
            assert(Rn_xor_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_xor_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "100") { // Rn = Rn NAND Rm + N
            string Rn_and_Rm = bitwise_and(register_Rn,register_Rm);
            string Rn_nand_Rm = bitwise_not(Rn_and_Rm);
            assert(Rn_nand_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_nand_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "101") { // Rn = Rn NOR Rm + N
            string Rn_or_Rm = bitwise_or(register_Rn,register_Rm);
            string Rn_nor_Rm = bitwise_not(Rn_or_Rm);
            assert(Rn_nor_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_nor_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "110") { // Rn = Rn XNOR Rm + N
            string Rn_xor_Rm = bitwise_xor(register_Rn,register_Rm);
            string Rn_xnor_Rm = bitwise_not(Rn_xor_Rm);
            assert(Rn_xnor_Rm.length() == 16);
            pair<string,bool> sum = addition(Rn_xnor_Rm,zero_register,n);
            result = sum.first;
        } else if(which_boolean_operation == "111") { // Rn = Rm + N
            pair<string,bool> sum = addition(register_Rm,zero_register,n);
            result = sum.first;
        }
        assert(result.length() == 16);
        string which_register = string(1,immediate[10-3])+string(1,immediate[10-2]);
        if(which_register == "00") {
            reg0 = result;
        } else if(which_register == "01") {
            reg1 = result;
        } else if(which_register == "10") {
            reg2 = result;
        } else if(which_register == "11") {
            reg3 = result;
        }
        pc++;
    }

    // Rn = Rm[A..B], carry NOT written
    void BFE(string immediate) {
        assert(immediate.length() == 12);
        string result = "0000000000000000";
        string register_Rm = get_register_value(string(1,immediate[11-1])+string(1,immediate[11-0]));
        assert(register_Rm.length() == 16);
        string index_A;
        for(int i = 11-11; i <= 11-8; i++) {
            index_A = index_A + immediate[i];
        }
        assert(index_A.length() == 4);
        string index_B;
        for(int i = 11-7; i <= 11-4; i++) {
            index_B = index_B + immediate[i];
        }
        assert(index_B.length() == 4);
        int a = stoi(index_A,nullptr,2);
        int b = stoi(index_B,nullptr,2);
        assert(a>b);
        for(int pos = 15-a; pos <= 15-b; pos++) {
            result[pos] = register_Rm[pos];
        }
        assert(result.length() == 16);
        string which_register = string(1,immediate[11-3])+string(1,immediate[11-2]);
        if(which_register == "00") {
            reg0 = result;
        } else if(which_register == "01") {
            reg1 = result;
        } else if(which_register == "10") {
            reg2 = result;
        } else if(which_register == "11") {
            reg3 = result;
        }
        pc++;
    }

    // Rn := Rn + N, carry always written
    void ADI(string immediate) {
        assert(immediate.length() == 11);
        string register_Rn = get_register_value(string(1,immediate[10-10])+string(1,immediate[10-9]));
        string immediate_value_16 = "0000000";
        for(int i = 10-8; i <= 10-0; i++) {
            immediate_value_16 = immediate_value_16 + immediate[i];
        }
        assert(immediate_value_16.length() == 16);
        pair<string,bool> result = addition(register_Rn,immediate_value_16,0);
        assert(result.first.length() == 16);
        string which_register = string(1,immediate[10-10])+string(1,immediate[10-9]);
        if(which_register == "00") {
            reg0 = result.first;
        } else if(which_register == "01") {
            reg1 = result.first;
        } else if(which_register == "10") {
            reg2 = result.first;
        } else if(which_register == "11") {
            reg3 = result.first;
        }
        if(result.second) {
            m_carry = 1;
        } else {
            m_carry = 0;
        }
        pc++;
    }

    // Rn := Rn - N
    void SBI(string immediate) {
        assert(immediate.length() == 11);
        string register_Rn = get_register_value(string(1,immediate[10-10])+string(1,immediate[10-9]));
        string immediate_value_16 = "0000000";
        for(int i = 10-8; i <= 10-0; i++) {
            immediate_value_16 = immediate_value_16 + immediate[i];
        }
        assert(immediate_value_16.length() == 16);
        pair<string,bool> result = subtraction(register_Rn,immediate_value_16,0);
        assert(result.first.length() == 16);
        string which_register = string(1,immediate[10-10])+string(1,immediate[10-9]);
        if(which_register == "00") {
            reg0 = result.first;
        } else if(which_register == "01") {
            reg1 = result.first;
        } else if(which_register == "10") {
            reg2 = result.first;
        } else if(which_register == "11") {
            reg3 = result.first;
        }
        if(result.second) {
            m_carry = 1;
        } else {
            m_carry = 0;
        }
        pc++;
    }

    // Rn := N
    void LDI(string immediate) { // only 3 bits for instruction, so 13 bits for immediate
        assert(immediate.length() == 13);
        string which_register = string(1,immediate[12-12])+string(1,immediate[12-11]);
        string immediate_value_16 = "00000";
        for(int i = 12-10; i <= 12-0; i++) {
            immediate_value_16 = immediate_value_16 + immediate[i];
        }
        assert(immediate_value_16.length() == 16);
        if(which_register == "00") {
            reg0 = immediate_value_16;
        } else if(which_register == "01") {
            reg1 = immediate_value_16;
        } else if(which_register == "10") {
            reg2 = immediate_value_16;
        } else if(which_register == "11") {
            reg3 = immediate_value_16;
        }
        pc++;
    }

    // Rn := Mem[N]
    void LDR(string immediate) {
        assert(immediate.length() == 13);
        string which_register = string(1,immediate[12-12])+string(1,immediate[12-11]);
        string register_holding_address = string(1,immediate[12-9])+string(1,immediate[12-8]);
        string r_a_value;
        if(register_holding_address == "00") {
            r_a_value = reg0;
        } else if(register_holding_address == "01") {
            r_a_value = reg1;
        } else if(register_holding_address == "10") {
            r_a_value = reg2;
        } else if(register_holding_address == "11") {
            r_a_value = reg3;
        }
        assert(r_a_value.length() == 16);
        pair<string,bool> sum;
        if(immediate[12-10] == '0') { // register with immediate offset. Rn := mem[Ra+N]
            string immediate_value_16 = "00000000";
            for(int i = 12-7; i <= 12-0; i++) {
                immediate_value_16 = immediate_value_16 + immediate[i];
            }
            assert(immediate_value_16.length() == 16);
            sum = addition(r_a_value,immediate_value_16,0);
        } else if(immediate[12-10] == '1') { // register with scaled register offset. Rn := mem[Ra+Rb<<s]
            string register_holding_offset = string(1,immediate[12-7])+string(1,immediate[12-6]);
            string r_b_value;
            if(register_holding_offset == "00") {
                r_b_value = reg0;
            } else if(register_holding_offset == "01") {
                r_b_value = reg1;
            } else if(register_holding_offset == "10") {
                r_b_value = reg2;
            } else if(register_holding_offset == "11") {
                r_b_value = reg3;
            }
            assert(r_b_value.length() == 16);
            string shift_value;
            for(int i = 12-5; i <= 12-0; i++) {
                shift_value = shift_value + immediate[i];
            }
            assert(shift_value.length() == 6);
            string shifted_r_b = left_shift(r_b_value,stoi(shift_value,nullptr,2));
            sum = addition(r_a_value,shifted_r_b,0);
        }
        assert(sum.first.length() == 16);
        map<string,string>::iterator it = memory.find(sum.first);
        string loaded_value;
        if(it != memory.end()) { // found
            loaded_value = it->second;
        } else { // not found
            loaded_value = "0000000000000000";
        }
        assert(loaded_value.length() == 16);
        if(which_register == "00") {
            reg0 = loaded_value;
        } else if(which_register == "01") {
            reg1 = loaded_value;
        } else if(which_register == "10") {
            reg2 = loaded_value;
        } else if(which_register == "11") {
            reg3 = loaded_value;
        }
        pc++;
    }

    // Mem[N] = Rn
    void STA(string immediate) {
        assert(immediate.length() == 13);
        string which_register = string(1,immediate[12-12])+string(1,immediate[12-11]);
        assert(which_register.length() == 2);
        string store_value;
        if(which_register == "00") {
            store_value = reg0;
        } else if(which_register == "01") {
            store_value = reg1;
        } else if(which_register == "10") {
            store_value = reg2;
        } else if(which_register == "11") {
            store_value = reg3;
        }
        assert(store_value.length() == 16);
        string location = "00000";
        for(int i = 12-10; i <= 12-0; i++) {
            location = location + immediate[i];
        }
        assert(location.length() == 16);
        map<string,string>::iterator it = memory.find(location);
        if(it != memory.end()) { // found
            it->second = store_value;
        } else { // not found
            memory.insert(pair<string,string>(location,store_value));
        }
        pc++;
    }

    // store indirect
    void STI(string immediate) {
        assert(immediate.length() == 11);
        string register_Rn = get_register_value(string(1,immediate[10-10])+string(1,immediate[10-9]));
        assert(register_Rn.length() == 16);
        if(immediate[10-8] == '0') { // mem[Ra + N] = Rn
            string register_Ra = get_register_value(string(1,immediate[10-7])+string(1,immediate[10-6]));
            assert(register_Ra.length() == 16);
            string N_value = "0000000000";
            for(int i = 10-5; i <= 10-0; i++) {
                N_value = N_value + immediate[i];
            }
            assert(N_value.length() == 16);
            pair<string,bool> sum = addition(register_Ra,N_value,0);
            assert(sum.first.length() == 16);
            map<string,string>::iterator it = memory.find(sum.first);
            if(it != memory.end()) { // found
                it->second = register_Rn;
            } else { // not found
                memory.insert(pair<string,string>(sum.first,register_Rn));
            }
        } else if(immediate[10-8] == '1') { // mem[Ra + Rb << s] = Rn
            string register_Ra = get_register_value(string(1,immediate[10-7])+string(1,immediate[10-6]));
            string register_Rb = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4]));
            string s_value = "";
            for(int i = 10-3; i <= 10-0; i++) {
                s_value = s_value + immediate[i];
            }
            assert(s_value.length() == 4);
            string shifted_Rb = left_shift(register_Rb,stoi(s_value,nullptr,2));
            assert(shifted_Rb.length() == 16);
            pair<string,bool> sum = addition(register_Ra,shifted_Rb,0);
            assert(sum.first.length() == 16);
            map<string,string>::iterator it = memory.find(sum.first);
            if(it != memory.end()) { // found
                it->second = register_Rn;
            } else { // not found
                memory.insert(pair<string,string>(sum.first,register_Rn));
            }
        }
    }

    // adding to the stack
    void PSH(string immediate) {
        assert(immediate.length() == 11);
        string which_register = string(1,immediate[10-10])+string(1,immediate[10-9]);
        string pushed_value;
        if(which_register == "00") {
            pushed_value = reg0;
        } else if(which_register == "01") {
            pushed_value = reg1;
        } else if(which_register == "10") {
            pushed_value = reg2;
        } else if(which_register == "11") {
            pushed_value = reg3;
        }
        assert(pushed_value.length() == 16);
        stack.push_back(pushed_value);
        pc++;
    }

    // removing from the stack
    void POP(string immediate) {
        assert(immediate.length() == 11);
        string which_register = string(1,immediate[10-10])+string(1,immediate[10-9]);
        string popped_value;
        if(stack.empty()) {
            popped_value = "0000000000000000";
        } else {
            popped_value = stack[stack.size()-1];
        }
        assert(popped_value.length() == 16);
        if(which_register == "00") {
            reg0 = popped_value;
        } else if(which_register == "01") {
            reg1 = popped_value;
        } else if(which_register == "10") {
            reg2 = popped_value;
        } else if(which_register == "11") {
            reg3 = popped_value;
        }
        stack.resize(stack.size()-1);
        pc++;
    }

    // Rn := Rm XSL N
    void XSL(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[10-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0]));
        assert(register_Rm.length() == 16);
        string which_carry_in = string(1,immediate[10-9])+string(1,immediate[10-8]);
        char carry_in = '0'; // assuming instr[9..8] = 00
        if(which_carry_in == "01") {
            carry_in = '1';
        } else if(which_carry_in == "10") {
            if(m_carry == 0) {
                carry_in = '0';
            } else if(m_carry == 1) {
                carry_in = '1';
            }
        } else if(which_carry_in == "11") {
            if(register_Rm[0] == '0') {
                carry_in = '0';
            } else if(register_Rm[0] == '1') {
                carry_in = '1';
            }
        }
        string immediate_value;
        for(int i = 10-7; i <= 10-4; i++) {
            immediate_value = immediate_value + immediate[i];
        }
        assert(immediate_value.length() == 4);
        string result = register_Rm;
        int n = stoi(immediate_value,nullptr,2);
        for(int i = 0; i <= 15-n; i++) {
            result[i] = register_Rm[i+n];
        }
        for(int i = 16-n; i < 16; i++) {
            result[i] = carry_in;
        }
        assert(result.length() == 16);
        string Rn = string(1,immediate[10-3])+string(1,immediate[10-2]);
        if(Rn == "00") {
            reg0 = result;
        } else if(Rn == "01") {
            reg1 = result;
        } else if(Rn == "10") {
            reg2 = result;
        } else if(Rn == "11") {
            reg3 = result;
        }
        if(write_carry) {
            if(n > 0 && register_Rm[0] == '1') {
                m_carry = 1;
            } else {
                m_carry = 0;
            }
        }
        pc++;
    }

    // Rn := Rm XSR N
    void XSR(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[10-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0]));
        assert(register_Rm.length() == 16);
        string which_carry_in = string(1,immediate[10-9])+string(1,immediate[10-8]);
        char carry_in = '0'; // assuming instr[9..8] = 00
        if(which_carry_in == "01") {
            carry_in = '1';
        } else if(which_carry_in == "10") {
            if(m_carry == 0) {
                carry_in = '0';
            } else if(m_carry == 1) {
                carry_in = '1';
            }
        } else if(which_carry_in == "11") {
            if(register_Rm[0] == '0') {
                carry_in = '0';
            } else if(register_Rm[0] == '1') {
                carry_in = '1';
            }
        }
        string immediate_value;
        for(int i = 10-7; i <= 10-4; i++) {
            immediate_value = immediate_value + immediate[i];
        }
        assert(immediate_value.length() == 4);
        string result = register_Rm;
        int n = stoi(immediate_value,nullptr,2);
        for(int i = n; i <= 15; i++) {
            result[i] = register_Rm[i-n];
        }
        for(int i = 0; i < n; i++) {
            result[i] = carry_in;
        }
        assert(result.length() == 16);
        string Rn = string(1,immediate[10-3])+string(1,immediate[10-2]);
        if(Rn == "00") {
            reg0 = result;
        } else if(Rn == "01") {
            reg1 = result;
        } else if(Rn == "10") {
            reg2 = result;
        } else if(Rn == "11") {
            reg3 = result;
        }
        if(write_carry) {
            if(n > 0 && register_Rm[15] == '1') {
                m_carry = 1;
            } else {
                m_carry = 0;
            }
        }
        pc++;
    }

    // PC = Rm with conditions
    void JMR(string immediate) {
        assert(immediate.length() == 11);
        string condition = string(1,immediate[10-10])+string(1,immediate[10-9])+string(1,immediate[10-8]);
        bool jump = false;
        string register_Rn = get_register_value(string(1,immediate[10-3])+string(1,immediate[10-2]));
        assert(register_Rn.length() == 16);
        int Rn = stoi(register_Rn,nullptr,2);
        string x_value = "";
        for(int i = 10-7; i <= 10-4; i++) {
            x_value = x_value + immediate[i];
        }
        assert(x_value.length() == 4);
        int x = stoi(x_value,nullptr,2);
        string register_Rx = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4]));
        assert(register_Rx.length() == 16);
        int Rx = stoi(register_Rx,nullptr,2);
        if(condition == "000") {
            if(Rn == 0) { // if Rn = 0
                jump = true;
            }
        } else if(condition == "001") {
            if(Rn != 0) {
                jump = true;
            }
        } else if(condition == "010") {
            if(Rn == Rx) {
                jump = true;
            }
        } else if(condition == "011") {
            if(Rn != Rx) {
                jump = true;
            }
        } else if(condition == "100") {
            if(Rn < Rx) {
                jump = true;
            }
        } else if(condition == "101") {
            if(Rn <= Rx) {
                jump = true;
            }
        } else if(condition == "110") {
            if(register_Rn[15-x] == '1') {
                jump = true;
            }
        } else if(condition == "111") {
            if(m_carry = 1) {
                jump = true;
            }
        }
        string register_Rm = get_register_value(string(1,immediate[10-1])+string(1,immediate[10-0]));
        assert(register_Rm.length() == 16);
        if(jump) {
            pc = stoi(register_Rm,nullptr,2);
        } else {
            pc++;
        }
    }

    // PC = N
    void JMP(string immediate) {
        assert(immediate.length() == 11);
        pc = stoi(immediate,nullptr,2);
    }

    // stops the program from executing further instructions
    void STP(string immediate) {
        stop = true;
        cout << "PC: " << pc << " Program stopped" << endl;
    }

    // logical shift left by n, used for immediate offset
    string left_shift(string immediate, int n) {
        assert(immediate.length() == 16);
        string result = immediate;
        for(int i = 0; i < immediate.length()-n; i++) {
            result[i] = immediate[i+n];
        }
        for(int i = immediate.length()-n; i < immediate.length(); i++) {
            result[i] = '0';
        }
        assert(result.length() == 16);
        return result;
    }

    // logical shift right by n, used for immediate offset
    string right_shift(string immediate, int n) {
        assert(immediate.length() == 16);
        string result = immediate;
        for(int i = n; i < immediate.length(); i++) {
            result[i] = immediate[i-n];
        }
        for(int i = 0; i < n; i++) {
            result[i] = '0';
        }
        assert(result.length() == 16);
        return result;
    }

    // return value at the chosen register
    string get_register_value(string immediate) {
        assert(immediate.length() == 2);
        if(immediate == "00") {
            return reg0;
        } else if(immediate == "01") {
            return reg1;
        } else if(immediate == "10") {
            return reg2;
        } else if(immediate == "11") {
            return reg3;
        }
    }

    // performs Rn + Rm + cin then returns sum and overflow flag
    pair<string,bool> addition(string Rn, string Rm, int cin) {
        bool overflow = false;
        assert(Rn.length() == 16);
        assert(Rm.length() == 16);
        int Rn_value = stoi(Rn, nullptr, 2);
        int Rm_value = stoi(Rm, nullptr, 2);
        int sum = Rn_value + Rm_value + cin;
        string sum_binary = decimal_to_binary(sum);
        if(stoi(sum_binary, nullptr, 2) < sum) { // overflow happened
            overflow = true;
        }
        return pair<string,bool>(sum_binary,overflow);
    }

    // performs Rn - Rm - cin then returns result and cout
    pair<string,bool> subtraction(string Rn, string Rm, int cin) {
        bool overflow = false;
        assert(Rn.length() == 16);
        assert(Rm.length() == 16);
        int Rn_value = stoi(Rn,nullptr,2);
        int Rm_value = stoi(Rm,nullptr,2);
        int result = Rn_value - Rm_value - cin;
        string result_binary = decimal_to_binary(result);
        if(result >= 0 && stoi(result_binary,nullptr,2) == result) {
            // overflow = false;
        } else {
            overflow = true; // might be wrong
        }
        return pair<string,bool>(result_binary,overflow);
    }

    pair<string,bool> multiplication(string Rn, string Rm, int cin) {
        bool overflow = false;
        assert(Rn.length() == 16);
        assert(Rm.length() == 16);
        int Rn_value = stoi(Rn, nullptr, 2);
        int Rm_value = stoi(Rm, nullptr, 2);
        int result = Rn_value * Rm_value + cin;
        string result_binary = decimal_to_binary(result);
        if(stoi(result_binary, nullptr, 2) < result) { // overflow happened
            overflow = true;
        }
        return pair<string,bool>(result_binary,overflow);
    }

    string bitwise_not(string s) {
        assert(s.length() == 16);
        string result = "";
        for(int i = 0; i < 16; i++) {
            if(s[i] == '0') {
                result = result + '1';
            } else if(s[i] == '1') {
                result = result + '0';
            }
        }
        assert(result.length() == 16);
        return result;
    }

    string bitwise_and(string s1, string s2) {
        assert(s1.length() == 16);
        assert(s2.length() == 16);
        string result = "";
        for(int i = 0; i < 16; i++) {
            if(s1[i] == '0' && s2[i] == '0') {
                result = result + '0';
            } else if(s1[i] == '0' && s2[i] == '1') {
                result = result + '0';
            } else if(s1[i] == '1' && s2[i] == '0') {
                result = result + '0';
            } else if(s1[i] == '1' && s2[i] == '1') {
                result = result + '1';
            }
        }
        assert(result.length() == 16);
        return result;
    }

    string bitwise_or(string s1, string s2) {
        assert(s1.length() == 16);
        assert(s2.length() == 16);
        string result = "";
        for(int i = 0; i < 16; i++) {
            if(s1[i] == '0' && s2[i] == '0') {
                result = result + '0';
            } else if(s1[i] == '0' && s2[i] == '1') {
                result = result + '1';
            } else if(s1[i] == '1' && s2[i] == '0') {
                result = result + '1';
            } else if(s1[i] == '1' && s2[i] == '1') {
                result = result + '1';
            }
        }
        assert(result.length() == 16);
        return result;
    }

    string bitwise_xor(string s1, string s2) {
        assert(s1.length() == 16);
        assert(s2.length() == 16);
        string result = "";
        for(int i = 0; i < 16; i++) {
            if(s1[i] == '0' && s2[i] == '0') {
                result = result + '0';
            } else if(s1[i] == '0' && s2[i] == '1') {
                result = result + '1';
            } else if(s1[i] == '1' && s2[i] == '0') {
                result = result + '1';
            } else if(s1[i] == '1' && s2[i] == '1') {
                result = result + '0';
            }
        }
        assert(result.length() == 16);
        return result;
    }

    void decimal_to_binary_test(int n) {
        cout << decimal_to_binary(n) << endl;
    }

    string decimal_to_binary(int decimal_value) {
        bool positive_value = true;
        if(decimal_value < 0) {
            positive_value = false;
        }
        string binary_value = "";
        int n = abs(decimal_value);
        while(n != 0) {
            if(n % 2 == 0) {
                binary_value = "0" + binary_value;
            } else {
                binary_value = "1" + binary_value;
            }
            n = n / 2;
        }
        if(positive_value) {
            if(binary_value.length() < 16) { // fill the rest with zeroes
                string temp = binary_value;
                for(int i = 0; i < 16-binary_value.length(); i++) {
                    temp = "0" + temp;
                }
                binary_value = temp;
            } else if(binary_value.length() > 16) {// discard excess bits
                string temp = "";
                for(int i = binary_value.length()-1; i >= binary_value.length()-16; i--) {
                    temp = binary_value[i] + temp;
                }
                binary_value = temp;
            }
        } else { // negative value
            string temp = "";
            for(int i = 0; i < binary_value.length(); i++) { // invert all bits
                if(binary_value[i] == '0') {
                    temp = temp + '1';
                } else if(binary_value[i] == '1') {
                    temp = temp + '0';
                }
            }
            temp = "1" + temp;
            int int_temp = stoi(temp,nullptr,2) + 1;
            string twos_complement_binary = "";
            while(int_temp != 0) {
                if(int_temp % 2 == 0) {
                    twos_complement_binary = "0" + twos_complement_binary;
                } else {
                    twos_complement_binary = "1" + twos_complement_binary;
                }
                int_temp = int_temp / 2;
            }
            if(twos_complement_binary.length() < 16) { // fill the rest with ones
                string t = twos_complement_binary;
                for(int i = 0; i < 16-twos_complement_binary.length(); i++) {
                    t = "1" + t;
                }
                twos_complement_binary = t;
            } else if(twos_complement_binary.length() > 16) { // discard the excess bits
                string t = "";
                for(int i = twos_complement_binary.length()-1; i >= twos_complement_binary.length()-16; i--) {
                    t = twos_complement_binary[i];
                }
                twos_complement_binary = t;
            }
            assert(twos_complement_binary.length() == 16);
            binary_value = twos_complement_binary;
        }
        assert(binary_value.length() == 16);
        return binary_value;
    }

    // turn a string to uppercase
    string transform_to_uppercase(string instr) {
        string transformed = "";
        for(int i = 0; i < instr.length(); i++) {
            transformed += toupper(instr[i]);
        }
        return transformed;
    }
};


#endif