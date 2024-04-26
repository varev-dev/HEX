//
// Created by varev on 26.04.2024.
//
#include <iostream>
#include "../include/board.h"

#define COMM_SIZE std::string("BOARD_SIZE")
#define COMM_PAWN std::string("PAWNS_NUMBER")
#define COMM_CORR std::string("IS_BOARD_CORRECT")
#define COMM_OVER std::string("IS_GAME_OVER")

Board::Board() {
    for (auto & row : fields) {
        for (char & field : row) {
            field = 0;
        }
    }
    pawns_counter[RED] = 0, pawns_counter[BLUE] = 0;
    currentRow = 0, size = 0, isFilled = false;
}

void Board::setSize(const std::string& firstLine) {
    this->size = (char) ((firstLine.size() - 1) / 3);
    pawns_counter[RED] = 0, pawns_counter[BLUE] = 0;
    currentRow = 1;
}

void Board::readFields() {
    std::string input;

    do {
        std::getline(std::cin, input);

        char row = currentRow <= size ? currentRow - 1 : size - (char) 1,
             col = currentRow <= size ? 0 : currentRow % size;

        for (int i = 0; i < input.size(); i++) {
            if (input[i] != '<')
                continue;

            i += 2;
            fields[row][col] = input[i];
            if (fields[row][col] == 'r') pawns_counter[RED]++;
            else if (fields[row][col] == 'b') pawns_counter[BLUE]++;
            row--;
            col++;
        }
        currentRow++;
    } while (input.size() != size * 3 + 1);

    isFilled = true;
}

bool Board::isCorrect() {
    int diff = pawns_counter[RED] - pawns_counter[BLUE];
    return diff == 0 || diff == 1;
}

bool* Board::isGameOver() {
    bool* win = new bool[2]{false, false};

    if (pawns_counter[RED] < size)
        return win;

    return win;
}

void Board::readCommand(const std::string& command) {
    if (command == COMM_SIZE)
        std::cout << (int) size << "\n";
    else if (command == COMM_PAWN)
        std::cout << pawns_counter[RED] + pawns_counter[BLUE] << "\n";
    else if (command == COMM_CORR)
        std::cout << (isCorrect() ? "YES" : "NO") << "\n";
    else if (command == COMM_OVER)
        std::cout << "";
}