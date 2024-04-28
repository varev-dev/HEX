//
// Created by varev on 26.04.2024.
//
#include <iostream>
#include "../include/board.h"

#define COMM_SIZE std::string("BOARD_SIZE")
#define COMM_PAWN std::string("PAWNS_NUMBER")
#define COMM_CORR std::string("IS_BOARD_CORRECT")
#define COMM_OVER std::string("IS_GAME_OVER")
#define COMM_POSS std::string("IS_BOARD_POSSIBLE")

Point::Point(char x, char y) : x(x), y(y) {}

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

/**
 * @todo fix cases where blue should win and red wins
 */
bool Board::checkWinner(char color, bool visited[MAX_SIZE][MAX_SIZE], Point point) {
    if (visited[point.x][point.y])
        return false;

    if (fields[point.x][point.y] != color)
        return false;

    visited[point.x][point.y] = true;

    if (color == 'r' && point.y == size - 1)
        return true;
    if (color == 'b' && point.x == size - 1)
        return true;

    for (char x = -1; x <= 1; x++) {
        for (char y = -1; y <= 1; y++) {
            if (x+y == 0)
                continue;
            if (point.x + x < 0 || point.y + y < 0)
                continue;
            if (point.x + x >= size || point.y + y >= size)
                continue;
            if (checkWinner(color, visited, Point(point.x + x, point.y + y)))
                return true;
        }
    }

    return false;
}

bool* Board::isGameOver() {
    bool* win = new bool[2]{false, false};
    bool visited[MAX_SIZE][MAX_SIZE];

    if (!isCorrect())
        return win;

    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            visited[i][j] = false;
        }
    }

    for (char i = 0; i < size; i++) {
        if (checkWinner('r', visited, Point(i, 0))) {
            win[0] = true;
            win[1] = RED;
            return win;
        }
        if (checkWinner('b', visited, Point(0, i))) {
            win[0] = true;
            win[1] = BLUE;
            return win;
        }
    }

    return win;
}

std::string Board::boolToYesNo(bool value) {
    return value ? "YES" : "NO";
}

std::string Board::boolToColor(bool color) {
    return color ? "BLUE" : "RED";
}

void Board::readCommand(const std::string& command) {
    if (command == COMM_SIZE)
        std::cout << (int) size;
    else if (command == COMM_PAWN)
        std::cout << pawns_counter[RED] + pawns_counter[BLUE];
    else if (command == COMM_CORR)
        std::cout << boolToYesNo(isCorrect());
    else if (command == COMM_POSS)
        std::cout << boolToYesNo(isBoardPossible());
    else if (command == COMM_OVER) {
        bool* res = isGameOver();
        std::cout << boolToYesNo(res[0]) << (res[0] ? " " + boolToColor(res[1]) : "");
        delete[] res;
    }

    std::cout << "\n";
}

bool Board::checkIsPawnPathPossible(char color) {
    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            if (fields[i][j] != color)
                continue;

            fields[i][j] = ' ';

            bool* go = isGameOver();
            bool win = go[0];
            delete go;

            fields[i][j] = color;

            if (!win)
                return true;
        }
    }
    return false;
}

bool Board::isBoardPossible() {
    if (!isCorrect())
        return false;

    bool* winner = isGameOver();
    bool output = false;

    if (!winner[0]) {
        output = true;
    } else if (winner[1] == RED) {
        if (pawns_counter[RED] - pawns_counter[BLUE] != 1) output = false;
        else output = checkIsPawnPathPossible('r');
    } else if (winner[1] == BLUE) {
        if (pawns_counter[RED] - pawns_counter[BLUE] != 0) output = false;
        else output = checkIsPawnPathPossible('b');
    }

    delete winner;
    return output;
}

void Board::printBoard() const {
    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            std::cout << fields[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
