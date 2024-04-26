#include <iostream>
#include "include/board.h"

int main() {
    std::string input;
    Hex board;
    std::cin >> std::noskipws;
    while (std::getline(std::cin, input)) {
        if (input[0] == ' ' && (!board.size || board.isFilled)) {
            board.setSize(input);
            board.readFields();
        } else {
            board.readCommand(input);
        }
    }
    return 0;
}
