#include <iostream>
#include "include/board.h"

int main() {
    std::string input;
    Hex* board = new Hex();
    std::cin >> std::noskipws;
    while (std::getline(std::cin, input)) {
        if (input[0] == ' ' && (!board->size || board->isFilled)) {
            delete board;
            board = new Board();
            board->setSize(input);
            board->readFields();
            //board->printBoard();
        } else {
            board->readCommand(input);
        }
    }
    return 0;
}
