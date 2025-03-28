#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <algorithm>

using namespace std;

const int WIDTH = 10;
const int HEIGHT = 20;
const wstring EMPTY = L"  "; // Two spaces for empty cell

// ANSI color codes for background colors with better block design
const wstring COLORS[7] = { 
    L"\033[44m▄▄\033[0m",  // Blue (I)
    L"\033[43m▄▄\033[0m",  // Yellow (O)
    L"\033[45m▄▄\033[0m",  // Purple (T)
    L"\033[42m▄▄\033[0m",  // Green (S)
    L"\033[41m▄▄\033[0m",  // Red (Z)
    L"\033[46m▄▄\033[0m",  // Cyan (J)
    L"\033[47m▄▄\033[0m"   // White (L)
};

const int tetrominoes[7][4][4] = {
    // I
    {
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // O
    {
        {1, 1, 0, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // T
    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // S
    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // Z
    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // J
    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },
    // L
    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

wstring screenBuffer;

void clearScreen() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    for (int i = 0; i < 50; i++) {
        wcout << L"                                                                                \n";
    }
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

class Tetromino {
public:
    int shape[4][4];
    int x, y;
    wstring color;

    Tetromino(int type) : color(COLORS[type]) {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = tetrominoes[type][i][j];
        x = WIDTH / 2 - 2;
        y = 0;
    }

    void rotate() {
        int temp[4][4] = {0};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                temp[j][3 - i] = shape[i][j];
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                shape[i][j] = temp[i][j];
    }
};

class GameBoard {
public:
    wstring grid[HEIGHT][WIDTH];
    int score;
    int level;
    int linesCleared;
    bool redrawNeeded;

    GameBoard() : score(0), level(1), linesCleared(0), redrawNeeded(true) {
        for (int i = 0; i < HEIGHT; i++)
            for (int j = 0; j < WIDTH; j++)
                grid[i][j] = EMPTY;
    }

    void draw(Tetromino &current, Tetromino &next) {
        if (!redrawNeeded) return;
        
        screenBuffer.clear();
        
        // Draw the game board and info side by side with proper spacing
        for (int i = 0; i < HEIGHT; i++) {
            // Game board
            screenBuffer += L"║";
            for (int j = 0; j < WIDTH; j++) {
                wstring displayChar = grid[i][j];
                for (int ti = 0; ti < 4; ti++) {
                    for (int tj = 0; tj < 4; tj++) {
                        if (current.shape[ti][tj] && (i == current.y + ti) && (j == current.x + tj))
                            displayChar = current.color;
                    }
                }
                screenBuffer += displayChar;
            }
            screenBuffer += L"║";
            
            // Right side info with better spacing
            if (i == 1) {
                screenBuffer += L"    Level: " + to_wstring(level);
            }
            else if (i == 3) {
                screenBuffer += L"    Score: " + to_wstring(score);
            }
            else if (i == 5) {
                screenBuffer += L"    Lines: " + to_wstring(linesCleared);
            }
            // else if (i == 7) {
            //     screenBuffer += L"    Next Piece:";
            // }
            
            screenBuffer += L"\n";
        }
        
        // Bottom border
        screenBuffer += L"╚";
        for (int i = 0; i < WIDTH; i++) 
            screenBuffer += L"══";
        screenBuffer += L"╝\n";
        
        // Controls legend with arrow key alternatives
        screenBuffer += L"\nControls:\n";
        screenBuffer += L"[←][→] or [A][D] : Move Left/Right\n";
        screenBuffer += L"[↓] or [S]      : Move Down\n";
        screenBuffer += L"[↑] or [W]      : Rotate\n";
        screenBuffer += L"[Space]         : Hard Drop\n";
        screenBuffer += L"[P]             : Pause\n";
        screenBuffer += L"[R]             : Restart\n";
        screenBuffer += L"[X]             : Exit\n";
        
        // Next piece preview with better design
        screenBuffer += L"\n  Next Piece:\n";
        screenBuffer += L"  ┌────────┐\n";
        for (int i = 0; i < 4; i++) {
            screenBuffer += L"  │";
            for (int j = 0; j < 4; j++) {
                screenBuffer += (next.shape[i][j] ? next.color : EMPTY);
            }
            screenBuffer += L"│\n";
        }
        screenBuffer += L"  └────────┘\n";

        // Set cursor position and draw
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        wcout << screenBuffer;
        
        redrawNeeded = false;
    }

    bool canMove(Tetromino &t, int dx, int dy) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (t.shape[i][j]) {
                    int newX = t.x + j + dx;
                    int newY = t.y + i + dy;
                    if (newX < 0 || newX >= WIDTH || newY >= HEIGHT || 
                        (newY >= 0 && grid[newY][newX] != EMPTY)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void placeTetromino(Tetromino &t) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (t.shape[i][j]) {
                    grid[t.y + i][t.x + j] = t.color;
                }
            }
        }
        clearLines();
        redrawNeeded = true;
    }

    void clearLines() {
        vector<int> linesToClear;
        
        for (int i = 0; i < HEIGHT; i++) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++) {
                if (grid[i][j] == EMPTY) {
                    full = false;
                    break;
                }
            }
            if (full) {
                linesToClear.push_back(i);
            }
        }
        
        if (linesToClear.empty()) return;
        
        int fullLines = linesToClear.size();
        score += (fullLines == 1 ? 100 : 
                 fullLines == 2 ? 300 : 
                 fullLines == 3 ? 500 : 
                 fullLines == 4 ? 800 : 0);
        linesCleared += fullLines;
        
        if (linesCleared >= level * 3) {
            level++;
        }
        
        sort(linesToClear.begin(), linesToClear.end());
        for (int line : linesToClear) {
            for (int i = line; i > 0; i--) {
                for (int j = 0; j < WIDTH; j++) {
                    grid[i][j] = grid[i-1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                grid[0][j] = EMPTY;
            }
        }
    }

    void clearBoard() {
        clearScreen();
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                grid[i][j] = EMPTY;
            }
        }
        score = 0;
        level = 1;
        linesCleared = 0;
        redrawNeeded = true;
    }
};

class TetrisGame {
public:
    GameBoard board;
    Tetromino *currentPiece;
    Tetromino *nextPiece;
    bool gameOver;
    bool isPaused;
    DWORD lastUpdateTime;

    TetrisGame() : gameOver(false), isPaused(false), lastUpdateTime(GetTickCount()) {
        srand((unsigned)time(0));
        currentPiece = new Tetromino(rand() % 7);
        nextPiece = new Tetromino(rand() % 7);
        board.clearBoard(); // Clear screen on start
    }

    ~TetrisGame() {
        delete currentPiece;
        delete nextPiece;
    }

    void run() {
        while (!gameOver) {
            handleInput();
            if (!isPaused) {
                update();
            }
            board.draw(*currentPiece, *nextPiece);
            Sleep(10);
        }
        
        COORD coord = {0, HEIGHT + 23};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        wcout << L"Game Over! Press R to restart or X to exit.\n";
        
        while (true) {
            char key = getInput();
            if (key == 'r' || key == 'R') {
                restartGame();
                run();
                break;
            } else if (key == 'x' || key == 'X') {
                break;
            }
            Sleep(50);
        }
    }

private:
    char getInput() {
        if (_kbhit()) {
            int ch = _getch();
            // Handle arrow keys (returns two codes)
            if (ch == 0xE0 || ch == 0) {
                ch = _getch();
                switch (ch) {
                    case 75: return 'a'; // Left arrow
                    case 77: return 'd'; // Right arrow
                    case 72: return 'w'; // Up arrow
                    case 80: return 's'; // Down arrow
                }
            }
            return ch;
        }
        return '\0';
    }

    void handleInput() {
        char key = getInput();
        if (key == 'p' || key == 'P') {
            isPaused = !isPaused;
            board.redrawNeeded = true;
            if (isPaused) {
                COORD coord = {0, HEIGHT + 22};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                wcout << L"Game Paused. Press 'p' to resume...\n";
            }else {
                COORD coord = {0, HEIGHT + 22};
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
                wcout << L"                                                                           \n"; // Clear the line
            }
            return;
        }
        if (isPaused) return;
        
        bool moved = false;
        if (key == 'a' || key == 'A' || key == 75) { // Left arrow or 'a'
            if (board.canMove(*currentPiece, -1, 0)) {
                currentPiece->x--;
                moved = true;
            }
        } else if (key == 'd' || key == 'D' || key == 77) { // Right arrow or 'd'
            if (board.canMove(*currentPiece, 1, 0)) {
                currentPiece->x++;
                moved = true;
            }
        } else if (key == 's' || key == 'S' || key == 80) { // Down arrow or 's'
            if (board.canMove(*currentPiece, 0, 1)) {
                currentPiece->y++;
                moved = true;
            }
        } else if (key == 'w' || key == 'W' || key == 72) { // Up arrow or 'w'
            Tetromino temp = *currentPiece;
            temp.rotate();
            if (board.canMove(temp, 0, 0)) {
                currentPiece->rotate();
                moved = true;
            }
        } else if (key == ' ') {
            hardDrop();
            moved = true;
        } else if (key == 'r' || key == 'R') {
            restartGame();
            moved = true;
        } else if (key == 'x' || key == 'X') {
            exit(0);
        }
        
        if (moved) {
            board.redrawNeeded = true;
        }
    }

    void update() {
        DWORD currentTime = GetTickCount();
        if (currentTime - lastUpdateTime >= (500 / board.level)) {
            if (!board.canMove(*currentPiece, 0, 1)) {
                board.placeTetromino(*currentPiece);
                delete currentPiece;
                currentPiece = nextPiece;
                nextPiece = new Tetromino(rand() % 7);
                if (!board.canMove(*currentPiece, 0, 0)) {
                    gameOver = true;
                }
            } else {
                currentPiece->y++;
            }
            lastUpdateTime = currentTime;
            board.redrawNeeded = true;
        }
    }

    void hardDrop() {
        while (board.canMove(*currentPiece, 0, 1)) {
            currentPiece->y++;
            board.score++;
        }
        board.placeTetromino(*currentPiece);
        delete currentPiece;
        currentPiece = nextPiece;
        nextPiece = new Tetromino(rand() % 7);
        if (!board.canMove(*currentPiece, 0, 0)) {
            gameOver = true;
        }
        board.redrawNeeded = true;
    }

    void restartGame() {
        board.clearBoard();
        delete currentPiece;
        delete nextPiece;
        currentPiece = new Tetromino(rand() % 7);
        nextPiece = new Tetromino(rand() % 7);
        gameOver = false;
        isPaused = false;
        lastUpdateTime = GetTickCount();
        board.redrawNeeded = true;
    }
};

int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    TetrisGame game;
    game.run();
    
    return 0;
}