#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

const int WIDTH = 30;
const int HEIGHT = 30;

void initializeBoard(std::vector<std::vector<bool>>& board) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            board[y][x] = (rand() % 2) == 0;
        }
    }
}

int countLiveNeighbors(const std::vector<std::vector<bool>>& board, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && board[ny][nx]) {
                count++;
            }
        }
    }
    return count;
}

void updateBoard(std::vector<std::vector<bool>>& board) {
    std::vector<std::vector<bool>> newBoard = board;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int liveNeighbors = countLiveNeighbors(board, x, y);

            if (board[y][x]) { // live cell
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    newBoard[y][x] = false; // death
                }
            } else { // dead cell
                if (liveNeighbors == 3) {
                    newBoard[y][x] = true; // birth
                }
            }
        }
    }

    board = newBoard;
}

void printBoard(const std::vector<std::vector<bool>>& board) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            std::cout << (board[y][x] ? 'O' : ' ');
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    srand(time(nullptr)); // Seed for random number generation
    std::vector<std::vector<bool>> board(HEIGHT, std::vector<bool>(WIDTH));

    initializeBoard(board);

    while (true) {
        printBoard(board);
        updateBoard(board);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        system("clear");
    }

    return 0;
}
