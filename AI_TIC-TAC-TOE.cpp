#include <iostream>
#include <climits>

#define SIZE 3

char board[SIZE][SIZE];
char player, ai;
int maxDepth;

void initBoard() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

void printBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            std::cout << " " << board[i][j] << " ";
            if (j < SIZE - 1) std::cout << "|";
        }
        std::cout << std::endl;
        if (i < SIZE - 1) std::cout << "---|---|---\n";
    }
    std::cout << std::endl;
}

bool isMovesLeft() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == ' ')
                return true;
    return false;
}

int countPotentialWins(char mark) {
    int count = 0;

    for (int row = 0; row < SIZE; row++) {
        bool potential = true;
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] != mark && board[row][col] != ' ') {
                potential = false;
                break;
            }
        }
        count += potential;
    }

    for (int col = 0; col < SIZE; col++) {
        bool potential = true;
        for (int row = 0; row < SIZE; row++) {
            if (board[row][col] != mark && board[row][col] != ' ') {
                potential = false;
                break;
            }
        }
        count += potential;
    }

    bool potential = true;
    for (int i = 0; i < SIZE; i++) {
        if (board[i][i] != mark && board[i][i] != ' ') {
            potential = false;
            break;
        }
    }
    count += potential;

    potential = true;
    for (int i = 0; i < SIZE; i++) {
        if (board[i][SIZE - 1 - i] != mark && board[i][SIZE - 1 - i] != ' ') {
            potential = false;
            break;
        }
    }
    count += potential;

    return count;
}

bool checkWin(char mark) {
    for (int row = 0; row < SIZE; row++) {
        if (board[row][0] == mark && board[row][1] == mark && board[row][2] == mark)
            return true;
    }

    for (int col = 0; col < SIZE; col++) {
        if (board[0][col] == mark && board[1][col] == mark && board[2][col] == mark)
            return true;
    }

    if (board[0][0] == mark && board[1][1] == mark && board[2][2] == mark)
        return true;
    if (board[0][2] == mark && board[1][1] == mark && board[2][0] == mark)
        return true;

    return false;
}

int evaluate() {
    int aiPotentialWins = countPotentialWins(ai);
    int playerPotentialWins = countPotentialWins(player);

    return (player == 'X') ? playerPotentialWins - aiPotentialWins : aiPotentialWins - playerPotentialWins;
}

void findBestMove() {
    int bestVal = (player == 'X') ? INT_MAX : INT_MIN;
    int bestMoveRow = -1, bestMoveCol = -1;

    std::cout << "AI is evaluating moves:\n";

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = ai;
                int moveVal;

                if (maxDepth == 1) {
                    moveVal = evaluate();
                } else {
                    int bestOpponentResponseVal = (player == 'X') ? INT_MIN : INT_MAX;
                    std::cout << "\tEvaluating opponent responses for move (" << i << ", " << j << "):\n";

                    for (int k = 0; k < SIZE; k++) {
                        for (int l = 0; l < SIZE; l++) {
                            if (board[k][l] == ' ') {
                                board[k][l] = player;
                                int opponentResponseVal = evaluate();
                                std::cout << "\t\tOpponent response move (" << k << ", " << l << ") has a heuristic value of: " << opponentResponseVal << std::endl;
                                board[k][l] = ' ';
                                if (player == 'X') {
                                    bestOpponentResponseVal = std::max(opponentResponseVal, bestOpponentResponseVal);
                                } else {
                                    bestOpponentResponseVal = std::min(opponentResponseVal, bestOpponentResponseVal);
                                }
                            }
                        }
                    }
                    moveVal = bestOpponentResponseVal;
                }

                std::cout << "Move (" << i << ", " << j << ") has a heuristic value of: " << moveVal << " (AI: " << ai << ", Player: " << player << ")\n\n";

                board[i][j] = ' ';
                if (player == 'X') {
                    if (moveVal < bestVal) {
                        bestMoveRow = i;
                        bestMoveCol = j;
                        bestVal = moveVal;
                    }
                } else {
                    if (moveVal > bestVal) {
                        bestMoveRow = i;
                        bestMoveCol = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }

    std::cout << "AI plays (" << bestMoveRow << ", " << bestMoveCol << ")\n";
    board[bestMoveRow][bestMoveCol] = ai;
}

int main() {
    initBoard();

    std::cout << "Choose your mark (X/O): ";
    std::cin >> player;
    ai = (player == 'X') ? 'O' : 'X';

    std::cout << "Choose AI difficulty level (1 or 2): ";
    std::cin >> maxDepth;

    printBoard();

    while (isMovesLeft()) {
        int x, y;
        bool validMove = false;

        if (player == 'X') {
            while (!validMove) {
                std::cout << "Enter your move (row and column): ";
                std::cin >> x >> y;
                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[x][y] == ' ') {
                    board[x][y] = player;
                    validMove = true;
                } else {
                    std::cout << "Invalid move! Try again.\n";
                }
            }
            printBoard();
            if (checkWin(player)) {
                std::cout << "You win!\n";
                break;
            }
            findBestMove();
            printBoard();
            if (checkWin(ai)) {
                std::cout << "AI wins!\n";
                break;
            }
        } else {
            findBestMove();
            printBoard();
            if (checkWin(ai)) {
                std::cout << "AI wins!\n";
                break;
            }
            while (!validMove) {
                std::cout << "Enter your move (row and column): ";
                std::cin >> x >> y;
                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE && board[x][y] == ' ') {
                    board[x][y] = player;
                    validMove = true;
                } else {
                    std::cout << "Invalid move! Try again.\n";
                }
            }
            printBoard();
            if (checkWin(player)) {
                std::cout << "You win!\n";
                break;
            }
        }

        if (!isMovesLeft()) {
            std::cout << "It's a draw!\n";
            break;
        }
    }

    return 0;
}
