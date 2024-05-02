# Hex Project (extension to board game)

### Project focuses on developing tools and algorithms to analyze HEX game boards, aiming to provide insights into optimal strategies, pattern recognition, and gameplay dynamics.
---
### Available commands:
#### Base:
- BOARD_SIZE
- PAWNS_NUMBER 
- IS_BOARD_CORRECT
- IS_GAME_OVER (dfs based)
- IS_BOARD_POSSIBLE

#### Advanced (parameterized):
```diff
+ COLOR - Red / Blue
+ TYPE - Naive / Perfect
+ X - Amount of moves to check
```
- **CAN_COLOR_WIN_IN_X_MOVES_WITH_TYPE_OPPONENT**

### Technology stack:
- C++(17)
- STL
