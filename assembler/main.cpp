#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using namespace std;

int getRegisterNumber(const string& reg);
int getOpCode(const string& instruction);

int encodeInstruction(int opCode, int srcA, int srcB, int dest);
int encodeLoad(int dest, int value);

string toHex(int number);

string readCirc(const string& fileName);

bool updateRomContents(string& circContent, const vector<string>& hexInstruction);
bool writeCirc(const string& fileName, const string& content);

int main() {
    ifstream file("program.asm");

    if (!file.is_open()) {
        cout << "Could not open program.asm" << endl;

        return -3;
    }

    string line;
    vector<string> hexInstruction;

    while (getline(file, line)) {
        stringstream stream(line);

        string instruction;
        string operand1;
        string operand2;
        string operand3;

        stream >> instruction >> operand1 >> operand2 >> operand3;

        if (instruction.empty()) continue;

        if (!operand1.empty() && operand1.back() == ',') operand1.pop_back();
        if (!operand2.empty() && operand2.back() == ',') operand2.pop_back();

        int opCode = getOpCode(instruction);

        if (opCode == -2) {
            cout << "Invalid instruction: " << instruction << endl;

            return -7;
        }

        int machineCode;

        if (opCode == 5) {
            int dest = getRegisterNumber(operand1);

            if (dest == -1) {
                cout << "Invalid register: " << operand1 << endl;

                return -8;
            }

            int value;

            try {
                size_t pos;
                value = stoi(operand2, &pos);

                if (pos != operand2.length() || value < 0 || value > 15) {
                    cout << "Invalid LOAD value: " << operand2 << endl;

                    return -9;
                }
            } catch (...) {
                cout << "Invalid LOAD value: " << operand2 << endl;

                return -9;
            }

            machineCode = encodeLoad(dest, value);
        } else {
            int reg1 = getRegisterNumber(operand1);
            int reg2 = getRegisterNumber(operand2);
            int reg3 = getRegisterNumber(operand3);

            if (reg1 == -1 || reg2 == -1 || reg3 == -1) {
                cout << "Invalid register in instruction: " << instruction << endl;

                return -8;
            }

            machineCode = encodeInstruction(opCode, reg1, reg2, reg3);
        }

        string hexCode = toHex(machineCode);

        hexInstruction.push_back(hexCode);

        if (hexInstruction.size() > 16) {
            cout << "Error: Program exceeds ROM capacity!" << endl;

            return -6;
        }

        cout << "Hex Code: " << hexCode << endl;
    }

    file.close();

    ofstream outputFile("program.hex");

    if (!outputFile.is_open()) {
        cout << "Could not create program.hex" << endl;

        return -1;
    }

    outputFile << "v2.0 raw" << endl;

    for (int i = 0; i < hexInstruction.size(); i++) outputFile << hexInstruction[i] << endl;

    outputFile.close();

    string circContent = readCirc("../CPU.circ");

    if (circContent.empty()) {
        cout << "\nCould not read CPU.circ" << endl;

        return -4;
    }

    cout << "\nCPU file loaded successfully!" << endl;

    if (!updateRomContents(circContent, hexInstruction)) {
        cout << "Could not update ROM contents!" << endl;

        return -4;
    }

    if (!writeCirc("../CPU.circ", circContent)) {
        cout << "Could not update CPU file!" << endl;

        return -5;
    }

    cout << "\nCPU project updated successfully!" << endl;

    return 0;
}

int getRegisterNumber(const string& reg) {
    if (reg == "R0") return 0;
    if (reg == "R1") return 1;
    if (reg == "R2") return 2;
    if (reg == "R3") return 3;

    return -1;
}

int getOpCode(const string& instruction) {
    if (instruction == "ADD") return 0;
    if (instruction == "SUB") return 1;
    if (instruction == "AND") return 2;
    if (instruction == "OR") return 3;
    if (instruction == "XOR") return 4;
    if (instruction == "LOAD") return 5;

    return -2;
}

int encodeInstruction(int opCode, int srcA, int srcB, int dest) {
    return (opCode << 6) | (srcA << 4) | (srcB << 2) | dest;
}

int encodeLoad(int dest, int value) {
    return (5 << 6) | (dest << 4) | value;
}

string toHex(int number) {
    stringstream stream;

    stream << hex << uppercase;

    stream.width(3);
    stream.fill('0');

    stream << number;

    return stream.str();
}

string readCirc(const string& fileName) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening CPU file" << endl;

        return "";
    }

    string content(
        (istreambuf_iterator<char>(file)),
        istreambuf_iterator<char>()
    );

    file.close();

    return content;
}

bool updateRomContents(string& circContent, const vector<string>& hexInstruction) {
    string openingTag = "<a name=\"contents\">";

    size_t start = circContent.find(openingTag);

    if (start == string::npos) {
        cout << "ROM contents section not found!" << endl;

        return false;
    }

    size_t contentStart = start + openingTag.length();

    size_t end = circContent.find("</a>", contentStart);

    if (end == string::npos) {
        cout << "ROM closing tag not found!" << endl;

        return false;
    }

    cout << "ROM contents section found!" << endl;

    string newContent = "addr/data: 4 9\n";

    for (int i = 0; i < hexInstruction.size(); i++) {
        newContent += hexInstruction[i];

        if (i < hexInstruction.size() - 1) newContent += " ";
    }

    newContent += "\n";

    circContent.replace(contentStart, end - contentStart, newContent);

    return true;
}

bool writeCirc(const string& fileName, const string& content) {
    ofstream file(fileName);

    if (!file.is_open()) return false;

    file << content;

    file.close();

    return true;
}
