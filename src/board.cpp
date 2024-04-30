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

Board::~Board() {
    while (!empty.empty()) {
        Point* p = empty.back();
        empty.pop_back();
        delete p;
    }
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
            else empty.push_back(new Point(row, col));
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
    } else if (command.find("NAIVE") != std::string::npos) {
        char color = command.find("RED") != std::string::npos ? 'r' : 'b';
        char moves = command.find("1") != std::string::npos ? 1 : 2;
        std::cout << boolToYesNo(simulate(color, moves, NAIVE));
    }

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

/*bool Board::findMoves(Point* point, char color, char moves) {
    for (char i = 0; i < empty.size(); i++) {
        if (fields[empty[i]->x][empty[i]->y] != ' ')
            continue;

        point[moves-1] = *empty[i];
        fields[point->x][point->y] = color;

        bool isMoveOk = isGameOver(false)[0] == (moves - 1 == 0);

        if (!isMoveOk) continue;

        if (moves - 1 == 0) return true;
        if (findMoves(point, color, moves - 1)) return true;
        else {
            for (char j = 0; j < moves; j++) {

            }
        }
    }
    return false;
}*/

/*Point* Board::findMoves(char color, char moves, bool winning, bool canBlock) {
    auto* points = (Point *) (malloc(moves * sizeof(Point)));
    if (moves < 1)
        return nullptr;

    for (char i = 0; i < moves; i++) {
        Point* p = findSingleMove(color, i + 1 == moves, canBlock);
        if (p) points[i] = *p;
        free(p);
    }

    return nullptr;
}*/

bool Board::findMove(Point* points, char color, char moves) {
    for (char i = 0; i < empty.size(); i++) {
        if (fields[empty[i]->x][empty[i]->y] != ' ')
            continue;

        fields[empty[i]->x][empty[i]->y] = color;
        bool* win = isGameOver(false);

        bool over = win[0];
        delete win;

        if (over && !moves) {
            points[moves] = *empty[i];
            return true;
        }

        if (moves && findMove(points, color, moves - 1)) {
            points[moves] = *empty[i];
            return true;
        }

        fields[empty[i]->x][empty[i]->y] = ' ';
    }

    return false;
}

bool Board::simulate(char color, char moves, Type type) {
    bool* win = isGameOver(false);
    if (!isCorrect() || win[0])
        return false;
    delete win;

    if (pawns_counter[color == 'b'] + moves < size)
        return false;

    int opponent;
    if (color == 'r') opponent = pawns_counter[RED] != pawns_counter[BLUE];
    else opponent = pawns_counter[RED] == pawns_counter[BLUE];

    if (moves > 1)
        opponent += moves-1;

    if (empty.size() < opponent + moves)
        return false;

    if (type == NAIVE) {
        auto* points = new Point[moves];
        bool output = findMove(points, color, moves - 1);
        for (int i = 0; i < moves; i++) {
            fields[points[i].x][points[i].y] = ' ';
        }
        delete[] points;
        return output;
    }

//    if (moves == 1) {
//        if (type == NAIVE) {
//            auto* points = new Point[moves];
//            return findMove(points, color, moves - 1);
//        }
//    } else if (moves == 2) {
//        if (type == NAIVE) {
//            for (char i = 0; i < empty.size(); i++) {
//                fields[empty[i]->x][empty[i]->y] = color;
//                win = isGameOver(false);
//                bool over = win[0];
//                delete win;
//
//                if (over) {
//                    fields[empty[i]->x][empty[i]->y] = ' ';
//                    continue;
//                }
//
//                for (char j = 0; j < empty.size(); j++) {
//                    if (i == j)
//                        continue;
//
//                    fields[empty[j]->x][empty[j]->y] = color;
//                    win = isGameOver(false);
//                    over = win[0];
//                    delete win;
//                    fields[empty[j]->x][empty[j]->y] = ' ';
//
//                    if (over) {
//                        fields[empty[i]->x][empty[i]->y] = ' ';
//                        return true;
//                    }
//                }
//                fields[empty[i]->x][empty[i]->y] = ' ';
//            }
//        }
//    }

    return false;
}

void Board::printBoard() const {
    for (char i = 0; i < size; i++) {
        for (char j = 0; j < size; j++) {
            std::cout << fields[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
