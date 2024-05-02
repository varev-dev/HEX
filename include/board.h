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
    Point() = default;
    Point(char x, char y);
} Point;

typedef enum SimulationType {
    NAIVE = 0,
    PERFECT
} Type;

typedef struct Board {
private:
    char currentRow;
    bool isCorrect() const;
    bool checkWinner(char color, bool visited[MAX_SIZE][MAX_SIZE], Point point);
    bool* isGameOver(bool checkIsCorrect = true);
    bool isBoardPossible();
    bool checkIsPawnPathPossible(char color, bool isCorrect = true);
    bool findMove(Point* points, char color, char moves);
    bool reactMove(char color, char moves, char opponent);
    bool predictMove(char color, char moves, char opponent);
    bool simulate(char color, char moves, Type type);
    static char swapColor(char color);
    static std::string boolToYesNo(bool value);
    static std::string boolToColor(bool color);
public:
    char fields[MAX_SIZE][MAX_SIZE];
    Point empty[MAX_SIZE*MAX_SIZE];
    char empty_size;
    char pawns_counter[2];
    char size;
    bool isFilled;

    Board();
    ~Board();
    void setSize(const std::string& firstLine);
    void readFields();
    void readCommand(const std::string &command);
} Hex;

#endif //HEX_BOARD_H
