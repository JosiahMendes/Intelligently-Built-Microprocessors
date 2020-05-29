#ifndef simulator_hpp
#define simulator_hpp

#include <iostream>
#include <string>
#include <assert.h>
#include <map>
using namespace std;

class CPU {
private:
    string reg0; // assuming reg 00
    string reg1; // assuming reg 01
    string reg2; // assuming reg 10
    string reg3; // assuming reg 11
    int pc;
    int m_carry;
    map<int,pair<string,int>> m_instructions;
    map<string,string> memory; // location and data written as string
public:
    CPU() {
        reg0 = "0000000000000000";
        reg1 = "0000000000000000";
        reg2 = "0000000000000000";
        reg3 = "0000000000000000";
        pc = 0;
        m_carry = 0;
    }

    void update() {
        reg0 = "0000000000000001"; // 0x1
        reg1 = "0000000000000010"; // 0x2
        reg2 = "0000000000000011"; // 0x3
        reg3 = "0000000000000100"; // 0x4
    }

    void show_content() {
        cout << "reg0: " << reg0 << endl;
        cout << "reg1: " << reg1 << endl;
        cout << "reg2: " << reg2 << endl;
        cout << "reg3: " << reg3 << endl;
        cout << "PC: " << pc << endl;
        cout << "Carry: " << m_carry << endl;
        for(map<string,string>::iterator it = memory.begin(); it != memory.end(); ++it) {
            cout << "Location: " << it->first << " " << "Data: " << it->second << endl;
        }
    }

    void ADM_test() {
        memory.insert(pair<string,string>("0000000000000100","0000000000001000"));
        memory.insert(pair<string,string>("0000000010001000","0000100000000000"));
    }

    // Rn := Rn + Rm + Cin
    void ADR(string immediate) {
        assert(immediate.length() == 11);
        bool write_carry = false; // assuming instr[10] = 0
        if(immediate[15-10] == '1') { // if instr[10] = 1, write carry
            write_carry = true;
        }
        string register_Rx = get_register_value(string(1,immediate[10-5])+string(1,immediate[10-4])); // instr[5..4] gives register Rx
        cout << "Rx: " << register_Rx << endl;
        cout << register_Rx.length() << endl;
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
                result.second == true;
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
    }

    // logical shift left by n
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

    // logical shift right by n
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

    string decimal_to_binary(int decimal_value) {
        string binary_value = "";
        int n = decimal_value;
        while(n != 0) {
            if(n % 2 == 0) {
                binary_value = "0" + binary_value;
            } else {
                binary_value = "1" + binary_value;
            }
            n = n / 2;
        }
        if(binary_value.length() < 16) { // fill the rest with zeroes
            string temp = binary_value;
            for(int i = 0; i < 16-binary_value.length(); i++) {
                temp = "0" + temp;
            }
            binary_value = temp;
        } else if(binary_value.length() > 16) { // discard excess bits
            string temp = "";
            for(int i = binary_value.length()-1; i >= binary_value.length()-16; i--) {
                temp = binary_value[i] + temp;
            }
            binary_value = temp;
        }
        assert(binary_value.length() == 16);
        return binary_value;
    }
};


#endif