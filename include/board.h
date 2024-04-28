//
// Created by varev on 26.04.2024.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include <cinttypes>
#include <string>

#define MAX_SIZE 11

typedef enum Color {
    RED = 0,
    BLUE
} Color;

typedef struct Point {
    char x, y;
    Point(char x, char y);
} Point;

typedef struct Board {
private:
    char currentRow;
    bool isCorrect();
    bool checkWinner(char color, bool visited[MAX_SIZE][MAX_SIZE], Point point);
    bool* isGameOver();
    bool isBoardPossible();
    bool checkIsPawnPathPossible(char color);
    static std::string boolToYesNo(bool value);
    static std::string boolToColor(bool color);
public:
    char fields[MAX_SIZE][MAX_SIZE];
    char pawns_counter[2];
    char size;
    bool isFilled;

    Board();
    void setSize(const std::string& firstLine);
    void readFields();
    void readCommand(const std::string &command);
    void printBoard() const;
} Hex;

#endif //HEX_BOARD_H
