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
    pawns_counter[RED] = 0, pawns_counter[BLUE] = 0, empty_size = 0;
    currentRow = 0, size = 0, isFilled = false;
}

Board::~Board() = default;

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
            else empty_size++;
            row--;
            col++;
        }
        currentRow++;
    } while (input.size() != size * 3 + 1);

    char ctr = 0;
    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            if (fields[i][j] != ' ')
                continue;

            empty[ctr] = Point(i, j);
            ctr++;
        }
    }

    isFilled = true;
}

char Board::swapColor(char color) {
    return color == 'r' ? 'b' : 'r';
}

bool Board::isCorrect() const {
    int diff = pawns_counter[RED] - pawns_counter[BLUE];
    return diff == 0 || diff == 1;
}

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

bool* Board::isGameOver(bool checkIsCorrect) {
    bool* win = new bool[2]{false, false};
    bool visited[MAX_SIZE][MAX_SIZE];

    if (checkIsCorrect && !isCorrect())
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
    } else if (command[0] == 'C'){
        Type type = command.find("NAIVE") != std::string::npos ? NAIVE : PERFECT;
        char color = command.find("RED") != std::string::npos ? 'r' : 'b';
        char moves = command.find('1') != std::string::npos ? 1 : 2;
        std::cout << boolToYesNo(simulate(color, moves, type)) << "\n";
    }

    if (command[0] != 'C')
        std::cout << "\n";
}

bool Board::checkIsPawnPathPossible(char color, bool isCorrect) {
    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            if (fields[i][j] != color)
                continue;

            fields[i][j] = ' ';

            bool* go = isGameOver(isCorrect);
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

bool Board::findMove(Point* points, char color, char moves) {
    for (char i = 0; i < empty_size; i++) {
        if (fields[empty[i].x][empty[i].y] != ' ')
            continue;

        fields[empty[i].x][empty[i].y] = color;
        bool* win = isGameOver(false);

        bool over = win[0];
        delete win;

        if (over && !moves) {
            points[moves] = empty[i];
            return true;
        }

        if (over) {
            fields[empty[i].x][empty[i].y] = ' ';
            continue;
        }

        if (moves && findMove(points, color, moves - 1)) {
            points[moves] = empty[i];
            return true;
        }

        fields[empty[i].x][empty[i].y] = ' ';
    }

    return false;
}


bool Board::simulate(char color, char moves, Type type) {
    bool* win = isGameOver(false);
    bool gameOver = win[0];
    delete win;

    if (!isCorrect() || gameOver)
        return false;

    if (pawns_counter[color == 'b'] + moves < size || moves > size*size)
        return false;

    int opponent;
    if (color == 'r') opponent = pawns_counter[RED] != pawns_counter[BLUE];
    else opponent = pawns_counter[RED] == pawns_counter[BLUE];

    opponent += moves-1;

    if (empty_size < opponent + moves)
        return false;

    if (type == NAIVE) {
        auto* points = new Point[moves];
        bool output = findMove(points, color, moves - 1);

        for (int i = 0; i < moves; i++) fields[points[i].x][points[i].y] = ' ';
        delete[] points;

        return output;
    } else {
        auto** winnable_paths = new Point*[opponent + 1];
        winnable_paths[0] = new Point[moves];
        char winnable_ctr = 0;

        if (findMove(winnable_paths[0], swapColor(color), 0)) {
            fields[(*winnable_paths)[0].x][(*winnable_paths)[0].y] = ' ';
            delete[] winnable_paths[0];
            delete[] winnable_paths;
        }

        while (winnable_ctr < opponent) {
            winnable_ctr++;
        }
    }

    return false;
}
