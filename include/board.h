//
// Created by varev on 26.04.2024.
//

#ifndef HEX_BOARD_H
#define HEX_BOARD_H

#include <cinttypes>
#include <string>

#define MAX_SIZE 11

typedef enum Colors {
    RED = 0,
    BLUE
} Colors;

typedef struct Board {
private:
    char currentRow;
    bool isCorrect();
    bool* isGameOver();
public:
    char fields[MAX_SIZE][MAX_SIZE];
    char pawns_counter[2];
    char size;
    bool isFilled;

    Board();
    void setSize(const std::string& firstLine);
    void readFields();
    void readCommand(const std::string &command);
} Hex;

#endif //HEX_BOARD_H
