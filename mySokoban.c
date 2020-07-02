#include <stdio.h>
#include <ncurses.h>

#define WIN_WIDTH 79
#define WIN_HEIGHT 23
#define WIDTH 10
#define HEIGHT 8

enum eDirection {LEFT = 0, RIGHT, UP, DOWN} dir;

int playerX, playerY, finalX[7], finalY[7], destCount = 0, winCount = 7;

char map[8][10] = {
    
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', ' ', 'B', ' ', 'x', '#', 'x', '#', 'x', '#'},
    {'#', ' ', ' ', '#', '#', '#', '@', '#', 'B', '#'},
    {'#', ' ', '#', ' ', '#', ' ', 'B', ' ', ' ', '#'},
    {'#', 'x', 'B', 'x', '#', 'B', ' ', ' ', '#', '#'},
    {'#', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '#', ' '},
    {'#', ' ', 'x', ' ', ' ', 'x', '#', '#', '#', ' '},
    {'#', '#', '#', '#', '#', '#', '#', ' ', ' ', ' '}
    
};

void printMap() {
    int localX, localY;
    localX = (WIN_WIDTH - WIDTH) / 2;
    localY = (WIN_HEIGHT - HEIGHT) / 2;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            const char tmpCh = map[i][j];
            mvprintw(i+localY, j+localX, &tmpCh);
        }
    }
}

void setup() {
    int tmp = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(map[i][j] == '@') {
                playerY = i;
                playerX = j;
            }
            if(map[i][j] == 'x') {
                finalY[tmp] = i;
                finalX[tmp] = j;
                tmp++;
            }
        }
    }
}

void rules() {
    switch (dir) {
        case LEFT:
            if(map[playerY][playerX-1] != '#') { //Если следующая клетка не #
                if(map[playerY][playerX-1] == 'B' && map[playerY][playerX-2] != '#' && map[playerY][playerX-2] != 'B') { //Если ящик можно сдвинуть
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX-1] = '@';
                    if(map[playerY][playerX-2] == 'x') { //Если ящик сдвигают на х
                        destCount++;
                        map[playerY][playerX-2] = 'P';
                    } else {
                        map[playerY][playerX-2] = 'B';
                    }
                    playerX--;
                } else if((map[playerY][playerX-1] == ' ' || map[playerY][playerX-1] == 'x') && map[playerY][playerX-1] != 'B') { //Если игрока можно сдвинуть
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX-1] = '@';
                    playerX--;
                }
            }
            break;
        case RIGHT:
            if(map[playerY][playerX+1] != '#') {
                if(map[playerY][playerX+1] == 'B' && map[playerY][playerX+2] != '#' && map[playerY][playerX+2] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX+1] = '@';
                    if(map[playerY][playerX+2] == 'x') {
                        destCount++;
                        map[playerY][playerX+2] = 'P';
                    } else {
                        map[playerY][playerX+2] = 'B';
                    }
                    playerX++;
                } else if((map[playerY][playerX+1] == ' ' || map[playerY][playerX+1] == 'x') && map[playerY][playerX+1] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY][playerX+1] = '@';
                    playerX++;
                }
            }
            break;
        case UP:
            if(map[playerY-1][playerX] != '#') {
                if(map[playerY-1][playerX] == 'B' && map[playerY-2][playerX] != '#' && map[playerY-2][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY-1][playerX] = '@';
                    if(map[playerY-2][playerX] == 'x') {
                        destCount++;
                        map[playerY-2][playerX] = 'P';
                    } else {
                        map[playerY-2][playerX] = 'B';
                    }
                    playerY--;
                } else if((map[playerY-1][playerX] == ' ' || map[playerY-1][playerX] == 'x') && map[playerY-1][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY-1][playerX] = '@';
                    playerY--;
                }
            }
            break;
        case DOWN:
            if(map[playerY+1][playerX] != '#') {
                if(map[playerY+1][playerX] == 'B' && map[playerY+2][playerX] != '#' && map[playerY+2][playerX] != 'B') {
                    if(map[playerY-2][playerX] == 'x') destCount++;
                    map[playerY][playerX] = ' ';
                    map[playerY+1][playerX] = '@';
                    if(map[playerY+2][playerX] == 'x') {
                        destCount++;
                        map[playerY+2][playerX] = 'P';
                    } else {
                        map[playerY+2][playerX] = 'B';
                    }
                    playerY++;
                } else if((map[playerY+1][playerX] == ' ' || map[playerY+1][playerX] == 'x') && map[playerY+1][playerX] != 'B') {
                    map[playerY][playerX] = ' ';
                    map[playerY+1][playerX] = '@';
                    playerY++;
                }
            }
            break;
    }
    for(int i = 0; i < 7; i++) {
         if(map[finalY[i]][finalX[i]] != 'x' && map[finalY[i]][finalX[i]] != 'P' && map[finalY[i]][finalX[i]] != '@') { //Если не игрок и не ящик рисовать х
             map[finalY[i]][finalX[i]] = 'x';
         }
    }
    refresh();
}

int main() {
    bool gameOver = false;
    initscr();
    curs_set(0);
    noecho();
    setup();
    while(!gameOver) {
        mvprintw(0,0,"dests: %d", destCount);
        printMap();
        int ch = getch();
        if (ch == 'a') dir = LEFT;
        if (ch == 'd') dir = RIGHT;
        if (ch == 'w') dir = UP;
        if (ch == 's') dir = DOWN;
        if (ch == 'e') gameOver = true;
        rules();
        if (destCount == winCount) {
            mvprintw(0,0,"            ");
            mvprintw(WIN_HEIGHT/2,WIN_WIDTH/3,"You win!");
            gameOver = true;
        }
    }
    int localX, localY;
    localX = (WIN_WIDTH - WIDTH) / 2;
    localY = (WIN_HEIGHT - HEIGHT) / 2;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            map[i][j] = ' ';
            const char tmpCh = map[i][j];
            mvprintw(i+localY, j+localX, &tmpCh);
        }
    }
    printMap();
    if (destCount != winCount) {
        mvprintw(0,0,"            ");
        mvprintw(WIN_HEIGHT/2,WIN_WIDTH/3+5,"Sorry but you lose!");
    }
    refresh();
    getch();
    endwin();
    return 0;
}
