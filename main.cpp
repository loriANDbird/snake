#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ncurses.h>

using namespace std;

#define WIDTH 60
#define HEIGHT 20 

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void Run();

private:
    void Init();
    void Draw();
    void Input();
    void Logic();

    int x, y;
    int fruitX, fruitY;
    int score;
    enum Direction {STOP = 0, LEFT, RIGHT, UP, DOWN};
    Direction dir;
    bool gameOver;
    int tailX[1000], tailY[1000];
    int nTail;
    int row, col;
};

SnakeGame::SnakeGame() {
    Init();
}

SnakeGame::~SnakeGame() {
}

void SnakeGame::Init() {
    for (int k = 0; k < nTail; k++) {
        tailX[k] = 0;
        tailY[k] = 0; 
    }
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    initscr();

    noecho();

    cbreak();
    getmaxyx(stdscr, col, row);
    // row = WIDTH;
    // col = HEIGHT;
    gameOver = false;
    dir = STOP;
    x = row / 2;
    y = col / 2;
    fruitX = rand() % row;
    fruitY = rand() % col;
    score = 0;
}

void SnakeGame::Draw() {
    clear();
    for (int i = 0; i < row+2; i++) {
        printw("#");
    }
    printw("\n");
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (j == 0) {
                printw("#");
            }
            if (i == y && j == x) {
                printw("O");
            } else if (i == fruitY && j == fruitX) {
                printw("F");
            } else {
                bool print = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        printw("o");
                        print = true;
                    }
                }
                if (!print) {
                    printw(" ");
                }
            }
            if (j == row-1) {
                printw("#");
            }
        }
        printw("\n");
    }

    for (int i = 0; i < row+2; i++) {   
        printw("#");
    }
    printw("\n");
    printw("Score: %d", score);
}

void SnakeGame::Input() {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        switch (c) {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
        }
    }
}

void SnakeGame::Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
  
    switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:  
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }
  
    if (x >= row) x = 0; else if (x < 0) x = row - 1;
    if (y >= col) y = 0; else if (y < 0) y = col - 1;

    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true; 
        }
    }
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % row;
        fruitY = rand() % col;
        nTail++;
    }
}

void SnakeGame::Run() {
    while (!gameOver) {
        Draw();
        refresh();
        Input();
        Logic();
        usleep(100000);
    }
}

int main() {
    SnakeGame game;
    game.Run();
    endwin();
    return 0;
}