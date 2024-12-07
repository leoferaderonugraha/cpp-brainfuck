#include <iostream>
#include <map>
#include <stack>
#include <fstream>
#include <sstream>
#include <vector>


bool populate_jump_address(std::map<int, int>& jump_address, std::string data)
{
    std::stack<int> bracket_stack;
    for (size_t i = 0; i < data.length(); i++) {
        switch (data[i])
        {
            case '[': {
                bracket_stack.emplace(i);
                break;
            }
            case ']': {
                if (bracket_stack.size() == 0) {
                    return false;
                }
                int start = bracket_stack.top();
                bracket_stack.pop();

                jump_address[start] = i;
                jump_address[i] = start;
                break;
            }
        }
    }
    return true;
}


void interpret(std::string data)
{
    std::map<int, int> jump_address;
    std::vector<int> memory(2048, 0);

    int dx = 0;  // register for data pointer
    int pc = 0;  // register for program counter

    if (!populate_jump_address(jump_address, data)) {
        std::cout << "Failed to initialize the jump addresses; "
                  << "invalid bracket(s)" << std::endl;
        return;
    }

    while (pc < data.length()) {
        char op = data[pc];

        switch (op)
        {
            case '[': {
                if (memory[dx] == 0) {
                    pc = jump_address[pc];
                    continue;
                }
                break;
            }
            case ']': {
                if (memory[dx] != 0) {
                    pc = jump_address[pc];
                }
                break;
            }
            case '>': dx = (dx + 1) % memory.size(); break;
            case '<': dx = (dx - 1) % memory.size(); break;
            case '+': memory[dx] = (memory[dx] + 1) % 256; break;
            case '-': memory[dx] = (memory[dx] - 1) % 256; break;
            case '.': std::cout << (char) memory[dx]; break;
            case ',': std::cin >> memory[dx]; break;
        }

        pc++;
    }
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << argv[0] << " <file.bf>" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    std::string data; 
    std::ifstream input_file(argv[1], std::ios_base::in);

    buffer << input_file.rdbuf();
    data = buffer.str();

    input_file.close();

    interpret(data);
}
